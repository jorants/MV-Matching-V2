#ifndef TYPEDEFS_H
#define TYPEDEFS_H


#define debug(...) 
//#define debug(...) printf(__VA_ARGS__)

typedef struct MVNode MVNode;
typedef struct MVGraph MVGraph;
typedef MVNode * MVNodeP;

typedef struct{
  MVNodeP n1;
  MVNodeP n2;
} MVBridge;

typedef struct{
  MVNodeP n1;
  MVNodeP n2;
} MVEdge;

typedef struct{
  MVNodeP n;
  int pos;
} MVNodePos;


#define UNSET -1
#define oo 1<<30
#define true 1
#define false 0
#define UNMATCHED NULL
#endif