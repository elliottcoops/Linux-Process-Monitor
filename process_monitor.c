#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#define MAX_TRACKING 10

typedef struct ProcessData {
    int pid;
    char* exe_filename;
    char* process_state;
    long usr_cpu_clocks;
    long krnl_cpu_clocks;
    long cpu_start_time;
    int process_priority;
    long v_mem_size;
    int last_cpu;

} ProcessData;
 
typedef struct DataNode {
    ProcessData* process_data;
    struct DataNode* next_data_node;
} DataNode;

ProcessData* msort_processes(DataNode*, int);
void msort(ProcessData*, int process_count);
void merge(ProcessData* arr, ProcessData* left, int left_size, ProcessData* right, int right_size);


void free_data_nodes(DataNode* node);
DIR* open_proc_dir();
int get_pid(struct dirent* entry);
FILE* open_pid_stats(int pid);
ProcessData* entries(char*);
void get_entry(ProcessData*, int, char*);

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



// file work
DIR* open_proc_dir() {
    DIR *dp = opendir("/proc");

    if (!dp) {
        printf("{Error} - NO DIRECTORY DETECTED\n");
        return NULL;
    } 

    return dp;
}

int get_pid(struct dirent* entry) {
    if (entry->d_type == DT_DIR) 
        return atoi(entry->d_name);
    return -1;
}

FILE* open_pid_stats(int pid) {
    char cmd_line_path[256];
    sprintf(cmd_line_path, "/proc/%d/stat", pid);
    FILE *f = fopen(cmd_line_path, "r");
    return f;
}
//


// Process info
ProcessData* entries(char* buffer) {
    ProcessData* process_data = malloc(sizeof(ProcessData));
    char* token = strtok(buffer, " ");
    int entry = 0;

    while (token != NULL) { 
        get_entry(process_data, entry, token); // Get relevant entry
        token = strtok(NULL, " "); // Get next token
        entry++; // Increase entry count
    }

    return process_data;
    
}

void get_entry(ProcessData* process_data, int entry, char* token) {
    switch (entry) {
        case 0:
            process_data->pid = atoi(token);
            break;
        case 1:
            process_data->exe_filename = token;
            break;
        case 2:
            process_data->process_state = token;
            break;
        case 13:
            process_data->usr_cpu_clocks = atol(token);
            break;
        case 14:
            process_data->krnl_cpu_clocks = atol(token);
            break;
        case 21:
            process_data->cpu_start_time = atol(token);
            break;
        case 17:
            process_data->process_priority = atoi(token);
            break;
        case 22:
            process_data->v_mem_size = atol(token);
            break;
        case 38:
            process_data->last_cpu = atoi(token);
            break;
        default:
            break;
    }
}
