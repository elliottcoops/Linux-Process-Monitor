#include <stdlib.h>
#include <stdio.h>
#include "../headers/config.h"
#include "../headers/sort.h"

// Conduct merge sort on the process data
void msort(ProcessData* arr, int size) {
    if (size <=1) 
        return;

    // Find mp
    int mp = size / 2;

    // Split array into left and right subarrays
    ProcessData* left = (ProcessData*)calloc(sizeof(ProcessData), mp);
    ProcessData* right = (ProcessData*)calloc(sizeof(ProcessData), size-mp);

    // Populate left and right subarrays
    for (int i = 0; i < mp; i++){ left[i] = arr[i];}
    for (int i = mp; i < size; i++){ right[i-mp] = arr[i];}

    // Sort left and right
    msort(left, mp);
    msort(right, size-mp);

    // Merge left and right
    merge(arr, left, mp, right, size-mp);

    // Post process memory free
    free(left);
    free(right);
}

// Merge a left and right subarray into original
void merge(ProcessData* arr, ProcessData* left, int left_size, ProcessData* right, int right_size) {
    int left_ptr, right_ptr, ptr;
    left_ptr = right_ptr = ptr = 0;

    // Sort in order of left and right subarray
    while (left_ptr < left_size && right_ptr < right_size) {
        if (left[left_ptr].cpu_start_time >= right[right_ptr].cpu_start_time) {
            arr[ptr++] = left[left_ptr++];
        } else {
            arr[ptr++] = right[right_ptr++];
        }
    }

    // Fill remaining spots
    for (int i = left_ptr; i < left_size; i++){ arr[ptr++] = left[i];}
    for (int i = right_ptr; i < right_size; i++){ arr[ptr++] = right[i];}
}

// Create suitable array to be sorted and initialise the sort
ProcessData* msort_processes(DataNode* data_node_head, int size) {
    // Put into process data array
    ProcessData* process_data_arr = (ProcessData*)calloc(sizeof(ProcessData), size);
    DataNode* data_node;
    int ptr;
    
    ptr = 0;
    data_node = data_node_head;   
     
    do {
        process_data_arr[ptr++] = *(data_node->process_data);
        data_node = data_node->next_data_node;
    } while (data_node->next_data_node);

    msort(process_data_arr, size);

    return process_data_arr;
}   

// Sort the processes by start time and log the information to console
void sort_and_log(DataNode* data_node_head, int process_count) {
    // Sort by start time and select the top 10 
    ProcessData* recent_processes = msort_processes(data_node_head, process_count);
    
    // Display the information about the top 10 most recent processes based don start time
    for (int process = 0; process < MAX_TRACKING; process++) {
        ProcessData pd = recent_processes[process];
        printf("PID: %d\nEXE FILENAME: %s\nPRIORITY: %d \nCURRENT STATE: %s\n{USER} CPU CLOCK TIME: %ld\n{KERNEL} CPU CLOCK TIME: %ld\nCPU START TIME: %ld\n", 
        pd.pid, pd.exe_filename, pd.process_priority, pd.process_state, pd.usr_cpu_clocks, pd.krnl_cpu_clocks, pd.cpu_start_time);

        printf("\n\n");
    }
}