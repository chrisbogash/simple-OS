#ifndef PROCESS_H
#define PROCESS_H

#define MAX_PROCESSES 10
#define MAX_PROCESS_NAME 32

typedef enum {
    PROCESS_NEW,
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_WAITING,
    PROCESS_TERMINATED
} ProcessState;

typedef struct {
    int pid;
    char name[MAX_PROCESS_NAME];
    ProcessState state;
    int priority;
    int cpu_time;
    int created_at;
} Process;

// Function declarations
void init_process_manager();
void cleanup_process_manager();
void create_process(const char* name);
void kill_process(int pid);
void list_processes();
void schedule_processes();

#endif
