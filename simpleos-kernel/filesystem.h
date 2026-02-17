#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#define MAX_FILES 20
#define MAX_FILENAME 32
#define MAX_FILE_SIZE 1024

typedef struct {
    char filename[MAX_FILENAME];
    char data[MAX_FILE_SIZE];
    int size;
    int created_at;
    int modified_at;
    int in_use;
} File;

// Function declarations
void init_filesystem();
void cleanup_filesystem();
void create_file(const char* filename);
void write_file(const char* filename, const char* data);
void read_file(const char* filename);
void delete_file(const char* filename);
void list_files();

#endif
