fact : directories ../TEMP/main.o ../TEMP/fact.o
	gcc -o fact ../TEMP/main.o ../TEMP/fact.o  -Wall -std=gnu9x -lpthread -lm -g
test : directories ../TEMP/test.o ../TEMP/fact.o
	gcc -o test ../TEMP/fact.o  ../TEMP/test.o -Wall -std=gnu9x -lcunit -lpthread -lm -g
../TEMP/test.o : ../TEST/test.c ../SRC/fact.h
	gcc -o ../TEMP/test.o -c ../TEST/test.c -Wall -std=gnu9x -lcunit -lm -g
../TEMP/main.o : ../SRC/main.c ../SRC/fact.h
	gcc -o ../TEMP/main.o -c ../SRC/main.c -Wall -std=gnu9x -lm -g
../TEMP/fact.o : ../SRC/fact.c
	gcc -o ../TEMP/fact.o -c ../SRC/fact.c -Wall -std=gnu9x -lpthread -lm -g
directories :
	mkdir -p ../TEMP

clean : 
	rm -rf ../TEMP/*.o
	rm -rf test
	rm -rf fact

val: fact
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./fact -N 4 -q ../IN-OUT/Input.txt ../IN-OUT/Output.txt

cppcheck: ../SRC/fact.c
	cppcheck --enable=all --inconclusive ../SRC/fact.c

run: fact
	./fact -N 4 -q ../IN-OUT/Input.txt ../IN-OUT/Output.txt

python : fact ../PYTHON/Optimisation.py
	python3 ../PYTHON/Optimisation.py

mesure_INGI : fact ../PYTHON/Opti_INGI.py
	python3 ../PYTHON/Opti_INGI.py

convert_INGI : ../PYTHON/INGI_transcripter.py
	python3 ../PYTHON/INGI_transcripter.py