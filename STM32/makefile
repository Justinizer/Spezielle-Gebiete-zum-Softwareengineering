##############################################
######### Makefile by Nils Kohlmeier #########
#########    Stand: 07.01.2107       #########
##############################################

# Name des Kompilats:
TARGET		= main
TARGET_EXT	= bin


# Verzeichnisse:
SRCDIR = src
OBJDIR = obj
DOCDIR = doc
LIBDIR =
INCDIR =

# Programme:
TOOL_PREFIX = arm-none-eabi

AS		= $(TOOL_PREFIX)-as
CCPP	= $(TOOL_PREFIX)-g++
CC		= $(TOOL_PREFIX)-gcc
LD		= $(TOOL_PREFIX)-ld.bfd
NM		= $(TOOL_PREFIX)-nm
SIZE	= $(TOOL_PREFIX)-size
OBJCOPY = $(TOOL_PREFIX)-objcopy
OBJDUMP = $(TOOL_PREFIX)-objdump -d
FLASHER = ../../stm32loader/stm32loader.py -e -w -v -p /dev/ttyUSB0
REMOVE	= rm -f

# CPP- und C-Standard
CPP_STD = c++11
C_STD	= c11

# Compilerflags:
CFLAGS	= -mcpu=cortex-m3 -mthumb -Wall -Os -ffunction-sections -fdata-sections

# Linkerflags:
LFLAGS	= --gc-sections --as-needed

# Quelltextdateien:
SRC_CPP	=
SRC_C	= main.c uart.c gpio.c sds011.c util.c
SRC_ASM	= startup.s

# Bibliotheken:
LIBS	=

# Dateiendungen:
CPP_EXT 	= cpp
C_EXT		= c
ASM_EXT		= s
DEBUG_EXT	= debug


LINKERSCRIPT	= LinkerScript.ld

########################################################################################################################
################################################## ENDE EINSTELLUNGEN ##################################################
############################################### AB HIER NICHTS VERÄNDERN! ##############################################
########################################################################################################################

OBJ_C			= $(SRC_C:%.$(C_EXT)=$(OBJDIR)/%.o)
OBJ_ASM			= $(SRC_ASM:%.$(ASM_EXT)=$(OBJDIR)/%.o)
OBJ_CPP			= $(SRC_CPP:%.$(CPP_EXT)=$(OBJDIR)/%.o)

LFLAGS += $(LIBS)

# Weitere include-Verzeichnisse spezifizieren, wenn vorhanden
ifneq ($(INCDIR),)
CFLAGS += -I$(INCDIR)/
endif

# Weitere lib-Verzeichnisse spezifizieren, wenn vorhanden
ifneq ($(LIBDIR),)
LFLAGS += $(LIBDIR)/
endif

CFLAGS += -c

all: init $(TARGET).bin disas size

# Verzeichnis obj anlegen
init:
	mkdir -p $(OBJDIR)

# Dokumentation mit doxygen erzeugen:
doc:
	mkdir -p $(DOCDIR)
	doxygen Doxyfile

# Kompilat auf Mikrocontroller flashen
flash: $(TARGET).bin
	$(FLASHER) $(TARGET).bin

# CPP-Dateien kompilieren
$(OBJDIR)/%.o: $(SRCDIR)/%.$(CPP_EXT)
	$(CCPP) $(CFLAGS) -std=$(CPP_STD) -o $@ $<

# C-Dateien kompilieren
$(OBJDIR)/%.o: $(SRCDIR)/%.$(C_EXT)
	$(CC) $(CFLAGS) -std=$(C_STD) -o $@ $<

#ASM-Dateien assemblieren
$(OBJDIR)/%.o: $(SRCDIR)/%.$(ASM_EXT)
	$(AS) $(ASMFLAGS) -o $@ $<

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) $(TARGET).elf $(TARGET).bin -O binary -S

$(TARGET).elf: $(OBJ_C) $(OBJ_ASM) $(OBJ_CPP)
	$(LD) $(LFLAGS) -T $(LINKERSCRIPT) -o $(TARGET).elf $(OBJ_C) $(OBJ_ASM) $(OBJ_CPP)

size: $(TARGET).elf
	$(SIZE) $(TARGET).elf
	$(NM) --size-sort --reverse-sort -S $(TARGET).elf

disas:
	$(OBJDUMP) -d $(TARGET).elf > $(TARGET).disas

# Aufräumen
clean:
	$(REMOVE) -r $(OBJDIR)
	$(REMOVE) $(TARGET).bin
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).disas

.SILENT: all clean init disas size $(TARGET).elf $(TARGET).bin
