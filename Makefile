default_target: app

APPNAME := app

#source file
# OBJS := tt_semaphore.o tt_msgqueue.o test_msgqueue.o
OBJS := tt_msgqueue.o test_msgqueue.o

#compile and lib parameter
CC      := gcc
CXX     := g++
LIBS    := -lpthread
LDFLAGS :=
DEFINES :=

INCLUDE :=
CFLAGS  := -g -Wall -O3 $(DEFINES) $(INCLUDE)
CXXFLAGS := $(CFLAGS)

${OBJS}: %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

${APPNAME}: ${OBJS}
	$(CC) ${OBJS} -o $@ $(LDFLAGS) $(LIBS)

clean:
	rm -f *.gch
	rm -f *.o
	rm -f $(APPNAME)

