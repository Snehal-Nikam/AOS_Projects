#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "utility.h"

processStat *create_ProcessStat(process *proc)
{
	processStat *ps = (processStat *)malloc(sizeof(processStat));
	ps->proc = proc;
	ps->wait_Time = 0;
	ps->turnAround_time = 0;
	ps->responseTime = 0;

	ps->runTime = 0;
	ps->start_Time = -1;  // Initialize start time as -1, indicating it hasn't started yet
	ps->end_Time = -1;    // Initialize end time as -1, indicating it hasn't finished yet

	return ps;
}

averageStats firstComeFirstServeNP(linked_List *processes)
{
	int t = 0;

	// Create a queue to hold processes waiting to be scheduled.
	
	queue *process_Queue = (queue *)createQueue();
	node *proc_Ptr = processes->head;
	if (processes->head == NULL)
	{
		fprintf(stderr, "No Process to schedule\n");
	}
	// while process queue is not empty or time quanta is less than 100
	processStat *scheduled_Process = NULL;   // To hold the process that is currently being scheduled

	linked_List *ll = createLinkedList();   // List to hold processes stats after they have been scheduled.

// Simulate for 100 time quanta or while there's a process being scheduled.

	printf("\nFirst Come First Serve:\n");
	while (t < 100 || scheduled_Process != NULL)
	{
		// Check for new processes arriving at the current time and add them to the queue.
		if (proc_Ptr != NULL)
		{
			process *newProcess = (process *)(proc_Ptr->data);
			if (newProcess->arrivalTime <= t)
			{
				enQueue(process_Queue, create_ProcessStat(newProcess));
				proc_Ptr = proc_Ptr->next;
			}
		}

		// If no process is currently scheduled, check the queue and schedule the next process.
		if (scheduled_Process == NULL && process_Queue->size > 0)
		{
			scheduled_Process = (processStat *)deQueue(process_Queue);
		}

// If there's a scheduled process, simulate its execution

		if (scheduled_Process != NULL)
		{
			process *proc = scheduled_Process->proc;

			
			printf("%c", proc->pid); // Print process ID for visualization


			// update current processes stat
			if (scheduled_Process->start_Time == -1)
			{
				scheduled_Process->start_Time = t;  // Mark the start time of process
			}
			scheduled_Process->runTime++;   // Increment the run time of the process


			if (scheduled_Process->runTime >= proc->runTime)
			{
				scheduled_Process->end_Time = t;   // Mark the end time
				addNode(ll, scheduled_Process);    // Add to the list of completed processes
				scheduled_Process = NULL;      // Reset the scheduled process
			
			}
		}
		else
		{
			printf("_");
		}
		// increase the time
		t++;
	}
	printf("\n");

	// Print Process Stat
	return printPolicy_Stat(ll);
}
