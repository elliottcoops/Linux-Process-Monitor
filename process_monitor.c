#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void entries(char*);

int main(int argc, char** argv) {

    struct dirent *entry;
    DIR *dp = opendir("/proc");

    if (!dp) {
        printf("{Error} - NO DIRECTORY DETECTED\n");
        return 1;
    } 

    while (entry = readdir(dp)){
        if (entry->d_type == DT_DIR) {
            int pid = atoi(entry->d_name);

            if (pid > 0) {
                char cmd_line_path[256];
                sprintf(cmd_line_path, "/proc/%d/stat", pid);
                
                FILE *f = fopen(cmd_line_path, "r");

                char buffer[256];
                while(fgets(buffer, 256, f)){
                    entries(buffer);
                }
                printf("\n\n");

                
        
            }
        }
    }

    return 0;
}

void entries(char* buffer) {
    int indexes[] = {14, 15, 16, 22};
    int num_indexes = 4;

    char* token = strtok(buffer, " ");
    int count = 1;

    while (token != NULL) { 
        for (int i = 0; i < num_indexes; i++) { 
            if (count == indexes[i]) {
                printf("Entry: %d\n", atoi(token));
            }
        }

        token = strtok(NULL, " ");
        count++;
    }



}