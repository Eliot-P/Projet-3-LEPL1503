echo "Execution de Prime_thread"
make clean > /dev/null
gcc -c Prime_thread.c -w -std=gnu9x -lpthread
gcc -c main.c -w -std=gnu9x
gcc -o Prime main.o Prime_thread.o  -w -std=gnu9x -lpthread
echo "Nombre de thread ?"
read N_thread
./Prime -N $N_thread Input.txt Output_thread.txt 
cat Output_thread.txt
make clean > /dev/null
rm -rf Output_thread.txt > /dev/null