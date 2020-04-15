echo "Execution de Prime_thread"
make clean > /dev/null
gcc -c Prime_thread.c -w -std=gnu9x -lpthread
gcc -c main.c -w -std=gnu9x
gcc -o Prime main.o Prime_thread.o  -w -std=gnu9x -lpthread
./Prime Input.txt
cat Output_thread.txt
make clean > /dev/null
rm -rf Output_thread.txt > /dev/null