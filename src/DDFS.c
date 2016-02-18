
#include "DDFS.h"


/*
  This macro will add all predecesors of a nodes bud_star to the stack. 
 */
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
    if(Nx->n1)					\
      Nx->n1->below = Nx->n2;			\
    Nx->n2 = bud_star(Nx->n2);			\
  }


#define node_from_stack(Nx,S)			\
  do{									\
    int succes = false;							\
    while(S.length > 0){	       /*Check wether we hit the end of a backtrack*/ \
      Nx = popp(S);							\
      debug("Nx->n2->N:%i\n",Nx->n2->N);				\
      debug("bud_star(Nx->n2)->N:%i\n",bud_star(Nx->n2)->N);		\
      debug("bud_star(Nx->n2)->visited:%i\n",bud_star(Nx->n2)->visited); \
      if(bud_star(Nx->n2)->visited !=true){				\
	succes = true;							\
	break;								\
      }									\
    }									\
    debug("hier\n");							\
    if(succes == false)							\
      Nx = NULL;							\
  }while(0)

/*
  Steps to the next node
 */
#define step_into(C,Nx,S)						\
    prepare_next(Nx);              /* Set below pointer and calculate bud_star*/ \
    Nx->n2->above = Nx->n1;						\
    C = Nx->n2;                   /*Change center of DFS*/		\
    C->visited = true;							\
    C->ddfs_green = green_top;						\
    C->ddfs_red = red_top;						\
    add_to_list(result->nodes_seen,C);					\
    add_pred_to_stack(C,S);      /*Add al unseen predecesors*/		\
    node_from_stack(Nx,S)

int DDFS(MVGraph * g,MVNodeP green_top,MVNodeP red_top){

  debug("--- DDFS(%i %i) --- \n",green_top->N,red_top->N);

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

  

  debug("Starting:");
  if(Nr->n2)
    debug(" %i",Nr->n2->N);
  if(Ng->n2)
    debug(" %i\n",Ng->n2->N);
 

  
  while( R==NULL || G==NULL || //we are starting
	 R->min_level >0 || G->min_level>0){    //we need some walking still


    debug("129: %p %p\n",Nr,Ng);
    while(Nr && Ng && L(Nr) != L(Ng)){
      while(Nr && L(Nr) > L(Ng)){ //quit if we have no stack left
	step_into(R,Nr,Sr);
      }
      debug("124: %p %p\n",Nr,Ng);
      if(Nr == NULL){ //backtracked till end of stack, jump back tpo lowest bottleneck
	debug("backtrack red failed\n");
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
      debug("147: %p %p\n",Nr,Ng);
      if(Ng == NULL){ //backtracked till end of stack, jump back tpo lowest bottleneck
	debug("backtrack green failed\n");
	Ng = &green_before;
	MVNodeP tmp_itt = green_before.n1; 
	while(tmp_itt->above){
	  tmp_itt->above->below = tmp_itt;
	  tmp_itt = tmp_itt->above;
	}
      }
    }
    
    debug("159: %p %p\n",Nr,Ng);

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
	debug("backtrack red:\n");
	red_before.n1 = Nr->n1;
	red_before.n2 = Nr->n2;
	prepare_next(Nr);
	node_from_stack(Nr,Sr);
	if(Nr)
	  R = Nr->n1;
	else
	  Nr = &red_before;
	
      }else if(Sg.length > 0){
	debug("backtrack green:\n");
	green_before.n1 = Nr->n1;
	green_before.n2 = Nr->n2;
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
      debug("186: %i %i\n",Nr->n2->N,Ng->n2->N);
      step_into(R,Nr,Sr);
      step_into(G,Ng,Sg);
      debug("189: %p %p\n",Nr,Ng);
    }
  }
  return DDFS_PATH;
}


