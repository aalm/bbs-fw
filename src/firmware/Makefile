.PHONY: all clean

ifeq '$(findstring ;,$(PATH))' ';'
	UNAME := Windows
else
	UNAME := $(shell uname 2>/dev/null || echo Unknown)
endif

# Select target controller (normally done from cmd line):
#TARGET_CONTROLLER = BBSHD
#TARGET_CONTROLLER = BBS02
#TARGET_CONTROLLER = TSDZ2
#TARGET_CONTROLLER = KUNTENG

# Compiler
CC = sdcc

# Target name
TARGET = bbs-fw

MAINSRC = main.c
SUBDIRS = 
		
CFLAGS = -Ddouble=float --std-sdcc23 -D$(TARGET_CONTROLLER)
	
# Target Specific
ifeq ($(TARGET_CONTROLLER), BBSHD)
	CFLAGS += -mmcs51 --model-large --xram-size 3840	
	SUBDIRS += bbsx	
endif

ifeq ($(TARGET_CONTROLLER), BBS02)
	CFLAGS += -mmcs51 --model-large --xram-size 1792
	SUBDIRS += bbsx
endif

ifeq ($(TARGET_CONTROLLER), TSDZ2)
	CFLAGS += -mstm8
	SUBDIRS += tsdz2
endif

ifeq ($(TARGET_CONTROLLER), KUNTENG)
	CFLAGS += -mstm8 --opt-code-speed
	SUBDIRS += kunteng
endif

	
INCS = $(wildcard *.h $(foreach fd, $(SUBDIRS), $(fd)/*.h))
SRCS = $(filter-out main.c, $(wildcard *.c $(foreach fd, $(SUBDIRS), $(fd)/*.c)))
RELS := $(SRCS:.c=.rel)

INC_DIRS = -I./ $(addprefix -I, $(SUBDIRS))


all: precheck $(TARGET) hex
	
$(TARGET): $(MAINSRC) $(RELS)
	$(CC) -o $(TARGET).ihx $(INC_DIRS) $(CFLAGS) $(MAINSRC) $(RELS)

%.rel: %.c $(INCS)
	$(CC) -o $@ -c $(INC_DIRS) $(CFLAGS) $<

echo:
	$(info SRCS: $(SRCS))
	$(info RELS: $(RELS))
	$(info INCS: $(INCS))

precheck:
ifndef TARGET_CONTROLLER
	$(info TARGET_CONTROLLER is not specified.)
	$(info Set to one of [BBSHD, BBS02, TSDZ2, KUNTENG])
	$(info Example:)
	$(info $(null)  make all TARGET_CONTROLLER=BBSHD)
	$(error )
endif
	$(info Building bbs-fw for $(TARGET_CONTROLLER))

hex:
ifeq ($(UNAME), Linux)
	@packihx bbs-fw.ihx > bbs-fw.hex
else
	@cmd /C tohex.bat
endif

clean:
ifeq ($(UNAME), Linux)
	@rm -f bbsx/*.hex tsdz2/*.hex kunteng/*.hex *.hex
	@rm -f bbsx/*.ihx tsdz2/*.ihx kunteng/*.ihx *.ihx
	@rm -f bbsx/*.asm tsdz2/*.asm kunteng/*.asm *.asm
	@rm -f bbsx/*.rel tsdz2/*.rel kunteng/*.rel *.rel
	@rm -f bbsx/*.lk  tsdz2/*.lk  kunteng/*.lk  *.lk
	@rm -f bbsx/*.lst tsdz2/*.lst kunteng/*.lst *.lst
	@rm -f bbsx/*.rst tsdz2/*.rst kunteng/*.rst *.rst
	@rm -f bbsx/*.sym tsdz2/*.sym kunteng/*.sym *.sym
	@rm -f bbsx/*.cdb tsdz2/*.cdb kunteng/*.cdb *.cdb
	@rm -f bbsx/*.map tsdz2/*.map kunteng/*.map *.map
	@rm -f bbsx/*.elf tsdz2/*.elf kunteng/*.elf *.elf
	@rm -f bbsx/*.adb tsdz2/*.adb kunteng/*.adb *.adb
	@rm -f bbsx/*.mem tsdz2/*.mem kunteng/*.mem *.mem
else
	@cmd /C clean.bat
endif
	$(info Clean Finished)

.PHONY = all hex clean precheck echo
.SUFFIXES: .c .rel
