#ifndef PROCESS_STATS
#define PROCESS_STATS

#include "../headers/config.h"

long get_clock_rate();
void calculate_cpu_utilisation(DataNode* data_node_head);
long get_cpu_delta_time(ProcessData* pd);
long get_process_delta_time(ProcessData* pd);

#endif