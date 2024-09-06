#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "../headers/config.h"
#include "../headers/file_handler.h"
#include "../headers/sort.h"

// Open the proc linux directory
DIR* open_proc_dir() {
    DIR *dp = opendir("/proc");
    if (!dp) {
        log_error("Cannot open /proc directory");
        return NULL;
    } 
    return dp;
}

// Get the pid from an active process
int get_pid(struct dirent* entry) {
    if (entry->d_type == DT_DIR) 
        return atoi(entry->d_name);
    return -1;
}

// Open the pid stats file
FILE* open_pid_stats(int pid) {
    char cmd_line_path[256];
    sprintf(cmd_line_path, "/proc/%d/stat", pid);
    FILE *f = fopen(cmd_line_path, "r");
    if (!f) { 
        char err_buffer[300]; 
        sprintf(err_buffer, "Cannot open %s\n", cmd_line_path);
        log_error(err_buffer);
        return NULL;
    }
    return f;
}

// Process a file entry
ProcessData* process_file(char* buffer) {
    ProcessData* process_data = get_process_data();
    if (!process_data) {
        log_error("Failed to allocate memory for process data in process_file\n");
        return NULL;
    }
    char* token = strtok(buffer, " ");
    int entry = 0;
    while (token != NULL) { 
        get_file_entry(process_data, entry, token); // Get relevant entry
        token = strtok(NULL, " "); // Get next token
        entry++; // Increase entry count
    }
    process_data->logged_clock_time = get_cpu_time();
    return process_data;
}

// Get an entry from a file (pid, time etc)
void get_file_entry(ProcessData* process_data, int entry, char* token) {
    switch (entry) {
        case 0:
            process_data->pid = atoi(token);
            break;
        case 1:
            strip_filename(token);
            process_data->exe_filename = malloc(strlen(token)+1);
            if (!process_data->exe_filename) 
                log_error("Failed to allocate memory for process_data->exe_file in get_file_entry\n");
            strcpy(process_data->exe_filename, token);
            break;
        case 2:
            process_data->process_state = malloc(strlen(token)+1);
            if (!process_data->process_state) 
                log_error("Failed to allocate memory for process_data->exe_file in get_file_entry\n");
            strcpy(process_data->process_state, token);
            break;
        case 13:
            process_data->usr_cpu_clocks = atol(token);
            break;
        case 14:
            process_data->krnl_cpu_clocks = atol(token);
            break;
        case 21:
            process_data->cpu_start_time = atol(token);
            break;
        case 17:
            process_data->process_priority = atoi(token);
            break;
        case 22:
            process_data->v_mem_size = atol(token) / 1048576;
            break;
        case 38:
            process_data->last_cpu = atoi(token);
            break;
        default:
            break;
    }
}

void read_and_log(DataNode* data_node, ProcessData* process_data, char* buffer, FILE* f) {
    if (!(fgets(buffer, 1024, f))) {
        log_error("Failed to read line in read_and_log\n");
        return;
    }
    if (!(process_data = process_file(buffer))) {
        log_error("Failed to process file in read_and_log\n");
        return;
    } 
    data_node->process_data = process_data;
    data_node->next_data_node = get_data_node();
    if (!data_node->next_data_node)
        log_error("Failed to allocate memory for next data node in read_and_log\n");
}

// Read the proc stat to get total cpu time
long get_cpu_time() {
    char cmd_line_path[256];
    sprintf(cmd_line_path, "/proc/stat");
    FILE *f = fopen(cmd_line_path, "r");
    char buffer[512];
    fgets(buffer, 512, f);
    long total =0;
    char* token = strtok(buffer, " ");
    int entry = 0;
    while (token != NULL) { 
        if (entry>0)
            total+=atol(token);
        token = strtok(NULL, " "); // Get next token
        entry++; // Increase entry count
    }
    fclose(f);
    
    return total;
}

long get_process_cpu_time(int pid) {
    FILE* pid_stats;
    if ((pid_stats = open_pid_stats(pid))==NULL){ return -1;}
    char buffer[512];
    fgets(buffer, 512, pid_stats);
    char* token = strtok(buffer, " ");
    long total = 0;
    int entry = 0;
    while (token != NULL) { 
        if (entry == 13 || entry == 14){ total += atol(token);}
        token = strtok(NULL, " "); // Get next token
        entry++; // Increase entry count
    }
    fclose(pid_stats);
    return total;

}

void strip_filename(char* filename) {
    char *src = filename; 
    char *dst = filename;

    while (*src != '\0') { 
        if (*src != '(' && *src != ')') { 
            *dst++ = *src; 
        }
        src++; 
    }
    *dst = '\0'; 
}

