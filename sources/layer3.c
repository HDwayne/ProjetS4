//
// Created by Florian Berlin on 14/02/2022.
//

#include "../headers/layer3.h"

/**
 * @brief write users table
 * @return int, Success code or error code depending on whether successful or failure
 */
int write_users_table(){
    int pos = USERS_START;
    for (int i = 0; i < NB_USERS; ++i) {
        user_t user = virtual_disk_sos->users_table[i];
        block_t block;
        for (int j = 0; j < compute_nblock(FILENAME_MAX_SIZE); j++) {
            block.data[0] = (unsigned char)user.login[0 + j*BLOCK_SIZE];
            block.data[1] = (unsigned char)user.login[1 + j*BLOCK_SIZE];
            block.data[2] = (unsigned char)user.login[2 + j*BLOCK_SIZE];
            block.data[3] = (unsigned char)user.login[3 + j*BLOCK_SIZE];
            if (write_block(block, pos) == ERROR) return ERROR;
            pos+=BLOCK_SIZE;
        }
        for (int j = 0; j < compute_nblock(SHA256_BLOCK_SIZE*2 + 1); j++) {
            block.data[0] = (unsigned char)user.passwd[0 + j*BLOCK_SIZE];
            block.data[1] = (unsigned char)user.passwd[1 + j*BLOCK_SIZE];
            block.data[2] = (unsigned char)user.passwd[2 + j*BLOCK_SIZE];
            block.data[3] = (unsigned char)user.passwd[3 + j*BLOCK_SIZE];
            if (write_block(block, pos) == ERROR) return ERROR;
            pos+=BLOCK_SIZE;
        }
    }
    return SUCCESS;
}

/**
 * @brief Read users table
 * @return int, Success code or error code depending on whether successful or failure
 */
int read_users_table(){
    int pos = USERS_START;
    for (int i = 0; i < NB_USERS; ++i) {
        block_t block;
        for (int j = 0; j < compute_nblock(FILENAME_MAX_SIZE); j++) {
            if(read_block(&block, pos) == ERROR) return ERROR;
            virtual_disk_sos->users_table[i].login[j*BLOCK_SIZE+0] = (char)block.data[0];
            virtual_disk_sos->users_table[i].login[j*BLOCK_SIZE+1] = (char)block.data[1];
            virtual_disk_sos->users_table[i].login[j*BLOCK_SIZE+2] = (char)block.data[2];
            virtual_disk_sos->users_table[i].login[j*BLOCK_SIZE+3] = (char)block.data[3];
            pos+=BLOCK_SIZE;
        }
        for (int j = 0; j < compute_nblock(SHA256_BLOCK_SIZE*2 + 1); j++) {
            if(read_block(&block, pos) == ERROR) return ERROR;
            virtual_disk_sos->users_table[i].passwd[j*BLOCK_SIZE+0] = (char)block.data[0];
            virtual_disk_sos->users_table[i].passwd[j*BLOCK_SIZE+1] = (char)block.data[1];
            virtual_disk_sos->users_table[i].passwd[j*BLOCK_SIZE+2] = (char)block.data[2];
            virtual_disk_sos->users_table[i].passwd[j*BLOCK_SIZE+3] = (char)block.data[3];
            pos+=BLOCK_SIZE;
        }
    }
    return SUCCESS;
}

// TODO Check if user is root (can do this action and cannot delete root)
/**
 * @brief Delete a user
 * 
 * @param id_user user id
 * @return int, Success code or error code depending on whether successful or failure
 */
int delete_user(int id_user){
    if (id_user >= NB_USERS || id_user <= 0){
        fprintf(stderr, "Incorrect userid\n");
        return ERROR;
    }
    for (int i = id_user; i < get_unused_user()-1; ++i) {
        virtual_disk_sos->users_table[i] = virtual_disk_sos->users_table[i+1];
    }
    virtual_disk_sos->users_table[get_unused_user()-1].login[0] = '\0';
    virtual_disk_sos->super_block.number_of_users--;
    return SUCCESS;
}

/**
 * @brief Get number of unused user object
 * @return int 
 */
int get_unused_user(){
    for (int i = 0; i < NB_USERS; ++i) {
        if(virtual_disk_sos->users_table[i].login[0] == '\0') return i;
    }
    return NB_USERS;
}

/**
 * @brief Initialize a user
 * 
 * @param login 
 * @param password 
 * @return int, Success code or error code depending on whether successful or failure  
 */
int init_user(char *login, char *password){
    int id_user = get_unused_user();
    if (id_user == NB_USERS){
        fprintf(stderr, "Maximum amount of users is reached\n");
        return ERROR;
    }
    if(is_login_in_users_table(login) != NB_USERS){
        fprintf(stderr, "User already exists\n");
        return ERROR;
    }
    for (int i = 0; i < FILENAME_MAX_SIZE; i++) {
        virtual_disk_sos->users_table[id_user].login[i] = login[i];
    }
    char password_hash[SHA256_BLOCK_SIZE*2 + 1];
    sha256ofString((BYTE *)password, password_hash);
    for (int i = 0; i < SHA256_BLOCK_SIZE*2 + 1; ++i) {
        virtual_disk_sos->users_table[id_user].passwd[i] = password_hash[i];
    }
    virtual_disk_sos->super_block.number_of_users++;
    return SUCCESS;
}

void cmd_dump_user(char *directory){
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
    delete_user(3);
    printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);
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

/**
 * @brief Look up the user's index in the table (if connected)
 * 
 * @param login 
 * @return int, index of the user
 */
int is_login_in_users_table(char *login){
    for (int i = 0; i < virtual_disk_sos->super_block.number_of_users; ++i) {
        if (strcmp(login, virtual_disk_sos->users_table[i].login) == 0) return i;
    }
    return NB_USERS;
}

// TODO @BerlinFlorian return ???
/**
 * @brief Check if the credentials are correct
 * 
 * @param login 
 * @param password 
 * @return int 
 */
int is_good_credentials(char *login, char *password){
    int uid = is_login_in_users_table(login);
    if(uid == NB_USERS) return NB_USERS;
    char hash_password[SHA256_BLOCK_SIZE*2 + 1];
    sha256ofString((BYTE *)password, hash_password);
    if (strcmp(virtual_disk_sos->users_table[uid].passwd, hash_password) == 0) return uid;
    return NB_USERS;
}
