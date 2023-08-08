
PROGRAM=doom-fire

build:
	gcc -std=c17 -Wall -Wpedantic gifenc.c main.c -o $(PROGRAM)

run: build 
	./$(PROGRAM)

clean:
	rm -f $(PROGRAM)
	rm -f *.gif
