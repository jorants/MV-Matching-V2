#ifndef DDFS_H
#define DDFS_H

#include<stdlib.h>
#include "Graph.h"
#include "Node.h"

#define DDFS_EMPTY 0
#define DDFS_PETAL 1
#define DDFS_PATH 2
#define DDFS_OK 3
#define DDFS_BOTTLENECK 4

/*
  This function will run a DDFS on green_top and red_top and store the result in g->ddfs_result;
  It will return DDFS_EMPTY, DDFS_PETAL or DDFS_PATH depending on the outcome of the DDFS
 */
int DDFS(MVGraph * g,MVNodeP green_top,MVNodeP red_top);

#endif
