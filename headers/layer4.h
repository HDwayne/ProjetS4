/**
 * @file layer4.h
 * @author  HERZBERG Dwayne and BERLIN Florian
 * @brief header for layer4.c
 * @version 0.1
 * @date 2022-02-14
 */

#ifndef SCRATCHOS_LAYER4_H
#define SCRATCHOS_LAYER4_H

#include "os_defines.h"
#include "language.h"
#include "layer1.h"
#include "layer2.h"
#include "layer3.h"
#include "layer5.h"

int is_file_in_inode(char * filename);
int write_file(char * filename, file_t filedata, session_t user);
int read_file(char *filename, file_t *filedata);
int delete_file(char *filename);
int load_file_from_host(char * hostfilename, session_t user);
int store_file_to_host(char *filenamesos);

#endif //SCRATCHOS_LAYER4_H
