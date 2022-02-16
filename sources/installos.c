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
    init_user("root", password);

    sha256ofString((BYTE *)password, password_hash);

    file_t file;
    // char chaine[strlen(password_hash+10)];
    char* chaine = (char*) malloc(strlen(password_hash+10)*sizeof(char));
    fprintf(stdout, "ici oui génie %s !\n", argv[1]);
    fprintf(stdout, "nb arg %d !\n", argc);

    strcpy(chaine, "root ");

    fprintf(stdout, "nb arg %d !\n", argc);
    fprintf(stdout, "ici oui génie %s !\n", argv[0]);

    strcat(chaine, password_hash);

    fprintf(stdout, "ici oui génie %s !\n", argv[1]);
    fprintf(stdout, "nb arg %d !\n", argc);

    memcpy(file.data, chaine, strlen(chaine));
    file.size = strlen(chaine);
    
    free(chaine);
    fprintf(stdout, "ouiii\n");



    write_file("coucou", file);
    cmd_t bla;
    bla.nbArgs=2;
    strcpy(bla.tabArgs[0], "cat");
    strcpy(bla.tabArgs[1], "coucou");
    execute_cmd(bla);
    store_file_to_host("coucou");
    printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);

    shutdown_disk_sos();
    free(password);
    free(password_hash);

    int returnCode = init_disk_sos(argv[1]);

    printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);
    shutdown_disk_sos();
    return SUCCESS;
}