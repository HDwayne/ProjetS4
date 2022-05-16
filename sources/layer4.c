/**
 * @file layer4.c
 * @author  HERZBERG Dwayne and BERLIN Florian
 * @brief Source code for layer4 of the ScratchOs : File System
 * @version 0.1
 * @date 2022-02-14
 */

#include "../headers/layer4.h"

/**
 * @brief Check if file exists.
 * @param filename
 * @return int, index of the file
 */
int is_file_in_inode(char *filename) {
    for (int i = 0; i < virtual_disk_sos->super_block.number_of_files; ++i) {
        if (strcmp(virtual_disk_sos->inodes[i].filename, (char *)filename) == 0)
            return i;
    }
    return INODE_TABLE_SIZE;
}

/**
 * @brief Create or edit a file in the disk storage and in the inodes table
 *
 * @param filename
 * @param filedata
 * @param user
 * @return int, Success code or error code depending on whether successful or failure
 */
int write_file(char *filename, file_t filedata, session_t user) {
    int i_inode = is_file_in_inode(filename);
    if (i_inode != INODE_TABLE_SIZE) {
        if (filedata.size <= virtual_disk_sos->inodes[i_inode].size) {
            strcpy(virtual_disk_sos->inodes[i_inode].mtimestamp, timestamp());
            uint pos = virtual_disk_sos->inodes[i_inode].first_byte;
            if (write_text_block_uchar(&pos, filedata.size, filedata.data) == ERROR)
                return ERROR;
            virtual_disk_sos->inodes[i_inode].size = filedata.size;
           // update_first_free_byte();
        } else {
            char *ctimestamp = malloc(sizeof(char) * TIMESTAMP_SIZE);
            if (ctimestamp == NULL) {
                fprintf(stdout, "%s\n", LangGet(ERROR_MALLOC));
                return ERROR;
            }
            strcpy(ctimestamp, virtual_disk_sos->inodes[i_inode].ctimestamp);
            if (delete_inode(i_inode) == ERROR)
                return ERROR;
            if (init_inode(filename, filedata.size, virtual_disk_sos->super_block.first_free_byte, ctimestamp, timestamp(), user) == ERROR)
                return ERROR;
            uint pos = virtual_disk_sos->inodes[is_file_in_inode(filename)].first_byte;
            if (write_text_block_uchar(&pos, filedata.size, filedata.data) == ERROR)
                return ERROR;
            free(ctimestamp);
            update_first_free_byte();
        }
    } else {
        if (init_inode(filename, filedata.size, virtual_disk_sos->super_block.first_free_byte, timestamp(), timestamp(), user) == ERROR)
            return ERROR;
        uint pos = virtual_disk_sos->inodes[is_file_in_inode(filename)].first_byte;
        if (write_text_block_uchar(&pos, filedata.size, filedata.data) == ERROR)
            return ERROR;
        update_first_free_byte();
    }
    return SUCCESS;
}

/**
 * @brief Read a file from the disk storage
 * @param filename
 * @param filedata
 * @return int, Success code or error code depending on whether successful or failure
 */
int read_file(char *filename, file_t *filedata) {
    int index_inode = is_file_in_inode(filename);
    if (index_inode == INODE_TABLE_SIZE) {
        fprintf(stderr, "%s\n", LangGet(ERROR_INODE_INDEX));
        return ERROR;
    }
    filedata->size = virtual_disk_sos->inodes[index_inode].size;
    strcpy((char *)filedata->data, "");
    uint pos = virtual_disk_sos->inodes[index_inode].first_byte;
    if (read_text_block_uchar(&pos, filedata->size, filedata->data) == ERROR)
        return ERROR;
    return SUCCESS;
}

/**
 * @brief Delete a file from the inode table
 * @param filename
 * @return int, Success code or error code depending on whether successful or failure
 */
int delete_file(char *filename) {
    int index_inode = is_file_in_inode(filename);
    if (index_inode == INODE_TABLE_SIZE)
        return ERROR;
    if (delete_inode(index_inode) == ERROR)
        return ERROR;
    return SUCCESS;
}

/**
 * @brief Write file from host to virtual disk
 *
 * @param filename
 * @param user
 * @return int, Success code or error code depending on whether successful or failure
 */
int load_file_from_host(char *filename, session_t user) {
    FILE *hostfile = fopen(filename, "r");
    if (hostfile == NULL) {
        fprintf(stderr, "%s\n", LangGet(ERROR_FILE_OPEN));
        return ERROR;
    }
    file_t sosfile;
    if (fseek(hostfile, 0, SEEK_END) != 0) {
        fprintf(stderr, "%s\n", LangGet(ERROR_FSEEK));
        return ERROR;
    }
    sosfile.size = ftell(hostfile);
    if (sosfile.size == -1) {
        fprintf(stderr, "%s", LangGet(ERROR_FTELL));
        return ERROR;
    }
    if (fseek(hostfile, 0, SEEK_SET) != 0) {
        fprintf(stderr, "%s\n", LangGet(ERROR_FSEEK));
        return ERROR;
    }

    int code = (int)fread(sosfile.data, sizeof(char), sosfile.size, hostfile);
    if (code != sosfile.size) {
        fprintf(stderr, "%s\n", LangGet(ERROR_READ));
        return ERROR;
    }
    sosfile.data[sosfile.size] = '\0';

    if (write_file(filename, sosfile, user) == ERROR)
        return ERROR;
    return SUCCESS;
}

/**
 * @brief Download file to host
 *
 * @param filenamesos
 * @return int, Success code or error code depending on whether successful or failure
 */
int store_file_to_host(char *filenamesos) {
    int index_inode = is_file_in_inode(filenamesos);
    if (index_inode == INODE_TABLE_SIZE)
        return ERROR;

    file_t file;
    if (read_file(filenamesos, &file) == ERROR)
        return ERROR;

    FILE *fd;
    fd = fopen(filenamesos, "w");
    if (fd == NULL)
        return 0;

    if (fseek(fd, 0, SEEK_SET) != 0) {
        fprintf(stderr, "%s\n", LangGet(ERROR_FSEEK));
        if (fclose(fd) == EOF)
            fprintf(stderr, "%s\n", LangGet(ERROR_FILE_CLOSE));
        return ERROR;
    }

    int code = (int)fwrite(file.data, sizeof(uchar), file.size, fd);
    if (code != file.size) {
        fprintf(stderr, "%s", LangGet(ERROR_WRITE_BLOCK));
        if (fclose(fd) == EOF)
            fprintf(stderr, "%s\n", LangGet(ERROR_FILE_CLOSE));
        return ERROR;
    }

    if (fclose(fd) == EOF) {
        fprintf(stderr, "%s\n", LangGet(ERROR_FILE_CLOSE));
        return ERROR;
    }
    return SUCCESS;
}
