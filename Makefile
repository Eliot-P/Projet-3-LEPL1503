Prog : main.o Prime.o test_lib.o
	gcc -o Prog main.o Prime.o test_lib.o
main.o : main.c Prime.h test_lib.h
	gcc -c main.c
test_lib.o : test_lib.c Prime.h
	gcc -c test_lib.c
Prime.o : Prime.c
	gcc -c Prime.c

clean : 
	rm -rf *.o
	rm -rf Prog