#include "Node.h"


void init_node(MVNodeP node,int label, int degree){
  node->N = label;
  init_list(node->edges,MVNodeP,degree);
  node->match = UNMATCHED;
  init_list(node->preds,MVNodeP,degree);
  init_list(node->pred_to,MVNodePos,degree);
  init_list(node->hanging_bridges,MVNodeP,4);
  reset_node(node);
}
void uninit_node(MVNodeP node){
  uninit_list(node->edges);
  uninit_list(node->preds);
  uninit_list(node->pred_to);
  uninit_list(node->hanging_bridges);  
}


inline void reset_node(MVNodeP node){
  node->min_level = oo;
  node->max_level = UNSET;
  node->odd_level = UNSET;
  node->even_level = UNSET;
  node->preds.length = 0;
  node->pred_to.length = 0;;
  node->bud = NULL;
  node->above  = NULL;
  node->below  = NULL;
  node->ddfs_green  = NULL;
  node->ddfs_red  = NULL;
  node->deleted = false;
  node->number_preds = 0;
  node->hanging_bridges.length = 0;
  node->visited = false;
}


MVNodeP bud_star(MVNodeP c){
  MVNodeP b = bud(c);
  return b ?  bud_star(b) : c;
}


int bud_star_includes(MVNodeP c,MVNodeP goal){
  if(c == goal)
    return true;
  
  MVNodeP b = bud(c);
  return b ?  bud_star_includes(b,goal) : false;
}
