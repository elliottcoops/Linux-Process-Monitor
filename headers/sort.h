#ifndef SORT_H
#define SORT_H

#include "config.h"


ProcessData* msort_processes(DataNode*, int);
void msort(ProcessData*, int process_count);
void merge(ProcessData* arr, ProcessData* left, int left_size, ProcessData* right, int right_size);
void sort_and_log(DataNode* data_node_head, int process_count);
int min(int, int);

#endif