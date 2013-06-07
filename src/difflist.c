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

int diffList_insert(priList *list, unsigned n)
{
 priNode	*Nodes;
	if(List_contents(list)==0) /* List is empty */
		return List_insert(list,n);
	else{
		/* reset current pointer to head */
		List_current(list) = List_head(list);
		Nodes = list->nodePool;
		do{
			if(Nodes[n].prio < Nodes[List_current(list)].prio){
				Nodes[List_current(list)].prio -= Nodes[n].prio;
				return List_insert(list, n);
			}
			else{
				Nodes[n].prio -= Nodes[List_current(list)].prio;
				list->current = List_next(list);
			}
		}while(list->current != -1);
		return List_insertTail(list, n);
	}
}

/* Test code begins:

#include <stdio.h>
priNode	nodes[10];
priList	list;

int main(int argc, char **argv)
{
	List_init(&list,nodes,10);
	printf("\nInsert node 2, diff(prio) 10...\n");
	nodes[2].prio = 10;
	diffList_insert(&list,2);
	printf("\nInsert node 4, diff(prio) 15...\n");
	nodes[4].prio = 15;
	diffList_insert(&list,4);
	printf("\nInsert node 6, diff(prio) 15...\n");
	nodes[6].prio = 15;
	diffList_insert(&list,6);
	printf("Head = %i, Tail = %i, Contents: %i\n",list.head,list.tail,list.contents);
	list.current = List_head((&list));
	do{
		printf("Node: %i,prio: %i, prev: %i, next: %i\n",list.current,
			nodes[list.current].prio,
			nodes[list.current].prev,
			nodes[list.current].next);
		list.current = List_next((&list));
	}while(list.current != -1);
	printf("Remove head...\n");
	list.current = List_head((&list));
	List_delete(&list);
	printf("Head = %i, Tail = %i, Contents: %i\n",list.head,list.tail,list.contents);
	list.current = List_head((&list));
	do{
		printf("Node: %i,prio: %i, prev: %i, next: %i\n",list.current,
			nodes[list.current].prio,
			nodes[list.current].prev,
			nodes[list.current].next);
		list.current = List_next((&list));
	}while(list.current != -1);

}

Test code ends. */
