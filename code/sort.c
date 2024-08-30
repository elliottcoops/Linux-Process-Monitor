#include <stdlib.h>
#include "../headers/config.h"
#include "../headers/sort.h"

// Sorting
void msort(ProcessData* arr, int size) {
    if (size <=1) 
        return;

    // Find mp
    int mp = size / 2;

    ProcessData* left = (ProcessData*)calloc(sizeof(ProcessData), mp);
    ProcessData* right = (ProcessData*)calloc(sizeof(ProcessData), size-mp);

    for (int i = 0; i < mp; i++){ left[i] = arr[i];}
    for (int i = mp; i < size; i++){ right[i-mp] = arr[i];}

    msort(left, mp);
    msort(right, size-mp);

    merge(arr, left, mp, right, size-mp);

    free(left);
    free(right);
}

void merge(ProcessData* arr, ProcessData* left, int left_size, ProcessData* right, int right_size) {
    int left_ptr, right_ptr, ptr;
    left_ptr = right_ptr = ptr = 0;

    while (left_ptr < left_size && right_ptr < right_size) {
        if (left[left_ptr].cpu_start_time >= right[right_ptr].cpu_start_time) {
            arr[ptr++] = left[left_ptr++];
        } else {
            arr[ptr++] = right[right_ptr++];
        }
    }

    for (int i = left_ptr; i < left_size; i++){ arr[ptr++] = left[i];}
    for (int i = right_ptr; i < right_size; i++){ arr[ptr++] = right[i];}

}

ProcessData* msort_processes(DataNode* data_node_head, int size) {
    // Put into process data array
    ProcessData* process_data_arr = (ProcessData*)calloc(sizeof(ProcessData), size);
    int ptr;
    DataNode* data_node;

    ptr = 0;
    data_node = data_node_head;

    
    do {
        process_data_arr[ptr++] = *(data_node->process_data);
        data_node = data_node->next_data_node;
    } while (data_node->next_data_node);

    msort(process_data_arr, size);

    return process_data_arr;

}   