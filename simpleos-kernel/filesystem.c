#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "filesystem.h"

static File file_table[MAX_FILES];
static int file_count = 0;

void init_filesystem() {
    for (int i = 0; i < MAX_FILES; i++) {
        memset(file_table[i].filename, 0, MAX_FILENAME);
        memset(file_table[i].data, 0, MAX_FILE_SIZE);
        file_table[i].size = 0;
        file_table[i].created_at = 0;
        file_table[i].modified_at = 0;
        file_table[i].in_use = 0;
    }
    file_count = 0;
}

void cleanup_filesystem() {
    // Nothing to clean up in this simple implementation
    printf("[FILESYSTEM] %d files in system\n", file_count);
}

int find_file(const char* filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (file_table[i].in_use && 
            strcmp(file_table[i].filename, filename) == 0) {
            return i;
        }
    }
    return -1;
}

void create_file(const char* filename) {
    // Check if file already exists
    if (find_file(filename) != -1) {
        printf("[FILESYSTEM] Error: File '%s' already exists!\n", filename);
        return;
    }
    
    if (file_count >= MAX_FILES) {
        printf("[FILESYSTEM] Error: File system full!\n");
        return;
    }
    
    // Find free slot
    int slot = -1;
    for (int i = 0; i < MAX_FILES; i++) {
        if (!file_table[i].in_use) {
            slot = i;
            break;
        }
    }
    
    if (slot == -1) {
        printf("[FILESYSTEM] Error: No free slots!\n");
        return;
    }
    
    // Create file
    File* file = &file_table[slot];
    strncpy(file->filename, filename, MAX_FILENAME - 1);
    file->filename[MAX_FILENAME - 1] = '\0';
    file->size = 0;
    file->created_at = (int)time(NULL);
    file->modified_at = file->created_at;
    file->in_use = 1;
    memset(file->data, 0, MAX_FILE_SIZE);
    
    file_count++;
    
    printf("[FILESYSTEM] Created file: %s\n", filename);
}

void write_file(const char* filename, const char* data) {
    int index = find_file(filename);
    
    if (index == -1) {
        printf("[FILESYSTEM] Error: File '%s' not found!\n", filename);
        return;
    }
    
    File* file = &file_table[index];
    
    int data_len = strlen(data);
    if (data_len >= MAX_FILE_SIZE) {
        printf("[FILESYSTEM] Error: Data too large! Max size: %d bytes\n", 
               MAX_FILE_SIZE - 1);
        return;
    }
    
    strncpy(file->data, data, MAX_FILE_SIZE - 1);
    file->data[MAX_FILE_SIZE - 1] = '\0';
    file->size = strlen(file->data);
    file->modified_at = (int)time(NULL);
    
    printf("[FILESYSTEM] Wrote %d bytes to %s\n", file->size, filename);
}

void read_file(const char* filename) {
    int index = find_file(filename);
    
    if (index == -1) {
        printf("[FILESYSTEM] Error: File '%s' not found!\n", filename);
        return;
    }
    
    File* file = &file_table[index];
    
    printf("\n--- %s (%d bytes) ---\n", filename, file->size);
    if (file->size > 0) {
        printf("%s\n", file->data);
    } else {
        printf("(empty file)\n");
    }
    printf("---\n\n");
}

void delete_file(const char* filename) {
    int index = find_file(filename);
    
    if (index == -1) {
        printf("[FILESYSTEM] Error: File '%s' not found!\n", filename);
        return;
    }
    
    File* file = &file_table[index];
    printf("[FILESYSTEM] Deleting file: %s\n", filename);
    
    file->in_use = 0;
    memset(file->filename, 0, MAX_FILENAME);
    memset(file->data, 0, MAX_FILE_SIZE);
    file->size = 0;
    
    file_count--;
}

void list_files() {
    printf("\n");
    printf("FILENAME                        SIZE    CREATED             MODIFIED\n");
    printf("------------------------------------------------------------------------\n");
    
    int found = 0;
    for (int i = 0; i < MAX_FILES; i++) {
        if (file_table[i].in_use) {
            char created[32], modified[32];
            time_t ct = file_table[i].created_at;
            time_t mt = file_table[i].modified_at;
            
            strftime(created, sizeof(created), "%Y-%m-%d %H:%M", localtime(&ct));
            strftime(modified, sizeof(modified), "%Y-%m-%d %H:%M", localtime(&mt));
            
            printf("%-31s %-7d %-19s %-19s\n",
                   file_table[i].filename,
                   file_table[i].size,
                   created,
                   modified);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No files in file system.\n");
    }
    printf("\n");
}
