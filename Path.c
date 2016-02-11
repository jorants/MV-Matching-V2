#include "Path.h"

//TODO expand with blossoms
void find_path(MVNodeP n1,MVNodeP n2,MVGraph* g){
  g->path_found.length = 0;
  MVNodeP cur = n1;
  while(cur){
    add_to_list(g->path_found,cur);
    cur = cur->below;
  }

  int i;
  int l = g->path_found.length;
  MVNodeP tmp;
  for(i=0;i<l/2;i++){
    tmp = g->path_found.list[i];
    g->path_found.list[i] = g->path_found.list[l-i-1];
    g->path_found.list[l-i-1] = tmp;
  }
  cur = n2;
  while(cur){
    add_to_list(g->path_found,cur);
    cur = cur->below;
  }
    
}

void augment_path(MVGraph* g){
  int i;
  MVNodeP* lll = g->path_found.list;
  int l = g->path_found.length;
  for(i=0;i<l-1;i+=2){
    lll[i]->match = lll[i+1];
    lll[i+1]->match = lll[i];
  }
  g->matchnum++;
}

void remove_path(MVGraph* g){
  while(g->path_found.length>0){
    MVNodeP current = pop(g->path_found);
    if(!current->deleted){
      //avoid double work
      current->deleted = true;
      MVNodePos * itt;
      for_eachp(itt,current->pred_to,{
	  if(!itt->n->deleted){
	    itt->n->preds.list[itt->pos] = NULL;
	    if((--(itt->n->number_preds)) <=0){
	      add_to_list(g->path_found,itt->n);
	    }
	  }
	});
    }
  }
}

