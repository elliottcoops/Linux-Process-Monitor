#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../headers/config.h"

// Get a data node
DataNode* get_data_node() { return (DataNode*)malloc(sizeof(DataNode));}

ProcessData* get_process_data() { return (ProcessData*)malloc(sizeof(ProcessData));}

ProcessData* get_process_data_arr(int size) { return (ProcessData*)calloc(sizeof(ProcessData), size);}

// Free process data memory
void free_data_nodes(DataNode* node) {
    DataNode* next_node;
    
    while (node) {
        ProcessData* process_data;
        process_data = node->process_data;

        if (process_data) {
            free(process_data->exe_filename);
            free(process_data->process_state);
            free(process_data);
        }
        next_node = node->next_data_node;
        free(node);
        node = next_node;        
    }
}

// Log errors
void log_error(const char* message) {
    FILE* log_file = fopen(ERR_LOG_FILE, "a");
    if (!log_file) {
        perror("FAILED TO OPEN LOG FILE\n");
        return;
    }
    fprintf(log_file, "{ERROR} - %s\n", message);
    fclose(log_file);
}
