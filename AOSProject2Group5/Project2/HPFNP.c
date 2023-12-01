#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "utility.h"

// Forward declarations
processStat * create_ProcessStat(process* proc);
int comparePriority(void * data1, void * data2);

// Implementation of Highest Priority First Non-Preemptive scheduling algorithm.

averageStats highestPriorityFirstNP(linked_List * processes)
{
  int t = 0; // Time quanta tracker.

  // Create separate queues for different priorities.
  queue * process_Queue_1 = createQueue();
  queue * process_Queue_2 = createQueue();
  queue * process_Queue_3 = createQueue();
  queue * process_Queue_4 = createQueue();

  // Linked lists for tracking processes' stats based on their priorities.
  linked_List * ll_1 = createLinkedList();
  linked_List * ll_2 = createLinkedList();
  linked_List * ll_3 = createLinkedList();
  linked_List * ll_4 = createLinkedList();
  node * process_Pointer = processes->head;  // Node pointer to traverse the process list.

// Check if process list is empty.

  if(processes->head == NULL) {
		fprintf(stderr,"No Process to schedule\n");
	}
  printf("\nHighest Priority First Non Preemptive:\n");
  //while process queue is not empty or time quanta is less than 100
  processStat * scheduled_Process = NULL;

  // Simulate for a total of 100 time quanta or until there's no more process to schedule.
  while(t<100 || scheduled_Process!=NULL )
  {
    // Enqueue processes that arrive at this time quanta based on their priority.
		if(process_Pointer != NULL) {
			process * newProcess = (process *)(process_Pointer->data);
			while(process_Pointer !=NULL && newProcess->arrivalTime <= t) {
        
        if(newProcess->priority == 1) enQueue(process_Queue_1,create_ProcessStat(newProcess));
        if(newProcess->priority == 2) enQueue(process_Queue_2,create_ProcessStat(newProcess));
        if(newProcess->priority == 3) enQueue(process_Queue_3,create_ProcessStat(newProcess));
        if(newProcess->priority == 4) enQueue(process_Queue_3,create_ProcessStat(newProcess));
				process_Pointer = process_Pointer->next;
				if(process_Pointer!=NULL)
        {
					newProcess = (process *)(process_Pointer->data);
        }
			}
    }
     // If no process is currently scheduled, dequeue the highest priority process.
		if(scheduled_Process == NULL) {
      if (process_Queue_1->size > 0) scheduled_Process = (processStat *) deQueue(process_Queue_1);
      else if (process_Queue_2->size > 0) scheduled_Process = (processStat *) deQueue(process_Queue_2);
      else if (process_Queue_3->size > 0) scheduled_Process = (processStat *) deQueue(process_Queue_3);
      else if (process_Queue_4->size > 0) scheduled_Process = (processStat *) deQueue(process_Queue_4);
      // If time has exceeded 100 and the process has never started, remove it.
      if (t>=100 && scheduled_Process->start_Time == -1){
        //free(scheduled_Process);
        scheduled_Process = NULL;
        continue;
      }
    }

// Simulate the running process.

    if(scheduled_Process != NULL) {
  			process * proc = scheduled_Process->proc;

  			printf("%c",proc->pid);  // Print the process ID for visualization.

  			// Update the stats for the running process.
  			if(scheduled_Process->start_Time == -1) {
  				scheduled_Process->start_Time = t;
  			}

  			scheduled_Process->runTime++;

        // If the process completes, record its stats based on its priority.

        if(scheduled_Process->runTime >= proc->runTime) {
          scheduled_Process->end_Time = t;
          if(scheduled_Process->proc->priority == 1) addNode(ll_1,scheduled_Process);
          else if(scheduled_Process->proc->priority == 2) addNode(ll_2,scheduled_Process);
          else if(scheduled_Process->proc->priority == 3) addNode(ll_3,scheduled_Process);
          else if(scheduled_Process->proc->priority == 4) addNode(ll_4,scheduled_Process);
          scheduled_Process = NULL;
        }
      }else {
    			printf("_");  // Indicate idle time.
  		}
  		
  		t++;    // Increment the time quanta.
    }
    //Print Process Stat
    averageStats avg1,avg2,avg3,avg4,avg;
    printf("\nFor Priority Queue 1");
    avg1 = printPolicy_Stat(ll_1);
    printf("\nFor Priority Queue 2");
    avg2 = printPolicy_Stat(ll_2);
    printf("\nFor Priority Queue 3");
    avg3 = printPolicy_Stat(ll_3);
    printf("\nFor Priority Queue 4");
    avg4 = printPolicy_Stat(ll_4);

    avg.averageResponseTime = (avg1.averageResponseTime + avg2.averageResponseTime + avg3.averageResponseTime + avg4.averageResponseTime)/4 ;
    avg.averageWaitTime = (avg1.averageWaitTime + avg2.averageWaitTime + avg3.averageWaitTime + avg4.averageWaitTime)/4 ;
    avg.averageTurnAround = (avg1.averageTurnAround + avg2.averageTurnAround + avg3.averageTurnAround + avg4.averageTurnAround)/4 ;
    avg.avg_Throughput = (avg1.avg_Throughput + avg2.avg_Throughput + avg3.avg_Throughput + avg4.avg_Throughput) ;

    printf("The average statistics High Priority First Non Preemptive considering all priority queues is:\n");
    printf("Average Response Time : %.1f\n",avg.averageResponseTime);
    printf("Average Wait Time : %.1f\n",avg.averageWaitTime);
    printf("Average Turn Around Time :%.1f\n",avg.averageTurnAround);

    return avg;
}
