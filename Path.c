#include "Path.h"

//TODO expand with blossoms
void find_path(MVNodeP n1,MVNodeP n2,MVGraph* g){
  g->path_found.length = 0;
  walk_down_path(&g->path_found, n1);
  reverse(&g->path_found, 0,g->path_found.length);
  walk_down_path(&g->path_found, n2);    
}


void reverse(list_MVNodeP *list,int from, int to){
  int i;
  MVNodeP tmp;
  for(i=from;i<from + (to-from)/2;i++){
    tmp = list->list[i];
    list->list[i] = list->list[to-i-1];
    list->list[to-i-1] = tmp;
  }
  
}

//function for the finding of a path, not for walking in a blossom
void walk_down_path(list_MVNodeP * list,MVNodeP start){
  debug("Walk_down_path (%i){ \n",start->N);
  MVNodeP cur = start;
  while(cur){
    if( cur->bud != NULL){
      //Stept into a blossom
      debug("Steped into blossom\n");
      cur = walk_blossom(list,cur);
    }else{
      debug( "%i\n",cur->N);
      add_to_list((*list),cur);
      cur = cur->below;
    }
  }
  debug("}\n");
}


inline MVNodeP jump_bridge(MVNodeP cur){
  debug("%i :: %p %p %i %i\n",cur->N,cur->ddfs_red,cur->ddfs_green,cur->ddfs_red->N,cur->ddfs_green->N);
  return cur->ddfs_green == cur ? cur->ddfs_red : cur->ddfs_green;
}


//Walks a blossom from the entry node to the bud.
//It does not add the bud, it does add the entry node
MVNodeP walk_blossom(list_MVNodeP * list,MVNodeP cur){
  debug("Walk_blossom (%i) { \n",cur->N);

  if(outer(cur)){
    //just walk down
    cur = walk_blossom_down(list,cur);
  }else{
    //walk up, then down
    cur = walk_blossom_up(list,cur);
    cur = jump_bridge(cur);
    cur = walk_blossom_down(list,cur);
  }
  debug("}\n");
  return cur;
}

//walks down until it reacheds the bud of a blossom
//It does not add the bud, it does add the first node
MVNodeP walk_blossom_down(list_MVNodeP * list,MVNodeP cur){
  debug("Walk_blossom down { \n");
  MVNodeP b = cur->bud;
  //todo add jump detection
  while(cur && cur != b){
    if( cur->bud != b){
      //Stept into a blossom
      debug("Steped into blossom\n");
      cur = walk_blossom(list,cur);
    }else{
      debug( "%i\n",cur->N);
      add_to_list((*list),cur);
      cur = cur->below;
    }
  }
  debug("}\n");
  return cur;
}


//Walks up untill it reaches a bridge
//It does add the bridge and the node it starts on
MVNodeP walk_blossom_up(list_MVNodeP * list,MVNodeP cur){
  debug("Walk_blossom up (%i){ \n",cur->N);

  while(cur){
    add_to_list((*list),cur);
    debug( "%i\n",cur->N);
    if(cur->above && cur->above->below != cur){
      //About to jump over something
      int before = list->length;
      MVNodeP bud = cur->above->below;
      while(bud != cur){
	bud = walk_blossom(list,bud);
      }
      reverse(list,before,list->length);
    }
    if(cur->above == NULL){
      break;
    }
    cur = cur->above;
  }
  debug("}\n");
  return cur;
}



void augment_path(MVGraph* g){
  int i;
  MVNodeP* lll = g->path_found.list;
  int l = g->path_found.length;
  debug("Path: (%i)\n",g->path_found.length);
  for(i=0;i<l-1;i+=2){
    debug(" %i -- %i \n",lll[i]->N,lll[i+1]->N);
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

