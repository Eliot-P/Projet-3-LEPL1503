fact : main.o fact.o
	gcc -o fact main.o fact.o  -w -std=gnu9x -lpthread -lm -g
test : test.o fact.o
	gcc -o test fact.o  test.o -w -std=gnu9x -lcunit -lpthread -lm -g
test.o : test.c fact.h
	gcc -c test.c -w -std=gnu9x -lcunit -lm -g
main.o : main.c fact.h
	gcc -c main.c -w -std=gnu9x -lm -g
fact.o : fact.c
	gcc -c fact.c -w -std=gnu9x -lpthread -lm -g

clean : 
	rm -rf *.o
	rm -rf test
	rm -rf fact

val: fact
	valgrind --tool=memcheck --leak-check=yes ./fact -N 4 Input.txt Output.txt

run: fact
	./fact -N 4 Input.txt Output.txt