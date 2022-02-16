//
// Created by Florian Berlin on 14/02/2022.
//

#include "../headers/layer1.h"


/**
 * @brief Converter unsigned int to block
 * @param number 
 * @param block 
 */
void uint_to_block(uint number, block_t *block){
    for (int i = 0; i < sizeof(uint); ++i) {
        block->data[i] = ((uchar *)&number)[i];
    }
}

/**
 * @brief Converter block to unsigned int
 * @param block 
 * @return uint 
 */
uint block_to_uint(block_t block){
    uint number;
    for (int i = 0; i < sizeof(uint); ++i) {
        ((uchar *)&number)[i] = block.data[i];
    }
    return number;
}

/**
 * @brief Initialize a virtual disk on directory
 * @param directory 
 * @return int, success code or error code depending on whether success or failure 
 */
int init_disk_sos(char *directory){
    fprintf(stdout, "step 1 !\n");

    virtual_disk_sos = malloc(sizeof(virtual_disk_t));
    if (virtual_disk_sos == NULL){
        perror("virtual_disk_sos");
        return ERROR;
    }
    fprintf(stdout, "step 2 !\n");


    char *diskFile = malloc(sizeof(char)*(strlen(directory)+4));
    if (diskFile == NULL){
        perror("diskFile");
        return ERROR;
    }
    fprintf(stdout, "step 3 !\n");

    strcpy(diskFile, directory);
    strcat(diskFile, "/d0");

    virtual_disk_sos->storage=fopen(diskFile, "r+w");
    if (virtual_disk_sos->storage==NULL){
        perror("Could not open diskFile");
        return ERROR;
    }
    fprintf(stdout, "step 4 !\n");

    if (read_super_block() == ERROR) return ERROR;
    fprintf(stdout, "step 5 !\n");
    if (read_inodes_table() == ERROR) return ERROR;
    fprintf(stdout, "step 6 !\n");
    if (read_users_table() == ERROR) return ERROR;
    fprintf(stdout, "step 7 !\n");
    free(diskFile);

    user.userid = 0;
    fprintf(stdout, "step 8 !\n");

    return SUCCESS;
}

/**
 * @brief Close disk storage and save tables
 * @return int, success code or error code depending on whether success or failure 
 */
int shutdown_disk_sos(){
    write_super_block();
    write_inodes_table();
    write_users_table();
    if (fclose(virtual_disk_sos->storage) == EOF) {
        fprintf(stderr, "Cannot close file\n" );
        return ERROR;
    }
    free(virtual_disk_sos);
    return SUCCESS;
}

/**
 * @brief Function that calculates the number of blocks needed for n octets
 * @param octets
 * @return uint, nb of blocks needed for n octets
 */
uint compute_nblock(uint octets){
    return octets%4 == 0 ? octets/4 : octets/4+1;
}

/**
 * @brief Function that writes a block on the disk storage
 * @param block
 * @param pos
 * @return int, error code or success code dpeending on what happened
 */
int write_block(block_t block, int pos){
    if (fseek(virtual_disk_sos->storage, 0, SEEK_END) != 0) {
        fprintf(stderr, "Changement de position impossible\n");
        return ERROR;
    }

    int currentPos = ftell(virtual_disk_sos->storage);
    if (currentPos == -1) {
        fprintf(stderr, "Cannot get the file position\n");
        return ERROR;
    }
    if( currentPos <= pos){
        fprintf(stderr, "The disk storage is full.\n");
        return ERROR;
    }

    if (fseek(virtual_disk_sos->storage, (long)pos, SEEK_SET) != 0) {
        fprintf(stderr, "Changement de position impossible\n");
        return ERROR;
    }

    int code = (int)fwrite(block.data, sizeof(uchar), BLOCK_SIZE, virtual_disk_sos->storage);
    if (code != BLOCK_SIZE){
        fprintf(stderr, "An error occurred while writing block\n");
        return ERROR;
    }
    return SUCCESS;
}

/**
 * @brief Function that read a block of data from the disk storage
 * @param block
 * @param pos
 * @return int, error code or success code dpeending on what happened
 */
int read_block(block_t *block, int pos){
    if (fseek(virtual_disk_sos->storage, 0, SEEK_END) != 0) {
        fprintf(stderr, "Changement de position impossible\n");
        return ERROR;
    }

    int currentPos = ftell(virtual_disk_sos->storage);
    if (currentPos == -1) {
        fprintf(stderr, "Cannot get the file position\n");
        return ERROR;
    }
    if( currentPos <= pos){
        fprintf(stderr, "The disk storage is full.\n");
        return ERROR;
    }

    if (fseek(virtual_disk_sos->storage, (long)pos, SEEK_SET) != 0) {
        fprintf(stderr, "Changement de position impossible\n");
        return ERROR;
    }

    int code = (int)fread(block->data, sizeof(uchar), BLOCK_SIZE, virtual_disk_sos->storage);
    if (code != BLOCK_SIZE){
        fprintf(stderr, "An error occurred while reading\n");
        return ERROR;
    }
    return SUCCESS;
}

/**
 * @brief Print a block on the console
 * @param block 
 */
void display_block(block_t block){
    fprintf(stdout, "[");
    for (int i = BLOCK_SIZE-1; i >= 0; i--) {
        fprintf(stdout, "%02x", block.data[i]);
    }
    fprintf(stdout, "]");
}

/**
 * @brief Print all disk blocks to console
 */
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
