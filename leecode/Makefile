Srcs := $(wildcard *.cpp)
Outs := $(patsubst %.cpp, %, $(Srcs))

CC := g++
CFLAGS = -Wall -g -w

ALL: $(Outs) 

%: %.cpp
	$(CC) $< -o $@ $(CFLAGS)

.PHONY: clean rebuild ALL

clean:
	$(RM) $(Outs)
rebuild: clean ALL

