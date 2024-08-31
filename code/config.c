#include <stdlib.h>
#include "../headers/config.h"

// Get a data node
DataNode* get_data_node(){ return malloc(sizeof(DataNode));}

// Free process data memory
void free_data_nodes(DataNode* node) {
    DataNode* next_node = NULL;
    do {
        if (node->process_data != NULL){
            ProcessData* process_data = node->process_data;
            free(process_data->exe_filename);
            free(process_data->process_state);
            free(process_data);
        }
        next_node = node->next_data_node;
        if (node != NULL)
            free(node);
        node = next_node;
    } while (node);
}