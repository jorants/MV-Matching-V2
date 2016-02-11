#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include <sys/time.h>

#define dt(b,a) ( (double) (a.tv_usec - b.tv_usec) / 1000000 + (double) (a.tv_sec - b.tv_sec))


#include "list.h"
#include "Graph.h"
#include "MV.h"

list_struct(int);

int main(int argc, char** argv){
  int gr = false;
  if(argc>1)
    gr = true;
  
  int V;
  int res = scanf("%i\n",&V);
  if(res!=1)
    {
      printf("Error in input\n");
      exit(0);
    }
  MVGraph * g = new_graph(V);
  res = 2;
  
  while(res == 2){
    int n1,n2;
    res = scanf("%i %i\n",&n1,&n2);
    MV_add_edge(g,n1,n2,gr);
  }

  /*
  MVGraph * g = new_graph(6);
  MV_add_edge(g,1,2,false);  
  */
  // times
  struct timeval start,end;
  
  gettimeofday(&start, NULL);

  max_match(g);
  gettimeofday(&end, NULL);
  /*
  MVNodeP node;
  for_eachp(node,g->nodes,{
      if(node->match)
	printf(":: %i -- %i\n",node->N,node->match->N);
      else
	printf(":: %i not matched\n",node->N);
    });
  */
  printf("%f\n",dt(start,end));
  return 0;
}
