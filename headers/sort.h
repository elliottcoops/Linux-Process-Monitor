#ifndef SORT_H
#define SORT_H

#include "config.h"

ProcessData* msort_processes(DataNode*, int);
void msort(ProcessData*, int process_count);
void merge(ProcessData* arr, ProcessData* left, int left_size, ProcessData* right, int right_size);

#endif