#include <stdlib.h>
#include "../headers/config.h"

// Free process data memory
void free_data_nodes(DataNode* node) {
    DataNode* next_node = NULL;
    do {
        if (node->process_data != NULL)
            free(node->process_data);
        next_node = node->next_data_node;
        if (node != NULL)
            free(node);
        node = next_node;
    } while (node);
}