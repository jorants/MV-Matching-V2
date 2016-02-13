#include "MV.h"

inline int tenacity(MVNodeP n1,MVNodeP n2){
  if(n1->match == n2){
    //matched bridge
    if(n1->odd_level != UNSET && n2->odd_level!= UNSET){
      return n1->odd_level + n2->odd_level +1;
    }
  }else{
    //unmatched bridge
    if(n1->even_level != UNSET && n2->even_level!= UNSET){
      return n1->even_level + n2->even_level +1;
    }
    
  }
  return UNSET;
}


void max_match(MVGraph * g){
  //for once set the levels by hand, next time reset does this
  MVNodeP itt3;
  for_eachp(itt3,g->nodes,{
      if(itt3->match == UNMATCHED){
	add_to_level(g,0,itt3);
	set_min_level(itt3,0);
      }
    });  
  int p = 0;
  debug("========== Phase %i ===========\n",p++);
  while(g->nodes.length / 2 > g->matchnum && max_match_phase(g)){
    debug("========== Phase %i ===========\n",p++);
    reset_graph(g);
  }

}

int max_match_phase(MVGraph * g){
  int found = false;
  int i;

  
  for(i=0;i<g->nodes.length/2+1 && (!found);i++){
    debug("----------------------- %i ------------------\n",i);
    //printf(":: %i %i \n",g->todonum ,g->bridgenum);
    if(g->todonum<=0 && g->bridgenum<=0){
      return false;
    }
    MIN(g,i);
    found = MAX(g,i);
    
  }
  
  return found;
}


inline void step_to(MVGraph * g,MVNodeP to,MVNodeP from,int level){

  if(to->min_level >= level+1){

    if(to->min_level != level+1){
      add_to_level(g,level+1,to);
    }
    set_min_level(to,level+1);
    add_to_list(to->preds,from);
    to->number_preds++;
    MVNodePos * np;
    alloc_in_list(from->pred_to,np);
    np->n = to;
    np->pos = to->preds.length-1;
  }else{
    //found bridge

    int ten = tenacity(to,from);

    if(ten == UNSET){
      //hanging bridge
      add_to_list(to->hanging_bridges,from);
      add_to_list(from->hanging_bridges,to);
    }else{
      add_to_bridges(g,(ten-1)/2,to,from);
    }

  }
}


void MIN(MVGraph * g,int i){
  MVNodeP current;
  
  for_each(current,get(g->levels,i),{
      g->todonum--;
      debug("Walking from %i {\n",current->N);
      if(i%2==0){
	//need unmatched edge
	MVNodeP edge;
	
	for_each(edge,current->edges,{

	    if(edge!= current->match){
	      step_to(g,edge,current,i);
	      
	    }

	  });

      }else if(current->match != UNMATCHED){
	//need matched edge
	step_to(g,current->match,current,i);
      }

    });
}

int MAX(MVGraph * g,int i){
  MVBridge * current;
  int found = false;
  for_eachp(current,get(g->bridges,i),{
      g->bridgenum--;
      MVNodeP  n1 = current->n1;
      MVNodeP  n2 = current->n2;
      if(n1->deleted || n2->deleted)
	continue;

      //      printf("Bridge: %i %i \n",n1->N,n2->N);
      int result = DDFS(g,n1,n2);
      if(result == DDFS_EMPTY){
	debug("EMPTY\n");
	
	continue;
      }
      else if(result == DDFS_PATH){
	find_path(n1,n2,g);
	augment_path(g);
	remove_path(g);
	found = true;
      }else{
	//Found a petal
	MVNodeP itt;
	MVNodeP b = g->last_ddfs.bottleneck;
	int current_ten = i*2+1;
	for_each(itt,g->last_ddfs.nodes_seen,{
	    //set the bbud for all nodes
	    debug("Bud (%i) = %i\n",itt->N,b->N);
	    itt->bud = b;
	    //set the maxlevel
	    set_max_level(itt,current_ten - itt->min_level);
	    MVNodeP hanging;
	    for_each(hanging,itt->hanging_bridges,{
		//check all the hanging bridges to see if they are known now
		int hanging_ten = tenacity(itt,hanging);
		if(hanging_ten != UNSET){
		  add_to_bridges(g,(hanging_ten-1)/2,itt,hanging);
		}
	      });
	    
	  });
	
      }
	 
    });
  
  return found;
}

