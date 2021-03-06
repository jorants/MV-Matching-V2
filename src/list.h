/*
This file defines a set of macros that can be used to make a list of any other datastrucure.
The basic idea is that the list is a malloced array with its size (number of objects allocated) and length (lenght of the list) stored with it.
If length reaches size, then the list is realloced.
There are some complications because sometimes we want to store the list as a pointer and sometimes as a local variable.
Also, sometimes we would like a pointer to the position in the list and sometimes we would like the value to be returned.
To solve this, all functions come in three flavors:
- expect list variable and return/set by value
- excpect a list pointer, not variable, return/set by value
- expect list variable and return/set by pointer
See the coresponding comments.

To define the structure of a new list just add list_struct(type) to some file.
This will create a new type list_type
*/
#ifndef LIST_H
#define LIST_H
#include<stdlib.h>

#define list_struct(type)					\
  typedef struct list_##type{					\
  int size;							\
  int length;							\
  type * list;							\
  } list_##type;							
  
//Loop over a list that is pointed to  and use the value
#define for_p_each(itt,l,f)						\
  do{									\
    int list_itterator;							\
    for(list_itterator=0;list_itterator < l->length ;list_itterator++){	\
      itt = (l)->list[list_itterator];					\
      f }								\
  }while(0);

//Loop over a list and use the value
#define for_each(itt,l,f)						\
  do{									\
    int list_itterator;							\
    for(list_itterator=0;list_itterator < l.length ;list_itterator++){	\
      itt = l.list[list_itterator];					\
      f }								\
  }while(0);

//loop over a list and use the pointers to the elements
#define for_eachp(itt,l,f)						\
  do{									\
    int list_itterator;							\
    for(list_itterator=0;list_itterator < l.length ;list_itterator++){	\
      itt = l.list+list_itterator;					\
      f }								\
  }while(0);



//Destroys a list created with make_list
#define destroy_list(l)						\
     if(l==NULL)						\
       return ;							\
     free(l.list);						\
     free(l);							

//Destroys a list created with init_list
#define uninit_list(l)						\
     free(l.list);						\

//Destroys a list created with init_p_list
#define uninit_p_list(l)						\
     free(l->list);						\



//Adds a item to the list by value
#define  add_to_list(l,a)						\
  if(l.length >= l.size){						\
    l.size = l.size << 1;						\
    l.list = realloc(l.list,sizeof(l.list[0])*l.size);		\
  }									\
  l.list[l.length++] = a;						
  

//Allocs space in a list and sets a to a pointer
#define alloc_in_list(l,a)					\
  if(l.length>= l.size){					\
    l.size = l.size << 1;					\
    l.list = realloc(l.list,sizeof(l.list[0])*l.size);	\
  }								\
  a =  l.list+l.length++;					
  

//Makes a new list and sets l to the pointer
#define make_list(l,type,startsize)				\
  l = malloc(sizeof(list_##type));				\
  (l)->list = malloc((startsize)*sizeof(type));			\
  (l)->length = 0;						\
  (l)->size= (startsize);					

//Inits an already malloced list l
#define init_list(l,type,startsize)				\
  (l).list = malloc((startsize)*sizeof(type));			\
  (l).length = 0;						\
  (l).size= (startsize);					

//Inits an already malloced list l that is apointer
#define init_p_list(l,type,startsize)				\
  (l)->list = malloc((startsize)*sizeof(type));				\
  (l)->length = 0;							\
  (l)->size= (startsize);					



#define len(l) (l.length)
#define get(l,p) (l.list[p])
#define getp(l,p) (l.list+p)
#define first(l) (l.list[0])
#define last(l) (l.list[l.length-1])
#define pop(l) l.length ? (l.list[(--l.length)]) : NULL
#define popp(l) l.length ? (l.list+(--l.length)) : NULL

#endif
