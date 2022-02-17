#include "../headers/layer1.h"
#include "../headers/layer4.h"
#include "../headers/layer5.h"

int main(int argc, char **argv){
    if (argc != 2){
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return ERROR;
    }

    init_disk_sos(argv[1]);
    update_first_free_byte();
    session_t user;
    user.userid = ROOT_UID;

    char *password = malloc(sizeof(char)*FILENAME_MAX_SIZE);
    char *password_hash = malloc(sizeof(char)*(SHA256_BLOCK_SIZE*2 + 1));

    printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);
    fprintf(stdout, "[INSTALL] Please enter the root password: \n");
    read_cmd(password, FILENAME_MAX_SIZE);
    fprintf(stdout, "[INSTALL] Password chosen: %s\n", password);
    init_user("root", password);

    sha256ofString((BYTE *)password, password_hash);

    file_t file;
    char* chaine = (char*) malloc((strlen(password_hash)+strlen("root "))*sizeof(char));
    strcpy(chaine, "root ");
    strcat(chaine, password_hash);
    memcpy(file.data, chaine, strlen(chaine));
    file.size = strlen(chaine);
    free(chaine);

    write_file("passwd", file, user);
    store_file_to_host("passwd");

    printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);

    shutdown_disk_sos();
    free(password);
    free(password_hash);
    return SUCCESS;
}