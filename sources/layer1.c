/**
 * @file layer1.c
 * @author  HERZBERG Dwayne and BERLIN Florian
 * @brief Source code for layer1 of the ScratchOs : Blocks
 * @version 0.1
 * @date 2022-02-14
 */

#include "../headers/layer1.h"

/**
 * @brief Converter unsigned int to block
 * @param number
 * @param block
 */
void uint_to_block(uint number, block_t *block) {
    for (int i = 0; i < sizeof(uint); ++i)
        block->data[i] = ((uchar *)&number)[i];
}

/**
 * @brief Converter block to unsigned int
 * @param block
 * @return uint
 */
uint block_to_uint(block_t block) {
    uint number;
    for (int i = 0; i < sizeof(uint); ++i)
        ((uchar *)&number)[i] = block.data[i];
    return number;
}

/**
 * @brief Initialize a virtual disk on directory
 * @param directory
 * @return int, success code or error code depending on whether success or failure
 */
int init_disk_sos(char *directory, int kid) {
    virtual_disk_sos = malloc(sizeof(virtual_disk_t));
    if (virtual_disk_sos == NULL) {
        fprintf(stderr, "%s\n", LangGet(ERROR_MALLOC));
        return ERROR;
    }

    char *diskFile = (char *)malloc(sizeof(char) * (strlen(directory) + 4));
    if (diskFile == NULL) {
        fprintf(stderr, "%s\n", LangGet(ERROR_MALLOC));
        return ERROR;
    }

    snprintf(diskFile, strlen(directory) + 5, "%s/d%d", directory, kid);

    virtual_disk_sos->storage = fopen(diskFile, "r+w");
    if (virtual_disk_sos->storage == NULL) {
        fprintf(stderr, "%s\n", LangGet(ERROR_ACCESS_DISKFILE));
        return ERROR;
    }

    if (read_super_block() == ERROR) {
        fprintf(stderr, "%s\n", LangGet(ERROR_READ_SUPERBLOCK));
        return ERROR;
    }
    if (read_inodes_table() == ERROR) {
        fprintf(stderr, "%s\n", LangGet(ERROR_READ_INODES_TABLE));
        return ERROR;
    }
    if (read_users_table() == ERROR) {
        fprintf(stderr, "%s\n", LangGet(ERROR_READ_USERS_TABLE));
        return ERROR;
    }

    free(diskFile);
    return SUCCESS;
}

/**
 * @brief Close disk storage and save tables
 * @return int, success code or error code depending on whether success or failure
 */
int shutdown_disk_sos() {
    if (write_super_block() == ERROR) {
        fprintf(stderr, "%s\n", LangGet(ERROR_WRITE_SUPERBLOCK));
        return ERROR;
    }
    if (write_inodes_table() == ERROR) {
        fprintf(stderr, "%s\n", LangGet(ERROR_WRITE_INODES_TABLE));
        return ERROR;
    }
    if (write_users_table() == ERROR) {
        fprintf(stderr, "%s\n", LangGet(ERROR_WRITE_USERS_TABLE));
        return ERROR;
    }
    if (fclose(virtual_disk_sos->storage) == EOF) {
        fprintf(stderr, "%s\n", LangGet(ERROR_FILE_CLOSE));
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
uint compute_nblock(uint octets) {
    return octets % 4 == 0 ? octets / 4 : octets / 4 + 1;
}

/**
 * @brief Positioning function for write and read block
 *
 * @param pos
 * @return int, error code or success code dpeending on what happened
 */
int rw_tool_block(int pos) {
    if (fseek(virtual_disk_sos->storage, 0, SEEK_END) != 0) {
        fprintf(stderr, "%s\n", LangGet(ERROR_FSEEK));
        return ERROR;
    }
    int currentPos = (int)ftell(virtual_disk_sos->storage);
    if (currentPos == -1) {
        fprintf(stderr, "%s\n", LangGet(ERROR_FTELL));
        return ERROR;
    }
    if (currentPos <= pos) {
        fprintf(stderr, "%s\n", LangGet(ERROR_DISK_FULL));
        return ERROR;
    }
    if (fseek(virtual_disk_sos->storage, (long)pos, SEEK_SET) != 0) {
        fprintf(stderr, "%s\n", LangGet(ERROR_FSEEK));
        return ERROR;
    }
    return SUCCESS;
}

/**
 * @brief Function that writes a block on the disk storage
 * @param block
 * @param pos
 * @return int, error code or success code dpeending on what happened
 */
int write_block(block_t block, int pos) {
    if (rw_tool_block(pos) == ERROR)
        return ERROR;
    int code = (int)fwrite(block.data, sizeof(uchar), BLOCK_SIZE, virtual_disk_sos->storage);
    if (code != BLOCK_SIZE) {
        fprintf(stderr, "%s\n", LangGet(ERROR_FWRITE));
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
int read_block(block_t *block, int pos) {
    if (rw_tool_block(pos) == ERROR)
        return ERROR;
    int code = (int)fread(block->data, sizeof(uchar), BLOCK_SIZE, virtual_disk_sos->storage);
    if (code != BLOCK_SIZE) {
        fprintf(stderr, "%s\n", LangGet(ERROR_READ));
        return ERROR;
    }
    return SUCCESS;
}

/**
 * @brief Print a block on the console
 * @param block
 */
void display_block(block_t block) {
    fprintf(stdout, "[");
    for (int i = BLOCK_SIZE - 1; i >= 0; i--)
        fprintf(stdout, "%02x", block.data[i]);
    fprintf(stdout, "]");
}

/**
 * @brief Write text (char) in block for write_inodes_table
 *
 * @param pos
 * @param size
 * @param source
 * @return int, Success code or error code depending on whether successful or failure
 */
int write_text_block_char(int *pos, int size, char *source) {
    block_t block;
    for (int j = 0; j < compute_nblock(size); j++) {
        for (int i = 0; i < BLOCK_SIZE; i++) { block.data[i] = '\0';}
        for (int i = 0; i < BLOCK_SIZE; i++) block.data[i] = (unsigned char)source[j*BLOCK_SIZE+i];
        if (write_block(block, *pos) == ERROR) return ERROR;
        (*pos)+=BLOCK_SIZE;
    }
    return SUCCESS;
}

/**
 * @brief Write text (uchar) in block for write_inodes_table
 *
 * @param pos
 * @param size
 * @param source
 * @return int, Success code or error code depending on whether successful or failure
 */
int write_text_block_uchar(uint *pos, int size, uchar *source) {
    block_t block;
    for (int j = 0; j < compute_nblock(size); j++) {
        for (int i = 0; i < 4; i++)
            block.data[i] = source[j * BLOCK_SIZE + i];
        if (write_block(block, (int)*pos) == ERROR)
            return ERROR;
        (*pos) += BLOCK_SIZE;
    }
    return SUCCESS;
}

/**
 * @brief read specific text (char)
 *
 * @param pos
 * @param size
 * @param destination
 * @return int, Success code or error code depending on whether successful or failure
 */
int read_text_block_char(int *pos, int size, char *destination) {
    block_t block;
    for (int j = 0; j < compute_nblock(size); j++) {
        if (read_block(&block, *pos) == ERROR)
            return ERROR;
        for (int i = 0; i < 4; i++)
            destination[j * BLOCK_SIZE + i] = (char)block.data[i];
        (*pos) += BLOCK_SIZE;
    }
    return SUCCESS;
}

/**
 * @brief read specific text (uchar)
 *
 * @param pos
 * @param size
 * @param destination
 * @return int, Success code or error code depending on whether successful or failure
 */
int read_text_block_uchar(uint *pos, int size, uchar *destination) {
    block_t block;
    for (int j = 0; j < compute_nblock(size); j++) {
        if (read_block(&block, (int)*pos) == ERROR)
            return ERROR;
        for (int i = 0; i < 4; i++)
            destination[j * BLOCK_SIZE + i] = block.data[i];
        (*pos) += BLOCK_SIZE;
    }
    return SUCCESS;
}

/**
 * @brief Print all disk blocks to console
 */
void display_disk_storage() {
    fprintf(stdout, "\n\n");
    block_t block;
    for (int i = 0; i < INODES_START; i += BLOCK_SIZE) {
        if (read_block(&block, i) == ERROR)
            return;
        display_block(block);
    }
    fprintf(stdout, "\n");
    for (int i = INODES_START; i < (INODES_START) + (INODE_SIZE) * (INODE_TABLE_SIZE) * (BLOCK_SIZE); i += BLOCK_SIZE) {
        if ((i - INODES_START) % 28 == 0)
            fprintf(stdout, "\n");
        if (read_block(&block, i) == ERROR)
            return;
        display_block(block);
    }
    fprintf(stdout, "\n");
    for (int i = USERS_START; i < (NB_USERS) * (USER_SIZE) * (BLOCK_SIZE) + (USERS_START); i += BLOCK_SIZE) {
        if ((i - USERS_START) % USER_SIZE == 0)
            fprintf(stdout, "\n");
        read_block(&block, i);
        display_block(block);
    }
    fprintf(stdout, "\n\n\n");
}
