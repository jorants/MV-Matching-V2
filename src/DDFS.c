
#include "DDFS.h"


/*
  This macro will add all predecesors of a nodes bud_star to the stack. 
 */
#define add_pred_to_stack(cur,stack)				\
  do{								\
    MVNodeP pred;						\
    for_each(pred,cur->preds,{					\
	if(pred != NULL){	\
	  MVEdge * tmp;						\
	  alloc_in_list((stack),tmp);				\
	  tmp->n1 = cur;					\
	  tmp->n2 = pred;					\
	}							\
      })							\
      }while(false);


//gives the level of the node you would step into
#define L(e) bud_star(e->n2)->min_level

//shorthands to acces the stacks quickly, they are stored in the graph to avoid mallocs
#define Sr g->red_stack
#define Sg g->green_stack

/*
  This macro sets the below pointer of the current node to the next one and calculates the bud_star.
  It does not yet step, as we might need to backtrack of have found  a petal
 */
#define prepare_next(Nx)			\
  if(Nx) {					\
    if(Nx->n1){					\
      Nx->n1->below = Nx->n2;			\
    }						\
    Nx->n2 = bud_star(Nx->n2);			\
  }


#define node_from_stack(Nx,S)			\
  if(S.length > 0)	       /*Check wether we hit the end of a backtrack*/ \
    Nx = popp(S);							\
  else									\
    Nx = NULL;								\

/*
  Steps to the next node
 */
#define step_into(C,Nx,S)						\
    prepare_next(Nx);              /* Set below pointer and calculate bud_star*/ \
    if(Nx->n2->visited == false){						\
      Nx->n2->above = Nx->n1;						\
      C = Nx->n2;                   /*Change center of DFS*/		\
      C->visited = true;						\
      C->ddfs_green = green_top;					\
      C->ddfs_red = red_top;						\
      add_to_list(result->nodes_seen,C);				\
      add_pred_to_stack(C,S);      /*Add al unseen predecesors*/	\
    }									\
    node_from_stack(Nx,S)

int DDFS(MVGraph * g,MVNodeP green_top,MVNodeP red_top){


  //result is stored in the graph to avoid free's and mallocs
  DDFS_result * result = &(g->last_ddfs);
  //reset result
  result->nodes_seen.length = 0;
  result->bottleneck = NULL;

  //reset stacks
  Sr.length = 0;
  Sg.length = 0;
  //Start at the bridges, no center yet
  MVNodeP G = NULL;
  MVNodeP R = NULL;


  //steel memory from the stack to create the first next struct
  MVEdge * Ng;
  alloc_in_list((Sg),Ng);			
  Ng = popp(Sg);
  Ng->n1 = NULL;  //there is no center yet
  Ng->n2 = green_top; //here is where we want to go 

  MVEdge * Nr;  
  alloc_in_list((Sr),Nr);			
  Nr = popp(Sr);
  Nr->n1 = NULL;
  Nr->n2 = red_top;

  //used to keep track later if we backtrack.
  //if a backtrack fails then we need to jump back to the DDFS collision that started it
  MVEdge red_before,green_before;
  

  if(bud_star(Nr->n2) == bud_star(Ng->n2)){
    return DDFS_EMPTY;   //avoid the whole loop thing
  }else if (Ng->n2->min_level == 0 && Nr->n2->min_level == 0){
    return DDFS_PATH; //return imidetly for one edge paths
  }

  
 

  
  while( R==NULL || G==NULL || //we are starting
	 R->min_level >0 || G->min_level>0){    //we need some walking still



    while(Nr && Ng && L(Nr) != L(Ng)){
      while(Nr && L(Nr) > L(Ng)){ //quit if we have no stack left
	step_into(R,Nr,Sr);
      }

      if(Nr == NULL){ //backtracked till end of stack, jump back tpo lowest bottleneck
	Nr = &red_before;
	MVNodeP tmp_itt = red_before.n1; 
	while(tmp_itt->above){
	  tmp_itt->above->below = tmp_itt;
	  tmp_itt = tmp_itt->above;
	}
      }
      while(Ng && L(Nr) < L(Ng)){
	step_into(G,Ng,Sg);
      }

      if(Ng == NULL){ //backtracked till end of stack, jump back tpo lowest bottleneck
	Ng = &green_before;
	MVNodeP tmp_itt = green_before.n1; 
	while(tmp_itt->above){
	  tmp_itt->above->below = tmp_itt;
	  tmp_itt = tmp_itt->above;
	}
      }
    }
    

    
    if(bud_star(Nr->n2)  == bud_star(Ng->n2)){
      //backtrack
      if(Sr.length > 0){
	red_before.n1 = Nr->n1;
	red_before.n2 = Nr->n2;
	prepare_next(Nr);
	node_from_stack(Nr,Sr);
	if(Nr)
	  R = Nr->n1;
	else
	  Nr = &red_before;
	
      }else if(Sg.length > 0){
	green_before.n1 = Ng->n1;
	green_before.n2 = Ng->n2;
	prepare_next(Ng);
	node_from_stack(Ng,Sg);
	
	if(Ng)
	  G = Ng->n1;
	else
	  Ng = &green_before;
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


