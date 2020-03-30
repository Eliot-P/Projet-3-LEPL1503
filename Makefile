Prog : main.o Prime.o test_lib.o
	gcc -o Prog main.o Prime.o test_lib.o -w -std=gnu99 
main.o : main.c Prime.h test_lib.h
	gcc -c main.c -w -std=gnu99 
test_lib.o : test_lib.c Prime.h
	gcc -c test_lib.c -w -std=gnu99 
Prime.o : Prime.c
	gcc -c Prime.c -w -std=gnu99 

clean : 
	rm -rf *.o
	rm -rf Prog
	clear