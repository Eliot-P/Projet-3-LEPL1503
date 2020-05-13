echo "Entrer le mode d'execution"
echo "1) Unit test"
echo "2) Debug"
echo "3) Auto prime"
read execution_mode

if [ $execution_mode -eq 1 ]; then
    echo "Compliation des tests CUnit"
    make test
    ./test
    make clean > /dev/null
fi

if [ $execution_mode -eq 2 ]; then
    make clean > /dev/null
    rm -rf ../IN-OUT/DebugOutput.txt > /dev/null
    make fact > /dev/null
	valgrind --leak-check=yes --log-file="../IN-OUT/DebugOutput.txt" -q ./fact -N 4 ../IN-OUT/Input.txt ../IN-OUT/Output.txt
	cppcheck --enable=all --inconclusive  ../CODE/fact.c 2>> ../IN-OUT/DebugOutput.txt
	./fact -N 4 -q ../IN-OUT/Input.txt ../IN-OUT/Output.txt >> ../IN-OUT/DebugOutput.txt
    cat ../IN-OUT/DebugOutput.txt
    rm -rf .../IN-OUT/Output.txt
    echo "Garder le fichier texte ?"
    echo "0 == Non"
    read choice
    if [ $choice -eq 0 ]; then
        rm -rf ../IN-OUT/DebugOutput.txt
        make clean > /dev/null
    else
        exit 0
    fi
fi

if [ $execution_mode -eq 3 ]; then
    echo "Execution de fact"
    make clean > /dev/null
    gcc -o ../OBJECT/fact.o -c ../CODE/fact.c -w -std=gnu9x -lpthread > /dev/null
    gcc -o ../OBJECT/main.o -c ../CODE/main.c -w -std=gnu9x > /dev/null
    gcc -o fact ../OBJECT/main.o ../OBJECT/fact.o  -w -std=gnu9x -lpthread > /dev/null
    echo "Nombre de thread ?"
    read N_thread
    ./fact -N $N_thread -q ../IN-OUT/Input.txt ../IN-OUT/Output_thread.txt 
    cat ../IN-OUT/Output_thread.txt
    make clean > /dev/null
    rm -rf ../IN-OUT/Output_thread.txt > /dev/null
    rm -rf fact > /dev/null
fi