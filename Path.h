#ifndef PATH_H
#define PATH_H

#include "typedefs.h"
#include "list.h"
#include "Node.h"
#include "Graph.h"
#include "lists.h"
#include<stdio.h>
#include<stdlib.h>

//TODO expand with blossoms
void find_path(MVNodeP n1,MVNodeP n2,MVGraph* g);

void reverse(list_MVNodeP *list,int from, int to);
//function for the finding of a path, not for walking in a blossom
void walk_down_path(list_MVNodeP * list,MVNodeP start);

inline MVNodeP jump_bridge(MVNodeP cur);

//Walks a blossom from the entry node to the bud.
//It does not add the bud, it does add the entry node
MVNodeP walk_blossom(list_MVNodeP * list,MVNodeP cur);
//walks down until it reacheds the bud of a blossom
//It does not add the bud, it does add the first node
MVNodeP walk_blossom_down(list_MVNodeP * list,MVNodeP cur);
//Walks up untill it reaches a bridge
//It does add the bridge and the node it starts on
MVNodeP walk_blossom_up(list_MVNodeP * list,MVNodeP cur);

void augment_path(MVGraph* g);

void remove_path(MVGraph* g);

#endif
