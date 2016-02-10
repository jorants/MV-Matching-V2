#include<stdio.h>
#include<stdlib.h>

#include "list.h"
#include "Graph.h"
#include "MV.h"

list_struct(int);

int main(){
  MVGraph * g = new_graph(6);
  MV_add_edge(g,0,1,true);
  MV_add_edge(g,0,2,true);
  MV_add_edge(g,1,3,true);
  MV_add_edge(g,3,4,true);
  MV_add_edge(g,4,5,true);
  
  max_match(g);
  return 0;
}
