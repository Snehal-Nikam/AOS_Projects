#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "utility.h"

processStat *create_ProcessStat(process *proc);

// Round Robin scheduling function, given a list of processes and a time slice
averageStats roundRobinP(linked_List *processes, int time_slice)
{
	int t = 0;  // Represents the current time

	// Initialize a new process queue
	queue *process_Queue = (queue *)createQueue();
	node *proc_Ptr = processes->head;  // Pointer to traverse through the process list

	// Check if the list is empty

	if (processes->head == NULL)
	{
		fprintf(stderr, "No Process to schedule\n");
	}
	// Initializing variables to monitor which process is currently being scheduled
	processStat *scheduled_Process = NULL;

	linked_List *ll = createLinkedList();
	printf("\nRound Robbin:\n");
	node *cur_Node = NULL;  // Node that represents the current process
	int cur_Run = 0;    // Time counter to monitor the time a process has been running

	// Main loop to schedule processes until time reaches 100 or the process queue becomes empty
	while (t < 100 || process_Queue->size > 0)
	{
		// Check for any new incoming processes with an arrival time less than 100, 
		// and enqueue them to the process queue if their arrival time is before or equal to the current time
		if (proc_Ptr != NULL && t < 100)
		{
			process *newProcess = (process *)(proc_Ptr->data);
			while (proc_Ptr != NULL && newProcess->arrivalTime <= t)
			{
				enQueue(process_Queue, create_ProcessStat(newProcess));
				proc_Ptr = proc_Ptr->next;
				if (proc_Ptr != NULL)
					newProcess = (process *)(proc_Ptr->data);
			}
		}

		// If no process is currently scheduled, or if a process has completed its time slice, move to the next process
		if (cur_Node == NULL)
		{
			cur_Run = 0;
			cur_Node = process_Queue->head;
		}
		else if (cur_Run == time_slice)
		{
			cur_Run = 0;
			cur_Node = cur_Node->next;
			if (cur_Node == NULL)
			{
				cur_Node = process_Queue->head;
			}
		}

		// If a process is scheduled
		if (cur_Node != NULL)
		{
			scheduled_Process = (processStat *)cur_Node->data;
			process *proc = scheduled_Process->proc;

			if (t >= 100)
			{
				if (scheduled_Process->start_Time == -1)
				{
					// Don't start any new process, remove it from process_Queue
					free(scheduled_Process);
					// deQueue(scheduled_Process);
					node *next = cur_Node->next;
					removeNode(process_Queue, cur_Node->data);
					cur_Node = next;
					cur_Run = 0;
					continue;
				}
			}
			// Print the PID of the current running process for visualization
			printf("%c", proc->pid);
			cur_Run++;
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
				node *next = cur_Node->next;
				removeNode(process_Queue, cur_Node->data);
				cur_Node = next;
				cur_Run = 0;
			}
		}
		else
		{
			printf("_");
		}
		// increase the time
		t++;
	}

	return printPolicy_Stat(ll);
}
