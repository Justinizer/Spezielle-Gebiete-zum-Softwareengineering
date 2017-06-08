#include "daemonize.h"
#include <sys/types.h>	// For umask()
#include <sys/file.h>	// For flock()
#include <sys/stat.h>	// For umask()
#include <unistd.h>		// For access(), fork(), setsid(), chdir(), dup(), close(), sleep()
#include <syslog.h>		// For openlog(), syslog(), closelog()
#include <signal.h>		// For signal(), sigemptyset(), sigprocmask(), sigaddset()
#include <stdlib.h>		// For atexit()
#include <string.h>		// For strerror()
#include <errno.h>		// For errno
#include <fcntl.h>		// For open()
#include <stdio.h>		// For strerror()

const char *lockfilepath = "/run/lock/stm32driver.lock";

int lockfile;

int daemonize(void) {
	int result;
	pid_t pid;


	pid = fork();
	if (pid == -1) {	// Error
		openlog("stm32driver", LOG_PID, LOG_USER);
		syslog(LOG_ERR, "Error while forking: %s", strerror(errno));
		closelog();
		return 1;
	}

	// Terminate parent
	if (pid > 0) {
		exit(0);
	}


	// Set new session id
	if (setsid() == -1) {
		openlog("stm32driver", LOG_PID, LOG_USER);
		syslog(LOG_ERR, "Error setting new session id: %s", strerror(errno));
		closelog();
		return 1;
	}


	// Temporarily ignore signals to prevent termination of process group after next fork
	signal(SIGCHLD, SIG_IGN);
	signal(SIGHUP, SIG_IGN);


	pid = fork();
	if (pid == -1) {
		openlog("stm32driver", LOG_PID, LOG_USER);
		syslog(LOG_ERR, "Error while forking: %s", strerror(errno));
		closelog();
		return 1;
	}

	// Terminate parent
	if (pid > 0) {
		exit(0);
	}

	// Set new file rights
	umask(0);

	// Change working directory
	chdir("/");

	// CLose all open file descriptors
	for (int i = sysconf(_SC_OPEN_MAX); i >= 0; i--) {
		close(i);
	}

	// Redirect standard input, standard output and standard error output to /dev/null
	int fd = open("/dev/null", O_RDWR);	// Standard input (stdin)
	dup(fd);	// Standard output			(stdout)
	dup(fd);	// Standard error output	(stderr)


	// Open syslog
	openlog("stm32driver", LOG_PID, LOG_USER);
	syslog(LOG_INFO, "Starting...");


	// Open lock file to prevent multiple instances
	lockfile = open(lockfilepath, O_RDWR | O_CREAT, 0666);
	if (lockfile == -1) {
		syslog(LOG_ERR, "Error creating lock file: %s", strerror(errno));
		closelog();
		return 1;
	}

	// Try to lock file
	result = flock(lockfile, LOCK_EX | LOCK_NB);
	if (result != 0) {
		syslog(LOG_WARNING, "There is already an instance running or lock was not removed: %s", strerror(errno));
		close(lockfile);
		closelog();
		return 1;
	}


	// Register function which will be called at exit
	result = atexit(quit_daemon);
	if (result != 0) {
		syslog(LOG_WARNING, "Error registering exit function: %s", strerror(errno));
		syslog(LOG_WARNING, "Lock file will not be removed at exit!");
	}

	// Register signal_handler() for signal SIGTERM
	signal(SIGTERM, signal_handler);

	return 0;
}

void quit_daemon(void) {
	int result;

	syslog(LOG_INFO, "Exiting...");

	// Remove file lock from lock file
	result = flock(lockfile, LOCK_UN);
	if (result == -1) {
		syslog(LOG_ERR, "Error removing lock from lock file: %s", strerror(errno));
	}

	close(lockfile);

	// Remove lock file
	result = unlink(lockfilepath);
	if (result == -1) {
		syslog(LOG_ERR, "Error removing lock file: %s", strerror(errno));
	}

	closelog();
}

void signal_handler(int signal) {
	if (signal == SIGTERM) {
		exit(0);
	}
}