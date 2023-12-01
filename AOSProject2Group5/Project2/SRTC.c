#include <stdio.h>
#include "stat.h"
#include "utility.h"

// Function declaration to create a process statistics object
processStat *create_ProcessStat(process *proc);

// Compare function for sorting processes based on their remaining runtime
int compare_RemainingTime(void *data1, void *data2)
{
    processStat *ps1 = (processStat *)data1;
    processStat *ps2 = (processStat *)data2;

    // Comparing the remaining runtime of two processes
    if (((((process *)ps1->proc)->runTime) - (ps1->runTime)) < ((((process *)ps2->proc)->runTime) - (ps2->runTime)))
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

// Function implementing the shortest remaining time preemptive scheduling algorithm
averageStats shortestRemainingTimeP(linked_List *processes)
{
    int t = 0; // Represents the current time unit or quanta

    // Initialize a new queue to manage processes
    queue *process_Queue = createQueue();
    // Create a linked list to store the order of processes during preemption
    linked_List *ll = createLinkedList();

    // Pointer to traverse through the process list
    node *process_Pointer = processes->head;
    // Check if the list of processes is empty
    if (processes->head == NULL)
    {
        fprintf(stderr, "No Process to schedule\n");
    }

    processStat *scheduled_Process = NULL; // Represents the currently scheduled process
    printf("\n\nShortest Remainging Time To Completion Preemptive:\n");

    // Main scheduling loop: runs until time reaches 100 or there is a scheduled process running
    while (t < 100 || scheduled_Process != NULL)
    {
        // If there's a currently scheduled process, enqueue it back into the process queue
        // This is to allow for preemption
        if (scheduled_Process != NULL)
        {
            enQueue(process_Queue, scheduled_Process);
            scheduled_Process = NULL;
        }

        if (process_Pointer != NULL)
        {
            process *newProcess = (process *)(process_Pointer->data);
            while (process_Pointer != NULL && newProcess->arrivalTime <= t)
            {
                enQueue(process_Queue, create_ProcessStat(newProcess));
                // sort(process_Queue,compare_RemainingTime);
                process_Pointer = process_Pointer->next;
                if (process_Pointer != NULL)
                {
                    newProcess = (process *)(process_Pointer->data);
                }
            }
            // Sort the processes based on their remaining runtime for proper preemption
            sort(process_Queue, compare_RemainingTime);
        }

        // If no process is currently scheduled, dequeue the shortest remaining process from the queue
        if (scheduled_Process == NULL && process_Queue->size > 0)
        {
            scheduled_Process = (processStat *)deQueue(process_Queue);

            // If the time has already reached 100 quanta and the dequeued process hasn't started yet,
            // then it should not be scheduled. Keep dequeuing the next process until a valid one is found.
            while (t >= 100 && scheduled_Process->start_Time == -1)
            {
                scheduled_Process = (processStat *)deQueue(process_Queue);
            }
        }

        if (scheduled_Process != NULL)
        {
            process *proc = scheduled_Process->proc;

            // add current running process to the time chart
            printf("%c", proc->pid);

            // update current processes stat
            if (scheduled_Process->start_Time == -1)
            {
                scheduled_Process->start_Time = t;
            }
            scheduled_Process->runTime++;

            if (scheduled_Process->runTime >= proc->runTime)
            {
                scheduled_Process->end_Time = t;
                addNode(ll, scheduled_Process);
                scheduled_Process = NULL;
            }
        }
        else
        {
            printf("_");
        }
        // increase the time
        t++;
    }
    // Print Process Stat
    return printPolicy_Stat(ll);
}
