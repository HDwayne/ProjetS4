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
    EN,
    FR
} Language;

typedef struct {
    char *text[1000];
} STRING;

typedef enum _STR_NAME {
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
    OUTPUT_COMMAND_ADDUSER_PWD_CONFIRM,
    OUTPUT_COMMAND_ADDUSER_CREATED,
    ERROR_COMMAND_RMUSER_USAGE,
    OUTPUT_COMMAND_RMUSER,
    ERROR_COMMAND_UNKNOWN,
    ERROR_COMMAND_HELP_USAGE,
    CONSOLE_LOGIN,
    CONSOLE_PWD,
    OUTPUT_OS_STARTING_DISK,
    OUTPUT_OS_STARTING_SHELL,
    OUTPUT_OS_STOP_SHELL,
    OUTPUT_OS_SHUTDOWN,
    WARNING_INSTALLOS_USAGE,
    ERROR_READ_DIR,
    ERROR_CLOSE_DIR,
    OUPTUT_OSINSTALL_FORMAT,
    OUPTUT_OSINSTALL_FORMAT_ASK,
    INPUT_YES,
    INPUT_Y,
    INPUT_NO,
    INPUT_N,
    INPUT_CANCEL,
    OUTPUT_OSINSTALL_FORAMT_SUCCESS,
    OUTPUT_OSINSTALL_CANCEL,
    OUTPUT_OSINSTALL_SUPERUSER_NAME,
    OUTPUT_OSINSTALL_SUPERUSER_PWD,
    OUTPUT_OSINSTALL_SUPERUSER_PWD2,
    OUTPUT_OSINSTALL_SUCCESS,
    OUTPUT_PRESS_EXIT,
    OUTPUT_OSINSTALL_SELECT_DISK_NAME,
    OUTPUT_CREDITS,
    OUTPUT_OSINSTALL_FORMAT_ASK_ALREADY_INSTALLED,
    OUTPUT_SELECT_CHOICE_HELP,
    WARNING_PASSWORD_NOT_MATCH,
    WARNING_PASSWORD_TOO_SHORT,
    WARNING_PASSWORD_NO_LOWERCASE,
    WARNING_PASSWORD_NO_UPPERCASE,
    WARNING_PASSWORD_NO_NUMBER,
    WARNING_OS_USAGE,
    OUTPUT_OS_SELECT_DISK,
    ERROR_OS_NOT_FOUND,
    OUTPUT_COMMAND_SUDO_ROOT_PWD_ASK,
    OUTPUT_COMMAND_SUDO_ROOT_PWD_FAIL,
    ERROR_COMMAND_SUDO_USAGE,
    ERROR_COMMAND_LOGOUT_USAGE,
    ERROR_COMMAND_CLEAR_USAGE,
    ERROR_COMMAND_ARGS_LOGIN_EMPTY,
    ERROR_COMMAND_ARGS_LOGIN_SELF,
    ERROR_COMMAND_ARGS_REMOVE_ROOT,
    ERROR_COMMAND_ARGS_FILE_NAME_SPECIAL_CHAR,
    ERROR_COMMAND_ARGS_FILE_LENGTH,
    ERROR_COMMAND_ARGS_FILE_EXIST_ALREADY,
    ERROR_LOGIN_ATTEMPT,
    ERROR_COMMAND_RENAME_USAGE,
    ERROR_COMMAND_CP_USAGE
} STR_NAME;

static const STRING strings[] = {
    {
        {
            "Memory allocation not possible",
            "Allocation de mémoire impossible",
        } // ERROR_MALLOC
    },
    {
        {
            "Could not open diskFile",
            "Impossible d'ouvrir le disque",
        } // ERROR_ACCESS_DISKFILE
    },
    {
        {
            "A problem occured while reading the super block",
            "Un problème est survenu lors de la lecture du super bloc",
        } // ERROR_READ_SUPERBLOCK
    },
    {
        {
            "A problem occured while reading the inodes table",
            "Un problème est survenu lors de la lecture de la table des inodes",
        } // ERROR_READ_INODES_TABLE
    },
    {
        {
            "A problem occured while reading the users table",
            "Un problème est survenu lors de la lecture de la table des utilisateurs",
        } // ERROR_READ_USERS_TABLE
    },
    {
        {
            "A problem occured while writing the super block",
            "Un problème est survenu lors de l'écriture du super bloc",
        } // ERROR_WRITE_SUPERBLOCK
    },
    {
        {
            "A problem occured while writing the block",
            "Un problème est survenu lors de l'écriture du bloc",
        } // ERROR_WRITE_BLOCK
    },
    {
        {"A problem occured while writing the inodes table",
            "Un problème est survenu lors de l'écriture de la table des inodes"} // ERROR_WRITE_INODES_TABLE
    },
    {
        {
            "A problem occured while writing the users table\n",
            "Un problème est survenu lors de l'écriture de la table des utilisateurs",
        } // ERROR_WRITE_USERS_TABLE
    },
    {
        {
            "Cannot close file",
            "Impossible de fermer le fichier",
        } // ERROR_FILE_CLOSE
    },
    {
        {
            "Cannot open file",
            "Impossible d'ouvrir le fichier",
        } // ERROR_FILE_OPEN
    },
    {
        {
            "Unable to change file position",
            "Impossible de changer de position dans le fichier"
        } // ERROR_FSEEK
    },
    {
        {
            "Cannot get the file position",
            "Impossible d'obtenir la position du fichier",
        } // ERROR_FTELL
    },
    {
        {
            "The disk storage is full",
            "Le stockage sur disque est plein",
        } // ERROR_DISK_FULL
    },
    {
        {
            "An error occurred while writing block",
            "Une erreur s'est produite lors de l'écriture du bloc",
        } // ERROR_FWRITE
    },
    {
        {
            "An error occurred while reading block",
            "Une erreur s'est produite lors de la lecture du bloc",
        } // ERROR_READ
    },
    {
        {
            "The inode table is full",
            "La table d'inodes est pleine",
        } // ERROR_INODE_TABLE_FULL
    },
    {
        {
            "Incorrect index of inode",
            "Index d'inode incorrect",
        } // ERROR_INODE_INDEX
    },
    {
        {
            "Inode not initialized",
            "Inode non initialisé",
        } // ERROR_INODE_INIT
    },
    {
        {
            "Incorrect userid",
            "ID utilisateur incorrect",
        } // ERROR_USER_ID
    },
    {
        {
            "Maximum amount of users is reached",
            "Le nombre maximum d'utilisateurs est atteint",
        } // ERROR_USER_MAX
    },
    {
        {
            "User already exists",
            "L'utilisateur existe déjà",
        } // ERROR_USER_EXIST
    },
    {
        {
            "Error reading user input",
            "Erreur lors de la lecture de l'entrée utilisateur",
        } // ERROR_USER_INPUT
    },
    {
        {
            "Usage: ls [-l]",
            "Usage: ls [-l]",
        } // ERROR_COMMAND_LS_USAGE
    },
    {
        {
            "Usage: cat <file name>",
            "Usage: cat <nom du fichier>",
        } // ERROR_COMMAND_CAT_USAGE
    },
    {
        {
            "Usage: rm <file name>",
            "Usage: rm <nom du fichier>",
        } // ERROR_COMMAND_RM_USAGE
    },
    {
        {
            "Usage: cr <file name>",
            "Usage: cr <nom du fichier>",
        } // ERROR_COMMAND_CR_USAGE
    },
    {
        {
            "Unknown argument",
            "Argument inconnu",
        } // ERROR_COMMAND_ARGS_UNKNOWN
    },
    {
        {
            "File does not exist:",
            "Le fichier ne existe pas:",
        } // ERROR_COMMAND_ARGS_FILE_EXIST
    },
    {
        {
            "You aren't authorized to access this file",
            "Vous n'êtes pas autorisé à accéder à ce fichier",
        } // ERROR_FILE_RIGHTS
    },
    {
        {
            "deleted",
            "supprimé",
        } // OUTPUT_COMMAND_RM
    },
    {
        {
            "has been created",
            "a été créé",
        } // OUTPUT_COMMAND_CR
    },
    {
        {
            "Usage: edit <file name>",
            "Usage: edit <nom du fichier>",
        } // ERROR_COMMAND_EDIT_USAGE
    },
    {
        {
            "Editing (Enter to stop editing) :",
            "Édition (Appuyez sur la touche entrée pour arrêter l'édition)",
        } // OUTPUT_COMMAND_EDIT_EDITING
    },
    {
        {
            "Line n°",
            "Ligne n°",
        } // OUTPUT_COMMAND_EDIT_LINE
    },
    {
        {
            "Your input may have been too long",
            "Votre saisie est peut-être trop longue",
        } // ERROR_COMMAND_EDIT_INPUT_MAX
    },
    {
        {
            "This file has reached its maximum size",
            "Ce fichier a atteint sa taille maximale",
        } // ERROR_COMMAND_EDIT_FILE_MAXSIZE
    },
    {
        {
            "has been edited",
            "a été édité",
        } // OUTPUT_COMMAND_EDIT_END
    },
    {
        {
            "Usage: load <file name>",
            "Usage: load <nom du fichier>",
        } // ERROR_COMMAND_LOAD_USAGE
    },
    {
        {
            "Error on loading file",
            "Erreur lors du chargement du fichier",
        } // ERROR_COMMAND_LOAD_LOADING_FILE
    },
    {
        {
            "is loaded",
            "est chargé",
        } // OUTPUT_COMMAND_LOAD
    },
    {
        {
            "Usage: store <file name>",
            "Usage: store <nom du fichier>",
        } // ERROR_COMMAND_STORE_USAGE
    },
    {
        {
            "Error on storing file:",
            "Erreur lors du stockage du fichier:",
        } // ERROR_COMMAND_STORE_STORING_FILE
    },
    {
        {"is stored",
            "est stocké"} // OUTPUT_COMMAND_STORE
    },
    {
        {
            "Usage: chown <file name> <login>",
            "Usage: chown <nom du fichier> <identifiant>",
        } // ERROR_COMMAND_CHOWN_USAGE
    },
    {
        {
            "Login does not exist:",
            "L'identifiant n'existe pas",
        } // ERROR_COMMAND_ARGS_LOGIN_EXIST
    },
    {
        {
            "Rights have been changed",
            "Les droits ont été modifiés",
        } // OUTPUT_COMMAND_CHOWN
    },
    {
        {
            "Usage: chmod <file name> <right>",
            "Usage: chmod <nom du fichier> <droit>",
        } // ERROR_COMMAND_CHMOD_USAGE
    },
    {
        {
            "Those rights does not exist:",
            "Ces droits n'existent pas:",
        } // ERROR_COMMAND_ARGS_RIGHT_EXIST
    },
    {
        {
            "Rights have been changed to",
            "Les droits ont été modifiés en",
        } // OUTPUT_COMMAND_CHMOD
    },
    {
        {
            "Usage: listusers",
            "Usage: listusers",
        } // ERROR_COMMAND_LISTEUSERS_USAGE
    },
    {
        {
            "List of users:",
            "Liste des utilisateurs:",
        } // OUTPUT_COMMAND_LISTUSERS
    },
    {
        {
            "Usage: adduser",
            "Usage: adduser",
        } // ERROR_COMMAND_ADDUSER_USAGE
    },
    {
        {
            "Only root user is allowed",
            "Seul l'utilisateur root est autorisé",
        } // ERROR_COMMAND_ARGS_ROOT_ONLY
    },
    {
        {
            "Please enter a login for the new user:",
            "Veuillez saisir un identifiant pour le nouvel utilisateur:",
        } // OUTPUT_COMMAND_ADDUSER_LOGIN
    },
    {
        {
            "Please enter a password for the new user:",
            "Veuillez entrer un mot de passe pour le nouvel utilisateur:",
        } // OUTPUT_COMMAND_ADDUSER_PWD
    },
    {
        {
            "Please confirm the password for the new user:",
            "Veuillez confirmer le mot de passe du nouvel utilisateur:",
        } // OUTPUT_COMMAND_ADDUSER_PWD_CONFIRM
    },
    {
        {
            "New user created",
            "Nouvel utilisateur créé",
        } // OUTPUT_COMMAND_ADDUSER_CREATED
    },
    {
        {
            "Usage: rmuser <login>",
            "Usage: rmuser <identifiant>",
        } // ERROR_COMMAND_RMUSER_USAGE
    },
    {
        {
            "has been removed",
            "a été supprimé",
        } // OUTPUT_COMMAND_RMUSER
    },
    {
        {
            "Unknown command",
            "Commande inconnue",
        } // ERROR_COMMAND_UNKNOWN
    },
    {
        {
            "Usage: help <command name>",
            "Usage: help <nom de la commande>",
        } // ERROR_COMMAND_HELP_USAGE
    },
    {
        {
            "login:",
            "identifiant:",
        } // CONSOLE_LOGIN
    },
    {
        {
            "password:",
            "mot de passe:",
        } // CONSOLE_PWD
    },
    {
        {
            "Booting disk...",
            "Démarrage du disque...",
        } // OUTPUT_OS_STARTING_DISK
    },
    {
        {
            "Starting shell...",
            "Démarrage du shell...",
        } // OUTPUT_OS_STARTING_SHELL
    },
    {
        {
            "Shell return with code",
            "Retour du shell avec le code",
        } // OUTPUT_OS_STOP_SHELL
    },
    {
        {
            "Shutting down...",
            "Arret de l'OS...",
        } // OUTPUT_OS_SHUTDOWN
    },
    {
        {
            "Usage: ./bin/os_installer <disk directory>",
            "Usage: ./bin/os_installer <répertoire du disque>",
        } // WARNING_INSTALLOS_USAGE
    },
    {
        {
            "Cannot read dir",
            "Impossible de lire le répertoire",
        } // ERROR_READ_DIR
    },
    {
        {
            "Cannot close dir",
            "Impossible de fermer le répertoire",
        } // ERROR_CLOSE_DIR
    },
    {
        {
            "Please format disk before installation",
            "Veuillez formater le disque avant l'installation",
        } // OUPTUT_OSINSTALL_FORMAT
    },
    {
        {
            "Accept disk formatting ? y/N",
            "Accepter le formatage du disque ? o/N",
        } // OUPTUT_OSINSTALL_FORMAT_ASK
    },
    {
        {
            "yes",
            "oui",
        } // INPUT_YES
    },
    {
        {
            "y",
            "o",
        } // INPUT_Y
    },
    {
        {
            "no",
            "non",
        } // INPUT_NO
    },
    {
        {
            "n",
            "n",
        } // INPUT_N
    },
    {
        {
            "cancel",
            "annuler",
        } // INPUT_CANCEL
    },
    {
        {
            "Formatting completed successfully",
            "Formatage terminé avec succès",
        } // OUTPUT_OSINSTALL_FORAMT_SUCCESS
    },
    {
        {
            "Installation canceled",
            "Installation annulée",
        } // OUTPUT_OSINSTALL_CANCEL
    },
    {
        {
            "Please enter the superuser nickname (root by defaut):",
            "Veuillez entrer le pseudo du superutilisateur (root par défaut):",
        } // OUTPUT_OSINSTALL_SUPERUSER_NAME
    },
    {
        {
            "Please enter the superuser password:",
            "Veuillez saisir le mot de passe du superutilisateur :",
        } // OUTPUT_OSINSTALL_SUPERUSER_PWD
    },
    {
        {
            "Please confirm the superuser password:",
            "Veuillez confirmer le mot de passe du superutilisateur :",
        } // OUTPUT_OSINSTALL_SUPERUSER_PWD2
    },
    {
        {
            "Installation completed successfully",
            "L'installation s'est terminée avec succès",
        } // OUTPUT_OSINSTALL_SUCCESS
    },
    {
        {
            "Touch any key on the keyboard to exit the program",
            "Appuyez sur n'importe quelle touche du clavier pour quitter le programme",
        } // OUTPUT_PRESS_EXIT
    },
    {
        {
            "Please select the installation disk",
            "Veuillez sélectionner le disque d'installation",
        } // OUTPUT_OSINSTALL_SELECT_DISK_NAME
    },
    {
        {
            "Made by Dwayne & Florian with ❤",
            "Fait par Dwayne & Florian avec ❤",
        } // OUTPUT_CREDITS
    },
    {
        {
            "An os is already installed on the disk. do you want to proceed with formatting the drive",
            "Un système d'exploitation est déjà installé sur le disque. voulez-vous procéder au formatage du disque",
        } // OUTPUT_OSINSTALL_FORMAT_ASK_ALREADY_INSTALLED
    },
    {
        {
            "Please use the up ↑ and down ↓ arrow keys to select your choice",
            "Veuillez utiliser les touches fléchées haut ↑ et bas ↓ pour sélectionner votre choix",
        } // OUTPUT_SELECT_CHOICE_HELP
    },
    {
        {
            "Passwords do not match",
            "Les mots de passe ne correspondent pas",
        } // WARNING_PASSWORD_NOT_MATCH
    },
    {
        {
            "Password must contain at least 8 characters",
            "Le mot de passe doit contenir au moins 8 caractères",
        } // WARNING_PASSWORD_TOO_SHORT
    },
    {
        {
            "Password must contain at least one lowercase letter",
            "Le mot de passe doit contenir au moins une lettre minuscule",
        } // WARNING_PASSWORD_NO_LOWERCASE
    },
    {
        {
            "Password must contain at least one capital letter",
            "Le mot de passe doit contenir au moins une majuscule",
        } // WARNING_PASSWORD_NO_UPPERCASE
    },
    {
        {
            "Password must contain at least one number",
            "le mot de passe doit contenir au moins un chiffre",
        } // WARNING_PASSWORD_NO_NUMBER
    },
    {
        {
            "Usage: ./bin/os <disk directory>",
            "Usage: ./bin/os <repertoire du disque>",
        } // WARNING_OS_USAGE
    },
    {
        {
            "Please select the disk you want to boot",
            "Veuillez sélectionner le disque que vous souhaitez démarrer",
        } // OUTPUT_OS_SELECT_DISK
    },
    {
        {
            "OS not found on disk",
            "OS introuvable sur le disque",
        } // ERROR_OS_NOT_FOUND
    },
    {
        {
            "Please enter the superuser password:",
            "Merci de renseigner le mot de passe du superutilisateur:",
        } // OUTPUT_COMMAND_SUDO_ROOT_PWD_ASK
    },
    {
        {
            "Password incorrect",
            "Mot de passe incorrect",
        } // OUTPUT_COMMAND_SUDO_ROOT_PWD_FAIL
    },
    {
        {
            "Usage: sudo <command name> <args>",
            "Usage: sudo <nom de la commande> <arguments>",
        } // ERROR_COMMAND_SUDO_USAGE
    },
    {
        {
            "Usage: logout",
            "usage: logout",
        } // ERROR_COMMAND_LOGOUT_USAGE
    },
    {
        {
            "Usage: clear",
            "usage: clear",
        } // ERROR_COMMAND_CLEAR_USAGE
    },
    {
        {
            "Login cannot be empty",
            "L'identifiant ne peut pas être vide",
        } // ERROR_COMMAND_ARGS_LOGIN_EMPTY
    },
    {
        {
            "Cannot delete the current user",
            "Impossible de supprimer l'utilisateur courant",
        } // ERROR_COMMAND_ARGS_LOGIN_SELF
    },
    {
        {
            "Cannot delete the superuser",
            "Impossible de supprimer le superutilisateur",
        } // ERROR_COMMAND_ARGS_REMOVE_ROOT
    },
    {
        {
            "Cannot create file with name containing special characters",
            "Impossible de créer un fichier avec un nom contenant des caractères spéciaux",
        } // ERROR_COMMAND_ARGS_FILE_NAME_SPECIAL_CHAR
    },
    {
        {
            "File name cannot contain more than 32 characters",
            "Le nom du fichier ne peut pas contenir plus de 32 caractères",
        } // ERROR_COMMAND_ARGS_FILE_LENGTH
    },
    {
        {
            "File already exists",
            "Le fichier existe déjà",
        } // ERROR_COMMAND_ARGS_FILE_EXIST_ALREADY
    },
    {
        {
            "Login failed, thanks to try again later",
            "Identifiant incorrect, merci de réessayer plus tard",
        } // ERROR_LOGIN_ATTEMPT
    },
    {
            {
                "Usage: rename <old name> <new name>",
                    "Usage: rename <ancien nom> <nouveau nom>",
            } // ERROR_COMMAND_RENAME_USAGE
    },
    {
            {
                    "Usage: cp <file name> <new name>",
                    "Usage: cp <nom fichier> <nouveau nom>",
            } // ERROR_COMMAND_CP_USAGE
    }
};

char const *LangGet(STR_NAME var);

#endif // SCRATCHOS_LABGUAGE_H