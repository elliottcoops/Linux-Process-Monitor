#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "../headers/config.h"
#include "../headers/file_handler.h"
#include "../headers/sort.h"

// Open the /proc linux directory
DIR* open_proc_dir(){
    DIR *dp = opendir("/proc");
    if (!dp){
        log_error("Cannot open /proc directory");
        return NULL;
    } 
    return dp;
}

// Get the pid from an active process
int get_pid(struct dirent* entry){
    if (entry->d_type == DT_DIR) return atoi(entry->d_name);
    return -1;
}

// Open the pid stats file
FILE* open_pid_stats(int pid){
    char cmd_line_path[256];
    FILE *fp;
    sprintf(cmd_line_path, "/proc/%d/stat", pid);
    fp=fopen(cmd_line_path, "r");
    if (!fp){ 
        char err_buffer[300]; 
        sprintf(err_buffer, "Cannot open %s\n", cmd_line_path);
        log_error(err_buffer);
        return NULL;
    }
    return fp;
}

// Process a file entry
ProcessData* process_file(char *buffer){
    ProcessData* process_data;
    char *token;
    int entry;
    process_data=get_process_data();
    if (!process_data){
        log_error("Failed to allocate memory for process data in process_file\n");
        return NULL;
    }
    token=strtok(buffer, " ");
    entry=0;
    while (token!=NULL){ 
        get_file_entry(process_data, entry, token);
        token=strtok(NULL, " ");
        entry++;
    }
    process_data->logged_clock_time=get_cpu_time();
    return process_data;
}

// Get an entry from a file (pid, time etc)
void get_file_entry(ProcessData* process_data, int entry, char *token){
    switch (entry){
        case 0:
            process_data->pid=atoi(token);
            break;
        case 1:
            strip_filename(token);
            process_data->exe_filename=malloc(strlen(token)+1);
            if (!process_data->exe_filename) 
                log_error("Failed to allocate memory for process_data->exe_file in get_file_entry\n");
            strcpy(process_data->exe_filename, token);
            break;
        case 2:
            process_data->process_state=malloc(strlen(token)+1);
            if (!process_data->process_state) 
                log_error("Failed to allocate memory for process_data->exe_file in get_file_entry\n");
            strcpy(process_data->process_state, token);
            break;
        case 13:
            process_data->usr_cpu_clocks=atol(token);
            break;
        case 14:
            process_data->krnl_cpu_clocks=atol(token);
            break;
        case 17:
            process_data->process_priority=atoi(token);
            break;
        case 22:
            process_data->v_mem_size=atol(token)/1048576; // Convert from bytes to megabytes
            break;
        case 38:
            process_data->last_cpu=atoi(token);
            break;
        default:
            break;
    }
}

void read_and_log(DataNode *data_node, ProcessData *process_data, char *buffer, FILE *fp){
    if (!(fgets(buffer, 1024, fp))){
        log_error("Failed to read line in read_and_log\n");
        return;
    }
    if (!(process_data = process_file(buffer))){
        log_error("Failed to process file in read_and_log\n");
        return;
    } 
    data_node->process_data=process_data;
    data_node->next_data_node=get_data_node();
    if (!data_node->next_data_node)
        log_error("Failed to allocate memory for next data node in read_and_log\n");
}

// Read the proc stat to get total cpu time
long get_cpu_time(){
    char cmd_line_path[256], buffer[512], *token;
    long total;
    int entry;
    FILE *fp;
    sprintf(cmd_line_path, "/proc/stat");
    fp=fopen(cmd_line_path, "r");
    fgets(buffer, 512, fp);
    total=0;
    token = strtok(buffer, " ");
    entry = 0;
    while (token!=NULL){ 
        if (entry>0)
            total+=atol(token);
        token=strtok(NULL, " ");
        entry++;
    }
    fclose(fp);
    return total;
}

long get_process_cpu_time(int pid){
    FILE* fp;
    char buffer[512], *token;
    long total;
    int entry;
    if ((fp=open_pid_stats(pid))==NULL){ return -1;}
    fgets(buffer, 512, fp);
    token=strtok(buffer, " ");
    total=entry=0;
    while (token!=NULL){ 
        if (entry==13||entry==14){ total+=atol(token);}
        token=strtok(NULL, " "); 
        entry++;
    }
    fclose(fp);
    return total;
}

void strip_filename(char *filename){
    char *src = filename; 
    char *dst = filename;
    while (*src != '\0'){ 
        if (*src != '(' && *src != ')'){ 
            *dst++ = *src; 
        }
        src++; 
    }
    *dst = '\0'; 
}

