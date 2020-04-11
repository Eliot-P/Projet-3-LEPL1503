#!/bin/sh
echo "Entrer le mode d'execution"
echo "1) Unit test"
echo "2) Test de perormance en fonction du temps"
echo "3) Simple execution"
echo "4) Debug"
read execution_mode

if [ $execution_mode -eq 1 ]; then
    echo "Compliation des tests CUnit"
    make clean > /dev/null
    gcc -c Prime.c -w -std=gnu9x
	gcc -c Prime_thread.c -w -std=gnu9x
	gcc -c test_lib.c -w -std=gnu9x -lcunit
	gcc -o test Prime.o Prime_thread.o  test_lib.o -w -std=gnu9x -lcunit
    ./test
    make clean > /dev/null
fi

if [ $execution_mode -eq 2 ]; then
    echo "1) Execution thread"
    echo "2) Execution normal"
    echo "3) Execution python /!\ preds BEAUCOUP de temps"
    echo "ex : 1 1 1 --> 1 et 2 et 3"
    read thread normal python
    make clean > /dev/null
    gcc -c Prime.c -w -std=gnu9x
	gcc -c Prime_thread.c -w -std=gnu9x
	gcc -c test_files.c -w -std=gnu9x -I/usr/include/python3.6 
	gcc -o test_files Prime.o Prime_thread.o  test_files.o -w -std=gnu9x -lpython2.7
    ./test_files $normal $thread $python
    make clean > /dev/null
fi

if [ $execution_mode -eq 3 ]; then
    echo "Entrer le fichier Input"
    read Inputfile
    make clean > /dev/null
	gcc -c Prime_thread.c -w -std=gnu9x
	gcc -c main.c -w -std=gnu9x
    gcc -o Prime main.o Prime_thread.o  -w -std=gnu9x
    ./Prime $Inputfile
    make clean > /dev/null
    cat Output_thread.txt
    echo "Garder le fichier texte ?"
    echo "0 == Non"
    read choice
    if [ $choice -eq 0 ]; then
        rm -rf Output_thread.txt
    else
        exit 0
    fi
fi

if [ $execution_mode -eq 4 ]; then
	gcc -c Prime_thread.c -std=gnu9x
	gcc -c main.c -std=gnu9x
	gcc -o debug main.o Prime_thread.o  -std=gnu9x
	valgrind --leak-check=yes --log-file="DebugOutput.txt" -q ./debug
	#cppcheck --enable=all --inconclusive  Prime_thread.c 2>> DebugOutput.txt
	./debug Input.txt >> DebugOutput.txt
    cat DebugOutput.txt
    cho "Garder le fichier texte ?"
    echo "0 == Non"
    read choice
    if [ $choice -eq 0 ]; then
        rm -rf DebugOutput.txt
    else
        exit 0
    fi
fi