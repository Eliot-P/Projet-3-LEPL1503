make clean > /dev/null
rm -rf Output_simple.txt > /dev/null
rm -rf Output_thread.txt > /dev/null
gcc -c Prime.c -w -std=gnu9x
gcc -c Prime_thread.c -w -std=gnu9x -lpthread
gcc -c miniscript.c -w -std=gnu9x
gcc -o minirun Prime.o Prime_thread.o  miniscript.o -w -std=gnu9x -lpthread
./minirun