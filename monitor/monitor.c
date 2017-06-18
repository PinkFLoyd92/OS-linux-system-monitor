#include "monitor.h"
#define BUFSIZE 200
ssize_t read;

void fill_list_buf(head_b *hb){

  const char *cmd = "ps up $(pgrep -f _hungry) 2>&-;";
  char *buf;//Output of the command
  FILE *ptr;
  size_t bufsize = BUFSIZE;
  struct buffer *item_buf; // struct that will be inside of our list.


  if (NULL == (ptr = popen(cmd, "r"))) {
    printf("An error ocurred");
    exit(EXIT_FAILURE);
  }

  while((read = getline(&buf, &bufsize, ptr))!= -1) {
    buf = (char *)malloc(bufsize * sizeof(char));
    item_buf = (struct buffer*)malloc(sizeof(struct buffer));
    item_buf->line = buf;
    LIST_INSERT_HEAD(hb, item_buf, buffers);
  }

  LIST_FOREACH(item_buf, hb, buffers){
    printf("Linea: %s\n", item_buf->line);
  }

  free(item_buf);
  free(buf);
  exit(EXIT_SUCCESS);
}

void init_heads(head_p* hp, head_b* hb){
  LIST_INIT(hp);
  LIST_INIT(hb);

}


// struct process *list = malloc(sizeof(struct process)*1);
//first we have to check the pids of the hunger process and mem process
//--------------------
//Now we parse each pid and add it to the list
