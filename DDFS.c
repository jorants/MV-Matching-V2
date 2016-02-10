
#include "DDFS.h"

int DDFS(MVGraph * g,MVNodeP green_top,MVNodeP red_top){
  DDFS_result * result = &(g->last_ddfs);
  list_MVNodeP * green_stack = &(g->green_stack);
  list_MVNodeP * red_stack = &(g->red_stack);

  result->nodes_seen.length = 0;
  
  result->bottleneck = NULL;
  green_stack->length = 0;
  red_stack->length = 0;
  MVNodeP green_cur = bud_star(green_top);
  MVNodeP red_cur = bud_star(red_top);
  if(green_cur == red_cur)
    return DDFS_EMPTY;

  int level_green = green_cur->min_level;
  int level_red = red_cur->min_level;
  add_to_list((*green_stack),green_cur);
  add_to_list((*red_stack),red_cur);
  while(true){
    
    
  }
  
}


