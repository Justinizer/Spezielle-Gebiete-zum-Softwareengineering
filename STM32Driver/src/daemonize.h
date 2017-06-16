#ifndef _DAEMONIZE_H_
#define _DAEMONIZE_H_


/**
 * Starts program as a daemon.
 * @return 0 if the daemon started successfully. 1 otherwise.
 */
int daemonize(void);

/**
 * Is called when exit() is called. Cleans up.
 */
void quit_daemon(void);

/**
 * Is called when the signal SIGTERM is received.
 * @param signal The received signal.
 */
void signal_handler(int signal);

#endif