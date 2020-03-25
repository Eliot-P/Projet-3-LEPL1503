Prog : main.o Prime.o test_lib.o
	gcc -o Prog main.o Prime.o test_lib.o -std=c99
main.o : main.c Prime.h test_lib.h
	gcc -c main.c -std=c99
test_lib.o : test_lib.c Prime.h
	gcc -c test_lib.c -std=c99
Prime.o : Prime.c
	gcc -c Prime.c -std=c99

clean : 
	rm -rf *.o
	rm -rf Prog