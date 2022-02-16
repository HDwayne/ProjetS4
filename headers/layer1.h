#ifndef SCRATCHOS_LAYER1_H
#define SCRATCHOS_LAYER1_H

#include "layer2.h"
#include "layer3.h"
#include "os_defines.h"

void uint_to_block(uint number, block_t *block);
uint block_to_uint(block_t block);
int init_disk_sos(char directory[FILENAME_MAX_SIZE]);
int shutdown_disk_sos();
uint compute_nblock(uint octets);
int write_block(block_t block, int pos); // return ERROR if error occurs
int read_block(block_t *block, int pos); // return ERROR if error occurs
void display_block(block_t block);
void display_disk_storage();

#endif //SCRATCHOS_LAYER1_H
