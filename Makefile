Prog : main.o Prime.o test_files.o
	gcc -o Prog main.o Prime.o test_files.o -w -std=gnu9x -lcunit
main.o : main.c Prime.h test_files.h
	gcc -c main.c -w -std=gnu9x -lcunit
test_files.o : test_files.c Prime.h 
	gcc -c test_files.c -w -std=gnu9x -lcunit
Prime.o : Prime.c
	gcc -c Prime.c -w -std=gnu9x -lcunit
clean : 
	rm -rf *.o
	rm -rf Prog
	rm -rf testPrincipaleOutput.txt
	rm -rf debug
	rm -rf *xml
	rm -rf DebugOutput.txt

debug : main.c Prime.c test_files.c Prime.h test_files.h
	make clean
	gcc -c Prime.c -w -std=gnu9x -lcunit
	gcc -c test_files.c -w -std=gnu9x -lcunit
	gcc -c main.c -w -std=gnu9x -lcunit
	gcc -o debug main.o Prime.o test_files.o -w -std=gnu9x -lcunit
	valgrind --leak-check=yes --log-file="DebugOutput.txt" -q ./debug
	echo  "\n" >> DebugOutput.txt
	cppcheck --enable=all --inconclusive  Prime.c 2>> DebugOutput.txt
	echo  "\n" >> DebugOutput.txt
	echo  "\n" >> DebugOutput.txt
	./debug >> DebugOutput.txt