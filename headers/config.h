#ifndef CONFIG_H
#define CONFIG_H

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

void free_data_nodes(DataNode* node);

#endif