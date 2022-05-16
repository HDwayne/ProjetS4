# Scratch OS
HERZBERG Dwayne (n°etudiant Dwayne) \
BERLIN Florian (22006270) \
TPA12

## Hiérarchisation

./ProjetS4 | Dossier principal contenant tous les fichiers et dossiers du projet 
- ./app/src | Dossier contenant les codes sources du programme JAVA pour l'analyse du système d'exploitation 
  - ./application | Dossier contenant les codes sources du package application nécessaire au fonctionnement du programme JAVA \
    - ./Block.java | Fichier contenant le code source de la classe Block 
    - ./File.java  | Fichier contenant le code source de la classe File 
    - ./Inode.java | Fichier contenant le code source de la classe Inode 
    - ./MainForm.form | Fichier contenant le nécessaire pour l'interface graphique du programme 
    - ./MainForm.java | Fichier contenant le code source de l'interface graphique 
    - ./OsDefines.java | Fichier contenant toutes les variables globales nécessaire au programme 
    - ./SuperBlock.java | Fichier contenant le code source de la classe SuperBlock 
    - ./User.java       | Fichier contenant le code source de la classe User 
    - ./VirtualDisk.java | Fichier contenant le code source de la classe VirtualDisk 
  - ./META-INF/MANIFEST.MF | Fichier contenant les informations pour générer l'exécutable du programme
- ./bin | Dossier contenant les exécutables du programme JAVA et C 
  - ./app.jar| Exécutable du programme d'analyse en JAVA 
  - ./dump_file | Exécutable du fichier de test sur la couche 4 (fichiers) 
  - ./dump_inode | Exécutable du fichier de test sur la couche 2 (inodes) 
  - ./dump_user | Exécutable du fichier de test sur la couche 1 (utilisateurs) 
  - ./os | Exécutable du système d'exploitation 
  - ./os_installer | Exécutable de l'installeur du système d'exploitation 
- ./Format | Dossier contenant les fichiers nécessaires au formatage du disque dur virtuel
  - ./cmd_format.c | Code source du programme de formatage du disque dur virtuel
  - ./MakeFil | MakeFile nécessaire au programme de formatage du disque dur virtuel
- ./headers | Dossier contenant les headers nécessaires au fonctionnement du système d'exploitation
  - ./language.h | Header contenant les définitions des textes utilisés dans le système d'exploitation 
  - ./layer1.h | Header contenant les définitions nécessaires à la couche 1 du système d'exploitation (gestion des blocks, de l'initialisation et de l'extinction de l'os)
  - ./layer2.h | Header contenant les définitions nécessaires à la couche 2 du système d'exploitation (gestion des inodes)
  - ./layer3.h | Header contenant les définitions nécessaires à la couche 3 du système d'exploitation (gestion des utilisateurs)
  - ./layer4.h | Header contenant les définitions nécessaires à la couche 4 du système d'exploitation (gestion des fichiers)
  - ./layer5.h | Header contenant les définitions nécessaires au terminal du système d'exploitation (commandes, terminal)
  - ./os_defines.h | Header contenant les définitions nécessaires au système d'exploitation
  - ./sha256.h & ./sha256_utils.h | Header contenant au cryptage SHA256 des mots de passe
  - ./terminal.h | Header contenant les définitions pour l'affichage du terminal
  - ./timestamp.h | Header contenant les définitions pour la gestion des dates
- ./repdisk | Dossier contenant les disques dur virtuels
- ./sources | Dossier contenant les codes sources nécessaires au fonctionnement du système d'exploitation
  - ./cmd_dump_file.c | Code source du programme test de la couche 4 (fichiers)
  - ./cmd_dump_inode.c | Code source du programme test de la couche 2 (inodes)
  - ./cmd_dump_user.c | Code source du programme test de la couche 1 (utilisateurs)
  - ./installos.c | Code source du programme d'installation de l'OS
  - ./language.c | Code source du programme de la gestion des langues
  - ./layer1.c | Code source de la couche 1 du système d'exploitation (gestion des blocks, de l'initialisation et de l'extinction de l'os)
  - ./layer2.c | Code source de la couche 2 du système d'exploitation (gestion des inodes)
  - ./layer3.c | Code source de la couche 3 du système d'exploitation (gestion des utilisateurs)
  - ./layer4.c | Code source de la couche 4 du système d'exploitation (gestion des fichiers)
  - ./layer5.c | Code source du terminal du système d'exploitation (commandes, terminal)
  - ./os.c | Code source du système d'exploitation
  - ./sha256.c & ./sha256_utils.c | Code source du cryptage SHA256 des mots de passe
  - ./terminal.c | Code source de l'affichage du terminal
  - ./timestamp.c | Code source de la gestion des dates
- ./.gitignore | Fichier contenant les fichiers à ignorer lors de la sauvegarde du projet
- ./MakeFile | MakeFile permettant de compiler la partie C du projet
- ./Readme.md | Fichier contenant les informations sur le projet
- ./TDA1-TDB-Projet-SOSS4-capture.xlsx | Fichier de suivi de gestion du projet

## Compilation et exécution du projet

### Compilation
#### Compilation du programme de formatage du disque dur virtuel
  > cd Format \
  > make \
  > cd ..

#### Compilation du système d'exploitation, de l'installeur et des programmes test + Formatage des disques virtuels
  > make 

### Exécution

#### Exécution de l'installeur du système d'exploitation
  > ./bin/os_installer ./repdisk

#### Exécution du système d'exploitation'
  > ./bin/os ./repdisk

#### Exécution du programme d'analyse et défragmentation JAVA
  > java -jar ./bin/app.jar


## Détails du MakeFile

#### Générer un disque virtuel
  > make disk

#### Compiler le système d'exploitation, l'installeur et formater les disques
>make sys

OU

> make clean \
> make os \
> make installer \
> make disk 

#### Compiler les programmes de test

> make dump

OU 

> make dump_file \
> make dump_inode \
> make dump_user 

## Documentation système d'exploitation

### Limites

  - 10 fichiers maximum (modifiable dans os_defines.h)
  - 5 utilisateurs maximum (modifiable dans os_defines.h)

### Commandes utilisables

- Afficher la liste des commandes utilisables dans le système d'exploitation

  > help 

## Version des langages utilisés:
  - C: C99;
  - Java: OpenJDK 17.0.2;