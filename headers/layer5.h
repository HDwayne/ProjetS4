/**
 * @file layer5.h
 * @author  HERZBERG Dwayne and BERLIN Florian
 * @brief header for layer5.c
 * @version 0.1
 * @date 2022-02-14
 */

#ifndef SCRATCHOS_LAYER5_H
#define SCRATCHOS_LAYER5_H

#include "os_defines.h"
#include "language.h"
#include "layer1.h"
#include "layer2.h"
#include "layer3.h"
#include "layer4.h"
#include "terminal.h"

#define CMD_HELP "help"
#define CMD_QUIT "quit"
#define CMD_LS "ls"
#define CMD_CAT "cat"
#define CMD_RM "rm"
#define CMD_CR "cr"
#define CMD_EDIT "edit"
#define CMD_LOAD "load"
#define CMD_STORE "store"
#define CMD_CHOWN "chown"
#define CMD_CHMOD "chmod"
#define CMD_LISTUSERS "listusers"
#define CMD_ADDUSER "adduser"
#define CMD_RMUSER "rmuser"
#define CMD_LOGOUT "logout"
#define CMD_SUDO "sudo"
#define CMD_CLEAR "clear"

#define CMD_MAX_SIZE ((9 + FILENAME_MAX_SIZE*2 + 1))

int terminal_shell();
int read_cmd(char * cmd, int size);
void interprete_cmd(char * cmd, cmd_t *args);
int execute_cmd(cmd_t args, session_t user, int sudo_mode_user);
int cmd_ls(cmd_t args, session_t user);
int cmd_cat(cmd_t args, session_t user);
int cmd_rm(cmd_t args, session_t user);
int cmd_cr(cmd_t args, session_t user);
int cmd_edit(cmd_t args, session_t user);
int cmd_load(cmd_t args, session_t user);
int cmd_store(cmd_t args, session_t user);
int cmd_chown(cmd_t args, session_t user);
int cmd_chmod(cmd_t args, session_t user);
int cmd_listusers(cmd_t args);
int cmd_adduser(cmd_t args, session_t user);
int cmd_rmuser(cmd_t args, session_t user, int index_user_real);
int cmd_clear();
int cmd_help(cmd_t args);

#endif //SCRATCHOS_LAYER5_H
