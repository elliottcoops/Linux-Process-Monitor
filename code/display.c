#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../headers/config.h"
#include "../headers/display.h"
#include "../headers/sort.h"

// Sort processes by cpu utilisation, get the top MAX_TRACKING processes and display
void log_process_data(DataNode *data_node_head, int process_count){
    ProcessData *recent_processes;
    int num_processes;
    num_processes=min(MAX_TRACKING, process_count);
    recent_processes=get_top_processes(data_node_head, process_count);
    display_process_data(recent_processes, num_processes);
}

// Display the top num_processes statistics
void display_process_data(ProcessData *recent_processes, int num_processes){
    ProcessData pd;
    char process_utilisation_bar[13];
    char utilisation_bar[]="[----------]";
    system("clear");
    printf("%-15s%-10s%-15s%-15s%-15s%-10s%-10s%-15s\n", 
        ".EXE NAME", "PID", "CURRENT STATE", "PRIORITY", "VRAM (MB)", "CPU", "% CPU", "UTILISATION");
    for (int i=0; i<num_processes; i++){ 
        pd=recent_processes[i];
        set_utilisation_bar(utilisation_bar, process_utilisation_bar, pd.cpu_utilisation);
        printf("%-15s%-10d%-15s%-15d%-15ld%-10d%-10.2f%-15s\n",
            format_filename(pd.exe_filename), pd.pid, pd.process_state, pd.process_priority, pd.v_mem_size, pd.last_cpu, pd.cpu_utilisation, process_utilisation_bar);
    }
}

// Sort processes by cpu utilisation
ProcessData* get_top_processes(DataNode *data_node_head, int process_count){ 
    ProcessData* recent_processes;
    int num_processes;
    recent_processes=msort_processes(data_node_head, process_count);
    return recent_processes;
}

// Strip the filename to 10 characters with + at the end
char* format_filename(char *exe_filename){
    int exe_filename_length, max_filename_length;
    char *new_filename;
    exe_filename_length=strlen(exe_filename);
    max_filename_length=10;
    if (exe_filename_length<=max_filename_length) return exe_filename;
    new_filename = malloc(max_filename_length+1);
    for (int i=0; i<max_filename_length-1; i++){ new_filename[i]=exe_filename[i];}
    new_filename[max_filename_length-1]='+'; // Indicates there is more which isn't being shown
    new_filename[max_filename_length]='\0'; // Null terminating character
    return new_filename;
}

// Set the cpu utilisation bar (each bar represents 10% utilisation)
void set_utilisation_bar(char* utilisation_bar, char* process_utilisation_bar, float cpu_utilisation){
    int bars;
    sprintf(process_utilisation_bar, "%s", utilisation_bar);
    bars=cpu_utilisation / 10;
    if (bars>0 && bars<11)
        process_utilisation_bar[bars]='|';
    for (int i=bars+1; i<11; i++)
        process_utilisation_bar[i]=' ';
}

int min(int a, int b){
    if (a<=b)
        return a;
    return b;
}