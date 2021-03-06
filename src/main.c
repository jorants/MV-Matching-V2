/*
This file is intended to serve as a usage example.
Call with:
./main [filename]
 */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include <sys/time.h>


#include "list.h"
#include "Graph.h"
#include "MV.h"

//for time measurement
#define dt(b,a) ( (double) (a.tv_usec - b.tv_usec) / 1000000 + (double) (a.tv_sec - b.tv_sec))

int is_edge(MVNodeP node,MVNodeP other){
  MVNodeP edge;
  for_each(edge,node->edges,{
      if(other==edge){
	return true;
      }
    });
  return false;
}

int main(int argc, char** argv){

  if (argc<2)
    exit(0);


  char *fn = argv[1];
  FILE * fp;
  if(strcmp(fn,"-") == 0){
    fp = stdin;
  }else
    {
      fp = fopen (fn, "r");
    }
  int V,E;
  int res = fscanf(fp,"%i %i\n",&V,&E);
  if(res!=2)
    {
      printf("Error in input\n");
      exit(0);
    }

  MVGraph * g = new_graph_edges(V,E);
  res = 2;
  
  while(res == 2){
    int n1,n2;
    res = fscanf(fp,"%i %i\n",&n1,&n2);
    if(res==2)
      MV_add_edge(g,n1,n2);
  }


  struct timeval start,end;
  greedy_init(g);  
  gettimeofday(&start, NULL);
  max_match(g);
  gettimeofday(&end, NULL);

  /*
  int correct  = true;
  
  MVNodeP node;
  for_eachp(node,g->nodes,{

      if(node->match && !is_edge(node,node->match)){
	printf("help");
	debug(": %i %i",node->N,node->match->N);
	printf("\n");
      }else if(node->match){
        debug("match: %i %i\n",node->N,node->match->N);	
      }
      
      if(node->match && node->match->match != node){
	correct = false;
	break;
      }
    });
  */
  delete_graph(g);
  printf("%f\n",dt(start,end));
  printf("%i\n",g->matchnum);  
  //printf("%i\n",correct);
  return 0;
}
