#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// create a file named di (i = diskid) in directory dirname of size bytes
// the file is initialized with 0
// dirname must be a valid directory
// 0 <= diskid < 10
void format(char *dirname, int size, int diskid){
  char filename[strlen(dirname)+4];
  snprintf(filename, strlen(dirname)+4, "%s/d%d", dirname, diskid);
  // printf("%s\n", filename);
  FILE *fp = fopen(filename, "w");
  assert(fp!=NULL);
  unsigned char zero=0;
  for(int i=0; i<size; i++) {
    int nb_write = fwrite(&zero, 1, 1, fp);
    assert(nb_write == 1);
  }
  fclose(fp);
}

int main(int argc, char **argv) {
/// \brief command nom_repertoire taille_fichier (octets)
    if (argc!=4) {
        printf("usage %s  existing_dir_to_store_disk disk_size disk_kid\n", argv[0]);
        return 0;
    }
    int size = atoi(argv[2]);
    int diskid = atoi(argv[3]);
    format(argv[1], size, diskid);
}
