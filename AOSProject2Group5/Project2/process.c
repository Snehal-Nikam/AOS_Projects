#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "utility.h"
#include <time.h>

// Function to compare two processes based on their arrival times
int compare(void *data1, void *data2)
{
	process *p1 = (process *)data1;
	process *p2 = (process *)data2;
	// Compare the arrival times of the two processes
	if (p1->arrivalTime < p2->arrivalTime)
	{
		return -1;
	}
	else if (p1->arrivalTime == p2->arrivalTime)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

// Function to generate a linked list of processes

linked_List *generate_Processes(int n)
{
	linked_List *process_List = createLinkedList();
	char pid = 'A';  // Initial process ID
	unsigned char priority;
	float arrivalTime, runTime;
	time_t t;

	/* Intializes random number generator */
	srand((unsigned)time(&t));

	// Check if the linked list was created successfully
	if (process_List == NULL)
		fprintf(stderr, "Unable to create Linked List\n");

	// Loop to generate random process attributes and add to the list

	while (n--)
	{
		arrivalTime = rand() % 100;
		runTime = (float)((rand() % 100) + 1) / 10;
		priority = (rand() % 4) + 1;
		process *p = create_Process(pid, arrivalTime, runTime, priority);

		addNode(process_List, p);
		pid++;
	}
	
	// Sort the list of processes based on their arrival times

	sort(process_List, compare);

	// Loop through the sorted list to reset process IDs (to ensure unique IDs)

	node *ptr = process_List->head;
	pid = 'A';
	while (ptr != NULL)
	{
		((process *)ptr->data)->pid = pid;
		if (pid == 'Z')  // Reset the process ID if it reaches 'Z'
			pid = 'a' - 1;
		pid++;
		ptr = ptr->next;
	}

	return process_List;
}

// Function to create a new process with given attributes
process *create_Process(char pid, float arrivalTime, float runTime, unsigned char priority)
{
	process *proc = (process *)malloc(sizeof(process));
	proc->pid = pid;
	proc->arrivalTime = arrivalTime;
	proc->runTime = runTime;
	proc->priority = priority;
	return proc;
}
