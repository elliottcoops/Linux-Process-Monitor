#ifndef DISPLAY_H
#define DISPLAY_H
#include "../headers/config.h"

void log_process_data(DataNode* data_node_head, int process_count);
ProcessData* get_top_processes(DataNode* data_node_head, int process_count);
void display_process_data(ProcessData* recent_processes, int num_processes);
char* format_filename(char* exe_filename);
void set_utilisation_bar(char* utilisation_bar, char* process_utilisation_bar, float cpu_utilisation);
int min(int a, int b);

#endif