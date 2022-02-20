/**
 * @file installos.c
 * @author  HERZBERG Dwayne and BERLIN Florian
 * @brief Source code for the OS installer
 * @version 0.1
 * @date 2022-02-14
 */

#include "../headers/os_defines.h"
#include "../headers/language.h"
#include "../headers/layer1.h"
#include "../headers/layer2.h"
#include "../headers/layer3.h"
#include "../headers/layer4.h"
#include "../headers/layer5.h"
#include <dirent.h>

/**
 * @brief Format disk
 * 
 * @param dirname 
 * @param size 
 * @param diskid 
 * @return int, Success code or error code depending on whether successful or failure
 */
int format(char *dirname, int size, int diskid){
    char filename[strlen(dirname)+4];
    snprintf(filename, strlen(dirname)+4, "%sd%d", dirname, diskid);
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) { fprintf(stderr, ERROR_FILE_OPEN); return ERROR; }
    unsigned char zero=0;
    for(int i=0; i<size; i++) {
        int nb_write = (int)fwrite(&zero, 1, 1, fp);
        if (nb_write != 1){ return ERROR; }
    }
    if (fclose(fp) == EOF) { fprintf(stderr, ERROR_FILE_CLOSE); return ERROR; }
    return SUCCESS;
}

/**
 * @brief OS installer
 * 
 * @param argc 
 * @param argv 
 * @return int, Success code or error code depending on whether successful or failure 
 */
int main(int argc, char **argv){
    if (argc != 2){ fprintf(stderr, ERROR_INSTALLOS_USAGE, argv[0]); return ERROR; }

    char** list_kid = (char**) malloc(MAX_DISK_KID*sizeof(char*));
    if (list_kid == NULL) { fprintf(stderr, ERROR_MALLOC); return ERROR; }
    for (int i = 0; i < MAX_DISK_KID; i++) {
        list_kid[i] = (char*) malloc(MAX_FILE_SIZE*sizeof(char));
        if (list_kid[i] == NULL) { fprintf(stderr, ERROR_MALLOC); return ERROR; }
    }
    
    struct dirent *dir;
    DIR *d = opendir(argv[1]); 
    int cpt=-1;
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp("..", dir->d_name) && strcmp("DO_NOT_DELETE", dir->d_name)){
                cpt++; strcpy(list_kid[cpt], dir->d_name);
            }
        }
        if (closedir(d) == -1) { fprintf(stderr, ERROR_CLOSE_DIR); return ERROR; }
    } else { fprintf(stderr, ERROR_READ_DIR); return ERROR; }

    for (int i = 0; i < cpt; i++){
        fprintf(stdout, "%d : %s\n", i, list_kid[i]);
    }

    fprintf(stdout, OUTPUT_OSINSTALL_SELECT_DISK);    

    char* select = (char*) malloc(sizeof(char));
    if (select == NULL) { fprintf(stderr, ERROR_MALLOC); return ERROR; }
    read_cmd(select, FILENAME_MAX_SIZE);
    init_disk_sos(argv[1], atoi(select));

    if (virtual_disk_sos->super_block.first_free_byte != 0){
        fprintf(stdout, OUPTUT_OSINSTALL_FORMAT);
        fprintf(stdout, OUPTUT_OSINSTALL_FORMAT_ASK);
        char* choice = (char*) malloc(sizeof(char)*FILENAME_MAX_SIZE);
        if (choice == NULL) { fprintf(stderr, ERROR_MALLOC); return ERROR; }
        read_cmd(choice, FILENAME_MAX_SIZE);
        if (!strcmp(choice, INPUT_YES) || !strcmp(choice, INPUT_Y)){
            if (shutdown_disk_sos() == ERROR) return ERROR;
            if (format(argv[1], 50000, atoi(select)) == ERROR) return ERROR; // TODO SIZE ?????
            if (init_disk_sos(argv[1], atoi(select)) == ERROR) return ERROR;
            fprintf(stdout, OUTPUT_OSINSTALL_FORAMT_SUCCESS);
        } else {
            fprintf(stdout, OUTPUT_OSINSTALL_CANCEL);
            shutdown_disk_sos();
            return ERROR;
        }
    }

    update_first_free_byte();
    session_t user;
    user.userid = ROOT_UID;

    char *username = malloc(sizeof(char)*FILENAME_MAX_SIZE);
    if (username == NULL) { fprintf(stderr, ERROR_MALLOC); return ERROR; }

    char *password = malloc(sizeof(char)*FILENAME_MAX_SIZE);
    if (password == NULL) { fprintf(stderr, ERROR_MALLOC); return ERROR; }

    char *verifpwd = malloc(sizeof(char)*FILENAME_MAX_SIZE);
    if (verifpwd == NULL) { fprintf(stderr, ERROR_MALLOC); return ERROR; }

    char *password_hash = malloc(sizeof(char)*(SHA256_BLOCK_SIZE*2 + 1));
    if (password_hash == NULL) { fprintf(stderr, ERROR_MALLOC); return ERROR; }

    fprintf(stdout, OUTPUT_OSINSTALL_SUPERUSER_NAME);
    if (read_cmd(username, FILENAME_MAX_SIZE) == ERROR) return ERROR;
    if(!strcmp(username, "")){ strcpy(username, "root"); }
    do {
        fprintf(stdout, OUTPUT_OSINSTALL_SUPERUSER_PWD);
        if (read_cmd(password, FILENAME_MAX_SIZE) == ERROR) return ERROR;
        fprintf(stdout, OUTPUT_OSINSTALL_SUPERUSER_PWD2);
        if (read_cmd(verifpwd, FILENAME_MAX_SIZE) == ERROR) return ERROR;
    } while (strcmp(password, verifpwd));
    
    if (init_user(username, password) == ERROR) return ERROR;
    sha256ofString((BYTE *)password, password_hash);

    // NEED ??
    file_t file;
    char* chaine = (char*) malloc((strlen(password_hash)+strlen(username))*sizeof(char));
    if (chaine == NULL) { fprintf(stderr, ERROR_MALLOC); return ERROR; }

    strcpy(chaine, username);
    strcat(chaine, " ");
    strcat(chaine, password_hash);
    memcpy(file.data, chaine, strlen(chaine));
    file.size = strlen(chaine);

    char* namefile = (char*) malloc((strlen("passwd")+5)*sizeof(char));
    if (namefile == NULL) { fprintf(stderr, ERROR_MALLOC); return ERROR; }
    snprintf(namefile, strlen("passwd")+5, "%s/d%d", "passwd", atoi(select));

    if (write_file(namefile, file, user) == ERROR) return ERROR;
    
    free(chaine);
    free(namefile);
    // STOP NEED

    if (shutdown_disk_sos() == ERROR) return ERROR;
    free(list_kid);
    free(username);
    free(verifpwd);
    free(password);
    free(password_hash);
    fprintf(stdout, OUTPUT_OSINSTALL_SUCCESS);
    return SUCCESS;
}