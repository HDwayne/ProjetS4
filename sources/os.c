#include "../headers/layer1.h"
#include "../headers/layer4.h"
#include "../headers/layer5.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "[USAGE] %s disk_path", argv[0]);
        return ERROR;
    }
    // fprintf(stdout, "Starting disk...\n");
    // init_disk_sos(argv[1]);
    // fprintf(stdout, "Starting shell...\n");
    
    // int returnCode = terminal_shell();

    // fprintf(stdout, "Shell return with %d code\n", returnCode);
    // fprintf(stdout, "Shutting down...\n");
    // shutdown_disk_sos();

    // return SUCCESS;

    init_disk_sos(argv[1]);
    update_first_free_byte();
    printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);

    if (init_user("root", "bonjour") == ERROR) {
        fprintf(stderr, "DEJA EXISTANT\n");
    }
    fprintf(stdout, "ouiiii\n");
    for (int i = 0; i < 5; ++i) {
        if (init_inode("bonjour.txt", 10, virtual_disk_sos->super_block.first_free_byte, timestamp(), timestamp()) == ERROR){
            fprintf(stderr, "ERREUR BONJOURS.TXT\n");
        }
        printf(" ici Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);
    }
    fprintf(stdout, "ouiiii\n");

    load_file_from_host("file.txt");
    cmd_t bla;
    bla.nbArgs=2;
    strcpy(bla.tabArgs[0], "cat");
    strcpy(bla.tabArgs[1], "file.txt");
    execute_cmd(bla);

    store_file_to_host("file.txt");

    // delete_inode(4);
    // for (int i = 1; i < NB_USERS; ++i) {
    //     init_user("Michel", "bonjour");
    // }

    // printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);
    // load_file_from_host("coucou");
    // store_file_to_host("coucou");
    // delete_file("coucou");
    // shutdown_disk_sos();
    // init_disk_sos(argv[1]);
    // display_disk_storage();

    // for (int i = 0; i < virtual_disk_sos->super_block.number_of_users; ++i) {
    //     fprintf(stdout, "userid %d: %s\n", i,virtual_disk_sos->users_table[i].login);
    //     fprintf(stdout, "pass:%s\n", virtual_disk_sos->users_table[i].passwd);
    // }
    // printf("Superblock : %d %d %d %d\n", virtual_disk_sos->super_block.number_of_files, virtual_disk_sos->super_block.number_of_users, virtual_disk_sos->super_block.nb_blocks_used, virtual_disk_sos->super_block.first_free_byte);

    shutdown_disk_sos();
    return SUCCESS;
}