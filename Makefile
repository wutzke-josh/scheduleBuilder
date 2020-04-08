CC=g++
LFLAGS=
CFLAGS=-c -Wall
OBJS=objects
PROGS=ourschedule 

all: $(PROGS)

ourschedule: objects/ourschedule.o objects/schedule.o
	$(CC) objects/schedule.o objects/ourschedule.o -o ourschedule $(LFLAGS)

objects/ourschedule.o: source/ourschedule.cpp 
	mkdir -p $(@D)
	$(CC) source/ourschedule.cpp -o objects/ourschedule.o $(CFLAGS)


objects/schedule.o: source/schedule.cpp 
	mkdir -p $(@D)
	$(CC) source/schedule.cpp -o objects/schedule.o $(CFLAGS)

clean:
	@rm -rf $(PROGS) $(OBJS)

run: ourschedule
	./ourschedule