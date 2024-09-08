#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include "../headers/config.h"
#include "../headers/sort.h"
#include "../headers/file_handler.h"
#include "../headers/display.h"

int main(int argc, char** argv) {
    DataNode *data_node_head, *data_node, *prev_data_node;
    ProcessData* process_data;
    FILE* fp;
    DIR* dp;
    struct dirent *entry;
    char buffer[1024];
    int process_count, pid;
    while (1){
        if (!(dp=open_proc_dir())) // Open /proc directory
            return 1;
        if (!(data_node_head=get_data_node())){ // Initialise data nodes
            log_error("Failed to create data node\n");
            return 1;
        }
        data_node=data_node_head;
        process_count=0;
        while (entry=readdir(dp)){ // Read each process in the /proc directory
            if ((pid=get_pid(entry)) > 0) {
                if (!(fp=open_pid_stats(pid)))
                    continue;
                read_and_log(data_node, process_data, buffer, fp); 
                prev_data_node = data_node;
                data_node = data_node->next_data_node;
                process_count++; 
                fclose(fp); 
            }
        }
        free(data_node);
        prev_data_node->next_data_node = NULL;
        usleep(WAIT_TIME); // Wait WAIT_TIME microseconds to to analyse cpu utilisation
        log_process_data(data_node_head, process_count); // Display process statistics
        free_data_nodes(data_node_head); // Free nodes
        closedir(dp); // Close the directory
    }
    return 0;
}

