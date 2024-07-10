Srcs := $(wildcard *.c)
Outs := $(patsubst %.c, %, $(Srcs))

CC := g++
CFLAGS = -Wall -g

ALL: $(Outs) 

%: %.c
	$(CC) $< -o $@ $(CFLAGS)

.PHONY: clean rebuild ALL

clean:
	$(RM) $(Outs)
rebuild: clean ALL

