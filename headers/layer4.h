//
// Created by Florian Berlin on 13/02/2022.
//

#ifndef UNTITLED_LAYER4_H
#define UNTITLED_LAYER4_H

#include "../headers/os_defines.h"



int is_file_in_inode(char * filename);
void write_file(char * filename, file_t filedata);
int read_file(char *filename, file_t *filedata); // return 1 if successful, 0 if unsuccessful
int delete_file(char *filename); // return 1 if successful, 0 if unsuccessful
int load_file_from_host(char * hostfilename);
int store_file_to_host(char *filenamesos);




#endif //UNTITLED_LAYER4_H
