#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../headers/config.h"
#include "../headers/display.h"
#include "../headers/sort.h"

// Sort the processes by start time and log the information to console
void log_process_data(DataNode* data_node_head, int process_count) {
    ProcessData* recent_processes = get_top_processes(data_node_head, process_count);
    int num_processes = min(MAX_TRACKING, process_count);
    display_process_data(recent_processes, num_processes);
}

void display_process_data(ProcessData* recent_processes, int num_processes) {
    system("clear");
    printf("%-15s%-10s%-15s%-15s%-10s%-10s%-15s\n", 
        ".EXE NAME", "PID", "CURRENT STATE", "PRIORITY", "% CPU", "CPU", "VRAM (MB)");
    
    for (int i = 0; i < num_processes; i++) { 
        ProcessData pd = recent_processes[i];
        printf("%-15s%-10d%-15s%-15d%-10.2f%-10d%-15ld\n",
            format_filename(pd.exe_filename), pd.pid, pd.process_state, pd.process_priority, pd.cpu_utilisation, pd.last_cpu, pd.v_mem_size);
    }
}

ProcessData* get_top_processes(DataNode* data_node_head, int process_count) { 
    ProcessData* recent_processes;
    int num_processes;
    recent_processes = msort_processes(data_node_head, process_count);
    return recent_processes;
}

// Strip the filename to 20 characters with + at the end
char* format_filename(char* exe_filename) {
    int exe_filename_length = strlen(exe_filename);
    int max_filename_length = 10;
    if (exe_filename_length <= max_filename_length)
        return exe_filename;
    char* new_filename = malloc(max_filename_length+1);
    for (int i = 0; i < max_filename_length-1; i++) { new_filename[i] = exe_filename[i];}
    new_filename[max_filename_length-1] = '+';
    new_filename[max_filename_length] = '\0';
    return new_filename;
}

int min(int a, int b) {
    if (a <=b)
        return a;
    return b;
}