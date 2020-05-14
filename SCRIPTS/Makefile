fact : directories ../OBJECT/main.o ../OBJECT/fact.o
	gcc -o fact ../OBJECT/main.o ../OBJECT/fact.o  -w -std=gnu9x -lpthread -lm -g
test : directories ../OBJECT/test.o ../OBJECT/fact.o
	gcc -o test ../OBJECT/fact.o  ../OBJECT/test.o -w -std=gnu9x -lcunit -lpthread -lm -g
../OBJECT/test.o : ../TEST/test.c ../SRC/fact.h
	gcc -o ../OBJECT/test.o -c ../TEST/test.c -w -std=gnu9x -lcunit -lm -g
../OBJECT/main.o : ../SRC/main.c ../SRC/fact.h
	gcc -o ../OBJECT/main.o -c ../SRC/main.c -w -std=gnu9x -lm -g
../OBJECT/fact.o : ../SRC/fact.c
	gcc -o ../OBJECT/fact.o -c ../SRC/fact.c -w -std=gnu9x -lpthread -lm -g
directories :
	mkdir -p ../OBJECT

clean : 
	rm -rf ../OBJECT/*.o
	rm -rf test
	rm -rf fact

val: fact
	valgrind --tool=memcheck --leak-check=yes ./fact -N 4 ../IN-OUT/Input.txt ../IN-OUT/Output.txt

cppcheck: ../SRC/fact.c
	cppcheck --enable=all --inconclusive ../SRC/fact.c

run: fact
	./fact -N 4 ../IN-OUT/Input.txt ../IN-OUT/Output.txt