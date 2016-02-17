#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include <sys/time.h>



#include "list.h"
#include "Graph.h"
#include "MV.h"

#define dt(b,a) ( (double) (a.tv_usec - b.tv_usec) / 1000000 + (double) (a.tv_sec - b.tv_sec))

int main(int argc, char** argv){
  int gr = false;
  if(argc>2)
    gr = true;
  else if (argc<2)
    exit(0);


  char *fn = argv[1];
  FILE * fp;
  if(strcmp(fn,"-") == 0){
    fp = stdin;
  }else
    {
      fp = fopen (fn, "r");
    }
  int V;
  int res = fscanf(fp,"%i\n",&V);
  if(res!=1)
    {
      printf("Error in input\n");
      exit(0);
    }
  MVGraph * g = new_graph(V);
  res = 2;
  
  while(res == 2){
    int n1,n2;
    res = fscanf(fp,"%i %i\n",&n1,&n2);
    if(res==2)
      MV_add_edge(g,n1,n2,gr);
  }


  struct timeval start,end;
  
  gettimeofday(&start, NULL);
  max_match(g);
  gettimeofday(&end, NULL);
  
  int correct  = true;
  
  MVNodeP node;
  for_eachp(node,g->nodes,{
      if(node->match && node->match->match != node){
	correct = false;
	break;
      }
    });
  delete_graph(g);
  printf("%f\n",dt(start,end));
  printf("%i\n",g->matchnum);  
  printf("%i\n",correct);
  return 0;
}
