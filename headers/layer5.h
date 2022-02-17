//
// Created by Florian Berlin on 13/02/2022.
//

#ifndef SCRATCHOS_LAYER5_H
#define SCRATCHOS_LAYER5_H

#include "os_defines.h"

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

#define CMD_MAX_SIZE ((9 + FILENAME_MAX_SIZE*2 + 1))

int terminal_shell();
void read_cmd(char * cmd, int size);
void interprete_cmd(char * cmd, cmd_t *args);
int execute_cmd(cmd_t args, session_t user);
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
int cmd_rmuser(cmd_t args, session_t user);

#endif //SCRATCHOS_LAYER5_H
