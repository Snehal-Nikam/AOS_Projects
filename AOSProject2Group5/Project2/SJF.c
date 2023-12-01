#include <stdio.h>
#include "stat.h"
#include "utility.h"

// Function declaration for creating process statistics
processStat * create_ProcessStat(process* proc);

// Comparison function for sorting processes based on their runTime
int compareRunTime(void * data1, void * data2) {
	processStat * ps1 = (processStat *) data1;
	processStat * ps2 = (processStat *) data2;
	// If runtime of the first process is less than the second process, return -1
	// otherwise, return 1
	if(((process *)ps1->proc)->runTime < ((process *)ps2->proc)->runTime) {
		return -1;
	} else {
		return 1;
	}
}

// Function to simulate the non-preemptive shortest job first scheduling algorithm
averageStats shortestJobFirstNP(linked_List * processes) {
	int t = 0;  // Represents the current time

	// Initialize a new process queue
	queue *process_Queue = (queue *)createQueue();
	node * proc_Ptr = processes->head;   // Pointer to traverse through the process list

	// Check if the list is empty
	if(processes->head == NULL) {
		fprintf(stderr,"No Process to schedule\n");
	}
	//while process queue is not empty or time quanta is less than 100
	processStat * scheduled_Process = NULL;   // Represents the currently scheduled process
	linked_List *ll = createLinkedList();    // List to store the processed processes
	printf("\nShortest Job First:\n");

	// Main scheduling loop runs until time reaches 100 or there is a scheduled process running
	while (t<100 || scheduled_Process!=NULL) {
		// Check for any new incoming processes and enqueue them to the process queue
		// Sort the process queue based on the runTime after enqueueing
		if (proc_Ptr != NULL) {
			process * newProcess = (process *)(proc_Ptr->data);
			while (proc_Ptr!=NULL && newProcess->arrivalTime <= t) {
				enQueue(process_Queue,create_ProcessStat(newProcess));
				sort(process_Queue,compareRunTime);
				proc_Ptr = proc_Ptr->next;
				if (proc_Ptr!=NULL)
					newProcess = (process *)(proc_Ptr->data);
			}
		}

		// If no process is currently scheduled, dequeue the shortest process from the process queue
		if (scheduled_Process == NULL && process_Queue->size > 0) {
			scheduled_Process = (processStat *) deQueue(process_Queue);
		}

		// If a process is scheduled
		if (scheduled_Process != NULL) {
			process * proc = scheduled_Process->proc;

			// Print the PID of the current running process for visualization
			printf("%c",proc->pid);

			// Update the stats of the currently running process
			if (scheduled_Process->start_Time == -1) {
				scheduled_Process->start_Time = t;
			}
			scheduled_Process->runTime++;

			if (scheduled_Process->runTime >= proc->runTime) {
				scheduled_Process->end_Time = t;
				addNode(ll,scheduled_Process);
				scheduled_Process = NULL;
			}
		} else {
			printf("_");
		}
		//increase the time
		t++;
	}
	printf("\n");

	// Calculate and return the average statistics for the processes
	return printPolicy_Stat(ll);
}
