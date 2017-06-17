#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "monitor/monitor.h" 
#include "main.h"

void print_usage(){
      printf("Usage: sys_monitor -C max_cpu -O ok_cpu -M max_memory \n");
}

int main(int argc, char *argv[])
{
  int option = 0;
  int MAX_CPU = -1;
  int OK_CPU = -1;
  int MAX_MEM = -1;

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

  exit(EXIT_SUCCESS);
}
