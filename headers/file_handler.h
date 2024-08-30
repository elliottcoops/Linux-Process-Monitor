#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <dirent.h>
#include <stdio.h>
#include "../headers/config.h"
#include "../headers/sort.h"

DIR* open_proc_dir();
int get_pid(struct dirent* entry);
FILE* open_pid_stats(int pid);
ProcessData* process_file(char*);
void get_file_entry(ProcessData*, int, char*);
void read_and_log(DataNode* data_node, ProcessData* process_data, char* buffer, FILE* f);

#endif