Prog : main.o Prime.o test_lib.o
	gcc -o Prog main.o Prime.o test_lib.o -w -std=gnu9x -lcunit
main.o : main.c Prime.h test_lib.h
	gcc -c main.c -w -std=gnu9x -lcunit
test_lib.o : test_lib.c Prime.h 
	gcc -c test_lib.c -w -std=gnu9x -lcunit
Prime.o : Prime.c
	gcc -c Prime.c -w -std=gnu9x -lcunit
clean : 
	rm -rf *.o
	rm -rf Prog
	rm -rf testPrincipaleOutput.txt
	clear
execute :
	make
	./Prog