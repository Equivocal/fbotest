CC=gcc
CXX=g++
RM=rm -rf
OPTS=-c -Wall
INCDIRS=$(shell find . -type d | grep "include")
INC=$(foreach d, $(INCDIRS), -I$d)
CPPFLAGS=-g -O0 -I/usr/local/include -I/usr/include/SDL2 -D_REENTRANT -I. -I/usr/include $(INC) -c -Wall -std=c++11
LDFLAGS=-g
LDLIBS=-L/usr/local/lib -L/usr/lib -L/usr/local/lib -lGLEW -lGLU -lGL -L/usr/lib/x86_64-linux-gnu -lSDL2 
SRCDIR=.
OBJDIR=obj
BINDIR=bin
DEPDIR=dep

MAKEDEPEND = touch $*.d && makedepend $(CPPFLAGS) -f $*.d $<

SRCS=$(shell find $(SRCDIR) -name "*.cpp")
OBJS=$(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

all: nexcpp

nexcpp:	$(OBJS)
	test -d $(BINDIR) || mkdir $(BINDIR)
	$(CXX) $(LDFLAGS) -o bin/nexcpp $(OBJS) $(LDLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ $(BINDIR) *.log

test:
	$(OBJS)
