CC=gcc
PROGRAMS=sys_monitor monitor.o $(PROCESSES_DIR)/cpu_hungry $(PROCESSES_DIR)/cpu_hungry
MONITOR_DIR="monitor"
PROCESSES_DIR="processes"

all: monitor.o 
	$(CC) main.c monitor.o -g -o sys_monitor
	$(CC) $(PROCESSES_DIR)/CPU_HUNGRY.c -o $(PROCESSES_DIR)/cpu_hungry
	$(CC) $(PROCESSES_DIR)/MEM_HUNGRY.c -o $(PROCESSES_DIR)/mem_hungry

monitor.o:
	$(CC) -g -c $(MONITOR_DIR)/monitor.c -o monitor.o

clean:
	@rm -f $(PROGRAMS) *.o
