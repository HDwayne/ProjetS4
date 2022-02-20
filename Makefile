#### variables ####

CC = gcc
CFLAGS = -Wall -std=c99

SRC = sources
BINDIR = bin

SRCS = $(wildcard $(SRC)/*.c)

SRC_OS = $(filter-out $(SRC)/installos.c $(SRC)/cmd_dump_file.c $(SRC)/cmd_dump_inode.c $(SRC)/cmd_dump_user.c,  $(SRCS))
SRC_INSTALLER = $(filter-out $(SRC)/os.c $(SRC)/cmd_dump_file.c $(SRC)/cmd_dump_inode.c $(SRC)/cmd_dump_user.c, $(SRCS))

SCR_DUMP_FILE = $(filter-out $(SRC)/os.c $(SRC)/installos.c $(SRC)/cmd_dump_inode.c $(SRC)/cmd_dump_user.c, $(SRCS))
SCR_DUMP_INODE = $(filter-out $(SRC)/os.c $(SRC)/installos.c $(SRC)/cmd_dump_file.c $(SRC)/cmd_dump_user.c, $(SRCS))
SCR_DUMP_USER = $(filter-out $(SRC)/os.c $(SRC)/installos.c $(SRC)/cmd_dump_file.c $(SRC)/cmd_dump_inode.c, $(SRCS))


BIN_INSTALL = $(BINDIR)/os_installer
BIN_OS = $(BINDIR)/os

BIN_DUMP_FILE = $(BINDIR)/dump_file
BIN_DUMP_INODE = $(BINDIR)/dump_inode
BIN_DUMP_USER = $(BINDIR)/dump_user

DISK = repdisk

all: clean os installer dump_file dump_inode dump_user disk

#### build OS / Installer ####

sys: clean os installer disk

os: $(BIN_OS)
	$(info    OK)

$(BIN_OS): $(SRC_OS)
	@$(CC) $(CFLAGS) $(SRC_OS) -o $@
	$(info    -------- OS -----------------)

installer: $(BIN_INSTALL)
	$(info    OK)

$(BIN_INSTALL): $(SRC_INSTALLER)
	@$(CC) $(CFLAGS) $(SRC_INSTALLER) -o $@
	$(info    -------- OS INSTALLER -------)

#### build dump prog ####

dump: dump_file dump_inode dump_user

dump_file: $(BIN_DUMP_FILE)
	$(info    OK)

$(BIN_DUMP_FILE): $(SCR_DUMP_FILE)
	@$(CC) $(CFLAGS) $(SCR_DUMP_FILE) -o $@
	$(info    -------- DUMP FILE -------)

dump_inode: $(BIN_DUMP_INODE)
	$(info    OK)

$(BIN_DUMP_INODE): $(SCR_DUMP_INODE)
	@$(CC) $(CFLAGS) $(SCR_DUMP_INODE) -o $@
	$(info    -------- DUMP INODE -------)

dump_user: $(BIN_DUMP_USER)
	$(info    OK)

$(BIN_DUMP_USER): $(SCR_DUMP_USER)
	@$(CC) $(CFLAGS) $(SCR_DUMP_USER) -o $@
	$(info    -------- DUMP USER -------)

#### Usefull ####

.SILENT: disk
disk:
	$(info    -------- DISK ---------------)
	./Format/cmd_format ./${DISK} 50000 0
	./Format/cmd_format ./${DISK} 50000 1
	./Format/cmd_format ./${DISK} 50000 2
	./Format/cmd_format ./${DISK} 50000 3
	./Format/cmd_format ./${DISK} 50000 4
	$(info    OK)

.SILENT: clean
clean:
	$(info    -------- CLEAN --------------)
	cd ${DISK}/ && rm -f `ls | grep -v DO_NOT_DELETE` && cd ..
	cd ${BINDIR}/ && rm -f `ls | grep -v DO_NOT_DELETE` && cd ..
	$(info    OK)
