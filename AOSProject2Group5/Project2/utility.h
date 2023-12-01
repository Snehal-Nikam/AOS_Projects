#ifndef UTILITYh
#define UTILITYh

struct nodeS
{
	struct nodeS *next;
	struct nodeS *prev;
	void *data;
};

typedef struct nodeS node;

struct linkedListS
{
	node *head;
	node *tail;
	int size;
};

typedef struct linkedListS linked_List;

node *createNode(void *data);
linked_List *createLinkedList();
void addNode(linked_List *ll, void *data);
void removeNode(linked_List *ll, void *data);
void sort(linked_List *ll, int (*cmp)(void *data1, void *data2));
void sNodes(node *a, node *b);

// Queue Implementation //
typedef struct linkedListS queue;

queue *createQueue();
void enQueue(queue *q, void *data);
void *deQueue(queue *q);

#endif
