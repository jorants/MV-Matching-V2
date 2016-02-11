
#include "DDFS.h"


#define add_pred_to_stack(cur,stack)				\
  do{								\
    MVNodeP pred;						\
    for_each(pred,cur->preds,{					\
	if(pred != NULL){					\
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


#define step_into(C,N,S)			\
  N->n2->above = N->n1;				\
  C = N->n2;					\
  C->ddfs_green = green_top;			\
  C->ddfs_red = red_top;			\
  add_to_list(result->nodes_seen,C);		\
  add_pred_to_stack(C,S);			\
  N = popp(S);					\
  if(N) {					\
    N->n1->below = N->n2;			\
    N->n2 = bud_star(N->n2);			\
  }//TODO check bud_star


int DDFS(MVGraph * g,MVNodeP green_top,MVNodeP red_top){
  DDFS_result * result = &(g->last_ddfs);

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
    Ng->n2 = R;
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
  }else{
    add_pred_to_stack(G,Sr);
    Nr = popp(Sr); //next green  
    Nr->n1->below = Nr->n2;
    Nr->n2 = bud_star(Nr->n2);
  }

    
  add_to_list(result->nodes_seen,R);
  add_to_list(result->nodes_seen,G);  


  R->ddfs_green = G;
  R->ddfs_red = R;
  G->ddfs_green = G;
  G->ddfs_red = R;
  

  //TODO visited
  while(R->min_level >0 || G->min_level>0){
    
    while(L(Nr) != L(Ng)){
      while(L(Nr) > L(Ng)){
	step_into(R,Nr,Sr);
      }
      while(L(Nr) < L(Ng)){
	step_into(G,Ng,Sg);
      }
    }
    
    if(Nr  == Ng){
      //TODO Backtrack
      printf("TODO, backtrack\n");
      exit(0);
      //backtrack makes Nr != Ng with both levels at least wath they where
      //if not possible, it returns
    }
    //now Nr!=Ng
    step_into(R,Nr,Sr);
    step_into(G,Ng,Sg);
  }
  return DDFS_PATH;
}


