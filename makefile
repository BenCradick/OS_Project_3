CC = cc
CFLAGS = -std=gnu99
OBJ = main palin
ALL: $(OBJ)
$(OBJ): %: %.c
	$(CC) $(CFLAGS) -o $@ $<
clean:
	/bin/rm $(OBJ)
