/**
 * @file cmd_dump_user.c
 * @author  HERZBERG Dwayne and BERLIN Florian
 * @brief Source code for cmd_dump_user of the ScratchOs : testing program for layer 3
 * @version 0.1
 * @date 2022-02-17
 */

#include "../headers/layer3.h"

int main(int argc, char **argv){
    if (argc != 2){
        fprintf(stderr, "Usage: %s <directory path>\n", argv[0]);
        return 1;
    }
    init_disk_sos(argv[1], 0);
    update_first_free_byte();
    session_t user;
    user.userid = ROOT_UID;
    printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);
    init_user("root", "bonjour");
    for (int i = 0; i < INODE_TABLE_SIZE; ++i) {
        init_inode("bonjour.txt", 10, virtual_disk_sos->super_block.first_free_byte, timestamp(), timestamp(), user);
    }
    delete_inode(4);
    for (int i = 1; i < NB_USERS; ++i) {
        init_user("Michel", "bonjour");
    }
    printf("Login Michel in users table : %d \n", is_login_in_users_table("Michel"));
    printf("Test login Michel : %d \n", is_good_credentials("Michel", "bonjour"));
    printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);
    delete_user(3);
    printf("Login Michel in users table : %d \n", is_login_in_users_table("Michel"));

    shutdown_disk_sos();
    init_disk_sos(argv[1], 0);
    display_disk_storage();

    for (int i = 0; i < virtual_disk_sos->super_block.number_of_users; ++i) {
        fprintf(stdout, "userid %d: %s\n", i,virtual_disk_sos->users_table[i].login);
        fprintf(stdout, "pass:%s\n", virtual_disk_sos->users_table[i].passwd);
    }
    printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);

    shutdown_disk_sos();
    return 0;
}
