Prog : main.o Prime.o test_files.o Prime_thread.o
	gcc -o Prog main.o Prime.o Prime_thread.o  test_files.o -w -std=gnu9x -lcunit -lpython2.7
main.o : main.c Prime.h test_files.h Prime_thread.h
	gcc -c main.c -w -std=gnu9x -lcunit 
test_files.o : test_files.c Prime.h Prime_thread.h
	gcc -c test_files.c -w -std=gnu9x -lcunit -I/usr/include/python3.6
Prime.o : Prime.c
	gcc -c Prime.c -w -std=gnu9x -lcunit
Prime_thread.o : Prime_thread.c
	gcc -c Prime_thread.c -w -std=gnu9x -lcunit
clean : 
	rm -rf *.o
	rm -rf Prog
	rm -rf testPrincipaleOutput.txt
	rm -rf testPrincipaleOutput_th.txt
	rm -rf debug
	rm -rf *xml
	rm -rf DebugOutput.txt
	rm -rf stats.txt
	rm -rf PythonOUT.txt
	rm -rf test

debug : main.c Prime.c Prime_thread.c test_files.c Prime.h test_files.h
	make clean
	gcc -c Prime.c -w -std=gnu9x -lcunit
	gcc -c Prime_thread.c -w -std=gnu9x -lcunit
	gcc -c test_files.c -w -std=gnu9x -lcunit -I/usr/include/python3.6 
	gcc -c main.c -w -std=gnu9x -lcunit
	gcc -o debug main.o Prime.o Prime_thread.o  test_files.o -w -std=gnu9x -lcunit -lpython2.7
	valgrind --leak-check=yes --log-file="DebugOutput.txt" -q ./debug
	echo  "\n" >> DebugOutput.txt
	cppcheck --enable=all --inconclusive  Prime.c 2>> DebugOutput.txt
	echo  "\n" >> DebugOutput.txt
	cppcheck --enable=all --inconclusive  Prime_thread.c 2>> DebugOutput.txt
	echo  "\n" >> DebugOutput.txt
	echo  "\n" >> DebugOutput.txt
	./debug >> DebugOutput.txt

stat : 
	echo "Statistiques git à la date du :">stats.txt
	date >>stats.txt
	gitinspector >> stats.txt
	echo "\n" >> stats.txt
	echo "nombres de commit" >>stats.txt
	git shortlog -sn >> stats.txt
push :
	git commit -a -m "git auto command"
	git push
