CC = gcc
CFLAGS = -Wall -std=c99

SRC = sources
OBJ = obj
OBJ_OS = $(OBJ)/os
OBJ_INSTALLER = $(OBJ)/installer
BINDIR = bin

SRCS = $(wildcard $(SRC)/*.c)
SRC_OS = $(filter-out $(SRC)/installos.c, $(SRCS))
SRC_INSTALLER = $(filter-out $(SRC)/os.c, $(SRCS))

# OBJS_OS = $(patsubst $(SRC_OS)/%.c, $(OBJ_OS)/%.o, $(SRC_OS))
# OBJS_installer = $(patsubst $(SRC_INSTALLER)/%.c, $(OBJ_INSTALLER)/%.o, $(SRC_INSTALLER))

BIN_INSTALL = $(BINDIR)/os_installer
BIN_OS = $(BINDIR)/os

DISK = repdisk

all: clean os installer disk

os: $(BIN_OS)

$(BIN_OS): $(SRC_OS)
	$(CC) $(CFLAGS) $(SRC_OS) -o $@


installer: $(BIN_INSTALL)

$(BIN_INSTALL): $(SRC_INSTALLER)
	$(CC) $(CFLAGS) $(SRC_INSTALLER) -o $@

disk:
	./Format/cmd_format ./${DISK} 50000

clean:
	rm -f ${DISK}/* ${BINDIR}/*