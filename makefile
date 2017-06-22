CC = gcc
CPP = g++
CFLAGS = -g 
CPPFLAGS = -g -std=c++11

DEFS = 
DEPS = predicate.h rule.h 
OBJS = rule.o predicate.o
LIBS = 

%.o: %.cpp $(DEPS)
#	    echo "~~~ Making "$@;
		    $(CPP) -c $(DEFS) $(CPPFLAGS) -o $@ $<

%.o: %.c $(DEPS)
#	    echo "~~~ Making "$@;
		    $(CC) --std=c99 -c $(DEFS) $(CFLAGS) -o $@ $<

#pcietest: test.o
#	    g++ --std=c++11 $(DEFS) $(CPPFLAGS) -o $@ $^ 
#
#memtest: memread.o
#	    g++ --std=c++11 $(DEFS) $(CPPFLAGS) -o $@ $^ 
#
#query: query.o
#	    g++ --std=c++11 $(DEFS) $(CPPFLAGS) -o $@ $^ 

all: 
	objs: $(OBJS)

