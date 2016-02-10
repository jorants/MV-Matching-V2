
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
  list_MVNodeP green_stack;
  list_MVNodeP red_stack;
} MVGraph;


MVGraph * new_graph(int size);
void delete_graph(MVGraph * g);
void reset_graph(MVGraph * g);
void MV_add_edge(MVGraph * g,int a,int b, int greedy);
void add_to_level(MVGraph * g,int level,MVNodeP node);
#endif
