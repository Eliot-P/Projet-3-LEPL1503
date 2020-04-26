fact : main.o fact.o
	gcc -o fact main.o fact.o  -w -std=gnu9x -lpthread
test : test.o fact.o
	gcc -o test fact.o  test.o -w -std=gnu9x -lcunit -lpthread
test.o : test.c fact.h
	gcc -c test.c -w -std=gnu9x -lcunit
main.o : main.c fact.h
	gcc -c main.c -w -std=gnu9x  
fact.o : fact.c
	gcc -c fact.c -w -std=gnu9x -lpthread

clean : 
	rm -rf *.o
	rm -rf test
	rm -rf fact