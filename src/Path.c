#include "Path.h"


void find_path(MVNodeP n1,MVNodeP n2,MVGraph* g){
  g->path_found.length = 0;
  walk_down_path(&g->path_found, n1);
  reverse(&g->path_found, 0,g->path_found.length);  
  walk_down_path(&g->path_found, n2);    
}


void reverse(list_MVNodeP *list,int from, int to){
  int i;
  MVNodeP tmp;
  int d = (to-from);
  for(i=0;i<d/2;i++){
    tmp = list->list[i+from];
    list->list[i+from] = list->list[to-i-1];
    list->list[to-i-1] = tmp;
  }
  
}

//function for the finding of a path, not for walking in a blossom
void walk_down_path(list_MVNodeP * list,MVNodeP start){
  MVNodeP cur = start;
  while(cur){
    if( cur->bud != NULL){
      //Stept into a blossom
      cur = walk_blossom(list,cur);

    }else{
      add_to_list((*list),cur);
      cur = cur->below;
    }
  }
}


inline MVNodeP jump_bridge(list_MVNodeP * list,MVNodeP cur){
  if(cur->ddfs_green == cur){
    return cur->ddfs_red;
  }else if(cur->ddfs_red == cur){
    return cur->ddfs_green;
  }else if(bud_star_includes(cur->ddfs_green,cur)){ //have to jump to get to the bridge

    int before = list->length;
    MVNodeP bud = cur->ddfs_green;
    while(bud != cur){
      bud = walk_blossom(list,bud);
    }
    reverse(list,before,list->length);
    return cur->ddfs_red;
  }else{ //jump and then step from red
    
    int before = list->length;
    MVNodeP bud = cur->ddfs_red;
    while(bud != cur){
      bud = walk_blossom(list,bud);
    }
    reverse(list,before,list->length);

    return cur->ddfs_green;
  }

  
}



//Walks a blossom from the entry node to the bud.
//It does not add the bud, it does add the entry node
MVNodeP walk_blossom(list_MVNodeP * list,MVNodeP cur){
  if(outer(cur)){
    //just walk down
    cur = walk_blossom_down(list,cur,NULL);
  }else{
    //walk up, then down
    cur = walk_blossom_up(list,cur);
    MVNodeP before = cur;
    cur = jump_bridge(list,cur);    
    cur = walk_blossom_down(list,cur,before);
  }
  return cur;
}

//walks down until it reacheds the bud of a blossom
//It does not add the bud, it does add the first node
MVNodeP walk_blossom_down(list_MVNodeP * list,MVNodeP cur,MVNodeP before){
  if(before==NULL)
    before = cur;
  MVNodeP b = cur->bud;
  while(cur && cur != b){
      if( cur->ddfs_green != before->ddfs_green ||
	cur->ddfs_red != before->ddfs_red){
      //Stept into a blossom

	cur = walk_blossom(list,cur);
    }else{
      add_to_list((*list),cur);
      cur = cur->below;
    }
  }
  return cur;
}


//Walks up untill it reaches a bridge
//It does add the bridge and the node it starts on
MVNodeP walk_blossom_up(list_MVNodeP * list,MVNodeP cur){

  while(cur){
    add_to_list((*list),cur);
    if(cur->above && cur->above->below != cur  &&//at a crossing or a jump 
       bud_star_includes(cur->above->below,cur)){
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
  return cur;
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

