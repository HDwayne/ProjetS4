/**
 * @file layer2.h
 * @author  HERZBERG Dwayne and BERLIN Florian
 * @brief header for layer2.c
 * @version 0.1
 * @date 2022-02-14
 */

#ifndef SCRATCHOS_LAYER2_H
#define SCRATCHOS_LAYER2_H

#include "os_defines.h"
#include "language.h"
#include "layer1.h"
#include "layer3.h"
#include "layer4.h"
#include "layer5.h"

int write_super_block();
int read_super_block();
void update_first_free_byte();
int write_inodes_table();
int write_param_inode(int *pos, uint source);
int read_inodes_table();
int delete_inode(int index_inode);
int get_unused_inode();
int init_inode(const char fileName[FILENAME_MAX], uint size, uint pos, char createTime[TIMESTAMP_SIZE], char modifyTime[TIMESTAMP_SIZE], session_t user);
bool has_rights(int i_inode, int user_id, int right);

#endif //SCRATCHOS_LAYER2_H
