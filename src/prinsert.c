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

int priList_insert(priList *list, unsigned n)
{
	if(list->contents == list->nodes)	return -1;
	if(list->contents == 0)	List_insert(list,n);
	else{
		list->current = list->head;
		do{
			if(list->nodePool[list->current].prio > list->nodePool[n].prio){
				List_insert(list,n);
				return 1;
			}
			else
				list->current = list->nodePool[list->current].next;
		}while(list->current != -1);
		/* If execution comes here, that means all nodes' priorities
		   are higher than node n's
		*/
		List_insertTail(list,n);
		return 1;
	}
}

/* Test code begins :
#include <stdio.h>

int main(int argc, char **argv)
{
 priNode	nodes[5];
 priList	list;
 int i;
	List_init(&list,nodes,5);

	nodes[0].prio = 1;
	nodes[1].prio = 2;
	nodes[2].prio = 0;
	nodes[3].prio = 5;
	nodes[4].prio = 5;

	for(i=0 ; i<5 ; i++)
		priList_insert(&list,i);
	list.current = list.head;
	printf("The prioirty list's sequence is:\n");
	do{
		printf(" node[%2i]\n",list.current);
		list.current = nodes[list.current].next;
	}while(list.current != -1);
	

}
Test code ends. */
