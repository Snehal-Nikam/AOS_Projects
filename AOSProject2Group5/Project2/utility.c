#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "process.h"
#include "stat.h"

// ---- Linked List Implementation ---- //

// Function to create a new linked list //
linked_List *createLinkedList()
{
	// Allocate memory for a new linked list and initialize its attributes
	linked_List *new_ll = (linked_List *)malloc(sizeof(linked_List));
	new_ll->head = NULL;
	new_ll->tail = NULL;
	new_ll->size = 0;
	return new_ll;
}

// Function to create a new node //
node *createNode(void *data)
{
	// Allocate memory for a new node and set its data

	node *new_Node = (node *)malloc(sizeof(node));
	new_Node->data = data;
	new_Node->next = NULL;
	new_Node->prev = NULL;
	return new_Node;
}

// Function to add a new node to the linked list
void addNode(linked_List *ll, void *data)
{
	node *new_Node = createNode(data);
	if (ll->size == 0)
	{
		ll->head = new_Node;
		ll->tail = new_Node;
		ll->size = 1;
	}
	else
	{
		// Link the new node at the end of the list
		new_Node->prev = ll->tail;
		ll->tail->next = new_Node;
		ll->tail = new_Node;
		ll->size += 1;
	}
}

// Remove a node from existing linked_List //
void removeNode(linked_List *ll, void *data)
{
	node *current_Node = ll->head;

	while (current_Node != NULL && current_Node->data != data)
	{
		current_Node = current_Node->next;
	}
	// If found, remove it and adjust pointers
	if (current_Node != NULL)
	{
		if (current_Node->prev != NULL)
		{
			current_Node->prev->next = current_Node->next;
		}
		if (current_Node->next != NULL)
		{
			current_Node->next->prev = current_Node->prev;
		}
		if (ll->head == current_Node)
		{
			ll->head = current_Node->next;
		}
		if (ll->tail == current_Node)
		{
			ll->tail = current_Node->prev;
		}
		ll->size--;
		free(current_Node);
	}
}

// Function to sort the linked list using a provided comparison function //

void sort(linked_List *ll, int (*cmp)(void *data1, void *data2))
{
	// Simple bubble sort implementation for the linked list

	node *i = ll->head;
	while (i != NULL)
	{
		node *j = i->next;
		while (j != NULL)
		{
			void *p1 = i->data;
			void *p2 = j->data;
			if ((*cmp)(p1, p2) > 0)
			{
				sNodes(i, j); // Swap nodes if needed
			}
			j = j->next;
		}
		i = i->next;
	}
}

// Utility function to swap the data of two nodes //
void sNodes(node *a, node *b)
{
	void *temp = a->data;
	a->data = b->data;
	b->data = temp;
}

// ---- Queue Implementation Using Linked List ---- //
// Function to create a new queue //

queue *createQueue()
{
	return createLinkedList();
}

// Function to enqueue (add) an item to the queue //
void enQueue(queue *q, void *data)
{
	node *new_Node = createNode(data);

	new_Node->prev = q->tail;
	if (q->tail != NULL)
	{
		q->tail->next = new_Node;
		q->tail = new_Node;
	}
	else
	{
		q->tail = new_Node;
		q->head = new_Node;
	}
	q->size += 1;
}

// Function to dequeue (remove) an item from the front of the queue //
void *deQueue(queue *q)
{
	if (q->head != NULL)
	{
		node *current_Node = q->head;
		void *data = current_Node->data;

		// Adjust the head pointer
		node *next_node = q->head->next;

		if (next_node != NULL)
			next_node->prev = NULL;
		q->head = next_node;

		// If dequeuing the last node, set the tail to NULL
		if (q->tail == current_Node)
		{
			q->tail = NULL;
		}

		q->size--;
		free(current_Node);
		return data;
	}
	else{
		return NULL;
	}
}

// Function to print process statistics //
averageStats printPolicy_Stat(linked_List *ll)
{
	averageStats avg;
	// Print Process Stat
	printf("\n");
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
	printf("Process Name\t| Arrival Time | Start Time | End Time | Run Time | Response Time | Wait Time | Turn Around Time | Priority |\n");
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
	node *ptr = ll->head;
	float averageResponseTime = 0;
	float averageWaitTime = 0;
	float averageTurnAround = 0;
	int processCount = 0;
	while (ptr != NULL)
	{
		processStat *stat = (processStat *)ptr->data;
		if (stat == NULL)
			printf("No Stat\n");
		process *proc = (process *)stat->proc;
		if (proc == NULL)
			printf("No Process\n");
		float arrivalTime = proc->arrivalTime;
		float runTime = proc->runTime;
		float responseTime = stat->start_Time - arrivalTime;
		float turnAround = stat->end_Time - proc->arrivalTime + 1;
		float wait_Time = turnAround - runTime;
		unsigned char priority = proc->priority;
		averageResponseTime += responseTime;
		averageWaitTime += wait_Time;
		averageTurnAround += turnAround;
		processCount++;
		printf("%16c|%14.1f|%12.1f|%10.1f|%10.1f|%15.1f|%11.1f| %17.1f|%10u|\n", proc->pid, arrivalTime, stat->start_Time, stat->end_Time, runTime, responseTime, wait_Time, turnAround, priority);
		ptr = ptr->next;
	}
	avg.avg_Throughput = processCount;
	if (processCount == 0)
		processCount = 1;
	averageResponseTime = averageResponseTime / processCount;
	averageWaitTime = averageWaitTime / processCount;
	averageTurnAround = averageTurnAround / processCount;
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
	printf("%16s|%14.1f|%12.1f|%10.1f|%10.1f|%15.1f|%11.1f| %17.1f|\n", " Average", 0.0, 0.0, 0.0, 0.0, averageResponseTime, averageWaitTime, averageTurnAround);
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
	// compute overall stat
	// return stat
	avg.averageResponseTime = averageResponseTime;
	avg.averageWaitTime = averageWaitTime;
	avg.averageTurnAround = averageTurnAround;

	return avg;
}
