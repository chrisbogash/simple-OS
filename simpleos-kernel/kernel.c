#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "process.h"
#include "filesystem.h"

#define KERNEL_VERSION "0.1.0"

// Kernel state
typedef struct {
    int running;
    int tick_count;
} KernelState;

KernelState kernel_state;

// Forward declarations
void kernel_boot();
void kernel_shutdown();
void print_banner();
void command_loop();

int main() {
    kernel_boot();
    command_loop();
    kernel_shutdown();
    return 0;
}

void kernel_boot() {
    print_banner();
    printf("[KERNEL] Booting SimpleOS v%s...\n", KERNEL_VERSION);
    
    // Initialize subsystems
    kernel_state.running = 1;
    kernel_state.tick_count = 0;
    
    printf("[KERNEL] Initializing process management...\n");
    init_process_manager();
    
    printf("[KERNEL] Initializing file system...\n");
    init_filesystem();
    
    printf("[KERNEL] Boot complete!\n\n");
}

void kernel_shutdown() {
    printf("\n[KERNEL] Shutting down...\n");
    
    printf("[KERNEL] Cleaning up processes...\n");
    cleanup_process_manager();
    
    printf("[KERNEL] Unmounting file system...\n");
    cleanup_filesystem();
    
    printf("[KERNEL] Shutdown complete. Goodbye!\n");
}

void print_banner() {
    printf("\n");
    printf("  _____ _                 _       ___  _____ \n");
    printf(" /  ___(_)               | |     / _ \\|  ___|\n");
    printf(" \\ `--. _ _ __ ___  _ __ | | ___/ /_\\ \\ `--. \n");
    printf("  `--. \\ | '_ ` _ \\| '_ \\| |/ _ \\  _  |`--. \\\n");
    printf(" /\\__/ / | | | | | | |_) | |  __/ | | /\\__/ /\n");
    printf(" \\____/|_|_| |_| |_| .__/|_|\\___|_| |_\\____/ \n");
    printf("                   | |                        \n");
    printf("                   |_|                        \n\n");
}

void print_help() {
    printf("\nAvailable commands:\n");
    printf("  Process Management:\n");
    printf("    create <name>     - Create a new process\n");
    printf("    list              - List all processes\n");
    printf("    kill <pid>        - Kill a process\n");
    printf("    schedule          - Run scheduler tick\n");
    printf("\n");
    printf("  File System:\n");
    printf("    touch <filename>  - Create a file\n");
    printf("    write <file> <data> - Write to a file\n");
    printf("    read <filename>   - Read a file\n");
    printf("    ls                - List files\n");
    printf("    rm <filename>     - Remove a file\n");
    printf("\n");
    printf("  System:\n");
    printf("    help              - Show this help\n");
    printf("    clear             - Clear screen\n");
    printf("    exit              - Shutdown kernel\n\n");
}

void command_loop() {
    char input[256];
    char command[64];
    char arg1[128];
    char arg2[128];
    
    print_help();
    
    while (kernel_state.running) {
        printf("SimpleOS> ");
        fflush(stdout);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        // Remove newline
        input[strcspn(input, "\n")] = 0;
        
        // Skip empty input
        if (strlen(input) == 0) {
            continue;
        }
        
        // Parse command
        memset(command, 0, sizeof(command));
        memset(arg1, 0, sizeof(arg1));
        memset(arg2, 0, sizeof(arg2));
        
        int parsed = sscanf(input, "%s %s %[^\n]", command, arg1, arg2);
        
        if (parsed < 1) {
            continue;
        }
        
        // Process commands
        if (strcmp(command, "help") == 0) {
            print_help();
        }
        else if (strcmp(command, "exit") == 0) {
            kernel_state.running = 0;
        }
        else if (strcmp(command, "clear") == 0) {
            system("clear");
            print_banner();
        }
        // Process commands
        else if (strcmp(command, "create") == 0) {
            if (strlen(arg1) > 0) {
                create_process(arg1);
            } else {
                printf("Usage: create <name>\n");
            }
        }
        else if (strcmp(command, "list") == 0) {
            list_processes();
        }
        else if (strcmp(command, "kill") == 0) {
            if (strlen(arg1) > 0) {
                int pid = atoi(arg1);
                kill_process(pid);
            } else {
                printf("Usage: kill <pid>\n");
            }
        }
        else if (strcmp(command, "schedule") == 0) {
            schedule_processes();
        }
        // File system commands
        else if (strcmp(command, "touch") == 0) {
            if (strlen(arg1) > 0) {
                create_file(arg1);
            } else {
                printf("Usage: touch <filename>\n");
            }
        }
        else if (strcmp(command, "write") == 0) {
            if (strlen(arg1) > 0 && strlen(arg2) > 0) {
                write_file(arg1, arg2);
            } else {
                printf("Usage: write <filename> <data>\n");
            }
        }
        else if (strcmp(command, "read") == 0) {
            if (strlen(arg1) > 0) {
                read_file(arg1);
            } else {
                printf("Usage: read <filename>\n");
            }
        }
        else if (strcmp(command, "ls") == 0) {
            list_files();
        }
        else if (strcmp(command, "rm") == 0) {
            if (strlen(arg1) > 0) {
                delete_file(arg1);
            } else {
                printf("Usage: rm <filename>\n");
            }
        }
        else {
            printf("Unknown command: %s\n", command);
            printf("Type 'help' for available commands.\n");
        }
    }
}
