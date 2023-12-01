#include <stdio.h>
#include <stdlib.h>
#include "utility.h"


linked_list * create_linked_list()
{
	linked_list * new_ll =  (linked_list*) malloc(sizeof(linked_list));
	new_ll->head = NULL;
	new_ll->tail = NULL;
	new_ll->size = 0;
	return new_ll;
}



node* create_node(void* data)
{
	node* new_node = (node*) malloc(sizeof(node));
	new_node->data = data;
	new_node->next = NULL;
	new_node->prev = NULL;
	return new_node;
}

void add_node(linked_list * ll, void * data)
{
	node * new_node = create_node(data);
	if(ll->size == 0)
	{
		ll->head = new_node;
		ll->tail = new_node;
		ll->size = 1 ;
	} else {
		new_node->prev = ll->tail;
		ll->tail->next = new_node;
		ll->tail = new_node;
		ll->size += 1;
	}
}

void remove_data(linked_list* ll, void * data)
{
	node* curr_node = ll->head;

	while(curr_node != NULL && curr_node->data != data) {
		curr_node = curr_node->next;
	}

	if(curr_node != NULL) {
		if(curr_node->prev != NULL) {
			curr_node->prev->next = curr_node->next;
		}
		if(curr_node->next != NULL) {
			curr_node->next->prev = curr_node->prev;
		}
		if(ll->head == curr_node) {
			ll->head = curr_node->next;
		}
		if(ll->tail == curr_node) {
			ll->tail = curr_node->prev;
		}
		ll->size --;
		free(curr_node);
	}
}

void remove_node(linked_list* ll, node * curr_node) {
	if(curr_node != NULL) {
		if(curr_node->prev != NULL) {
			curr_node->prev->next = curr_node->next;
		}
		if(curr_node->next != NULL) {
			curr_node->next->prev = curr_node->prev;
		}
		if(ll->head == curr_node) {
			ll->head = curr_node->next;
		}
		if(ll->tail == curr_node) {
			ll->tail = curr_node->prev;
		}
		ll->size --;
		free(curr_node);
	}
}

// Remove a node from existing linked_list //
void remove_head(linked_list* ll)
{
	node * curr_node = ll->head;
	if(curr_node != NULL) {
		ll->head = curr_node->next;
		if(ll->tail == curr_node) {
			ll->tail = curr_node->prev;
		}
		ll->size --;
		free(curr_node);
	}
}

// Add a new node after a particular node in an existing linked_list//
void add_after(linked_list* ll, node *after_node, void *data)
{
	node* new_node = create_node(data);

	node* next_node = after_node->next;
	new_node->next = next_node;
	if(next_node != NULL) next_node->prev = new_node;

	new_node->prev = after_node;
	after_node->next = new_node;

	if(ll->tail == after_node) {
		ll->tail = new_node;
	}

	ll->size++;
}

void sort(linked_list *ll, int (*cmp)(void *data1, void *data2)) {
	node *i = ll->head;
	while(i!=NULL) {
		node *j = i->next;
		while(j!=NULL) {
			void * p1 = i->data;
			void * p2 = j->data;
			if((*cmp)(p1,p2) > 0) {
				swap_nodes(i,j);
			}
			j=j->next;
		}
		i = i->next;
	}
}

void swap_nodes(node *a, node *b) {
	void * temp = a->data;
	a->data = b->data;
	b->data = temp;
}

// Queue Implementation //

queue * create_queue() {
	return create_linked_list();
}

// Enqueue function to add process at the end of the queue //
void enqueue(queue* queue, void * data)
{
	node* new_node = create_node(data);
	
	new_node->prev = queue->tail;
	if(queue->tail != NULL) {
		queue->tail->next = new_node;
		queue->tail = new_node;
	} else {
		queue->tail = new_node;
		queue->head = new_node;
	}
	queue->size += 1;
}

// Dequeue function to remove process from the end of the queue //
void* dequeue(queue* queue)
{
	if(queue->head != NULL) {
		node * curr_node = queue->head;//Address of queue->head
		void * data = curr_node->data;

		//Moving Head to next Node
		node * next_node = queue->head->next;

		if(next_node != NULL) next_node->prev = NULL;
		queue->head = next_node; //curr_node = queue->head = next_node
		

		//Maintaining boundary tail condition
		if(queue->tail == curr_node) {
			queue->tail = NULL;
		}

		queue->size--;
		free(curr_node);
		return data;
	}
}
