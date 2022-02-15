#include "../headers/layer1.h"
#include "../headers/layer2.h"
#include "../headers/layer3.h"
#include "../headers/layer4.h"
#include "../headers/layer5.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "[USAGE] %s disk_path", argv[0]);
        return ERROR;
    }
    fprintf(stdout, "Starting disk...\n");
    init_disk_sos(argv[1]);
    fprintf(stdout, "Starting shell...\n");
    int retur = terminal_shell();
    fprintf(stdout, "Shell return with %d code\n", retur);
    fprintf(stdout, "Shutting down...\n");
    shutdown_disk_sos();
    return SUCCESS;
}