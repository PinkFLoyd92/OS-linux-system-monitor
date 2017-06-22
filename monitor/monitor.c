#include "monitor.h"

#define BUFSIZE 200
#define COMMAND "ps -o pid,pcpu=CPU_USAGE,pmem,comm,stat $(pgrep -f _hungry) 2>&-;"

ssize_t read;

void fill_list_buf(head_b *hb, head_p *hp){

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

  while((read = getline(&buf, &bufsize, ptr))!= -1) {
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
	  item_proc->name = pch;
	  break;
	}
	case 4: {
	  if (strstr(pch, "S") != NULL) {
	    item_proc->stat = "activado";
	  }else if(strstr(pch, "T") != NULL){
	    item_proc->stat = "suspendido";
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

  LIST_FOREACH(item_proc, hp, processes){
    printf("\nName: %s Memory Usage: %.4f CPU usage %.4f, PID: %d, STAT: %s",item_proc->name, item_proc->mem_usage, item_proc->cpu_usage, item_proc->pid, item_proc->stat);
  }

  stop_buffer_process(hp, item_proc, hb, item_buf);

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


// struct process *list = malloc(sizeof(struct process)*1);
//first we have to check the pids of the hunger process and mem process
//--------------------
//Now we parse each pid and add it to the list
