
#ifndef GRAPH_H
#define GRAPH_H
#include "list.h"
#include "Node.h"
#include "lists.h"
#include<stdio.h>
#include<stdlib.h>

//needs lists and so can not be defined earlier
typedef struct DDFS_result{
  list_MVNodeP nodes_seen;
  MVNodeP bottleneck;
} DDFS_result;


typedef struct MVGraph{
  list_MVNode nodes;
  list_list_MVNodeP levels;
  list_list_MVBridge bridges;
  int matchnum;
  int bridgenum;
  int todonum;
  DDFS_result last_ddfs;
  list_MVEdge green_stack;
  list_MVEdge red_stack;
  list_MVNodeP path_found;
} MVGraph;


MVGraph * new_graph(int size,int degree);
void delete_graph(MVGraph * g);
void reset_graph(MVGraph * g);
void add_to_level(MVGraph * g,int level,MVNodeP node);
void add_to_bridges(MVGraph * g,int level,MVNodeP n1,MVNodeP n2);

MVGraph * new_graph_edges(int size,int num_edges);
MVGraph * new_graph_constant(int size);


#define MV_add_edge(g,a, b) \
  do{\
    MVNodeP na = getp(g->nodes,a);		\
    MVNodeP nb = getp(g->nodes,b);		\
    add_to_list(na->edges,nb);			\
    add_to_list(nb->edges,na);			\
  }while(0);


#endif
