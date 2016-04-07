/*
This file makes the most simple typedefs.
Namres of lists are declared in lists.h
 */

#ifndef TYPEDEFS_H
#define TYPEDEFS_H


#define debug(...) printf(__VA_ARGS__)
#define debug(...)

//Used to store vertex information
typedef struct MVNode MVNode;
//Used to store the graph as a whole
typedef struct MVGraph MVGraph;
//symantics
typedef MVNode * MVNodeP;

//Used in case a bridge is found. It will never me malloced direcly but always be in a list.
typedef struct{
  MVNodeP n1;
  MVNodeP n2;
} MVBridge;

//Used for the DDFS, otherwise edges are not stored in there own datastructure.
//Even here they are not malloced every time, there is a big buffer instead.
typedef struct{
  MVNodeP n1;
  MVNodeP n2;
} MVEdge;

//Used to make a list of PredecessorTo relations.
//These require the node you are predecessor to but also the possition in its list, for quick removal.
typedef struct{
  MVNodeP n;
  int pos;
} MVNodePos;


//Some trivial constants.
#define UNSET -1
#define oo 1<<30
#define true 1
#define false 0
#define UNMATCHED NULL
#endif
