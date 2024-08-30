#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "../headers/config.h"
#include "../headers/sort.h"
#include "../headers/file_handler.h"

int main(int argc, char** argv) {

    DIR* dp;
    struct dirent *entry;
    int process_count, pid;

    DataNode* data_node_head = NULL;
    DataNode* data_node = NULL;

    dp = open_proc_dir();

    data_node_head = malloc(sizeof(DataNode));
    data_node = data_node_head;

    process_count = 0;

    while (entry = readdir(dp)){
        
        pid = get_pid(entry);
        
        if (pid > 0) {
            FILE* f;
            ProcessData* process_data;
            char buffer[1024];
            
            f = open_pid_stats(pid); // Open pid stats file
            fgets(buffer, 1024, f); // Read line

            process_data = entries(buffer); // Log data

            // Track data
            data_node->process_data = process_data;
            data_node->next_data_node = malloc(sizeof(DataNode));
            data_node = data_node->next_data_node;

            process_count++; 

            fclose(f); // Close file
        }
    }

    // Sort by start time and select the top 10 
    ProcessData* recent_processes = msort_processes(data_node_head, process_count);
    
    // Display the information about the top 10 most recent processes based don start time
    for (int process = 0; process < MAX_TRACKING; process++) {
        ProcessData pd = recent_processes[process];
        printf("PID: %d\nEXE FILENAME: %s\nPRIORITY: %d \nCURRENT STATE: %s\n{USER} CPU CLOCK TIME: %ld\n{KERNEL} CPU CLOCK TIME: %ld\nCPU START TIME: %ld\n", 
        pd.pid, pd.exe_filename, pd.process_priority, pd.process_state, pd.usr_cpu_clocks, pd.krnl_cpu_clocks, pd.cpu_start_time);

        printf("\n\n");
    }

    // Free memory
    free_data_nodes(data_node_head);
    closedir(dp);

    return 0;
}
