/**
 * @file layer3.h
 * @author  HERZBERG Dwayne and BERLIN Florian
 * @brief header for layer3.c
 * @version 0.1
 * @date 2022-02-14
 */

#ifndef SCRATCHOS_LAYER3_H
#define SCRATCHOS_LAYER3_H

#include "os_defines.h"
#include "language.h"
#include "layer1.h"
#include "layer2.h"
#include "layer4.h"
#include "layer5.h"

int write_users_table();
int read_users_table();
int delete_user(int id_user);
int get_unused_user();
int init_user(char login[FILENAME_MAX_SIZE], char password[FILENAME_MAX_SIZE]);
int is_login_in_users_table(char login[FILENAME_MAX_SIZE]);
int is_good_credentials(char login[FILENAME_MAX_SIZE], char password[FILENAME_MAX_SIZE]);

#endif //SCRATCHOS_LAYER3_H
