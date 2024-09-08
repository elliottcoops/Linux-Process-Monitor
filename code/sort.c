#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../headers/config.h"
#include "../headers/sort.h"
#include "../headers/process_stats.h"

// Conduct merge sort on the process data
void msort(ProcessData* arr, int size){
    ProcessData* left, *right;
    int mp;
    if (size<=1) return; // Base case  
    mp=size/2; // Find midpoint of the array
    left=get_process_data_arr(mp); // Split into left subarr
    right=get_process_data_arr(size-mp); // Split into right subarr
    if (!left || !right){
        log_error("Failed to allocate memory for left or right array\n");
        return;
    }
    for (int i=0; i<mp; i++){left[i]=arr[i];} // Populate left array
    for (int i=mp; i<size; i++){right[i-mp]=arr[i];} // Populate right array
    msort(left, mp); // Sort left
    msort(right, size-mp); // Sort right
    merge(arr, left, mp, right, size-mp); // Merge left and right
    free(left); // Free left subarr
    free(right); // Free right subarr
}

// Merge a left and right subarray into original
void merge(ProcessData* arr, ProcessData* left, int left_size, ProcessData* right, int right_size){
    int left_ptr, right_ptr, ptr;
    float left_cpu_util, right_cpu_util;
    left_ptr=right_ptr=ptr=0;
    while (left_ptr < left_size && right_ptr < right_size){
        left_cpu_util=left[left_ptr].cpu_utilisation;
        right_cpu_util=right[right_ptr].cpu_utilisation;
        if (left_cpu_util>=right_cpu_util){
            arr[ptr++]=left[left_ptr++];
        } else{
            arr[ptr++]=right[right_ptr++];
        }
    }
    for (int i=left_ptr; i<left_size; i++){ arr[ptr++]=left[i];}
    for (int i=right_ptr; i<right_size; i++){ arr[ptr++]=right[i];}
}

// Create suitable array to be sorted and initialise the sort
ProcessData* msort_processes(DataNode *data_node_head, int size) {
    DataNode *data_node;
    ProcessData *process_data_arr;
    int ptr;
    calculate_cpu_utilisation(data_node_head); // Calculate cpu utilisation for each process
    process_data_arr=get_process_data_arr(size); // Get the process data array
    if (!process_data_arr){
        log_error("Failed to alloate memory for process_data_arr in msort_processes\n");
        return NULL;
    }
    ptr = 0;
    data_node = data_node_head;   
    do{
        process_data_arr[ptr++] = *(data_node->process_data);
        data_node = data_node->next_data_node;
    } while (data_node->next_data_node);
    msort(process_data_arr, size);
    return process_data_arr;
}   