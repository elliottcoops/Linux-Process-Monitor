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

    // Error handling
    if (!dp) {
        printf("{Error} - NO DIRECTORY DETECTED\n");
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
    return f;
}

// Process a file entry
ProcessData* process_file(char* buffer) {
    ProcessData* process_data = malloc(sizeof(ProcessData));
    char* token = strtok(buffer, " ");
    int entry = 0;

    while (token != NULL) { 
        get_file_entry(process_data, entry, token); // Get relevant entry
        token = strtok(NULL, " "); // Get next token
        entry++; // Increase entry count
    }
    return process_data;
}

// Get an entry from a file (pid, time etc)
void get_file_entry(ProcessData* process_data, int entry, char* token) {
    switch (entry) {
        case 0:
            process_data->pid = atoi(token);
            break;
        case 1:
            process_data->exe_filename = token;
            break;
        case 2:
            process_data->process_state = token;
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
            process_data->v_mem_size = atol(token);
            break;
        case 38:
            process_data->last_cpu = atoi(token);
            break;
        default:
            break;
    }
}

void read_and_log(DataNode* data_node, ProcessData* process_data, char* buffer, FILE* f) {
    fgets(buffer, 1024, f); // Read line
    process_data = process_file(buffer); // Log data
    data_node->process_data = process_data;
    data_node->next_data_node = malloc(sizeof(DataNode));
    data_node = data_node->next_data_node;
}

