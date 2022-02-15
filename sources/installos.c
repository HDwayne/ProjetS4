#include "../headers/layer1.h"
#include "../headers/layer2.h"
#include "../headers/layer3.h"
#include "../headers/layer4.h"
#include "../headers/layer5.h"

int main(int argc, char **argv){
    if (argc != 2){
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return ERROR;
    }
    init_disk_sos(argv[1]);
    update_first_free_byte();
    char *password = malloc(sizeof(char)*FILENAME_MAX_SIZE);
    char *password_hash = malloc(sizeof(char)*(SHA256_BLOCK_SIZE*2 + 1));
    printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);
    fprintf(stdout, "[INSTALL] Please enter the root password: \n");
    read_cmd(password, FILENAME_MAX_SIZE);
    fprintf(stdout, "[INSTALL] Password chosen: %s\n", password);
    sha256ofString((BYTE *)password, password_hash);
    init_user("root", password);
    file_t file;
    for (int i = 0; i < strlen("root"); ++i) {
        file.data[i] = (unsigned char)"root"[i];
    }
    file.data[strlen("root")] = ' ';
    for (int i = strlen("root")+1; i < strlen(password)+strlen("root")+1; ++i) {
        file.data[i] = (unsigned char)password_hash[i-strlen("root")-1];
    }
    file.data[strlen("root")+1+strlen(password)] = '\0';
    file.size= strlen("root")+1+strlen(password)+1;

    write_file("coucou", file);
    store_file_to_host("coucou");
    printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);

    display_disk_storage();
    shutdown_disk_sos();
    free(password);
    free(password_hash);
    init_disk_sos(argv[1]);
    printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);


}