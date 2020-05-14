fact : directories ../TEMP/main.o ../TEMP/fact.o
	gcc -o fact ../TEMP/main.o ../TEMP/fact.o  -w -std=gnu9x -lpthread -lm -g
test : directories ../TEMP/test.o ../TEMP/fact.o
	gcc -o test ../TEMP/fact.o  ../TEMP/test.o -w -std=gnu9x -lcunit -lpthread -lm -g
../TEMP/test.o : ../TEST/test.c ../SRC/fact.h
	gcc -o ../TEMP/test.o -c ../TEST/test.c -w -std=gnu9x -lcunit -lm -g
../TEMP/main.o : ../SRC/main.c ../SRC/fact.h
	gcc -o ../TEMP/main.o -c ../SRC/main.c -w -std=gnu9x -lm -g
../TEMP/fact.o : ../SRC/fact.c
	gcc -o ../TEMP/fact.o -c ../SRC/fact.c -w -std=gnu9x -lpthread -lm -g
directories :
	mkdir -p ../TEMP

clean : 
	rm -rf ../TEMP/*.o
	rm -rf test
	rm -rf fact

val: fact
	valgrind --tool=memcheck --leak-check=yes ./fact -N 4 ../IN-OUT/Input.txt ../IN-OUT/Output.txt

cppcheck: ../SRC/fact.c
	cppcheck --enable=all --inconclusive ../SRC/fact.c

run: fact
	./fact -N 4 -q ../IN-OUT/Input.txt ../IN-OUT/Output.txt