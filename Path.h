#ifndef PATH_H
#define PATH_H

#include "typedefs.h"
#include "list.h"
#include "Node.h"
#include "Graph.h"
#include "lists.h"
#include<stdio.h>
#include<stdlib.h>

void find_path(MVNodeP n1,MVNodeP n2,MVGraph* g);
void augment_path(MVGraph* g);
void remove_path(MVGraph* g);

#endif
