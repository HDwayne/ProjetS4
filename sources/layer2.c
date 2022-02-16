//
// Created by Florian Berlin on 14/02/2022.
//

#include "../headers/layer2.h"

/**
 * @brief Function that writes the super_block into the disk storage
 * @return int, Success code or error code depending on whether successful or failure
 */
int write_super_block(){
    super_block_t superblock = virtual_disk_sos->super_block;
    block_t data;

    uint_to_block(virtual_disk_sos->super_block.number_of_files, &data);
    if (write_block(data, 0) == ERROR) return ERROR;
    uint_to_block(superblock.number_of_users, &data);
    if (write_block(data, BLOCK_SIZE) == ERROR) return ERROR;
    uint_to_block(superblock.nb_blocks_used, &data);
    if ( write_block(data, 2*BLOCK_SIZE) == ERROR) return ERROR;
    uint_to_block(superblock.first_free_byte, &data);
    if (write_block(data, 3*BLOCK_SIZE) == ERROR) return ERROR;
    return SUCCESS;
}

/**
 * @brief Function that read the super_block from the disk storage
 * @return int, Success code or error code depending on whether successful or failure
 */
int read_super_block(){
    block_t data;
    if (read_block(&data, 0) == ERROR) return ERROR;
    virtual_disk_sos->super_block.number_of_files = block_to_uint(data);
    if (read_block(&data, BLOCK_SIZE) == ERROR) return ERROR;
    virtual_disk_sos->super_block.number_of_users = block_to_uint(data);
    if (read_block(&data, 2*BLOCK_SIZE) == ERROR) return ERROR;
    virtual_disk_sos->super_block.nb_blocks_used = block_to_uint(data);
    if (read_block(&data, 3*BLOCK_SIZE) == ERROR) return ERROR;
    virtual_disk_sos->super_block.first_free_byte = block_to_uint(data);
    return SUCCESS;
}

/**
 * @brief Function that updates the first_free_byte field of super_block
 */
void update_first_free_byte(){
    if (virtual_disk_sos->inodes[0].first_byte == 0){
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
    while (virtual_disk_sos->super_block.first_free_byte%4!= 0) virtual_disk_sos->super_block.first_free_byte ++;
}

/**
 * @brief Function that read inode table from the disk storage
 * @return int, Success code or error code depending on whether successful or failure
 */
int read_inodes_table(){
    int pos = INODES_START;
    for (int i = 0; i < INODE_TABLE_SIZE; ++i) {
        block_t block;
        for (int j = 0; j < compute_nblock(FILENAME_MAX_SIZE); ++j) {
            if (read_block(&block, pos) == ERROR) return ERROR;
            virtual_disk_sos->inodes[i].filename[j*BLOCK_SIZE+0] = (char)block.data[0];
            virtual_disk_sos->inodes[i].filename[j*BLOCK_SIZE+1] = (char)block.data[1];
            virtual_disk_sos->inodes[i].filename[j*BLOCK_SIZE+2] = (char)block.data[2];
            virtual_disk_sos->inodes[i].filename[j*BLOCK_SIZE+3] = (char)block.data[3];
            pos+=BLOCK_SIZE;
        }
        if (read_block(&block, pos) == ERROR) return ERROR;
        virtual_disk_sos->inodes[i].size = block_to_uint(block);
        pos+=BLOCK_SIZE;
        if (read_block(&block, pos) == ERROR) return ERROR;
        virtual_disk_sos->inodes[i].uid = block_to_uint(block);
        pos+=BLOCK_SIZE;
        if (read_block(&block, pos) == ERROR) return ERROR;
        virtual_disk_sos->inodes[i].uright = block_to_uint(block);
        pos+=BLOCK_SIZE;
        if (read_block(&block, pos) == ERROR) return ERROR;
        virtual_disk_sos->inodes[i].oright = block_to_uint(block);
        pos+=BLOCK_SIZE;
        for (int j = 0; j < compute_nblock(TIMESTAMP_SIZE); ++j) {
            if (read_block(&block, pos) == ERROR) return ERROR;
            virtual_disk_sos->inodes[i].ctimestamp[j*BLOCK_SIZE+0] = (char)block.data[0];
            virtual_disk_sos->inodes[i].ctimestamp[j*BLOCK_SIZE+1] = (char)block.data[1];
            virtual_disk_sos->inodes[i].ctimestamp[j*BLOCK_SIZE+2] = (char)block.data[2];
            virtual_disk_sos->inodes[i].ctimestamp[j*BLOCK_SIZE+3] = (char)block.data[3];
            pos+=BLOCK_SIZE;
        }
        for (int j = 0; j < compute_nblock(TIMESTAMP_SIZE); ++j) {
            if (read_block(&block, pos) == ERROR) return ERROR;
            virtual_disk_sos->inodes[i].ctimestamp[j*BLOCK_SIZE+0] = (char)block.data[0];
            virtual_disk_sos->inodes[i].ctimestamp[j*BLOCK_SIZE+1] = (char)block.data[1];
            virtual_disk_sos->inodes[i].ctimestamp[j*BLOCK_SIZE+2] = (char)block.data[2];
            virtual_disk_sos->inodes[i].ctimestamp[j*BLOCK_SIZE+3] = (char)block.data[3];
            pos+=BLOCK_SIZE;
        }
        if (read_block(&block, pos) == ERROR) return ERROR;
        virtual_disk_sos->inodes[i].nblock = block_to_uint(block);
        pos+=BLOCK_SIZE;
        if (read_block(&block, pos) == ERROR) return ERROR;
        virtual_disk_sos->inodes[i].first_byte = block_to_uint(block);
        pos+=BLOCK_SIZE;
    }
    return SUCCESS;
}

/**
 * @brief Function that write inode table to the disk storage
 * @return int, Success code or error code depending on whether successful or failure
 */
int write_inodes_table(){
    int pos = INODES_START;
    for (int i = 0; i < INODE_TABLE_SIZE; ++i) {
        inode_t inode = virtual_disk_sos->inodes[i];
        block_t block;
        for (int j = 0; j < compute_nblock(FILENAME_MAX_SIZE); ++j) {
            block.data[0] = (unsigned char)inode.filename[0 + j*BLOCK_SIZE];
            block.data[1] = (unsigned char)inode.filename[1 + j*BLOCK_SIZE];
            block.data[2] = (unsigned char)inode.filename[2 + j*BLOCK_SIZE];
            block.data[3] = (unsigned char)inode.filename[3 + j*BLOCK_SIZE];
            if (write_block(block, pos) == ERROR) return ERROR;
            pos+=BLOCK_SIZE;
        }
        uint_to_block(inode.size, &block);
        if (write_block(block, pos) == ERROR) return ERROR;
        pos+=BLOCK_SIZE;
        uint_to_block(inode.uid, &block);
        if (write_block(block, pos) == ERROR) return ERROR;
        pos+=BLOCK_SIZE;
        uint_to_block(inode.uright, &block);
        if (write_block(block, pos) == ERROR) return ERROR;
        pos+=BLOCK_SIZE;
        uint_to_block(inode.oright, &block);
        if (write_block(block, pos) == ERROR) return ERROR;
        pos+=BLOCK_SIZE;
        for (int j = 0; j < compute_nblock(TIMESTAMP_SIZE); ++j) {
            block.data[0] = (unsigned char)inode.ctimestamp[0 + j*BLOCK_SIZE];
            block.data[1] = (unsigned char)inode.ctimestamp[1 + j*BLOCK_SIZE];
            block.data[2] = (unsigned char)inode.ctimestamp[2 + j*BLOCK_SIZE];
            block.data[3] = (unsigned char)inode.ctimestamp[3 + j*BLOCK_SIZE];
           if (write_block(block, pos) == ERROR) return ERROR;
            pos+=BLOCK_SIZE;
        }
        for (int j = 0; j < compute_nblock(TIMESTAMP_SIZE); ++j) {
            block.data[0] = (unsigned char)inode.mtimestamp[0 + j*BLOCK_SIZE];
            block.data[1] = (unsigned char)inode.mtimestamp[1 + j*BLOCK_SIZE];
            block.data[2] = (unsigned char)inode.mtimestamp[2 + j*BLOCK_SIZE];
            block.data[3] = (unsigned char)inode.mtimestamp[3 + j*BLOCK_SIZE];
            if (write_block(block, pos) == ERROR) return ERROR;
            pos+=BLOCK_SIZE;
        }
        uint_to_block(inode.nblock, &block);
        if (write_block(block, pos) == ERROR) return ERROR;
        pos+=BLOCK_SIZE;
        uint_to_block(inode.first_byte, &block);
        if (write_block(block, pos) == ERROR) return ERROR;
        pos+=BLOCK_SIZE;
    }
    return SUCCESS;
}

/**
 * @brief Function that delete an inode using index of the inode
 * 
 * @param index_inode 
 * @return int, Success code or error code depending on whether successful or failure
 */
int delete_inode(int index_inode){
    if (index_inode >= INODE_TABLE_SIZE || index_inode < 0){
        fprintf(stderr, "Incorrect index of inode\n");
        return ERROR;
    }
    virtual_disk_sos->super_block.nb_blocks_used-=virtual_disk_sos->inodes[index_inode].nblock;
    for (int i = index_inode; i < get_unused_inode()-1; ++i) {
        virtual_disk_sos->inodes[i] = virtual_disk_sos->inodes[i+1];
    }
    virtual_disk_sos->inodes[get_unused_inode()-1].first_byte=0;
    virtual_disk_sos->super_block.number_of_files--;
    update_first_free_byte();
    return SUCCESS;
}

/**
 * @brief Get the index of unused inode
 * 
 * @return int, 0 <= x <= 10
 */
int get_unused_inode(){
    for (int i = 0; i < INODE_TABLE_SIZE; i++) {
        if (virtual_disk_sos->inodes[i].first_byte == 0) return i;
    }
    return INODE_TABLE_SIZE;
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
int init_inode(const char *fileName, uint size, uint pos, char *createTime, char *modifyTime){
    int index_inode = get_unused_inode();
    if (index_inode == INODE_TABLE_SIZE || virtual_disk_sos->super_block.number_of_files >= 10 ){
        fprintf(stderr, "The inode table is full.\n");
        return ERROR;
    }
    for (int i = 0; i < FILENAME_MAX_SIZE; ++i) {
        virtual_disk_sos->inodes[index_inode].filename[i] = fileName[i];
    }
    virtual_disk_sos->inodes[index_inode].size = size;
    virtual_disk_sos->inodes[index_inode].nblock = compute_nblock(size);
    virtual_disk_sos->inodes[index_inode].first_byte = pos;

    //TO DO ON LAYER 5
    virtual_disk_sos->inodes[index_inode].uid = user->userid;
    virtual_disk_sos->inodes[index_inode].uright = RW;
    virtual_disk_sos->inodes[index_inode].oright = rw;

    for (int i = 0; i < TIMESTAMP_SIZE; ++i) {
        virtual_disk_sos->inodes[index_inode].ctimestamp[i] = createTime[i];
    }
    for (int i = 0; i < TIMESTAMP_SIZE; ++i) {
        virtual_disk_sos->inodes[index_inode].mtimestamp[i] = modifyTime[i];
    }
    virtual_disk_sos->super_block.nb_blocks_used+=compute_nblock(size);
    virtual_disk_sos->super_block.number_of_files++;
    update_first_free_byte();
    return SUCCESS;
}

/**
 * @brief @BerlinFlorian EXPLIQUE MOI STP
 * 
 * @param directory 
 * @return int, Success code or error code depending on whether successful or failure 
 */
int cmd_dump_inode(char *directory){
    if (init_disk_sos(directory) == ERROR) return ERROR;
    for (int i = 0; i < INODE_TABLE_SIZE; ++i) {
        if (init_inode("\0", 0, 0, timestamp(), timestamp()) == ERROR) return ERROR;
        if (delete_inode(0) == ERROR) return ERROR;
    }
    update_first_free_byte();
    printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);

    for (int i = 0; i < INODE_TABLE_SIZE; ++i) {
        if (init_inode("bonjour.txt", 10, virtual_disk_sos->super_block.first_free_byte, timestamp(), timestamp()) == ERROR) return ERROR;
    }
    if (delete_inode(4) == ERROR) return ERROR;
    for (int i = 0; i < get_unused_inode(); ++i) {
        fprintf(stdout, "File n° %d : %s \n",i, virtual_disk_sos->inodes[i].filename);
        fprintf(stdout, "size:  %d\n", virtual_disk_sos->inodes[i].size);
        fprintf(stdout, "Blocks  : %d \n",virtual_disk_sos->inodes[i].nblock);
        fprintf(stdout, "Pos  : %d \n",virtual_disk_sos->inodes[i].first_byte);
    }
    printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);
    if (shutdown_disk_sos() == ERROR) return ERROR;
    if (init_disk_sos(directory) == ERROR) return ERROR;
    display_disk_storage();
    printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);
    for (int i = 0; i < virtual_disk_sos->super_block.number_of_files; i++) {
        fprintf(stdout, "File n° %d : %s \n",i, virtual_disk_sos->inodes[i].filename);
        fprintf(stdout, "size:  %d\n", virtual_disk_sos->inodes[i].size);
        fprintf(stdout, "Blocks  : %d \n",virtual_disk_sos->inodes[i].nblock);
        fprintf(stdout, "Pos  : %d \n",virtual_disk_sos->inodes[i].first_byte);
    }
    if (shutdown_disk_sos() == ERROR) return ERROR;
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
bool has_rights(int i_inode, int user_id, int right){
    if (virtual_disk_sos->inodes[i_inode].first_byte == 0){
        fprintf(stderr, "Inode not initialized\n");
        return false;
    }
    if (user_id == 0) return true;
    if (virtual_disk_sos->inodes[i_inode].uid == user_id){
        if (virtual_disk_sos->inodes[i_inode].uright == RW || virtual_disk_sos->inodes[i_inode].uright == right) return true;
        return false;
    }
    if (virtual_disk_sos->inodes[i_inode].oright == RW || virtual_disk_sos->inodes[i_inode].oright == right) return true;
    return false;
}
