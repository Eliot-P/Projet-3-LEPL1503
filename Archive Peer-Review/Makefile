fact : main.o fact.o
	gcc -o fact main.o fact.o  -w -std=gnu9x -lpthread -lm
test : test.o fact.o
	gcc -o test fact.o  test.o -w -std=gnu9x -lcunit -lpthread -lm
test.o : test.c fact.h
	gcc -c test.c -w -std=gnu9x -lcunit -lm
main.o : main.c fact.h
	gcc -c main.c -w -std=gnu9x -lm 
fact.o : fact.c
	gcc -c fact.c -w -std=gnu9x -lpthread -lm

clean : 
	rm -rf *.o
	rm -rf test
	rm -rf fact