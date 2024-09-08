#ifndef CONFIG_H
#define CONFIG_H

#define MAX_TRACKING 10
#define ERR_LOG_FILE "err_log.txt"
#define WAIT_TIME 3000000

typedef struct ProcessData{
    int pid;
    char* exe_filename;
    char* process_state;
    long usr_cpu_clocks;
    long krnl_cpu_clocks;
    long logged_clock_time;
    float cpu_utilisation;
    int process_priority;
    long v_mem_size;
    int last_cpu;

} ProcessData;
 
typedef struct DataNode{
    ProcessData* process_data;
    struct DataNode* next_data_node;
} DataNode;

DataNode* get_data_node();
ProcessData* get_process_data();
ProcessData* get_process_data_arr(int size);
void free_data_nodes(DataNode* node);
void log_error(const char* message);

#endif