/**
* \file layer4.c
 * \brief Source code for layer4 of the ScratchOs: File System
 * \author HERZBERG Dwayne and BERLIN Florian
 * \version 0.1
 * \date 13 February 2022
*/
#include "../headers/layer1.h"
#include "../headers/layer2.h"
#include "../headers/layer3.h"
#include "../headers/layer4.h"



/**
 * @brief Check if file exists.
 * @param filename
 * @return int, index of the file
 */
int is_file_in_inode(char *filename){
    for (int i = 0; i < virtual_disk_sos->super_block.number_of_files; ++i) {
        if (strcmp(virtual_disk_sos->inodes[i].filename, (char *)filename) == 0) return i;
    }
    return INODE_TABLE_SIZE;
}

/**
 * \brief Create or edit a file in the disk storage and in the inodes table
 * @param filename
 * @param filedata
 */
void write_file(char * filename, file_t filedata){
    int i_inode = is_file_in_inode(filename);
    if (i_inode != INODE_TABLE_SIZE){
        if (filedata.size <= virtual_disk_sos->inodes[i_inode].size){
            strcpy(virtual_disk_sos->inodes[i_inode].mtimestamp, timestamp());
            int pos = (int)virtual_disk_sos->inodes[i_inode].first_byte;
            block_t block;
            for (int i = 0; i < compute_nblock(filedata.size); i++) {
                block.data[0] = filedata.data[i*BLOCK_SIZE+0];
                block.data[1] = filedata.data[i*BLOCK_SIZE+1];
                block.data[2] = filedata.data[i*BLOCK_SIZE+2];
                block.data[3] = filedata.data[i*BLOCK_SIZE+3];
                write_block(block, pos);
                pos+=BLOCK_SIZE;
            }
        }
        else{
            char *ctimestamp = malloc(sizeof(char)*TIMESTAMP_SIZE);
            strcpy(ctimestamp, virtual_disk_sos->inodes[i_inode].ctimestamp);
            delete_inode(i_inode);
            init_inode(filename, filedata.size, virtual_disk_sos->super_block.first_free_byte, ctimestamp, timestamp());
            int pos = (int)virtual_disk_sos->inodes[is_file_in_inode(filename)].first_byte;
            block_t block;
            for (int i = 0; i < compute_nblock(filedata.size); i++) {
                block.data[0] = filedata.data[i*BLOCK_SIZE+0];
                block.data[1] = filedata.data[i*BLOCK_SIZE+1];
                block.data[2] = filedata.data[i*BLOCK_SIZE+2];
                block.data[3] = filedata.data[i*BLOCK_SIZE+3];
                write_block(block, pos);
                pos+=BLOCK_SIZE;
            }
            free(ctimestamp);
        }
    }
    else{
        init_inode(filename, filedata.size, virtual_disk_sos->super_block.first_free_byte, timestamp(), timestamp());
        block_t block;
        int pos = (int)virtual_disk_sos->inodes[is_file_in_inode(filename)].first_byte;
        for (int i = 0; i < compute_nblock(filedata.size); i++) {
            block.data[0] = filedata.data[i*BLOCK_SIZE+0];
            block.data[1] = filedata.data[i*BLOCK_SIZE+1];
            block.data[2] = filedata.data[i*BLOCK_SIZE+2];
            block.data[3] = filedata.data[i*BLOCK_SIZE+3];
            write_block(block, pos);
            pos+=BLOCK_SIZE;
        }
    }
}


/**
 * \brief Read a file from the disk storage
 * @param filename
 * @param filedata
 * @return
 */
int read_file(char *filename, file_t *filedata) {
    int index_inode = is_file_in_inode(filename);
    if (index_inode == INODE_TABLE_SIZE) return 0;
    filedata->size = virtual_disk_sos->inodes[index_inode].size;
    int pos = (int)virtual_disk_sos->inodes[index_inode].first_byte;
    block_t block;
    for (int j = 0; j < compute_nblock(filedata->size); j++) {
        read_block(&block, pos);
        filedata->data[j*BLOCK_SIZE+0] = block.data[0];
        filedata->data[j*BLOCK_SIZE+1] = block.data[1];
        filedata->data[j*BLOCK_SIZE+2] = block.data[2];
        filedata->data[j*BLOCK_SIZE+3] = block.data[3];
        pos+=BLOCK_SIZE;
    }
    //fprintf("Find : %s\n", filedata->data);
    return 1;
}

/**
 * \brief Delete a file from the inode table
 * @param filename
 * @return
 */
int delete_file(char *filename){
    int index_inode = is_file_in_inode(filename);
    if (index_inode == INODE_TABLE_SIZE) return 0;
    delete_inode(index_inode);
    return 1;
}

/**
 * \brief Write file from host to virtual disk
 * @param filename
 * @return
 */
int load_file_from_host(char *filename){
    FILE * hostfile = fopen(filename, "r");
    if (hostfile == NULL){
        perror("fopen");
        return 0;
    }
    file_t sosfile;
    fseek(hostfile, 0, SEEK_END);
    sosfile.size = ftell(hostfile);
    fseek(hostfile, 0, SEEK_SET);
    fread(sosfile.data, sizeof(uchar), sosfile.size, hostfile);
    fprintf(stdout, "Red : %s\n", sosfile.data);
    write_file(filename, sosfile);
    return 1;
}


int store_file_to_host(char *filenamesos){
    int index_inode = is_file_in_inode(filenamesos);
    if (index_inode == INODE_TABLE_SIZE) return 0; // TODO: Error handling
    file_t file;
    read_file(filenamesos, &file);
    FILE * fd;
    fd = fopen(filenamesos, "w");
    if (fd == NULL) return 0;
    fseek(fd, 0, SEEK_SET);
    fwrite(file.data, sizeof(uchar), file.size, fd);
    return 1;
}


void cmd_dump_file(char *directory){
    init_disk_sos(directory);
    update_first_free_byte();
    printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);
    init_user("root", "bonjour");
    for (int i = 0; i < INODE_TABLE_SIZE; ++i) {
        init_inode("bonjour.txt", 10, virtual_disk_sos->super_block.first_free_byte, timestamp(), timestamp());
    }
    delete_inode(4);
    for (int i = 1; i < NB_USERS; ++i) {
        init_user("Michel", "bonjour");
    }
    printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);
    load_file_from_host("coucou");
    load_file_from_host("coucou");
    store_file_to_host("coucou");
    delete_file("coucou");
    shutdown_disk_sos();
    init_disk_sos(directory);
    display_disk_storage();

    for (int i = 0; i < virtual_disk_sos->super_block.number_of_users; ++i) {
        fprintf(stdout, "userid %d: %s\n", i,virtual_disk_sos->users_table[i].login);
        fprintf(stdout, "pass:%s\n", virtual_disk_sos->users_table[i].passwd);
    }
    printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);

    shutdown_disk_sos();
}
