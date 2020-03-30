Prog : main.o Prime.o test_lib.o
	gcc -o Prog main.o Prime.o test_lib.o  -w
main.o : main.c Prime.h test_lib.h
	gcc -c main.c  -w
test_lib.o : test_lib.c Prime.h
	gcc -c test_lib.c  -w
Prime.o : Prime.c
	gcc -c Prime.c  -w

clean : 
	rm -rf *.o
	rm -rf Prog
	clear