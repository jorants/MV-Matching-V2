#include "Graph.h"

MVGraph * new_graph(int size){
  MVGraph * res = malloc(sizeof(MVGraph));
  init_list(res->nodes,MVNode,size);
  res->nodes.length = size;//just alloc al the space, we will init it.
  int i = 0;
  MVNodeP itt;
  for_eachp(itt,res->nodes,{
      init_node(itt,i++);
    });

  init_list(res->levels,list_MVNodeP,size/2+1);
  init_list(res->bridges,list_MVBridge,size/2+1);

  init_list(res->last_ddfs.nodes_seen,MVNodeP,3);
  init_list(res->green_stack,MVNodeP,3);
  init_list(res->red_stack,MVNodeP,3);
  
  res->matchnum = 0;
  res->bridgenum = 0;
  res->todonum = 0;
  return res;
}

void delete_graph(MVGraph * g){

  list_MVNodeP * itt;
  for_eachp(itt,g->levels,{
      uninit_p_list(itt);
    });

  list_MVBridge * itt2;
  for_eachp(itt2,g->bridges,{
      uninit_p_list(itt2);
    });

  uninit_list(g->bridges);
  uninit_list(g->levels);
  MVNodeP itt3;
  for_eachp(itt3,g->nodes,{
      uninit_node(itt3);
    });

  uninit_list(g->nodes);
  free(g);
}



void reset_graph(MVGraph * g){

  list_MVNodeP * itt;
  for_eachp(itt,g->levels,{
      itt->length = 0;
    });

  list_MVBridge * itt2;
  for_eachp(itt2,g->bridges,{
      itt2->length = 0;
    });

  g->bridgenum = 0;
  g->todonum = 0;
  MVNodeP itt3;
  for_eachp(itt3,g->nodes,{
      reset_node(itt3);
      
      if(itt3->match == UNMATCHED){
	add_to_level(g,0,itt3);
	set_min_level(itt3,0);
      }
    });  
}


void MV_add_edge(MVGraph * g,int a,int b, int greedy){
  MVNodeP na = getp(g->nodes,a);
  MVNodeP nb = getp(g->nodes,b);
  add_to_list(na->edges,nb);
  add_to_list(nb->edges,na);
  if(greedy && na->match == UNMATCHED && nb->match == UNMATCHED){
    na->match = nb;
    nb->match = na;
    g->matchnum++;
  }
}


void add_to_level(MVGraph * g,int level,MVNodeP node){
  while(level>=g->levels.length){
    list_MVNodeP * level_list;
    alloc_in_list(g->levels,level_list);
    init_p_list(level_list,MVNodeP,2);
  }
  add_to_list(get(g->levels,level),node);  
}


void add_to_bridges(MVGraph * g,int level,MVNodeP n1,MVNodeP n2){
  
  while(level>=g->bridges.length){

    list_MVBridge * bridge_list;
    
    alloc_in_list(g->bridges,bridge_list);
    
    init_p_list(bridge_list,MVBridge,2);
    
  }
  
  MVBridge * b;
  alloc_in_list(get(g->bridges,level),b);
  b->n1 = n1;
  b->n2 = n2;
}
