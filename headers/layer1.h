/**
 * @file layer1.h
 * @author  HERZBERG Dwayne and BERLIN Florian
 * @brief header for layer1.c
 * @version 0.1
 * @date 2022-02-14
 */

#ifndef SCRATCHOS_LAYER1_H
#define SCRATCHOS_LAYER1_H

#include "os_defines.h"
#include "language.h"
#include "layer2.h"
#include "layer3.h"
#include "layer4.h"
#include "layer5.h"

void uint_to_block(uint number, block_t *block);
uint block_to_uint(block_t block);
int init_disk_sos(char directory[FILENAME_MAX_SIZE], int kid);
int shutdown_disk_sos();
uint compute_nblock(uint octets);
int rw_tool_block(int pos);
int write_block(block_t block, int pos);
int read_block(block_t *block, int pos);
void display_block(block_t block);
int write_text_block_char(int *pos, int size, char *source);
int write_text_block_uchar(uint *pos, int size, uchar *source);
int read_text_block_char(int *pos, int size, char* destination);
int read_text_block_uchar(uint *pos, int size, uchar* destination);
void display_disk_storage();

#endif //SCRATCHOS_LAYER1_H
