CC=g++
LFLAGS=
CFLAGS=-c -Wall
OBJS=ourschedule.o schedule.o
PROGS=ourschedule 

all: $(PROGS)

ourschedule: ourschedule.o schedule.o
	$(CC) schedule.o ourschedule.o -o ourschedule $(LFLAGS)

ourschedule.o: ourschedule.cpp 
	$(CC) ourschedule.cpp -o ourschedule.o $(CFLAGS)


schedule.o: schedule.cpp 
	$(CC) schedule.cpp -o schedule.o $(CFLAGS)

clean:
	@rm -f $(PROGS) $(OBJS)

run: ourschedule
	./ourschedule