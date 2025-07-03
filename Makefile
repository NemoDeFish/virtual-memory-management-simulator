CC = cc
CFLAGS = -Wall -g
EXE = translate
OBJ = task1.o task2.o task3.o task4.o linked_list.o file_io.o bit_utils.c

$(EXE): main.c $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) $<

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $< 

clean:
	rm -f *.o $(EXE)

format:
	clang-format -style=file -i *.c *.h