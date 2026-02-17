# ============================================
# SIMPLEOS KERNEL - COMPLETE DEMO SCRIPT
# ============================================

# START THE KERNEL
./simpleos

# ============================================
# STEP 1: INTRODUCTION & HELP
# ============================================
SimpleOS> help
# This shows all available commands - demonstrates good UI design

# ============================================
# STEP 2: PROCESS CREATION
# ============================================
SimpleOS> create webserver
# Creates process with random priority (1-5)

SimpleOS> create database
# Creates second process

SimpleOS> create cache
# Creates third process

SimpleOS> create logger
# Creates fourth process

# ============================================
# STEP 3: VIEW PROCESS TABLE
# ============================================
SimpleOS> list
# Shows all processes in READY state with:
# - PID (unique identifier)
# - Name
# - State (all should be READY)
# - Priority (random 1-5)
# - CPU Time (all 0 initially)

# ============================================
# STEP 4: DEMONSTRATE SCHEDULING (Round-Robin)
# ============================================
SimpleOS> schedule
# Runs PID 1, increments its CPU time to 1

SimpleOS> schedule
# Runs PID 2, increments its CPU time to 1

SimpleOS> schedule
# Runs PID 3, increments its CPU time to 1

SimpleOS> schedule
# Runs PID 4, increments its CPU time to 1

SimpleOS> schedule
# Back to PID 1 (round-robin), CPU time becomes 2

# ============================================
# STEP 5: OBSERVE STATE CHANGES
# ============================================
SimpleOS> list
# Shows processes with updated CPU times
# May show some as RUNNING or WAITING (simulated I/O)
# Demonstrates state transitions

# ============================================
# STEP 6: MORE SCHEDULING (Show I/O & Termination)
# ============================================
SimpleOS> schedule
SimpleOS> schedule
SimpleOS> schedule
SimpleOS> schedule
SimpleOS> schedule
# Keep scheduling - you may see:
# - Processes go to WAITING state (I/O simulation)
# - Processes return to READY from WAITING
# - After 10+ CPU time, processes auto-terminate

SimpleOS> list
# Shows current state after multiple scheduler ticks

# ============================================
# STEP 7: PROCESS TERMINATION
# ============================================
SimpleOS> kill 1
# Manually kills PID 1

SimpleOS> list
# Shows PID 1 is gone, other processes remain

# ============================================
# STEP 8: FILE SYSTEM - CREATE FILES
# ============================================
SimpleOS> touch config.txt
# Creates empty file

SimpleOS> touch database.cfg
# Creates another file

SimpleOS> touch app.log
# Creates third file

# ============================================
# STEP 9: WRITE DATA TO FILES
# ============================================
SimpleOS> write config.txt port=8080 host=localhost debug=true
# Writes configuration data

SimpleOS> write database.cfg user=admin password=secret123 db=production
# Writes database config

SimpleOS> write app.log [2026-02-17] System started successfully
# Writes log entry

# ============================================
# STEP 10: LIST FILES WITH METADATA
# ============================================
SimpleOS> ls
# Shows all files with:
# - Filename
# - Size (in bytes)
# - Creation timestamp
# - Modification timestamp

# ============================================
# STEP 11: READ FILE CONTENTS
# ============================================
SimpleOS> read config.txt
# Displays file contents

SimpleOS> read database.cfg
# Displays database config

SimpleOS> read app.log
# Displays log file

# ============================================
# STEP 12: MODIFY FILE (Update)
# ============================================
SimpleOS> write config.txt port=9090 host=0.0.0.0 debug=false
# Overwrites config.txt with new data

SimpleOS> read config.txt
# Shows updated content

SimpleOS> ls
# Notice modified timestamp changed for config.txt

# ============================================
# STEP 13: DELETE FILE
# ============================================
SimpleOS> rm app.log
# Deletes the log file

SimpleOS> ls
# Shows app.log is gone, others remain

# ============================================
# STEP 14: COMBINED OPERATIONS
# ============================================
SimpleOS> create worker-1
# Create new process

SimpleOS> create worker-2
# Create another process

SimpleOS> list
# Shows all current processes

SimpleOS> schedule
SimpleOS> schedule
# Run some scheduler ticks

SimpleOS> touch results.dat
# Create new file

SimpleOS> write results.dat task1:complete task2:pending
# Write results

SimpleOS> list
# Show process states

SimpleOS> ls
# Show file list

# ============================================
# STEP 15: FINAL STATE CHECK
# ============================================
SimpleOS> list
# Shows all remaining processes

SimpleOS> ls
# Shows all remaining files

# ============================================
# STEP 16: GRACEFUL SHUTDOWN
# ============================================
SimpleOS> exit
# Demonstrates:
# - Proper cleanup of all processes
# - File system unmounting
# - Graceful shutdown sequence
```

---

# Talking Points While Demoing

## Introduction (30 seconds)
> "I've created SimpleOS, an educational operating system kernel in C that demonstrates core OS concepts: process management and file systems."

## During Process Demo (2 minutes)
> "The kernel supports up to 10 concurrent processes, each with its own PID, state, and priority. The round-robin scheduler ensures fair CPU time allocation."

**Point out:**
- Process states: NEW → READY → RUNNING → WAITING → TERMINATED
- Round-robin ensures no process starves
- Random I/O simulation shows realistic behavior
- Processes auto-terminate after 10 CPU time units

## During File System Demo (2 minutes)
> "The file system provides basic CRUD operations with automatic metadata tracking."

**Point out:**
- In-memory storage (up to 20 files, 1KB each)
- Automatic timestamp tracking
- File modification updates timestamp
- Supports up to 31-character filenames

## During Combined Demo (1 minute)
> "The kernel manages both subsystems simultaneously, showing how a real OS handles multiple resources."

## During Shutdown (30 seconds)
> "Notice the graceful shutdown - all processes are properly terminated and the file system is unmounted cleanly."

---

# Quick Reference Card

Print this or have it handy:
```
KEY FEATURES TO HIGHLIGHT:
✓ Process Management (10 max processes)
✓ Round-Robin Scheduling
✓ 5 Process States
✓ Priority Assignment (1-5)
✓ CPU Time Tracking
✓ File System (20 max files)
✓ CRUD Operations
✓ Metadata Tracking
✓ Graceful Shutdown

ARCHITECTURE:
- kernel.c: Main loop, boot, shutdown
- process.c/h: Process management
- filesystem.c/h: File operations
- Makefile: Build system

CODE STATS:
~500 lines of C code
Modular design
Static memory allocation
No external dependencies