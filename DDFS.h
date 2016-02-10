#ifndef DDFS_H
#define DDFS_H

#include<stdlib.h>
#include "Graph.h"
#include "Node.h"

#define DDFS_EMPTY 1
#define DDFS_PETAL 1
#define DDFS_PATH 2


int DDFS(MVGraph * g,MVNodeP green_top,MVNodeP red_top);
#endif
