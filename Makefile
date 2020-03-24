Prog : main.o Premier_pas.o test_lib.o
	gcc -o Prog main.o Premier_pas.o test_lib.o
main.o : main.c Premier_pas.h test_lib.h
	gcc -c main.c
test_lib.o : test_lib.c Premier_pas.h
	gcc -c test_lib.c
Premier_pas.o : Premier_pas.c
	gcc -c Premier_pas.c