
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



#define L(e) e->n2->min_level
#define Sr g->red_stack
#define Sg g->green_stack

#define prepare_next(Nx)			\
  if(Nx && Nx->n1) {				\
    Nx->n1->below = Nx->n2;			\
    Nx->n2 = bud_star(Nx->n2);			\
  }//TODO check bud_star
//TODO fix for one side empty (Done?)

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
  MVNodeP G = bud_star(green_top);
  MVNodeP R = bud_star(red_top);
  if(R == G){
    return DDFS_EMPTY;
  }else if (G->min_level == 0 && R->min_level == 0){
    return DDFS_PATH; //return imidetly for one edge paths
  }

  MVEdge * Ng;
  if(G->min_level == 0){
    alloc_in_list((Sg),Ng);					\
    Ng = popp(Sg);
    Ng->n1 = NULL;
    Ng->n2 = G;
    G = NULL;
  }else{
    add_pred_to_stack(G,Sg);
    Ng = popp(Sg); //next green

    Ng->n1->below = Ng->n2;
    Ng->n2 = bud_star(Ng->n2);
  }

  MVEdge * Nr;
  if(R->min_level == 0){
    alloc_in_list((Sr),Nr);					\
    Nr = popp(Sr);
    Nr->n1 = NULL;
    Nr->n2 = R;
    R = NULL;
  }else{
    add_pred_to_stack(R,Sr);
    Nr = popp(Sr); //next red
    Nr->n1->below = Nr->n2;
    Nr->n2 = bud_star(Nr->n2);
  }
  
  debug("Starting:");
  if(Nr->n2)
    debug(" %i",Nr->n2->N);
  if(Ng->n2)
    debug(" %i\n",Ng->n2->N);

  if(R){
    add_to_list(result->nodes_seen,R);
    R->ddfs_green = G;
    R->ddfs_red = R;
  }
  if(G){
    add_to_list(result->nodes_seen,G);  
    G->ddfs_green = G;
    G->ddfs_red = R;
  }

  while((R && R->min_level >0) || (G && G->min_level>0)){
    
    while(L(Nr) != L(Ng)){
      while(L(Nr) > L(Ng)){
	step_into(R,Nr,Sr);

      }
      while(L(Nr) < L(Ng)){

	step_into(G,Ng,Sg);
      }
    }
    debug(":: %i %i \n",Nr->n2->N,Ng->n2->N);
    if(Nr->n2  == Ng->n2){
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


