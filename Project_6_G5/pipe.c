#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>


//Considering two type of child A and B. 
//Child A : generates message at random interval of 0,1,or 2 second
//Child B  : prompts on stdout and take inputs from stdin

#define CHILD_COUNT 5
#define CHILD_DURATION 30
#define READ_FD 0
#define WRITE_FD 1

struct timeval global_start_time;

void spawn_child(int child_number, int pipe_write_end);

int main(int argc, char **argv) {
    gettimeofday(&global_start_time, NULL);

    int child_number = 1;
    int pipe_fd[CHILD_COUNT][2];

    fd_set read_fds;
    FD_ZERO(&read_fds);
    int max_fd = 0;
    int is_child_process = 0;
    int pid_of_child_5 = -1;

    // Create pipes
    for (int i = 0; i < CHILD_COUNT; i++) {
        pipe(pipe_fd[i]);
        max_fd = pipe_fd[i][READ_FD] + 1;
    }

    // Parent process creates 5 child processes
    while (child_number <= CHILD_COUNT) {
        int pid = fork(); 
        if (pid == 0) { 
            is_child_process = 1;
            break;
        }
        if (child_number == CHILD_COUNT) {
            pid_of_child_5 = pid;
        }
        child_number++;
    }

    if (is_child_process) {
        // Close unused pipe file descriptors
        for (int i = 0; i < CHILD_COUNT; i++) {
            if (i != child_number - 1) {
                close(pipe_fd[i][WRITE_FD]);
            }
            close(pipe_fd[i][READ_FD]);
        }

        spawn_child(child_number, pipe_fd[child_number - 1][WRITE_FD]);
        return 0;
    } else {
        // Close unused pipe write file descriptors
        for (int i = 0; i < CHILD_COUNT; i++) {
            close(pipe_fd[i][WRITE_FD]);
        }

        //Reading from pipe
        struct timeval timeout;
        int is_pipe_active[CHILD_COUNT];
        for (int i = 0; i < CHILD_COUNT; i++) {
            is_pipe_active[i] = 1;
        }

        struct timeval current_time;
        FILE *output_file = fopen("output.txt", "w");

        while (1) {
            gettimeofday(&current_time, NULL);
            double current_timestamp = (double)current_time.tv_sec + (double)current_time.tv_usec / 1000000;

            timeout.tv_sec = 0;
            timeout.tv_usec = 1; // Wait for 1000 milliseconds

            // Reinitialize read file descriptor set
            FD_ZERO(&read_fds);

            int total_active_pipes = 0;
            for (int i = 0; i < CHILD_COUNT; i++) {
                if (is_pipe_active[i]) {
                    FD_SET(pipe_fd[i][READ_FD], &read_fds);
                    total_active_pipes++;
                }
            }

            if (total_active_pipes == 0) {
                break;
            }

            int status = select(max_fd, &read_fds, NULL, NULL, &timeout);

            if (status == -1) {
                printf("Select Failure\n");
                break;
            } else if (status) {
                char message_buffer[255];

                for (int i = 0; i < CHILD_COUNT; i++) {
                    if (FD_ISSET(pipe_fd[i][READ_FD], &read_fds)) {
                        int bytes_read = read(pipe_fd[i][READ_FD], message_buffer, sizeof(char) * 255);

                        if (bytes_read) {
                            fprintf(output_file, "0:%.3f: %s", (current_timestamp - global_start_time.tv_sec), message_buffer);
                            fflush(output_file);
                        } else {
                            printf("Child %d has closed the pipe\n", i + 1);
                            close(pipe_fd[i][READ_FD]);
                            is_pipe_active[i] = 0;
                        }
                    }
                }
            }
        }
        
        fclose(output_file);
        printf("Terminating main process\n");
    }
    // Each child has lifespan of 30 sec
	// Each 4 childs sleep at random time of 0,1 or 2 seconds
	// Time stamped message to nearest 1000th of second gettimeofday(&tv, NULL), sends it to the parent process
    return 0;
}

void spawn_child(int child_number, int pipe_write_end) {
    int random_wait_time = 0;
    struct timeval current_time;
    gettimeofday(&current_time, NULL);

    time_t seed;
    srand((unsigned)time(&seed));

    double current_timestamp = (double)current_time.tv_sec + (double)current_time.tv_usec / 1000000;
    int message_number = 1;

    if (child_number == CHILD_COUNT) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        struct timeval timeout;
        printf("Input Child 5 >> ");

        while ((current_timestamp - global_start_time.tv_sec) < CHILD_DURATION) {
            char user_input[255];
            fflush(stdout);

            FD_ZERO(&read_fds);
            FD_SET(0, &read_fds);

            timeout.tv_sec = 0;
            timeout.tv_usec = 1;

            if (select(1, &read_fds, NULL, NULL, &timeout) > 0) {
                read(0, user_input, 255);
                char output_message[255];
                sprintf(output_message, "0:%.3f: Child %d %s", (current_timestamp - global_start_time.tv_sec), child_number, user_input);
                write(pipe_write_end, output_message, sizeof(char) * strlen(output_message) + 1);
                printf("Input Child 5 >> ");
            }

            gettimeofday(&current_time, NULL);
            current_timestamp = (double)current_time.tv_sec + (double)current_time.tv_usec / 1000000;
        }

        printf("\n");
    } else {
        while ((current_timestamp - global_start_time.tv_sec) < CHILD_DURATION) {
            random_wait_time = rand() % 4;

            char output_message[255];
            sprintf(output_message, "0:%.3f: Child %d message %d\n", (current_timestamp - global_start_time.tv_sec), child_number, message_number);
            write(pipe_write_end, output_message, sizeof(char) * strlen(output_message) + 1);
            message_number++;

            sleep(random_wait_time);
            gettimeofday(&current_time, NULL);
            current_timestamp = (double)current_time.tv_sec + (double)current_time.tv_usec / 1000000;
        }
    }

    close(pipe_write_end);
}
