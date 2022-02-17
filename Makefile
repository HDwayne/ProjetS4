CC = gcc
CFLAGS = -Wall -std=c99

SRC = sources
BINDIR = bin

SRCS = $(wildcard $(SRC)/*.c)
SRC_OS = $(filter-out $(SRC)/installos.c, $(SRCS))
SRC_INSTALLER = $(filter-out $(SRC)/os.c, $(SRCS))

BIN_INSTALL = $(BINDIR)/os_installer
BIN_OS = $(BINDIR)/os

DISK = repdisk

all: clean os installer disk

os: $(BIN_OS)
	$(info    OK)

$(BIN_OS): $(SRC_OS)
	@$(CC) $(CFLAGS) $(SRC_OS) -o $@
	$(info    -------- OS -----------------)

.SILENT: installer
installer: $(BIN_INSTALL)
	$(info    OK)

$(BIN_INSTALL): $(SRC_INSTALLER)
	@$(CC) $(CFLAGS) $(SRC_INSTALLER) -o $@
	$(info    -------- OS INSTALLER -------)

.SILENT: disk
disk:
	$(info    -------- DISK ---------------)
	./Format/cmd_format ./${DISK} 50000
	$(info    OK)


.SILENT: clean
clean:
	$(info    -------- CLEAN --------------)
	cd ${DISK}/ && rm -f `ls | grep -v DO_NOT_DELETE` && cd ..
	cd ${BINDIR}/ && rm -f `ls | grep -v DO_NOT_DELETE` && cd ..
	$(info    OK)
