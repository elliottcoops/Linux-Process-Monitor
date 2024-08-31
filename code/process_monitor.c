#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include "../headers/config.h"
#include "../headers/sort.h"
#include "../headers/file_handler.h"


int main(int argc, char** argv) {
    DataNode* data_node_head;
    DataNode* data_node;
    DataNode* prev_data_node;
    FILE* f;
    DIR* dp;
    struct dirent *entry;
    char buffer[1024];
    ProcessData* process_data;
    int process_count, pid;

    while (1){

        dp = open_proc_dir(); // Open the process directory
        if (!dp) {
            log_error("Failed to open /proc directory\n");
            return 1;
        }
        
        data_node_head = get_data_node(); // Create a new data node
        if (!data_node_head) {
            log_error("Failed to create data node\n");
            return 1;
        }
        data_node = data_node_head;

        process_count = 0;
        while (entry = readdir(dp)){
            // Get the process id from entry
            if ((pid = get_pid(entry)) > 0) {
                if (!(f = open_pid_stats(pid)))
                    continue;
                read_and_log(data_node, process_data, buffer, f); // Read from from stats file and log
                prev_data_node = data_node;
                data_node = data_node->next_data_node;
                process_count++; // Increment process count
                fclose(f); // Close file
            }
        }

        free(data_node);
        prev_data_node->next_data_node = NULL;
        
        sort_and_log(data_node_head, process_count); // Sort the processes and display to console

        // Cleanup
        free_data_nodes(data_node_head);
        closedir(dp);

        usleep(1000000);
        system("clear");

    }

    return 0;
}

