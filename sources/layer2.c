/**
 * @file layer2.c
 * @author  HERZBERG Dwayne and BERLIN Florian
 * @brief Source code for layer2 of the ScratchOs : inodes
 * @version 0.1
 * @date 2022-02-14
 */

#include "../headers/layer2.h"

/**
 * @brief Function that writes the super_block into the disk storage
 * @return int, Success code or error code depending on whether successful or failure
 */
int write_super_block() {
    block_t data;
    for (int i = 0; i < 4; i++) {
        switch (i) {
        case 0:
            uint_to_block(virtual_disk_sos->super_block.number_of_files, &data);
            break;
        case 1:
            uint_to_block(virtual_disk_sos->super_block.number_of_users, &data);
            break;
        case 2:
            uint_to_block(virtual_disk_sos->super_block.nb_blocks_used, &data);
            break;
        case 3:
            uint_to_block(virtual_disk_sos->super_block.first_free_byte, &data);
            break;
        }
        if (write_block(data, i * BLOCK_SIZE) == ERROR)
            return ERROR;
    }
    return SUCCESS;
}

/**
 * @brief Function that read the super_block from the disk storage
 * @return int, Success code or error code depending on whether successful or failure
 */
int read_super_block() {
    block_t data;
    for (int i = 0; i < 4; i++) {
        if (read_block(&data, i * BLOCK_SIZE) == ERROR)
            return ERROR;
        switch (i) {
        case 0:
            virtual_disk_sos->super_block.number_of_files = block_to_uint(data);
            break;
        case 1:
            virtual_disk_sos->super_block.number_of_users = block_to_uint(data);
            break;
        case 2:
            virtual_disk_sos->super_block.nb_blocks_used = block_to_uint(data);
            break;
        case 3:
            virtual_disk_sos->super_block.first_free_byte = block_to_uint(data);
            break;
        }
    }
    return SUCCESS;
}

/**
 * @brief Function that updates the first_free_byte field of super_block
 */
void update_first_free_byte() {
    if (virtual_disk_sos->inodes[0].first_byte == 0) {
        virtual_disk_sos->super_block.first_free_byte = USERS_START + (USER_SIZE * NB_USERS * BLOCK_SIZE);
    } else {
        uint maxfirstbyte = USERS_START + (USER_SIZE * NB_USERS * BLOCK_SIZE);
        int imax = 0;
        for (int i = 0; i < get_unused_inode(); i++) {
            if (virtual_disk_sos->inodes[i].first_byte > maxfirstbyte)
                maxfirstbyte = virtual_disk_sos->inodes[i].first_byte;
            imax = i;
        }
        virtual_disk_sos->super_block.first_free_byte = virtual_disk_sos->inodes[imax].first_byte + virtual_disk_sos->inodes[imax].nblock * BLOCK_SIZE;
    }
    while (virtual_disk_sos->super_block.first_free_byte % BLOCK_SIZE != 0)
        virtual_disk_sos->super_block.first_free_byte++;
}

/**
 * @brief Write data in blok for write_inodes_table
 *
 * @param pos
 * @param source
 * @return int, Success code or error code depending on whether successful or failure
 */
int read_param_inode(int *pos, uint *source) {
    block_t block;
    if (read_block(&block, *pos) == ERROR)
        return ERROR;
    (*source) = block_to_uint(block);
    (*pos) += BLOCK_SIZE;
    return SUCCESS;
}

/**
 * @brief Function that read inode table from the disk storage
 * @return int, Success code or error code depending on whether successful or failure
 */
int read_inodes_table() {
    int pos = INODES_START;
    for (int i = 0; i < INODE_TABLE_SIZE; ++i) {
        if (read_text_block_char(&pos, FILENAME_MAX_SIZE, virtual_disk_sos->inodes[i].filename) == ERROR)
            return ERROR;
        if (read_param_inode(&pos, &virtual_disk_sos->inodes[i].size) == ERROR)
            return ERROR;
        if (read_param_inode(&pos, &virtual_disk_sos->inodes[i].uid) == ERROR)
            return ERROR;
        if (read_param_inode(&pos, &virtual_disk_sos->inodes[i].uright) == ERROR)
            return ERROR;
        if (read_param_inode(&pos, &virtual_disk_sos->inodes[i].oright) == ERROR)
            return ERROR;
        if (read_text_block_char(&pos, TIMESTAMP_SIZE, virtual_disk_sos->inodes[i].ctimestamp) == ERROR)
            return ERROR;
        if (read_text_block_char(&pos, TIMESTAMP_SIZE, virtual_disk_sos->inodes[i].mtimestamp) == ERROR)
            return ERROR;
        if (read_param_inode(&pos, &virtual_disk_sos->inodes[i].nblock) == ERROR)
            return ERROR;
        if (read_param_inode(&pos, &virtual_disk_sos->inodes[i].first_byte) == ERROR)
            return ERROR;
    }
    return SUCCESS;
}

/**
 * @brief Write data in blok for write_inodes_table
 *
 * @param pos
 * @param source
 * @return int, Success code or error code depending on whether successful or failure
 */
int write_param_inode(int *pos, uint source) {
    block_t block;
    uint_to_block(source, &block);
    if (write_block(block, *pos) == ERROR)
        return ERROR;
    (*pos) += BLOCK_SIZE;
    return SUCCESS;
}

/**
 * @brief Function that write inode table to the disk storage
 * @return int, Success code or error code depending on whether successful or failure
 */
int write_inodes_table() {
    int pos = INODES_START;
    for (int i = 0; i < INODE_TABLE_SIZE; ++i) {
        inode_t inode = virtual_disk_sos->inodes[i];
        if (write_text_block_char(&pos, FILENAME_MAX_SIZE, inode.filename) == ERROR)
            return ERROR;
        if (write_param_inode(&pos, inode.size) == ERROR)
            return ERROR;
        if (write_param_inode(&pos, inode.uid) == ERROR)
            return ERROR;
        if (write_param_inode(&pos, inode.uright) == ERROR)
            return ERROR;
        if (write_param_inode(&pos, inode.oright) == ERROR)
            return ERROR;
        if (write_text_block_char(&pos, TIMESTAMP_SIZE, inode.ctimestamp) == ERROR)
            return ERROR;
        if (write_text_block_char(&pos, TIMESTAMP_SIZE, inode.mtimestamp) == ERROR)
            return ERROR;
        if (write_param_inode(&pos, inode.nblock) == ERROR)
            return ERROR;
        if (write_param_inode(&pos, inode.first_byte) == ERROR)
            return ERROR;
    }
    return SUCCESS;
}

/**
 * @brief Function that delete an inode using index of the inode
 *
 * @param index_inode
 * @return int, Success code or error code depending on whether successful or failure
 */
int delete_inode(int index_inode) {
    if (index_inode >= INODE_TABLE_SIZE || index_inode < 0) {
        fprintf(stderr, "%s\n", LangGet(ERROR_INODE_INDEX));
        return ERROR;
    }
    virtual_disk_sos->super_block.nb_blocks_used -= virtual_disk_sos->inodes[index_inode].nblock;
    for (int i = index_inode; i < get_unused_inode() - 1; ++i)
        virtual_disk_sos->inodes[i] = virtual_disk_sos->inodes[i + 1];
    virtual_disk_sos->inodes[get_unused_inode() - 1].first_byte = 0;
    virtual_disk_sos->super_block.number_of_files--;
    update_first_free_byte();
    return SUCCESS;
}

/**
 * @brief Get the index of unused inode
 *
 * @return int, 0 <= x <= 10
 */
int get_unused_inode() {
    return virtual_disk_sos->super_block.number_of_files;
}

/**
 * @brief Initialize an inode
 *
 * @param fileName
 * @param size
 * @param pos
 * @param createTime
 * @param modifyTime
 * @return int, Success code or error code depending on whether successful or failure
 */
int init_inode(const char *fileName, uint size, uint pos, char *createTime, char *modifyTime, session_t user) {
    int index_inode = get_unused_inode();
    if (index_inode == INODE_TABLE_SIZE || virtual_disk_sos->super_block.number_of_files >= 10) {
        // fprintf(stderr, "%s\n", LangGet(ERROR_INODE_TABLE_FULL));
        terminal_print(LangGet(ERROR_INODE_TABLE_FULL), TERMINAL_RED);
        return ERROR;
    }
    int i = 0;
    for (; fileName[i] != '\0' && i < FILENAME_MAX_SIZE; ++i)
        virtual_disk_sos->inodes[index_inode].filename[i] = fileName[i];
    for (; i < FILENAME_MAX_SIZE; ++i)
        virtual_disk_sos->inodes[index_inode].filename[i] = '\0';
    virtual_disk_sos->inodes[index_inode].size = size;
    virtual_disk_sos->inodes[index_inode].nblock = compute_nblock(size);
    virtual_disk_sos->inodes[index_inode].first_byte = pos;
    virtual_disk_sos->inodes[index_inode].uid = user.userid;
    virtual_disk_sos->inodes[index_inode].uright = RW;
    virtual_disk_sos->inodes[index_inode].oright = rw;
    strcpy(virtual_disk_sos->inodes[index_inode].ctimestamp, createTime);
    strcpy(virtual_disk_sos->inodes[index_inode].mtimestamp, modifyTime);
    virtual_disk_sos->super_block.nb_blocks_used += compute_nblock(size);
    virtual_disk_sos->super_block.number_of_files++;
    update_first_free_byte();
    return SUCCESS;
}

/**
 * @brief Checks if the user has sufficient rights in the inode
 *
 * @param i_inode
 * @param user_id
 * @param right
 * @return true
 * @return false
 */
bool has_rights(int i_inode, int user_id, int right) {
    if (virtual_disk_sos->inodes[i_inode].first_byte == 0) {
        fprintf(stderr, "%s\n", LangGet(ERROR_INODE_INIT));
        return false;
    }
    if (user_id == ROOT_UID)
        return true;
    if (virtual_disk_sos->inodes[i_inode].uid == user_id) {
        if (virtual_disk_sos->inodes[i_inode].uright == RW || virtual_disk_sos->inodes[i_inode].uright == right)
            return true;
        return false;
    }
    if (virtual_disk_sos->inodes[i_inode].oright == RW || virtual_disk_sos->inodes[i_inode].oright == right)
        return true;
    return false;
}
