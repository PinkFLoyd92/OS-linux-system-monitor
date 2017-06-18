CC=gcc
PROGRAMS=sys_monitor monitor.o
MONITOR_DIR="monitor"

all: monitor.o 
	$(CC) main.c monitor.o -g -o sys_monitor

monitor.o:
	$(CC) -g -c $(MONITOR_DIR)/monitor.c -o monitor.o

clean:
	@rm -f $(PROGRAMS) *.o
