#ifndef _MONITOR_H_
#define _MONITOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#define MEM 0
#define CPU 1;

// COLORS
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


struct process{
  char* name;
  int pid;
  float cpu_usage;
  float mem_usage;
  int type;
  char* stat;
  LIST_ENTRY(process) processes;
};


struct buffer{
  char* line;
  LIST_ENTRY(buffer) buffers;
};

typedef LIST_HEAD(pr_list, process) head_p;
typedef LIST_HEAD(bf_list, buffer) head_b;


void fill_list_buf(head_b *hb, head_p* hp, int* max_cpu, int* ok_cpu, int* max_mem);
void finish_processes(head_b *hb, head_p* hp);
void init_heads(head_p* hp, head_b *hb);
int suspend_process(int pid);
int start_process(int pid);
int kill_process(int pid);
void stop_buffer_process(head_p* hp, struct process* p, head_b* hb, struct buffer* b);
int get_free_ram();
float get_total_ram();
int get_max_cpu_pid(head_p* hp);
int get_max_mem_pid(head_p* hp);
void print_processes(head_p* hp, struct process* item_proc);
void check_processes(head_p* hp, struct process* item_proc, float max_cpu, float ok_cpu, float max_mem);

void end_processes(head_p* hp, struct process* item_proc);
#endif
