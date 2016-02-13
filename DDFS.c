
#include "DDFS.h"


#define add_pred_to_stack(cur,stack)				\
  do{								\
    MVNodeP pred;						\
    for_each(pred,cur->preds,{					\
	if(pred != NULL &&  bud_star(pred)->visited!=true){	\
	  MVEdge * tmp;						\
	  alloc_in_list((stack),tmp);				\
	  tmp->n1 = cur;					\
	  tmp->n2 = pred;					\
	}							\
      })							\
      }while(false);



#define L(e) bud_star(e->n2)->min_level
#define Sr g->red_stack
#define Sg g->green_stack

#define prepare_next(Nx)			\
  if(Nx) {					\
    if(Nx->n1)					\
      Nx->n1->below = Nx->n2;			\
    Nx->n2 = bud_star(Nx->n2);			\
  }


#define step_into(C,Nx,S)				\
  prepare_next(Nx)					\
  Nx->n2->above = Nx->n1;				\
  C = Nx->n2;						\
  C->visited = true;					\
  C->ddfs_green = green_top;				\
  C->ddfs_red = red_top;				\
  add_to_list(result->nodes_seen,C);			\
  add_pred_to_stack(C,S);				\
  Nx = popp(S);					



int DDFS(MVGraph * g,MVNodeP green_top,MVNodeP red_top){
  DDFS_result * result = &(g->last_ddfs);
  debug("--- DDFS(%i %i) --- \n",green_top->N,red_top->N);
  result->nodes_seen.length = 0;
  result->bottleneck = NULL;
  g->green_stack.length = 0;
  g->red_stack.length = 0;
  MVNodeP G = NULL;
  MVNodeP R = NULL;
  
  MVEdge * Ng;
  alloc_in_list((Sg),Ng);			
  Ng = popp(Sg);
  Ng->n1 = NULL;
  Ng->n2 = green_top;

  MVEdge * Nr;
  alloc_in_list((Sr),Nr);			
  Nr = popp(Sr);
  Nr->n1 = NULL;
  Nr->n2 = red_top;


  if(bud_star(Nr->n2) == bud_star(Ng->n2)){
    return DDFS_EMPTY;
  }else if (Ng->n2->min_level == 0 && Nr->n2->min_level == 0){
    return DDFS_PATH; //return imidetly for one edge paths
  }

  

  debug("Starting:");
  if(Nr->n2)
    debug(" %i",Nr->n2->N);
  if(Ng->n2)
    debug(" %i\n",Ng->n2->N);
  
  
  while( R==NULL || G==NULL || //we are starting
	 R->min_level >0 || G->min_level>0){    //we need some walking still
    
    while(L(Nr) != L(Ng)){
      while(L(Nr) > L(Ng)){
	step_into(R,Nr,Sr);
      }
      while(L(Nr) < L(Ng)){
	step_into(G,Ng,Sg);
      }
    }
    
    debug(":: %i %i \n",Nr->n2->N,Ng->n2->N);
    debug("  -> Sr : ");
    MVEdge itt;
    for_each(itt,Sr,{debug(" %i",itt.n2->N);});
    debug("\n  -> Sg : ");
    for_each(itt,Sg,{debug(" %i",itt.n2->N);});
    debug("\n");

    
    if(bud_star(Nr->n2)  == bud_star(Ng->n2)){
      //backtrack
      if(Sr.length > 0){ 
	prepare_next(Nr);
	Nr = popp(Sr);
	R = Nr->n1;
      }else if(Sg.length > 0){
	prepare_next(Ng);
	Ng = popp(Sg);
	G = Ng->n1;
      }else{
	prepare_next(Nr);
	prepare_next(Ng);
	g->last_ddfs.bottleneck = Nr->n2;
	return DDFS_PETAL;
      }
    }else{
      //now Nr!=Ng
      step_into(R,Nr,Sr);
      step_into(G,Ng,Sg);
    }
  }
  return DDFS_PATH;
}


