#include "main.h"
#define TIME_SLEEP 2
void print_usage(){
      printf("Usage: sys_monitor -C max_cpu -O ok_cpu -M max_memory \n");
}

int main(int argc, char *argv[])
{
  int option = 0;
  int MAX_CPU = -1;
  int OK_CPU = -1;
  int MAX_MEM = -1;
  head_p proc_head;
  head_b buf_head;

  while ((option = getopt(argc, argv,"C:O:M:")) != -1) {
    switch (option) {
    case 'C' : MAX_CPU = atoi(optarg); 
      break;
    case 'O' : OK_CPU = atoi(optarg);
      break;
    case 'M' : MAX_MEM = atoi(optarg);
      break;
    default: print_usage(); 
      exit(EXIT_FAILURE);
    }
  }
  if (MAX_CPU == -1 || MAX_MEM ==-1 || OK_CPU == -1) {
    print_usage();
    exit(EXIT_FAILURE);
  }

  //INITIALIZING HEADS OF LISTS
  init_heads(&proc_head, &buf_head);
  for (int i = 0; i < 5; i++) {
    printf("\nIteracion %d:\n", i);
    fill_list_buf(&buf_head, &proc_head);
    sleep(TIME_SLEEP);
  }
  exit(EXIT_SUCCESS);
}
