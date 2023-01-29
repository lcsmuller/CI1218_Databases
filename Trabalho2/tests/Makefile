TOP = ..

INP_EXT = .in
OUT_EXT = .out
NO_EXT  = test1 test2 test3 test4 test5 test6

EXEC = $(TOP)/escalona

all: $(EXEC) $(NO_EXT)

.DEFAULT:
	@ echo "Checking $@"
	@ $(EXEC) < $@$(INP_EXT) | diff - $@$(OUT_EXT) && echo "Success!"

.NOTPARALLEL: all
