/* ----------------------------------------------------------------------- 
*
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

/*
	PriList.h:
	Declaration of priority list
*/

#ifndef	PRILIST_H
#define	PRILIST_H

typedef struct	_priLNode{
	int			next,prev;
	unsigned int	prio;
	unsigned int 	data;
}priNode;

typedef struct _priList{
	priNode	*nodePool;
	int 		current,head,tail;
	unsigned	nodes, contents;
}priList;

void	List_init(priList *list, priNode *NodePool, unsigned nodes);
int	List_insert(priList *list,unsigned n);
int	List_delete(priList *list);
int	List_insertTail(priList *list, unsigned n);
int	diffList_insert(priList *list, unsigned n);
int	priList_insert(priList *list, unsigned n);

#define List_next(list)	list->nodePool[list->current].next
#define List_prev(list)	list->nodePool[list->current].prev
#define List_head(list)	list->head
#define List_tail(list)	list->tail
#define List_current(list)	list->current
#define List_nodes(list)		list->nodes
#define List_contents(list)	list->contents
#define List_nodePool(list)	list->nodePool

#define Queue_init	List_init

#endif
