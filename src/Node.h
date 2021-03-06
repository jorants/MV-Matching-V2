#ifndef NODE_H
#define NODE_H
#include<stdlib.h>
#include "list.h"
#include "lists.h"
#include "typedefs.h"


typedef struct MVNode{
  int N; //Label for easy printing
  int min_level;
  int max_level;
  int odd_level;
  int even_level;
  list_MVNodeP edges;
  MVNodeP match;
  list_MVNodeP preds;
  list_MVNodePos pred_to;
  MVNodeP bud;
  MVNodeP above;
  MVNodeP below;
  MVNodeP ddfs_green;
  MVNodeP ddfs_red;
  int deleted;
  int visited;
  int number_preds;
  list_MVNodeP hanging_bridges;
} MVNode;



void init_node(MVNodeP node,int label,int degree);
void uninit_node(MVNodeP node);
void reset_node(MVNodeP node);

#define set_min_level(nodep,level)			\
  nodep->min_level = level;				\
  if(((level) % 2) != 0)				\
    nodep->odd_level = level;				\
  else							\
    nodep->even_level = level;				\

#define set_max_level(nodep,level)		\
  nodep->max_level = level;			\
  if((level) % 2)				\
    nodep->odd_level = level;			\
  else						\
    nodep->even_level = level;			\

#define outer(node) node->even_level < node->odd_level
#define inner(node) node->even_level >= node->odd_level

#define bud(node) node->bud;

MVNodeP bud_star(MVNodeP c);
int bud_star_includes(MVNodeP c,MVNodeP goal);

#endif
