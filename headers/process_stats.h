#ifndef PROCESS_STATS
#define PROCESS_STATS

#include "../headers/config.h"

long get_clock_rate();
void calculate_cpu_utilisation(DataNode* data_node_head);

#endif