/**
 * @file os.c
 * @author  HERZBERG Dwayne and BERLIN Florian
 * @brief main file of the OS
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

int main(int argc, char **argv) {
    if (argc != 2) { fprintf(stderr, ERROR_OS_USAGE, argv[0]); return ERROR; }
    fprintf(stdout, OUTPUT_OS_STARTING_DISK);
    init_disk_sos(argv[1]);
    fprintf(stdout, OUTPUT_OS_STARTING_SHELL);
    fprintf(stdout, OUTPUT_OS_STOP_SHELL, terminal_shell());
    fprintf(stdout, OUTPUT_OS_SHUTDOWN);
    return shutdown_disk_sos();
}