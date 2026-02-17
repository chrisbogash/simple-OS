# SimpleOS - Educational Operating System Kernel

A simple educational operating system kernel written in C that demonstrates core OS concepts including process management and file system operations.

## Building and Running

### Compilation
```bash
make
```

### Running
```bash
./simpleos
```

### Cleaning
```bash
make clean
```

## Available Commands

### Process Management
- `create <name>` - Create a new process
- `list` - List all processes
- `kill <pid>` - Kill a process
- `schedule` - Run scheduler tick

### File System
- `touch <filename>` - Create a file
- `write <filename> <data>` - Write to a file
- `read <filename>` - Read a file
- `ls` - List files
- `rm <filename>` - Delete a file

### System
- `help` - Show help
- `clear` - Clear screen
- `exit` - Shutdown kernel

## Example Usage
```
SimpleOS> create webserver
SimpleOS> create database
SimpleOS> list
SimpleOS> schedule
SimpleOS> touch config.txt
SimpleOS> write config.txt port=8080
SimpleOS> read config.txt
SimpleOS> ls
```
