#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "../headers/config.h"
#include "../headers/sort.h"
#include "../headers/file_handler.h"


int main(int argc, char** argv) {
    DataNode* data_node_head;
    DataNode* data_node;
    FILE* f;
    DIR* dp;
    struct dirent *entry;
    char buffer[1024];
    ProcessData* process_data;
    int process_count, pid;

    dp = open_proc_dir(); // Open the process directory
    data_node_head = get_data_node(); // Create a new data node
    data_node = data_node_head;
    process_count = 0;
    while (entry = readdir(dp)){
        pid = get_pid(entry); // Get the process id from entry
        if (pid > 0) {
            f = open_pid_stats(pid); // Open pid stats file 
            read_and_log(data_node, process_data, buffer, f); // Read from from stats file and log
            data_node = data_node->next_data_node;
            process_count++; // Increment process count
            fclose(f); // Close file
        }
    }
    
    sort_and_log(data_node_head, process_count); // Sort the processes and display to console

    // Cleanup
    free_data_nodes(data_node_head);
    closedir(dp);

    return 0;
}

