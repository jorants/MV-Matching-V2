#include "greedy.h"



int compare(MVNodeP a, MVNodeP b)
{
    if (a > b)
        return 1;
    else if (a < b)
        return -1;
    else
        return 0;
}

void heap_sort(MVNodeP * a, int (*func_pointer)(MVNodeP,MVNodeP), uint len)
{
    /* heap sort */
    uint half;
    uint parents;

    if (len <= 1)
        return;
    half = len >> 1;
    for (parents = half; parents >= 1; --parents)
    {
        MVNodeP tmp;
        int level = 0;
        uint child;

        child = parents;
        /* bottom-up downheap */

        /* leaf-search for largest child path */
        while (child <= half)
        {
            ++level;
            child += child;
            if ((child < len) &&  ((*func_pointer)(a[child], a[child - 1]) > 0))
                ++child;
        }
        /* bottom-up-search for rotation point */
        tmp = a[parents - 1];
        for (;;)
        {
            if (parents == child)
                break;
            if ((*func_pointer)(tmp, a[child - 1]) <= 0)
                break;
            child >>= 1;
            --level;
        }
        /* rotate nodes from parents to rotation point */
        for (; level > 0; --level)
        {
            a[(child >> level) - 1] =
                a[(child >> (level - 1)) - 1];
        }
        a[child - 1] = tmp;
    }

    --len;
    do
    {
        MVNodeP tmp;
        int level = 0;
        uint child;

        /* move max element to back of array */
        tmp = a[len];
        a[len] = a[0];
        a[0] = tmp;

        child = parents = 1;
        half = len >> 1;

        /* bottom-up downheap */

        /* leaf-search for largest child path */
        while (child <= half)
        {
            ++level;
            child += child;
            if ((child < len) && ((*func_pointer)(a[child], a[child - 1]) > 0))
                ++child;
        }
        /* bottom-up-search for rotation point */
        for (;;)
        {
            if (parents == child)
                break;
            if ((*func_pointer)(tmp, a[child - 1]) <= 0)
                break;
            child >>= 1;
            --level;
        }
        /* rotate nodes from parents to rotation point */
        for (; level > 0; --level)
        {
            a[(child >> level) - 1] =
                a[(child >> (level - 1)) - 1];
        }
        a[child - 1] = tmp;
    }
    while (--len >= 1);
}





void greedy_init(MVGraph * g){
  MVNodeP current;
  for_eachp(current,g->nodes,{
      if(current->match == UNMATCHED){
	MVNodeP other;
	for_each(other,current->edges,{
	    if(other->match == UNMATCHED){
	      //todo match
	      other->match = current;
	      current->match = other;
	      g->matchnum++;
	      break;
	    }
	  });
      }
    });
}



void greedy_sort_init(MVGraph * g){
  list_MVNodeP tmp_list;
  init_list(tmp_list,MVNodeP,g->nodes.length);
  MVNodeP current = NULL;
  for_eachp(current,g->nodes,{
      add_to_list(tmp_list,current);
    });
  heap_sort(tmp_list.list,compare,tmp_list.length);  
  
  
  for_each(current,tmp_list,{
      if(current->match == UNMATCHED){
	MVNodeP other;
	for_each(other,current->edges,{
	    if(other->match == UNMATCHED){
	      //todo match
	      other->match = current;
	      current->match = other;
	      g->matchnum++;
	      break;
	    }
	  });
      }
    });
}

