CC = cc
CFLAGS = -std=gnu99 -pthread
OBJ = main palin
DEP = shared_memory.c
ALL: $(OBJ)
$(OBJ): %: %.c
	$(CC) $(CFLAGS) -o $@ $< $(DEP)
clean:
	/bin/rm $(OBJ)
