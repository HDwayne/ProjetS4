//
// Created by Florian Berlin on 14/02/2022.
//

#ifndef SCRATCHOS_LAYER2_H
#define SCRATCHOS_LAYER2_H

#include "os_defines.h"

int write_super_block();
int read_super_block();
void update_first_free_byte();
void write_inodes_table();
void read_inodes_table();
int delete_inode(int index_inode);
int get_unused_inode();
int init_inode(const char fileName[FILENAME_MAX], uint size, uint pos, char createTime[TIMESTAMP_SIZE], char modifyTime[TIMESTAMP_SIZE]);
void cmd_dump_inode(char *directory);
bool has_rights(int i_inode, int user_id, int right);

#endif //SCRATCHOS_LAYER2_H
