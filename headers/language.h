/**
 * @file language.h
 * @author HERZBERG Dwayne and BERLIN Florian
 * @brief Contains all the output intended for the user.
 * @version 0.1
 * @date 2022-02-21
 */

#ifndef SCRATCHOS_LABGUAGE_H
#define SCRATCHOS_LABGUAGE_H

typedef enum _Language {
    EN, FR
} Language;

typedef struct {
    char * text[1000];
} STRING;

typedef enum _STR_NAME{
    ERROR_MALLOC,
    ERROR_ACCESS_DISKFILE,
    ERROR_READ_SUPERBLOCK,
    ERROR_READ_INODES_TABLE,
    ERROR_READ_USERS_TABLE,
    ERROR_WRITE_SUPERBLOCK,
    ERROR_WRITE_BLOCK,
    ERROR_WRITE_INODES_TABLE,
    ERROR_WRITE_USERS_TABLE,
    ERROR_FILE_CLOSE,
    ERROR_FILE_OPEN,
    ERROR_FSEEK,
    ERROR_FTELL,
    ERROR_DISK_FULL,
    ERROR_FWRITE,
    ERROR_READ,
    ERROR_INODE_TABLE_FULL,
    ERROR_INODE_INDEX,
    ERROR_INODE_INIT,
    ERROR_USER_ID,
    ERROR_USER_MAX,
    ERROR_USER_EXIST,
    ERROR_USER_INPUT,
    ERROR_COMMAND_LS_USAGE,
    ERROR_COMMAND_CAT_USAGE,
    ERROR_COMMAND_RM_USAGE,
    ERROR_COMMAND_CR_USAGE,
    ERROR_COMMAND_ARGS_UNKNOWN,
    ERROR_COMMAND_ARGS_FILE_EXIST,
    ERROR_FILE_RIGHTS,
    OUTPUT_COMMAND_RM,
    OUTPUT_COMMAND_CR,
    ERROR_COMMAND_EDIT_USAGE,
    OUTPUT_COMMAND_EDIT_EDITING,
    OUTPUT_COMMAND_EDIT_LINE,
    ERROR_COMMAND_EDIT_INPUT_MAX,
    ERROR_COMMAND_EDIT_FILE_MAXSIZE,
    OUTPUT_COMMAND_EDIT_END,
    ERROR_COMMAND_LOAD_USAGE,
    ERROR_COMMAND_LOAD_LOADING_FILE,
    OUTPUT_COMMAND_LOAD,
    ERROR_COMMAND_STORE_USAGE,
    ERROR_COMMAND_STORE_STORING_FILE,
    OUTPUT_COMMAND_STORE,
    ERROR_COMMAND_CHOWN_USAGE,
    ERROR_COMMAND_ARGS_LOGIN_EXIST,
    OUTPUT_COMMAND_CHOWN,
    ERROR_COMMAND_CHMOD_USAGE,
    ERROR_COMMAND_ARGS_RIGHT_EXIST,
    OUTPUT_COMMAND_CHMOD,
    ERROR_COMMAND_LISTEUSERS_USAGE,
    OUTPUT_COMMAND_LISTUSERS,
    ERROR_COMMAND_ADDUSER_USAGE,
    ERROR_COMMAND_ARGS_ROOT_ONLY,
    OUTPUT_COMMAND_ADDUSER_LOGIN,
    OUTPUT_COMMAND_ADDUSER_PWD,
    OUTPUT_COMMAND_ADDUSER_CREATED,
    ERROR_COMMAND_RMUSER_USAGE,
    OUTPUT_COMMAND_RMUSER,
    ERROR_COMMAND_UNKNOWN,
    ERROR_COMMAND_HELP_USAGE,
    CONSOLE_LOGIN,
    CONSOLE_PWD,
    ERROR_OS_USAGE,
    OUTPUT_OS_STARTING_DISK,
    OUTPUT_OS_STARTING_SHELL,
    OUTPUT_OS_STOP_SHELL,
    OUTPUT_OS_SHUTDOWN,
    ERROR_INSTALLOS_USAGE,
    ERROR_READ_DIR,
    ERROR_CLOSE_DIR,
    OUTPUT_OSINSTALL_SELECT_DISK,
    OUPTUT_OSINSTALL_FORMAT,
    OUPTUT_OSINSTALL_FORMAT_ASK,
    INPUT_YES,
    INPUT_Y,
    OUTPUT_OSINSTALL_FORAMT_SUCCESS,
    OUTPUT_OSINSTALL_CANCEL,
    OUTPUT_OSINSTALL_SUPERUSER_NAME,
    OUTPUT_OSINSTALL_SUPERUSER_PWD,
    OUTPUT_OSINSTALL_SUPERUSER_PWD2,
    OUTPUT_OSINSTALL_SUCCESS,
    OUTPUT_OSINSTALL_PRESS_EXIT
} STR_NAME;

static const STRING strings[] = 
{
    { 
        {"memory allocation not possible",} // ERROR_MALLOC
    },
    { 
        {"Could not open diskFile",} // ERROR_ACCESS_DISKFILE
    },
    { 
        {"A problem occured while reading the super block",} // ERROR_READ_SUPERBLOCK
    },
    { 
        {"A problem occured while reading the inodes table",} // ERROR_READ_INODES_TABLE
    },
    { 
        {"A problem occured while reading the users table",} // ERROR_READ_USERS_TABLE
    },
    { 
        {"A problem occured while writing the super block",} // ERROR_WRITE_SUPERBLOCK
    },
    { 
        {"A problem occured while writing the block",} // ERROR_WRITE_INODES_TABLE
    },
    { 
        {"A problem occured while writing the inodes table",} // ERROR_WRITE_USERS_TABLE
    },
    { 
        {"A problem occured while writing the users table\n",}
    },
    { 
        {"Cannot close file",} // ERROR_FILE_CLOSE
    },
    { 
        {"Cannot open file",} // ERROR_FILE_OPEN
    },
    { 
        {"Unable to change position",} // ERROR_FSEEK
    },
    { 
        {"Cannot get the file position",} // ERROR_FTELL
    },
    { 
        {"The disk storage is full.",} // ERROR_DISK_FULL
    },
    { 
        {"An error occurred while writing block",} // ERROR_FWRITE
    },
    { 
        {"An error occurred while reading block",} // ERROR_READ
    },
    { 
        {"The inode table is full.",} // ERROR_INODE_TABLE_FULL
    },
    { 
        {"Incorrect index of inode",} // ERROR_INODE_INDEX
    },
    { 
        {"Inode not initialized",} // ERROR_INODE_INIT
    },
    { 
        {"Incorrect userid",} // ERROR_USER_ID
    },
    { 
        {"Maximum amount of users is reached",} // ERROR_USER_MAX
    },
    { 
        {"User already exists",} // ERROR_USER_EXIST
    },
    { 
        {"Reading user input error",} // ERROR_USER_INPUT
    },
    { 
        {"Usage: ls [-l]",} // ERROR_COMMAND_LS_USAGE
    },
    { 
        {"Usage: cat <file name>",} // ERROR_COMMAND_CAT_USAGE
    },
    { 
        {"Usage: rm <file name>",} // ERROR_COMMAND_RM_USAGE
    },
    { 
        {"Usage: cr <file name>",} // ERROR_COMMAND_CR_USAGE
    },
    { 
        {"Unknown argument",} // ERROR_COMMAND_ARGS_UNKNOWN
    },
    { 
        {"File does not exist:",} // ERROR_COMMAND_ARGS_FILE_EXIST
    },
    { 
        {"You aren't authorized to access this file",} // ERROR_FILE_RIGHTS
    },
    { 
        {"deleted",} // OUTPUT_COMMAND_RM
    },
    { 
        {"has been created",} // OUTPUT_COMMAND_CR
    },
    { 
        {"Usage: edit <file name>",} // ERROR_COMMAND_EDIT_USAGE
    },
    { 
        {"Editing (Enter to stop editing) :",} // OUTPUT_COMMAND_EDIT_EDITING
    },
    { 
        {"Line n°",} // OUTPUT_COMMAND_EDIT_LINE
    },
    { 
        {"Your input may have been too long",} // ERROR_COMMAND_EDIT_INPUT_MAX
    },
    { 
        {"This file has reached its maximum size",} // ERROR_COMMAND_EDIT_FILE_MAXSIZE
    },
    { 
        {"has been edited",} // OUTPUT_COMMAND_EDIT_END
    },
    { 
        {"Usage: load <file name>",} // ERROR_COMMAND_LOAD_USAGE
    },
    { 
        {"Error on loading file",} // ERROR_COMMAND_LOAD_LOADING_FILE
    },
    { 
        {"is loaded",} // OUTPUT_COMMAND_LOAD
    },
    { 
        {"Usage: store <file name>",} // ERROR_COMMAND_STORE_USAGE
    },
    { 
        {"Error on storing file:",} // ERROR_COMMAND_STORE_STORING_FILE
    },
    { 
        {"is stored",} // OUTPUT_COMMAND_STORE
    },
    { 
        {"Usage: chown <file name> <login>",} // ERROR_COMMAND_CHOWN_USAGE
    },
    { 
        {"Login does not exist:",} // ERROR_COMMAND_ARGS_LOGIN_EXIST
    },
    { 
        {"Rights have been changed",} // OUTPUT_COMMAND_CHOWN
    },
    { 
        {"Usage: chmod <file name> <right>",} // ERROR_COMMAND_CHMOD_USAGE
    },
    { 
        {"Those rights does not exist:",} // ERROR_COMMAND_ARGS_RIGHT_EXIST
    },
    { 
        {"Rights have been changed to",} // OUTPUT_COMMAND_CHMOD
    },
    { 
        {"Usage: listusers",} // ERROR_COMMAND_LISTEUSERS_USAGE 
    },
    { 
        {"List of users:",} // OUTPUT_COMMAND_LISTUSERS
    },
    { 
        {"Usage: adduser",} // ERROR_COMMAND_ADDUSER_USAGE
    },
    { 
        {"Only root user is allowed",} // ERROR_COMMAND_ARGS_ROOT_ONLY
    },
    { 
        {"Please enter a login for the new user:",} // OUTPUT_COMMAND_ADDUSER_LOGIN
    },
    { 
        {"Please enter a password for the new user:",} // OUTPUT_COMMAND_ADDUSER_PWD
    },
    { 
        {"New user created",} // OUTPUT_COMMAND_ADDUSER_CREATED
    },
    { 
        {"Usage: rmuser <login>",} // ERROR_COMMAND_RMUSER_USAGE
    },
    { 
        {"has been removed",} // OUTPUT_COMMAND_RMUSER
    },
    { 
        {"Unknown command",} // ERROR_COMMAND_UNKNOWN
    },
    { 
        {"Usage: help <command name>",} // ERROR_COMMAND_HELP_USAGE
    },
    { 
        {"login:",} // CONSOLE_LOGIN
    },
    { 
        {"password:",} // CONSOLE_PWD
    },
    { 
        {"disk_path",} // ERROR_OS_USAGE
    },
    { 
        {"Starting disk...",} // OUTPUT_OS_STARTING_DISK
    },
    { 
        {"Starting shell...",} // OUTPUT_OS_STARTING_SHELL
    },
    { 
        {"Shell return with code",} // OUTPUT_OS_STOP_SHELL
    },
    { 
        {"Shutting down...",} // OUTPUT_OS_SHUTDOWN
    },
    { 
        {"<directory>",} // ERROR_INSTALLOS_USAGE
    },
    { 
        {"Cannot read dir",} // ERROR_READ_DIR
    },
    { 
        {"Cannot close dir",} // ERROR_CLOSE_DIR
    },
    { 
        {"[INSTALL] Please select the disk for installation. (by number)",} // OUTPUT_OSINSTALL_SELECT_DISK
    },
    { 
        {"[WARNING] Please format disk before installation.",} // OUPTUT_OSINSTALL_FORMAT
    },
    { 
        {"[INSTALL] accept disk format ? y/N",} // OUPTUT_OSINSTALL_FORMAT_ASK
    },
    { 
        {"yes",} // INPUT_YES
    },
    { 
        {"y",} // INPUT_Y
    },
    { 
        {"[INSTALL] formatting completed successfully",} // OUTPUT_OSINSTALL_FORAMT_SUCCESS
    },
    { 
        {"[INSTALL] Installation cancel.",} // OUTPUT_OSINSTALL_CANCEL
    },
    { 
        {"[INSTALL] Please enter the superuser nickname (root by defaut):",} // OUTPUT_OSINSTALL_SUPERUSER_NAME
    },
    { 
        {"[INSTALL] Please enter the superuser password:",} // OUTPUT_OSINSTALL_SUPERUSER_PWD
    },
    { 
        {"[INSTALL] Please confirm the superuser password:",} // OUTPUT_OSINSTALL_SUPERUSER_PWD2
    },
    { 
        {"[INSTALL] installation completed successfully",} // OUTPUT_OSINSTALL_SUCCESS
    }, 
    {
        {"[INSTALL] Touch any key on the keyboard to exit the program",} // OUTPUT_OSINSTALL_PRESS_EXIT
    }
};

char const *LangGet(STR_NAME var);

#endif //SCRATCHOS_LABGUAGE_H