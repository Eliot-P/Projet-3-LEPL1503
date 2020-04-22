make clean > /dev/null
rm -rf Output_simple.txt > /dev/null
rm -rf Output_thread.txt > /dev/null
gcc -c Prime.c -w -std=gnu9x
gcc -c Prime_thread.c -w -std=gnu9x -lpthread
gcc -c miniscript.c -w -std=gnu9x
gcc -o minirun Prime.o Prime_thread.o  miniscript.o -w -std=gnu9x -lpthread
<<<<<<< HEAD
./minirun
make clean > /dev/null
=======
gcc -o ui Prime_thread.o  miniscript.o -w -std=gnu9x -lpthread
>>>>>>> b193194b2801c84f91235a61f09b57ebeba3a7b9
