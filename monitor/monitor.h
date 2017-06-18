#ifndef _MONITOR_H_
#define _MONITOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>

struct process{
  char* name;
  int uuid;
  int cpu_usage;
  int mem_usage;
  LIST_ENTRY(node) nodes;
};


struct buffer{
  char* line;
  LIST_ENTRY(buffer) buffers;
};

typedef LIST_HEAD(pr_list, process) head_p;
typedef LIST_HEAD(bf_list, buffer) head_b;


void fill_list_buf(head_b *hb);
void init_heads(head_p* hp, head_b *hb);

#endif
