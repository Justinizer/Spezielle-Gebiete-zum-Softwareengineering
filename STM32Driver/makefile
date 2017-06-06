##############################################
######### Makefile by Nils Kohlmeier #########
#########    Stand: 07.01.2107       #########
##############################################

# Name des Kompilats:
TARGET = main


# Verzeichnisse:
SRCDIR = src
OBJDIR = obj
DOCDIR = doc
LIBDIR =
INCDIR =

# Programme:
CCPP	= g++
CC		= gcc
REMOVE	= rm -f
NEMIVER	= nemiver

# CPP- und C-Standard
CPP_STD = c++11
C_STD	= c11

# Compilerflags:
CFLAGS	= -Wall -O2 -ffunction-sections -fdata-sections
CDEBUG	= -Wall -O0 -ffunction-sections -fdata-sections -g3

# Linkerflags:
LFLAGS	= -Wl,--gc-sections -Wl,--as-needed -s
LDEBUG	= -Wl,--gc-sections -Wl,--as-needed

# Quelltextdateien:
SRC_CPP	=
SRC_C	= main.c

# Bibliotheken:
LIBS	=

# Dateiendungen:
CPP_EXT 	= cpp
C_EXT		= c
DEBUG_EXT	= debug

########################################################################################################################
################################################## ENDE EINSTELLUNGEN ##################################################
############################################### AB HIER NICHTS VERÄNDERN! ##############################################
########################################################################################################################

OBJ_C			= $(SRC_C:%.$(C_EXT)=$(OBJDIR)/%.o)
OBJ_CPP			= $(SRC_CPP:%.$(CPP_EXT)=$(OBJDIR)/%.o)
OBJ_C_DEBUG		= $(SRC_C:%.$(C_EXT)=$(OBJDIR)/%_debug.o)
OBJ_CPP_DEBUG	= $(SRC_CPP:%.$(CPP_EXT)=$(OBJDIR)/%_debug.o)

LFLAGS += $(LIBS)
LDEBUG += $(LIBS)

# Weitere include-Verzeichnisse spezifizieren, wenn vorhanden
ifneq ($(INCDIR),)
CFLAGS += -I$(INCDIR)/
CDEBUG += -I$(INCDIR)/
endif

# Weitere lib-Verzeichnisse spezifizieren, wenn vorhanden
ifneq ($(LIBDIR),)
LFLAGS += $(LIBDIR)/
LDEBUG += $(LIBDIR)/
endif

CFLAGS += -c
CDEBUG += -c

all: init build

# Verzeichnis obj anlegen
init:
	mkdir -p $(OBJDIR)


# Dokumentation mit doxygen erzeugen:
doc:
	mkdir -p $(DOCDIR)
	doxygen Doxyfile

# Linken
build: $(OBJ_CPP) $(OBJ_C)
	$(CCPP) -o $(TARGET) $(OBJ_CPP) $(OBJ_C) $(LFLAGS)

# CPP-Dateien kompilieren
$(OBJDIR)/%.o: $(SRCDIR)/%.$(CPP_EXT)
	$(CCPP) $(CFLAGS) -std=$(CPP_STD) -o $@ $<

# C-Dateien kompilieren
$(OBJDIR)/%.o: $(SRCDIR)/%.$(C_EXT)
	$(CC) $(CFLAGS) -std=$(C_STD) -o $@ $<

# CPP-Dateien im Debug-Modus kompilieren
$(OBJDIR)/%_debug.o: $(SRCDIR)/%.$(CPP_EXT)
	$(CCPP) $(CDEBUG) -std=$(CPP_STD) -o $@ $<

# C-Dateien im Debug-Modus kompilieren
$(OBJDIR)/%_debug.o: $(SRCDIR)/%.$(C_EXT)
	$(CC) $(CDEBUG) -std=$(C_STD) -o $@ $<

# Debug-Kompilat erstellen
debug: init $(OBJ_CPP_DEBUG) $(OBJ_C_DEBUG)
	$(CCPP) -o $(TARGET).$(DEBUG_EXT) $(OBJ_CPP_DEBUG) $(OBJ_C_DEBUG) $(LDEBUG)

# Nemiver mit Debug-Kompilat starten
nemiver: debug
	$(NEMIVER) $(TARGET).$(DEBUG_EXT)

# Aufräumen
clean:
	$(REMOVE) -r $(OBJDIR)
#	$(REMOVE) -r $(DOCDIR)
	$(REMOVE) $(TARGET)
	$(REMOVE) $(TARGET).$(DEBUG_EXT)

.SILENT: all build clean init debug nemiver
