#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "process.h"

static Process process_table[MAX_PROCESSES];
static int next_pid = 1;
static int current_process_index = -1;
static int process_count = 0;

void init_process_manager() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].pid = -1;
        process_table[i].state = PROCESS_TERMINATED;
        memset(process_table[i].name, 0, MAX_PROCESS_NAME);
    }
    next_pid = 1;
    current_process_index = -1;
    process_count = 0;
}

void cleanup_process_manager() {
    // Clean up any running processes
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid != -1) {
            printf("[PROCESS] Terminating PID %d (%s)\n", 
                   process_table[i].pid, process_table[i].name);
            process_table[i].state = PROCESS_TERMINATED;
        }
    }
}

void create_process(const char* name) {
    if (process_count >= MAX_PROCESSES) {
        printf("[PROCESS] Error: Process table full!\n");
        return;
    }
    
    // Find free slot
    int slot = -1;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid == -1) {
            slot = i;
            break;
        }
    }
    
    if (slot == -1) {
        printf("[PROCESS] Error: No free slots available!\n");
        return;
    }
    
    // Create process
    Process* proc = &process_table[slot];
    proc->pid = next_pid++;
    strncpy(proc->name, name, MAX_PROCESS_NAME - 1);
    proc->name[MAX_PROCESS_NAME - 1] = '\0';
    proc->state = PROCESS_READY;
    proc->priority = 1 + (rand() % 5); // Random priority 1-5
    proc->cpu_time = 0;
    proc->created_at = (int)time(NULL);
    
    process_count++;
    
    printf("[PROCESS] Created process PID %d: %s (Priority: %d)\n", 
           proc->pid, proc->name, proc->priority);
}

void kill_process(int pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid == pid) {
            printf("[PROCESS] Killing process PID %d (%s)\n", 
                   pid, process_table[i].name);
            
            process_table[i].state = PROCESS_TERMINATED;
            process_table[i].pid = -1;
            process_count--;
            
            if (current_process_index == i) {
                current_process_index = -1;
            }
            return;
        }
    }
    
    printf("[PROCESS] Error: Process PID %d not found!\n", pid);
}

const char* state_to_string(ProcessState state) {
    switch (state) {
        case PROCESS_NEW: return "NEW";
        case PROCESS_READY: return "READY";
        case PROCESS_RUNNING: return "RUNNING";
        case PROCESS_WAITING: return "WAITING";
        case PROCESS_TERMINATED: return "TERMINATED";
        default: return "UNKNOWN";
    }
}

void list_processes() {
    printf("\n");
    printf("PID   NAME                    STATE       PRIORITY  CPU_TIME\n");
    printf("----------------------------------------------------------------\n");
    
    int found = 0;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid != -1) {
            printf("%-5d %-23s %-11s %-9d %d\n",
                   process_table[i].pid,
                   process_table[i].name,
                   state_to_string(process_table[i].state),
                   process_table[i].priority,
                   process_table[i].cpu_time);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No processes running.\n");
    }
    printf("\n");
}

// Simple round-robin scheduler
void schedule_processes() {
    if (process_count == 0) {
        printf("[SCHEDULER] No processes to schedule.\n");
        return;
    }
    
    // Set current process back to READY if it was RUNNING
    if (current_process_index != -1) {
        if (process_table[current_process_index].state == PROCESS_RUNNING) {
            process_table[current_process_index].state = PROCESS_READY;
        }
    }
    
    // Find next READY process (round-robin)
    int start = (current_process_index + 1) % MAX_PROCESSES;
    int checked = 0;
    
    while (checked < MAX_PROCESSES) {
        int i = (start + checked) % MAX_PROCESSES;
        
        if (process_table[i].pid != -1 && 
            process_table[i].state == PROCESS_READY) {
            
            // Schedule this process
            current_process_index = i;
            process_table[i].state = PROCESS_RUNNING;
            process_table[i].cpu_time++;
            
            printf("[SCHEDULER] Running PID %d (%s) - CPU Time: %d\n",
                   process_table[i].pid,
                   process_table[i].name,
                   process_table[i].cpu_time);
            
            // Simulate some work
            if (rand() % 3 == 0) {
                process_table[i].state = PROCESS_WAITING;
                printf("[SCHEDULER] PID %d is now WAITING (I/O)\n", 
                       process_table[i].pid);
            } else if (process_table[i].cpu_time > 10) {
                printf("[SCHEDULER] PID %d completed execution\n", 
                       process_table[i].pid);
                process_table[i].state = PROCESS_TERMINATED;
                process_table[i].pid = -1;
                process_count--;
                current_process_index = -1;
            }
            
            return;
        }
        
        checked++;
    }
    
    // Check for WAITING processes to wake up
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid != -1 && 
            process_table[i].state == PROCESS_WAITING) {
            if (rand() % 2 == 0) {
                process_table[i].state = PROCESS_READY;
                printf("[SCHEDULER] PID %d is now READY (I/O complete)\n", 
                       process_table[i].pid);
            }
        }
    }
    
    printf("[SCHEDULER] No READY processes, system idle.\n");
}
