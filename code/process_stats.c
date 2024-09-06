#include <unistd.h>
#include "../headers/process_stats.h"
#include "../headers/file_handler.h"

// Get clock rate
long get_clock_rate() {
    long ticks_per_second;
    ticks_per_second = sysconf(_SC_CLK_TCK);
    if (ticks_per_second == -1)
        log_error("Failed to get clock rate in get_clock_rate");
    return ticks_per_second;
}

void calculate_cpu_utilisation(DataNode* data_node_head) {
    DataNode* node = data_node_head;
    while (node) {
        ProcessData* pd = node->process_data;
        int c1 = pd->logged_clock_time;
        int c2 = get_cpu_time();
        int delta_time = c2-c1;
        long new_clock_time = get_process_cpu_time(pd->pid);
        long old_clock_time = pd->usr_cpu_clocks + pd->krnl_cpu_clocks;
        long delta_clock_time = new_clock_time - old_clock_time;
        float cpu_utilisation = (float)delta_clock_time / delta_time;
        pd->cpu_utilisation = 100*cpu_utilisation;
        node=node->next_data_node;
    }
}