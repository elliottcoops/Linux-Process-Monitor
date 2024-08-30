#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <dirent.h>
#include <stdio.h>
#include "../headers/config.h"
#include "../headers/sort.h"

DIR* open_proc_dir();
int get_pid(struct dirent* entry);
FILE* open_pid_stats(int pid);
ProcessData* entries(char*);
void get_entry(ProcessData*, int, char*);

#endif