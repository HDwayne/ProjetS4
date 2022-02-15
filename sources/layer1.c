//
// Created by Florian Berlin on 14/02/2022.
//

#include "../headers/os_defines.h"
#include "../headers/layer1.h"
#include "../headers/layer2.h"
#include "../headers/layer3.h"

/**
 * \brief Initialize the virtual disk
 * @param directory
 * @return error code or success code depending on the opening of the disk file
 */
void uint_to_block(uint number, block_t *block){
    for (int i = 0; i < sizeof(uint); ++i) {
        block->data[i] = ((uchar *)&number)[i];
    }
}

uint block_to_uint(block_t block){
    uint number;
    for (int i = 0; i < sizeof(uint); ++i) {
        ((uchar *)&number)[i] = block.data[i];
    }
    return number;
}

int init_disk_sos(char *directory){
    virtual_disk_sos = malloc(sizeof(virtual_disk_t));
    char *diskFile = malloc(sizeof(char)*(strlen(directory)+4));
    strcpy(diskFile, directory);
    strcat(diskFile, "/d0");
    virtual_disk_sos->storage=fopen(diskFile, "r+w");
    if (virtual_disk_sos->storage==NULL){
        perror("Could not open diskFile");
        return ERROR;
    }
    read_super_block();
    read_inodes_table();
    read_users_table();
    user = malloc(sizeof(session_t));
    user->userid = 0;
    return SUCCESS;
}

/**
 * \brief Close disk storage and save tables
 * @return success code or error code depending on whether success or failure
 */
int shutdown_disk_sos(){
    write_super_block();
    write_inodes_table();
    write_users_table();
    free(user);
    fclose(virtual_disk_sos->storage);
    free(virtual_disk_sos);
    return SUCCESS;
}

/**
 * \brief Function that calculates the number of blocks needed for n octets
 * @param octets
 * @return nb of blocks needed for n octets
 */
uint compute_nblock(uint octets){
    return octets%4 == 0 ? octets/4 : octets/4+1;
}

/**
 * \brief Function that writes a block on the disk storage
 * @param block
 * @param pos
 * @return error code or success code dpeending on what happened
 */
int write_block(block_t block, int pos){
    fseek(virtual_disk_sos->storage, 0, SEEK_END);
    if( ftell(virtual_disk_sos->storage) <= pos){
        fprintf(stderr, "The disk storage is full.\n");
        return ERROR;
    }
    fseek(virtual_disk_sos->storage, (long)pos, SEEK_SET);
    int code = (int)fwrite(block.data, sizeof(uchar), BLOCK_SIZE, virtual_disk_sos->storage);
    if (code != BLOCK_SIZE){
        fprintf(stderr, "An error occurred while writing block\n");
        return ERROR;
    }
    return SUCCESS;
}

/**
 * \brief Function that read a block of data from the disk storage
 * @param block
 * @param pos
 * @return
 */
int read_block(block_t *block, int pos){
    fseek(virtual_disk_sos->storage, 0, SEEK_END);
    if( ftell(virtual_disk_sos->storage) <= pos){
        fprintf(stderr, "The disk storage is full.\n");
        return ERROR;
    }
    fseek(virtual_disk_sos->storage, (long)pos, SEEK_SET);
    int code = (int)fread(block->data, sizeof(uchar), BLOCK_SIZE, virtual_disk_sos->storage);
    if (code != BLOCK_SIZE){
        fprintf(stderr, "An error occurred while reading\n");
        return ERROR;
    }
    return SUCCESS;
}

void display_block(block_t block){
    fprintf(stdout, "[");
    for (int i = BLOCK_SIZE-1; i >= 0; i--) {
        fprintf(stdout, "%02x", block.data[i]);
    }
    fprintf(stdout, "]");
}

void display_disk_storage(){
    for (int i = 0; i < INODES_START; i+=BLOCK_SIZE) {
        block_t block;
        read_block(&block, i);
        display_block(block);
    }
    fprintf(stdout, "\n");
    int k = 0;
    for (int i = INODES_START; i < (INODES_START)+(INODE_SIZE)*(INODE_TABLE_SIZE)*(BLOCK_SIZE); i += BLOCK_SIZE) {
        if (k == 28){
            fprintf(stdout, "\n");
            k=0;
        }
        block_t block;
        read_block(&block, i);
        display_block(block);
        k++;
    }
    fprintf(stdout, "\n");
    k=0;
    for (int i = USERS_START; i < (NB_USERS)*(USER_SIZE)*(BLOCK_SIZE)+(USERS_START); i+=BLOCK_SIZE) {
        if (k == USER_SIZE){
            fprintf(stdout, "\n");
            k=0;
        }
        block_t block;
        read_block(&block, i);
        display_block(block);
        k++;

    }
    fprintf(stdout, "\n");
}

/*int main(){
    init_disk_sos("../Format");
    fseek(virtual_disk_sos->storage, 0, SEEK_END);
    int size = ftell(virtual_disk_sos->storage);
    block_t block;
    uint test;
    uint_to_block(244, &block);
    block_to_uint(block);
    for (int i = 0; i < compute_nblock(size); ++i) {
        write_block(block, i*BLOCK_SIZE);
    }
    display_disk_storage();
    shutdown_disk_sos();
    fprintf(stdout, "%d %d", sizeof(uint), sizeof(uchar));
}*/