EXECUTABLE=doom_hand.elf
TTY=/dev/ttyUSB0
all:
	gcc -g doom.c -lxdo -o $(EXECUTABLE)

debug:
	gdb --args $(EXECUTABLE) $(TTY)

clean:
	rm $(EXECUTABLE)

cheap:
	gcc -g doom_cheap.c -lxdo -o $(EXECUTABLE)

run:
	./$(EXECUTABLE) $(TTY)
