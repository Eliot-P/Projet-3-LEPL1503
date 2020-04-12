Prime : main.o Prime_thread.o
	gcc -o Prime main.o Prime_thread.o  -w -std=gnu9x
main.o : main.c Prime_thread.h
	gcc -c main.c -w -std=gnu9x -lcunit 
Prime_thread.o : Prime_thread.c
	gcc -c Prime_thread.c -w -std=gnu9x -lcunit

clean : 
	rm -rf *.o
	rm -rf Prime
	rm -rf debug
	rm -rf test_files

	rm -rf Output_simple.txt
	rm -rf Output_thread.txt
	rm -rf Output_python.txt

	rm -rf DebugOutput.txt
	rm -rf stats.txt
	rm -rf *xml

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