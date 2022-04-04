/**
 * @file layer3.c
 * @author  HERZBERG Dwayne and BERLIN Florian
 * @brief Source code for layer3 of the ScratchOs : users
 * @version 0.1
 * @date 2022-02-14
 */

#include "../headers/layer3.h"

/**
 * @brief write users table
 * @return int, Success code or error code depending on whether successful or failure
 */
int write_users_table() {
    int pos = USERS_START;
    for (int i = 0; i < NB_USERS; ++i) {
        if (write_text_block_char(&pos, FILENAME_MAX_SIZE, virtual_disk_sos->users_table[i].login) == ERROR)
            return ERROR;
        if (write_text_block_char(&pos, SHA256_BLOCK_SIZE * 2 + 1, virtual_disk_sos->users_table[i].passwd) == ERROR)
            return ERROR;
    }
    return SUCCESS;
}

/**
 * @brief Read users table
 * @return int, Success code or error code depending on whether successful or failure
 */
int read_users_table() {
    int pos = USERS_START;
    for (int i = 0; i < NB_USERS; ++i) {
        if (read_text_block_char(&pos, FILENAME_MAX_SIZE, virtual_disk_sos->users_table[i].login) == ERROR)
            return ERROR;
        if (read_text_block_char(&pos, SHA256_BLOCK_SIZE * 2 + 1, virtual_disk_sos->users_table[i].passwd) == ERROR)
            return ERROR;
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
int delete_user(int id_user) {
    if (id_user >= NB_USERS || id_user <= 0) {
        fprintf(stderr, "%s\n", LangGet(ERROR_USER_ID));
        return ERROR;
    }
    for (int i = id_user; i < get_unused_user() - 1; ++i)
        virtual_disk_sos->users_table[i] = virtual_disk_sos->users_table[i + 1];
    virtual_disk_sos->users_table[get_unused_user() - 1].login[0] = '\0';
    virtual_disk_sos->super_block.number_of_users--;
    return SUCCESS;
}

/**
 * @brief Get number of unused user object
 * @return int
 */
int get_unused_user() {
    for (int i = 0; i < NB_USERS; ++i) {
        if (virtual_disk_sos->users_table[i].login[0] == '\0')
            return i;
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
int init_user(char *login, char *password) {
    int id_user = get_unused_user();
    if (id_user == NB_USERS) {
        fprintf(stderr, "%s\n", LangGet(ERROR_USER_MAX));
        return ERROR;
    }
    if (is_login_in_users_table(login) != NB_USERS) {
        fprintf(stderr, "%s\n", LangGet(ERROR_USER_EXIST));
        return ERROR;
    }
    for (int i = 0; i < FILENAME_MAX_SIZE; i++)
        virtual_disk_sos->users_table[id_user].login[i] = login[i];
    char password_hash[SHA256_BLOCK_SIZE * 2 + 1];
    sha256ofString((BYTE *)password, password_hash);
    for (int i = 0; i < SHA256_BLOCK_SIZE * 2 + 1; ++i)
        virtual_disk_sos->users_table[id_user].passwd[i] = password_hash[i];
    virtual_disk_sos->super_block.number_of_users++;
    return SUCCESS;
}

/**
 * @brief Look up the user's index in the table (if connected)
 *
 * @param login
 * @return int, index of the user
 */
int is_login_in_users_table(char *login) {
    for (int i = 0; i < virtual_disk_sos->super_block.number_of_users; ++i){
        if (strcmp(login, virtual_disk_sos->users_table[i].login) == 0)
            return i;
    }
    return NB_USERS;
}

/**
 * @brief Check if the credentials are correct
 *
 * @param login
 * @param password
 * @return int
 */
int is_good_credentials(char *login, char *password) {
    int uid = is_login_in_users_table(login);
    if (uid != NB_USERS){
        char hash_password[SHA256_BLOCK_SIZE * 2 + 1];
        sha256ofString((BYTE *)password, hash_password);
        if (strcmp(virtual_disk_sos->users_table[uid].passwd, hash_password) == 0)
            return uid;
    }
    return NB_USERS;
}

/**
 * It checks if the password is valid
 *
 * @param pwd1 The first password.
 * @param pwd2 The password that the user has entered.
 *
 * @return True is the password is valid. Esle False.
 */
bool is_valid_password(char *pwd1, char *pwd2) {
    if (strcmp(pwd1, pwd2) != 0) {
        terminal_print(LangGet(WARNING_PASSWORD_NOT_MATCH), TERMINAL_ORANGE);
        return false;
    }
    if (strlen(pwd1) < 8) {
        terminal_print(LangGet(WARNING_PASSWORD_TOO_SHORT), TERMINAL_ORANGE);
        return false;
    }

    int i = 0;
    bool min = false;
    bool maj = false;
    bool num = false;
    while (pwd1[i] != '\0') {
        if (!min && pwd1[i] >= 'a' && pwd1[i] <= 'z')
            min = true;
        if (!maj && pwd1[i] >= 'A' && pwd1[i] <= 'Z')
            maj = true;
        if (!num && pwd1[i] >= '0' && pwd1[i] <= '9')
            num = true;
        i++;
    }
    if (!min)
        terminal_print(LangGet(WARNING_PASSWORD_NO_LOWERCASE), TERMINAL_ORANGE);
    if (!maj)
        terminal_print(LangGet(WARNING_PASSWORD_NO_UPPERCASE), TERMINAL_ORANGE);
    if (!num)
        terminal_print(LangGet(WARNING_PASSWORD_NO_NUMBER), TERMINAL_ORANGE);
    return min && maj && num;
}