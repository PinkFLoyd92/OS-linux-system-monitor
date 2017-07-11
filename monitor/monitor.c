#include "monitor.h"

#define BUFSIZE 200
#define BUFMEM 100
#define TIME_FREE 2
#define COMMAND "ps -o pid,pcpu=CPU_USAGE,pmem,comm,stat $(pgrep -f _hungry) 2>&-;"
#define FREE "free -m | awk '/Mem/ {print $7;}'"
#define TOTAL_RAM "free | grep Mem | awk '{print $3/$2 * 100.0}'"

ssize_t _read;
void finish_processes(head_b *hb, head_p *hp){

  const char *cmd = COMMAND;
  char *buf;//Output of the command
  FILE *ptr;
  char * pch;
  char* tmp_buf;
  size_t bufsize = BUFSIZE;
  struct buffer *item_buf; // struct that will be inside of our list.
  struct process *item_proc; // struct that will be inside of our list.
  tmp_buf = (char*)malloc(bufsize*sizeof(char));
  buf = (char *)malloc(bufsize * sizeof(char));

  if (NULL == (ptr = popen(cmd, "r"))) {
    printf("An error ocurred");
    exit(EXIT_FAILURE);
  }

  while((_read = getline(&buf, &bufsize, ptr))!= -1) {
    item_buf = (struct buffer*)malloc(sizeof(struct buffer));
    item_buf->line = buf;
    if(strstr(buf, "PID") == NULL && (strstr(buf, "sh") == NULL)){
      LIST_INSERT_HEAD(hb, item_buf, buffers);
    }
    buf = (char *)malloc(bufsize * sizeof(char));
  }

  LIST_FOREACH(item_buf, hb, buffers){
    int cnt_spc = 0;
    item_proc = (struct process*)malloc(sizeof(struct process));

    strcpy(tmp_buf, item_buf->line);
    pch = strtok(tmp_buf," ");
    // printf("Linea: %s\n", item_buf->line);
    LIST_INSERT_HEAD(hp, item_proc, processes);
    while (pch != NULL)
      {
	//printf ("%s\n",pch);

	switch (cnt_spc) {
	case 0: {
	  item_proc->pid = atoi(pch);
	  break;
	}

	case 1: {
	  item_proc->cpu_usage = atof(pch);
	  break;
	}
	case 2: {
	  item_proc->mem_usage = atof(pch);
	  break;
	}
	case 3: {
	  if (item_proc->mem_usage > 0.1) {
	    item_proc->name = "mem_hungry";
	  }else{
	    item_proc->name = "cpu_hungry";
	    }
	  /* item_proc->name = pch; */
	  //strcpy(item_proc->name, pch);
	  break;
	}
	case 4: {
	  if (strstr(pch, "S") != NULL || strstr(pch, "R") != NULL) {
	    item_proc->stat =(char*) "ACTIVE";
	  }else if(strstr(pch, "T") != NULL){
	    item_proc->stat = (char*)"SUSPENDED";
	  }else{
	    item_proc->stat = pch;
	  }
	  break;
	}
	default:
	  break;
	}

	pch = strtok (NULL, " ");
	cnt_spc++;
      }

  }

  end_processes(hp, item_proc);
  //stop_buffer_process(hp, item_proc, hb, item_buf);

}

void end_processes(head_p* hp, struct process* item_proc){
    LIST_FOREACH(item_proc, hp, processes){
      kill_process(item_proc->pid);
      LIST_REMOVE(item_proc, processes);
      printf("\n" ANSI_COLOR_CYAN     "TERMINATED PROCESS: %s PID: %d"     ANSI_COLOR_RESET "\n", item_proc->name, item_proc->pid);
    }

}

void fill_list_buf(head_b *hb, head_p *hp, int* max_cpu, int* ok_cpu, int* max_mem){

  const char *cmd = COMMAND;
  char *buf;//Output of the command
  FILE *ptr;
  char * pch;
  char* tmp_buf;
  size_t bufsize = BUFSIZE;
  struct buffer *item_buf; // struct that will be inside of our list.
  struct process *item_proc; // struct that will be inside of our list.
  tmp_buf = (char*)malloc(bufsize*sizeof(char));
  buf = (char *)malloc(bufsize * sizeof(char));

  if (NULL == (ptr = popen(cmd, "r"))) {
    printf("An error ocurred");
    exit(EXIT_FAILURE);
  }

  while((_read = getline(&buf, &bufsize, ptr))!= -1) {
    item_buf = (struct buffer*)malloc(sizeof(struct buffer));
    item_buf->line = buf;
    if(strstr(buf, "PID") == NULL && (strstr(buf, "sh") == NULL)){
      LIST_INSERT_HEAD(hb, item_buf, buffers);
    }
    buf = (char *)malloc(bufsize * sizeof(char));
  }

  LIST_FOREACH(item_buf, hb, buffers){
    int cnt_spc = 0;
    item_proc = (struct process*)malloc(sizeof(struct process));

    strcpy(tmp_buf, item_buf->line);
    pch = strtok(tmp_buf," ");
    // printf("Linea: %s\n", item_buf->line);
    LIST_INSERT_HEAD(hp, item_proc, processes);
    while (pch != NULL)
      {
	//printf ("%s\n",pch);

	switch (cnt_spc) {
	case 0: {
	  item_proc->pid = atoi(pch);
	  break;
	}

	case 1: {
	  item_proc->cpu_usage = atof(pch);
	  break;
	}
	case 2: {
	  item_proc->mem_usage = atof(pch);
	  break;
	}
	case 3: {
	  if (item_proc->mem_usage > 0.1) {
	    item_proc->name = "mem_hungry";
	  }else{
	    item_proc->name = "cpu_hungry";
	    }
	  /* item_proc->name = pch; */
	  //strcpy(item_proc->name, pch);
	  break;
	}
	case 4: {
	  if (strstr(pch, "S") != NULL || strstr(pch, "R") != NULL) {
	    item_proc->stat =(char*) "ACTIVE";
	  }else if(strstr(pch, "T") != NULL){
	    item_proc->stat = (char*)"SUSPENDED";
	  }else{
	    item_proc->stat = pch;
	  }
	  break;
	}
	default:
	  break;
	}

	pch = strtok (NULL, " ");
	cnt_spc++;
      }

  }

  print_processes(hp, item_proc);
  check_processes(hp, item_proc, *max_cpu, *ok_cpu, (*max_mem));
  stop_buffer_process(hp, item_proc, hb, item_buf);

}


void check_processes(head_p* hp, struct process* item_proc, float max_cpu, float ok_cpu, float max_mem){
  LIST_FOREACH(item_proc, hp, processes){
    if (item_proc->cpu_usage > max_cpu) {
      suspend_process(item_proc->pid);
      printf("\n" ANSI_COLOR_CYAN     "SUSPENDED PROCESS: %s PID: %d"     ANSI_COLOR_RESET "\n", item_proc->name, item_proc->pid);
    }
    /* if (item_proc->mem_usage > max_mem) { */
    /*   kill_process(item_proc->pid); */
    /*   printf("\n" ANSI_COLOR_CYAN     "TERMINATED PROCESS: %s PID: %d"     ANSI_COLOR_RESET "\n", item_proc->name, item_proc->pid); */
    /* } */

    if (item_proc->cpu_usage <= ok_cpu && strcmp(item_proc->stat ,(char*)"SUSPENDED") == 0) {
      start_process(item_proc->pid);
      printf("\n" ANSI_COLOR_CYAN     "RESTARTING PROCESS: %s PID: %d"     ANSI_COLOR_RESET "\n", item_proc->name, item_proc->pid);
    }
  }

  float ram_used = get_total_ram();
  struct process* pointer;
  while (ram_used>max_mem) { // max_mem is the max percentege of memory used.
    float maxRamUsed = -1;
    ram_used = get_total_ram();
    LIST_FOREACH(item_proc, hp, processes){
      if (item_proc->mem_usage > maxRamUsed) {
	pointer = item_proc;
	maxRamUsed = item_proc->mem_usage;
      }
    }
    kill_process(pointer->pid);
    LIST_REMOVE(pointer, processes);
    printf("\n" ANSI_COLOR_CYAN     "TERMINATED PROCESS: %s PID: %d"     ANSI_COLOR_RESET "\n", pointer->name, pointer->pid);
    sleep(TIME_FREE);  //esperamos un tiempo para que se reasigne la memoria
  }

  // print_processes(hp, item_proc);
}

void print_processes(head_p* hp, struct process* item_proc){
  printf("\n" ANSI_COLOR_RED     "PROCESSES:"     ANSI_COLOR_RESET "\n");
  printf(ANSI_COLOR_YELLOW  "NAME          MEM     CPU       PID      STAT"  ANSI_COLOR_RESET "\n");
  LIST_FOREACH(item_proc, hp, processes){
    printf(ANSI_COLOR_GREEN   "%s"   ANSI_COLOR_RESET " -- ", item_proc->name );
    printf(ANSI_COLOR_GREEN   "%.2f"   ANSI_COLOR_RESET " -- ", item_proc->mem_usage );
    printf(ANSI_COLOR_GREEN   "%.2f"   ANSI_COLOR_RESET " -- ", item_proc->cpu_usage );
    printf( ANSI_COLOR_GREEN   "%d"   ANSI_COLOR_RESET " -- ", item_proc->pid);
    printf( ANSI_COLOR_GREEN   "%s"   ANSI_COLOR_RESET "\n", item_proc->stat);
    /* printf("\nName: %s Memory Usage: %.4f CPU usage %.4f, PID: %d, STAT: %s",item_proc->name, item_proc->mem_usage, item_proc->cpu_usage, item_proc->pid, item_proc->stat); */
  }
}
int get_max_cpu_pid(head_p* hp){
  struct process *proc;

  proc = (struct process*)malloc(sizeof(struct process));

  float max = -1;
  int pid = 0;
  LIST_FOREACH(proc, hp, processes){
    if(proc->cpu_usage>max){
      max = proc->cpu_usage;
      pid = proc->pid;
    }
  }
  return pid;
}


int get_max_mem_pid(head_p* hp){
  struct process *proc;

  proc = (struct process*)malloc(sizeof(struct process));

  float max = -1;
  int pid = 0;
  LIST_FOREACH(proc, hp, processes){
    if(proc->mem_usage>max){
      max = proc->mem_usage;
      pid = proc->pid;
    }
  }
  return pid;
}

void stop_buffer_process(head_p* hp, struct process* p, head_b* hb, struct buffer* b){
  LIST_FOREACH(b, hb, buffers){
    LIST_REMOVE(b, buffers);	
    free(b);
  }
  LIST_FOREACH(p, hp, processes){
    LIST_REMOVE(p, processes);	
    free(p);
  }
}

void init_heads(head_p* hp, head_b* hb){
  LIST_INIT(hp);
  LIST_INIT(hb);

}

int suspend_process(int pid){
  kill(pid, SIGSTOP);
  return pid;
}

int start_process(int pid){
  kill(pid, SIGCONT);
  return pid;
}

int kill_process(int pid){
  kill(pid, SIGTERM);
  return pid;
}

int get_free_ram(){
  FILE *ptr;
  const char *cmd = FREE;
  char *buf;//Output of the command
  size_t bufsize = BUFSIZE;

  if (NULL == (ptr = popen(cmd, "r"))) {
    printf("An error ocurred");
    exit(EXIT_FAILURE);
  }
  buf = (char *)malloc(bufsize * sizeof(char));
  while((_read = getline(&buf, &bufsize, ptr))!= -1) {

    printf("\nThe memory is %s \n", buf);
    return atoi(buf);
  }
  return -1;
}

float get_total_ram(){
  FILE *ptr;
  const char *cmd = TOTAL_RAM;
  char *buf;//Output of the command
  size_t bufsize = BUFSIZE;

  if (NULL == (ptr = popen(cmd, "r"))) {
    printf("An error ocurred");
    exit(EXIT_FAILURE);
  }
  buf = (char *)malloc(bufsize * sizeof(char));
  while((_read = getline(&buf, &bufsize, ptr))!= -1) {

    /* printf("\nThe total memory is %s \n", buf); */
    return atof(buf);
  }
  return -1;
}
// struct process *list = malloc(sizeof(struct process)*1);
//first we have to check the pids of the hunger process and mem process
//--------------------
//Now we parse each pid and add it to the list
