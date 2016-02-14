#include "Node.h"


void init_node(MVNodeP node,int label){
  node->N = label;
  init_list(node->edges,MVNodeP,4);
  node->match = UNMATCHED;
  init_list(node->preds,MVNodeP,4);
  init_list(node->pred_to,MVNodePos,4);
  init_list(node->hanging_bridges,MVNodeP,2);
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


inline MVNodeP bud_star(MVNodeP c){
  MVNodeP b = bud(c);
  //debug(">> %i\n",c->N);
  return b ?  bud_star(b) : c;
}
