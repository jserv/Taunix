/* ----------------------------------------------------------------------- 
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, Inc., 675 Mass Ave, Cambridge MA 02139,
*   USA; either version 2 of the License, or (at your option) any later
*   version.
*   
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
* ----------------------------------------------------------------------- */

#include "priList.h"

void List_init(priList *list, priNode *NodePool,unsigned nodes)
{
 unsigned int i;
	list->nodePool = NodePool;
	list->nodes = nodes;
	list->tail = list->head = 0;
	list->contents = 0;
	list->current = 0;
	for(i=0 ; i<nodes ; i++){
		NodePool[i].next = NodePool[i].prev = NodePool[i].prio = -1;
	}
}

int List_insert(priList	*list,unsigned n)
{
	if(list->contents == list->nodes)	return -1;
	if(list->contents){	/* There are nodes in list */
		list->nodePool[n].next = list->current;
		list->nodePool[n].prev = list->nodePool[list->current].prev;
		list->nodePool[list->current].prev = n;
		if(list->current != list->head)
			list->nodePool[list->nodePool[n].prev].next = n;
		else
			list->head = n;
	}
 	else	/* Insert first one */
		list->tail = list->head = n;

	list->current = n;
	list->contents ++;

	return 1;
}

int List_delete(priList	*list)
{int deleted_node = list->current;
	if(list->contents == 0)	return -1;
	if(list->current == list->head){
		list->nodePool[list->nodePool[deleted_node].next].prev = -1;
		list->head = list->current = list->nodePool[deleted_node].next;
	}
	else if(list->current == list->tail){
		list->nodePool[list->nodePool[deleted_node].prev].next = -1;
		list->tail = list->current = list->nodePool[deleted_node].prev;
	}
	else{
		list->nodePool[list->nodePool[deleted_node].next].prev = list->nodePool[deleted_node].prev;
		list->nodePool[list->nodePool[deleted_node].prev].next = list->nodePool[deleted_node].next;	
		list->current = list->nodePool[deleted_node].next;
	}
     list->nodePool[deleted_node].prev = list->nodePool[deleted_node].next = -1;
	list->contents --;
}


int List_insertTail(priList *list, unsigned n)
{
	if(list->contents == list->nodes)	return -1;
	list->nodePool[n].prev = list->tail;
	list->nodePool[list->tail].next = n;
	list->current = list->tail = n;
	list->contents++;
	return 1;
}

/* Test code begins:

#include <stdio.h>
priNode	nodes[12];
priList	list;
int main(int argc, char **argv)
{int i;
	List_init(&list,nodes,10);
	for(i=0 ; i<10 ; i++){
		List_insert(&list,i);
		printf("node:  0  1  2  3  4  5  6  7  8  9 10 11 current:%i content:%i\n",list.current,list.contents);
		printf("next:%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i head:%i\n",
			nodes[0].next,nodes[1].next,nodes[2].next,nodes[3].next,nodes[4].next,nodes[5].next,
			nodes[6].next,nodes[7].next,nodes[8].next,nodes[9].next,nodes[10].next,nodes[11].next,list.head);
		printf("prev:%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i tail:%i\n",
			nodes[0].prev,nodes[1].prev,nodes[2].prev,nodes[3].prev,nodes[4].prev,nodes[5].prev,
			nodes[6].prev,nodes[7].prev,nodes[8].prev,nodes[9].prev,nodes[10].prev,nodes[11].prev,list.tail);
	}
	printf("Press enter...\n");
	getchar();
	printf("Delete node 2:\n");
	list.current = 2;
	List_delete(&list);
		printf("node:  0  1  2  3  4  5  6  7  8  9 10 11 current:%i content:%i\n",list.current,list.contents);
		printf("next:%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i head:%i\n",
			nodes[0].next,nodes[1].next,nodes[2].next,nodes[3].next,nodes[4].next,nodes[5].next,
			nodes[6].next,nodes[7].next,nodes[8].next,nodes[9].next,nodes[10].next,nodes[11].next,list.head);
		printf("prev:%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i tail:%i\n",
			nodes[0].prev,nodes[1].prev,nodes[2].prev,nodes[3].prev,nodes[4].prev,nodes[5].prev,
			nodes[6].prev,nodes[7].prev,nodes[8].prev,nodes[9].prev,nodes[10].prev,nodes[11].prev,list.tail);

	printf("Delete node 4:\n");
	list.current = 4;
	List_delete(&list);
		printf("node:  0  1  2  3  4  5  6  7  8  9 10 11 current:%i content:%i\n",list.current,list.contents);
		printf("next:%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i head:%i\n",
			nodes[0].next,nodes[1].next,nodes[2].next,nodes[3].next,nodes[4].next,nodes[5].next,
			nodes[6].next,nodes[7].next,nodes[8].next,nodes[9].next,nodes[10].next,nodes[11].next,list.head);
		printf("prev:%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i tail:%i\n",
			nodes[0].prev,nodes[1].prev,nodes[2].prev,nodes[3].prev,nodes[4].prev,nodes[5].prev,
			nodes[6].prev,nodes[7].prev,nodes[8].prev,nodes[9].prev,nodes[10].prev,nodes[11].prev,list.tail);

	printf("Delete node 0:\n");
	list.current = 0;
	List_delete(&list);
		printf("node:  0  1  2  3  4  5  6  7  8  9 10 11 current:%i content:%i\n",list.current,list.contents);
		printf("next:%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i head:%i\n",
			nodes[0].next,nodes[1].next,nodes[2].next,nodes[3].next,nodes[4].next,nodes[5].next,
			nodes[6].next,nodes[7].next,nodes[8].next,nodes[9].next,nodes[10].next,nodes[11].next,list.head);
		printf("prev:%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i tail:%i\n",
			nodes[0].prev,nodes[1].prev,nodes[2].prev,nodes[3].prev,nodes[4].prev,nodes[5].prev,
			nodes[6].prev,nodes[7].prev,nodes[8].prev,nodes[9].prev,nodes[10].prev,nodes[11].prev,list.tail);

	printf("Delete node 9:\n");
	list.current = 9;
	List_delete(&list);
		printf("node:  0  1  2  3  4  5  6  7  8  9 10 11 current:%i content:%i\n",list.current,list.contents);
		printf("next:%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i head:%i\n",
			nodes[0].next,nodes[1].next,nodes[2].next,nodes[3].next,nodes[4].next,nodes[5].next,
			nodes[6].next,nodes[7].next,nodes[8].next,nodes[9].next,nodes[10].next,nodes[11].next,list.head);
		printf("prev:%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i%3i tail:%i\n",
			nodes[0].prev,nodes[1].prev,nodes[2].prev,nodes[3].prev,nodes[4].prev,nodes[5].prev,
			nodes[6].prev,nodes[7].prev,nodes[8].prev,nodes[9].prev,nodes[10].prev,nodes[11].prev,list.tail);
}


Test code ends. */
