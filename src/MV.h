#ifndef MV_H
#define MV_H
#include<stdlib.h>
#include "list.h"
#include "typedefs.h"
#include "lists.h"
#include "Graph.h"
#include "Path.h"
#include "Node.h"
#include "DDFS.h"

void max_match(MVGraph * g);
int max_match_phase(MVGraph * g);

void MIN(MVGraph * g,int i);
int MAX(MVGraph * g,int i);


void step_to(MVGraph * g,MVNodeP to,MVNodeP from,int level);
int tenacity(MVNodeP n1,MVNodeP n2);

void greedy_init(MVGraph * g);

#endif
