/**
 * @file installos.c
 * @author  HERZBERG Dwayne and BERLIN Florian
 * @brief Source code for the OS installer
 * @version 0.1
 * @date 2022-02-14
 */

// TODO minimal size

#include "../headers/os_defines.h"
#include "../headers/language.h"
#include "../headers/layer1.h"
#include "../headers/layer2.h"
#include "../headers/layer3.h"
#include "../headers/layer4.h"
#include "../headers/layer5.h"
#include "../headers/terminal.h"
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
    if (fp == NULL) { fprintf(stderr, "%s\n", LangGet(ERROR_FILE_OPEN)); return ERROR; }
    unsigned char zero=0;
    for(int i=0; i<size; i++) {
        int nb_write = (int)fwrite(&zero, 1, 1, fp);
        if (nb_write != 1){ return ERROR; }
    }
    if (fclose(fp) == EOF) { fprintf(stderr, "%s\n", LangGet(ERROR_FILE_CLOSE)); return ERROR; }
    return SUCCESS;
}

/**
 * @brief init terminal
 */
void init(){
    terminal_clear();
	terminal_non_canonique();
	printf("\e[?25l");
}

/**
 * @brief reset terminal
 */
void exitInstaller(){
    fprintf(stdout, "%s\n", LangGet(OUTPUT_OSINSTALL_PRESS_EXIT));
    char lettre;
    read(STDIN_FILENO, &lettre, sizeof(char));
    printf("\e[?25h");
    terminal_cursor(0,0);
    terminal_canonique();
    terminal_clear();
}

#define OSINSTALLER_TITLE "Scratch OS"

/**
 * @brief OS installer
 * 
 * @param argc 
 * @param argv 
 * @return int, Success code or error code depending on whether successful or failure 
 */
int main(int argc, char **argv){
    init();
    int col; int row;
	terminal_get_max_size(&col, &row);
    terminal_cursor((int)((col/2)-(strlen(OSINSTALLER_TITLE))/2),2);
	fprintf(stdout, "%s\n", OSINSTALLER_TITLE);
    terminal_cursor(0,3);

    if (argc != 2){ 
        fprintf(stdout, "Usage: %s %s\n", argv[0], LangGet(ERROR_INSTALLOS_USAGE)); exitInstaller(); return ERROR; 
    }

    char** list_kid = (char**) malloc((MAX_DISK_KID+1)*sizeof(char*));
    if (list_kid == NULL) { fprintf(stdout, "%s\n", LangGet(ERROR_MALLOC)); exitInstaller(); return ERROR; }
    for (int i = 0; i < (MAX_DISK_KID+1); i++) {
        list_kid[i] = (char*) malloc(MAX_FILE_SIZE*sizeof(char));
        if (list_kid[i] == NULL) { fprintf(stdout, "%s\n", LangGet(ERROR_MALLOC)); exitInstaller(); return ERROR; }
    }
    
    struct dirent *dir;
    DIR *d = opendir(argv[1]); 
    int cpt=-1;
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(".", dir->d_name) && strcmp("..", dir->d_name) && strcmp("DO_NOT_DELETE", dir->d_name)){
                cpt++; strcpy(list_kid[cpt], dir->d_name);
            }
        }
        if (closedir(d) == -1) { fprintf(stdout, "%s\n", LangGet(ERROR_CLOSE_DIR)); exitInstaller(); return ERROR; }
    } else { fprintf(stdout, "%s\n", LangGet(ERROR_READ_DIR)); exitInstaller(); return ERROR; }

    cpt++;
    strcpy(list_kid[cpt], "cancel");

    terminal_cursor(4,3);
	fprintf(stdout, "Please select the installation disk");
    terminal_cursor(4, row-4);
    fprintf(stdout, "Please use the up ↑ and down ↓ arrow keys to select the disk.");
    terminal_cursor(4, row-3);
    fprintf(stdout, "Press esc to exit. Or select cancel.");
    terminal_cursor((int)((col/2)-(strlen("Made by Dwayne & Florian with ❤"))/2),row-1);
	fprintf(stdout, "Made by HERZBERG & BERLIN with ❤");
    
    setborder();
    int selected = menu(list_kid, cpt);

	terminal_clear();

    terminal_cursor((int)((col/2)-(strlen(OSINSTALLER_TITLE))/2),2);
    fprintf(stdout, "%s\n", OSINSTALLER_TITLE);
    terminal_cursor(0,3);

    if (selected == cpt){ 
        fprintf(stdout, "%s\n", LangGet(OUTPUT_OSINSTALL_CANCEL)); exitInstaller(); return SUCCESS; 
    }

    if (init_disk_sos(argv[1], selected) == ERROR) { exitInstaller(); return ERROR;}

    if (virtual_disk_sos->super_block.first_free_byte != 0){
        char** yes_no = (char**) malloc(3*sizeof(char*));
        if (yes_no == NULL) { fprintf(stdout, "%s\n", LangGet(ERROR_MALLOC)); exitInstaller(); return ERROR; }
        for (int i = 0; i < 3; i++) {
            yes_no[i] = (char*) malloc(MAX_FILE_SIZE*sizeof(char));
            if (yes_no[i] == NULL) { fprintf(stdout, "%s\n", LangGet(ERROR_MALLOC)); exitInstaller(); return ERROR; }
        }
        strcpy(yes_no[0], "yes"); strcpy(yes_no[1], "no"); strcpy(yes_no[2], "cancel");

        terminal_cursor(4,3);
        fprintf(stdout, "An os is already installed on the disk. do you want to proceed with formatting the drive");
        terminal_cursor(4, row-4);
        fprintf(stdout, "Please use the up ↑ and down ↓ arrow keys to select your choice.");
        terminal_cursor(4, row-3);
        fprintf(stdout, "Press esc to exit. Or select cancel.");
        terminal_cursor((int)((col/2)-(strlen("Made by Dwayne & Florian with ❤"))/2),row-1);
        fprintf(stdout, "Made by HERZBERG & BERLIN with ❤");

        setborder();
        int choix = menu(yes_no, 2);       

        terminal_clear();

        terminal_cursor((int)((col/2)-(strlen(OSINSTALLER_TITLE))/2),2);
        fprintf(stdout, "%s\n", OSINSTALLER_TITLE);
        terminal_cursor(0,3); 

        if (choix == 0){
            if (shutdown_disk_sos() == ERROR) {  exitInstaller(); return ERROR; }
            if (format(argv[1], 50000, selected) == ERROR) {  exitInstaller(); return ERROR; } // TODO SIZE ?????
            if (init_disk_sos(argv[1], selected) == ERROR) {  exitInstaller(); return ERROR; }
            fprintf(stdout, "%s\n", LangGet(OUTPUT_OSINSTALL_FORAMT_SUCCESS));
        } else {
            fprintf(stdout, "%s\n", LangGet(OUTPUT_OSINSTALL_CANCEL));
            exitInstaller();
            return ERROR;
        }
    }

    update_first_free_byte();
    session_t user;
    user.userid = ROOT_UID;

    char *username = malloc(sizeof(char)*FILENAME_MAX_SIZE);
    if (username == NULL) { fprintf(stdout, "%s\n", LangGet(ERROR_MALLOC)); exitInstaller(); return ERROR; }

    char *password = malloc(sizeof(char)*FILENAME_MAX_SIZE);
    if (password == NULL) { fprintf(stdout, "%s\n", LangGet(ERROR_MALLOC)); exitInstaller(); return ERROR; }

    char *verifpwd = malloc(sizeof(char)*FILENAME_MAX_SIZE);
    if (verifpwd == NULL) { fprintf(stdout, "%s\n", LangGet(ERROR_MALLOC)); exitInstaller(); return ERROR; }

    char *password_hash = malloc(sizeof(char)*(SHA256_BLOCK_SIZE*2 + 1));
    if (password_hash == NULL) { fprintf(stdout, "%s\n", LangGet(ERROR_MALLOC)); exitInstaller(); return ERROR; }
    
    fprintf(stdout, "%s\n", LangGet(OUTPUT_OSINSTALL_SUPERUSER_NAME));

    terminal_canonique();
    if (read_cmd(username, FILENAME_MAX_SIZE) == ERROR) { exitInstaller(); return ERROR; }
    terminal_non_canonique();

    if (!strcmp(username, "")){ 
        strcpy(username, "root"); 
        fprintf(stdout, "%s\n", username);
    }

    do {
        fprintf(stdout, "%s\n", LangGet(OUTPUT_OSINSTALL_SUPERUSER_PWD));
        if (read_cmd(password, FILENAME_MAX_SIZE) == ERROR) { exitInstaller(); return ERROR; }
        fprintf(stdout, "%s\n", LangGet(OUTPUT_OSINSTALL_SUPERUSER_PWD2));
        if (read_cmd(verifpwd, FILENAME_MAX_SIZE) == ERROR) { exitInstaller(); return ERROR; }
    } while (strcmp(password, verifpwd));
    
    if (init_user(username, password) == ERROR) { exitInstaller(); return ERROR; }
    sha256ofString((BYTE *)password, password_hash);

    file_t file;
    char* chaine = (char*) malloc((strlen(password_hash)+strlen(username))*sizeof(char));
    if (chaine == NULL) { fprintf(stdout, "%s\n", LangGet(ERROR_MALLOC)); exitInstaller(); return ERROR; }

    strcpy(chaine, username);
    strcat(chaine, " ");
    strcat(chaine, password_hash);
    memcpy(file.data, chaine, strlen(chaine));
    file.size = strlen(chaine);

    char* namefile = (char*) malloc((strlen("passwd")+5)*sizeof(char));
    if (namefile == NULL) { fprintf(stdout, "%s\n", LangGet(ERROR_MALLOC)); exitInstaller(); return ERROR; }
    snprintf(namefile, strlen("passwd")+5, "%s/d%d", "passwd", selected);

    if (write_file(namefile, file, user) == ERROR) { exitInstaller(); return ERROR; }
    if (shutdown_disk_sos() == ERROR) { exitInstaller(); return ERROR; }
    
    free(chaine);
    free(namefile);
    free(list_kid);
    free(username);
    free(verifpwd);
    free(password);
    free(password_hash);

    fprintf(stdout, "%s\n", LangGet(OUTPUT_OSINSTALL_SUCCESS));
    exitInstaller();
    return SUCCESS;
}