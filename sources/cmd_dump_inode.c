#include "../headers/layer2.h"

int main(int argc, char **argv){
    if (argc !=2 ){
        fprintf(stderr, "Usage: %s <directory>", argv[1]);
        return 1;
    }
    if (init_disk_sos(argv[1]) == ERROR) return ERROR;
    session_t user;
    user.userid = 0;
    for (int i = 0; i < INODE_TABLE_SIZE; ++i) {
        if (init_inode("\0", 0, 0, timestamp(), timestamp(), user) == ERROR) return ERROR;
        if (delete_inode(0) == ERROR) return ERROR;
    }
    update_first_free_byte();
    printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);

    for (int i = 0; i < INODE_TABLE_SIZE; ++i) {
        if (init_inode("bonjour.txt", 10, virtual_disk_sos->super_block.first_free_byte, timestamp(), timestamp(), user) == ERROR) return ERROR;
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
    if (init_disk_sos(argv[1]) == ERROR) return ERROR;
    display_disk_storage();
    printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);
    for (int i = 0; i < virtual_disk_sos->super_block.number_of_files; i++) {
        fprintf(stdout, "File n° %d : %s \n",i, virtual_disk_sos->inodes[i].filename);
        fprintf(stdout, "size:  %d\n", virtual_disk_sos->inodes[i].size);
        fprintf(stdout, "Blocks  : %d \n",virtual_disk_sos->inodes[i].nblock);
        fprintf(stdout, "Pos  : %d \n",virtual_disk_sos->inodes[i].first_byte);
        fprintf(stdout, "File n° %d : %s \n",i, virtual_disk_sos->inodes[i].ctimestamp);
        fprintf(stdout, "File n° %d : %s \n",i, virtual_disk_sos->inodes[i].mtimestamp);
    }
    if (shutdown_disk_sos() == ERROR) return ERROR;
    return SUCCESS;
}
