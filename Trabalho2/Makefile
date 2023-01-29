CXX      = g++
CXXFLAGS = -I.

TESTSDIR = tests

EXEC = escalona
OBJS = alg_core.o

all: $(EXEC)

test: $(EXEC)
	@ $(MAKE) -C $(TESTSDIR)

$(EXEC): $(OBJS)

clean:
	@ rm -f $(OBJS) $(EXEC)

.PHONY: all test clean
