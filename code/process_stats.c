#include <unistd.h>
#include "../headers/process_stats.h"
#include "../headers/file_handler.h"

// Get clock rate of the cpu
long get_clock_rate(){
    long ticks_per_second;
    ticks_per_second = sysconf(_SC_CLK_TCK);
    if (ticks_per_second == -1)
        log_error("Failed to get clock rate in get_clock_rate");
    return ticks_per_second;
}

// Calculate the cpu utilisation of each process
void calculate_cpu_utilisation(DataNode *data_node_head){
    DataNode *node;
    ProcessData *pd;
    long delta_cpu_time, delta_process_time;
    float cpu_utilisation;
    node=data_node_head;
    while (node){
        pd=node->process_data;
        delta_cpu_time=get_cpu_delta_time(pd); // Get the cpu delta time
        delta_process_time=get_process_delta_time(pd); // Get the process delta time
        cpu_utilisation=(float)delta_process_time/delta_cpu_time;  // Get the cpu utilisation
        pd->cpu_utilisation=100*cpu_utilisation; // Set it in process data struct
        node=node->next_data_node;
    }
}

// Get the cpu delta time
long get_cpu_delta_time(ProcessData *pd){
    long logged_cpu_time, current_cpu_time, delta_cpu_time;
    logged_cpu_time=pd->logged_clock_time; // Get the logged cpu time of the process
    current_cpu_time=get_cpu_time(); // Get the current cpu time
    delta_cpu_time=current_cpu_time-logged_cpu_time; // Find the delta time
    return delta_cpu_time;
}

// Get the process delta time
long get_process_delta_time(ProcessData *pd){
    long previous_process_time, current_process_time, delta_process_time;
    previous_process_time=pd->usr_cpu_clocks + pd->krnl_cpu_clocks; // Find logged clock time of process
    current_process_time=get_process_cpu_time(pd->pid); // Get the new clock time of the process
    delta_process_time=current_process_time - previous_process_time; // Find the delta process time
    return delta_process_time;
}