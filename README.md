# LEPL1503 - PROJET 3 - GROUPE Q5

## Petite explication des differents fichiers

|Fichier|Dossier|Utilisation|
|---|---|---|
|fact.c|SRC|le code c|
|main.c|SRC|la fonction main de fact.c permettant aussi de voir le temps d'éxécution ainsi que la mémoire utilisée|
|fact.h|SRC|le fichier header de fact.c|
|Optimisation.py|PYTHON|un programme python permettant de pratiquer des mesures expérimentales sur le code fact.c|
|Opti_INGI.py|PYTHON|le programme permettant de faire des mesures experimentales sur les machines en ligne|
|INGI_transcripter.py|PYTHON|le programme permettant de convertir les mesures experimentales en graphique|
|test.c|TEST|le fichier contenant les tests CUnit du code|
|...|TEMP|le dossier ou se trouver les fichiers objet lors de la compilation|
|Makefile|...|le make file de ce projet|
|Fact_shell.sh|...|un script shell permettant plusieurs mode d'execution|
|...|OTHER|un dossier ou se trouve des fichiers annexes au projet mais inutile à la compilation|
|...|IN-OUT|un dossier ou se trouve des exemples d'input à tester|
|...|DATA|un dossier ou sont stockées les mesures experimentales|
|...|IMAGES|un dossier contenant les graphiques et images nécessaire au rapport|

## Description du programme 

Le programme réalisé est un programme écrit en C, avec du multithreading. Celui-ci prend en entrée un fichier contenant une liste de nombre (un par ligne), et renvoie dans le fichier de sortie ces entiers suivis de leurs diviseurs premiers (un par ligne).
L'architecture employée est un double producer - consummer. Les tableaux servant à stocker les lignes du fichier/leur diviseurs premiers sont des structures de types "entrepots" qui contiennent différentes variables suffisamment explicites. Il est important de remarquer que les tableaux ne sont pas des tableaux de unsigned long long mais bien des tableaux de structure "repertoire".
3 types de threads sont utilisés:

- 1 thread de lecture associé à la fonction "lecture" qui s'occupe de lire les lignes du fichier et de mettre les nombres dans le premier tableau
- N threads de calculs associé à la fonction "calcul", qui prend un nombre dans le premier tableau, calcule ses diviseurs premiers et stocke le résultat dans le 2e tableau (toujours sous la forme d'une structure de type "Repertoire_th")
- 1 thread d'écriture associé à la fonction "écriture" qui se charge de prendre les résultats du 2e tableau et de les écrire dans le fichier de sortie

De manière générale le code contient des commentaires permettant de mieux comprendre son fonctionnement.

## Description des fonctions principales

- La fonction is_div : Retourne True si i divise number, sinon False.
- La fonction prime_divs_opti : Retourne un pointeur vers une structure Reperetoire_t contenant une liste de tout les diviseurs premiers de number et le nombre de diviseurs ou NULL pour une erreur de malloc.
- La fonction principale : Écrit dans chaque ligne de output_file la liste des diviseurs premiers des nombres situés dans le fichier input.
- La fonction lecture : Lit la ligne, et place dans le lect->buffer le résultat.
- La fonction calcul : Calcul les diviseurs premiers d’un nombre dans buffer.
- La fonction ecriture : Écrit les résultats dans le fichier de sortie.

## Comment exécuter le programme ?

Pour éxecuter le programme, il y a plusieurs manières : soit par le makefile soit par le script shell.

Pour le script shell il est nécessaire de d'abord faire que Linux accepte de l'éxecuter avec la commande 
```bash
chmod +x ./Fact_Shell.sh
```
Ensuite avec la commande suivante vous pouvez accéder à plusieurs mode d'éxécution : 
```bash
./Fact_Shell.sh
```

Pour le makefile, il suffit d'éxecuter la commande 
```bash 
make fact
```
Le makefile se chargera lui même de créer le répetoire TEMP ou seront placer les fichiers objet. Ensuite il suffit d'executer le programme fact. Ce dernier dispose du mode d'éxecution -q permettant d'afficher certainnes informations dans la sortie standart. L'option -N permet de choisir le nombre de thread d'éxecution. Si cette option n'est pas activée, le nombre de thread par défaut est 4, cependant ce n'est pas d'office le nombre de threads optimal, ceci dépend du processeur utilisé et du nombre de coeur ou thead dont il dispose. En règle général fact s'exécute ainsi : 
```bash
./fact [-N n] [-q] IN-OUT/input.txt IN-OUT/output.txt
```
Il est aussi possible d'utiliser la commande 'make run' afin d'exécuter le programme avec un exemple d'input fournis.

## Test unitaires et débug

Pour exécuter les test unitaires ou débug le programme, on peut utiliser les commandes :
```bash
make val #Pour utiliser valgrind
make cppcheck #Pour utiliser cppcheck
make test #Pour compiler les tests
./test #Pour exécuter les tests
```

## Performance du programme

Nous continuons d'essayer d'améliorer les performances du programme au quotidien, cependant voici un graphique obtenu avec le programme Python disponible sur le git.

![Graph](https://raw.githubusercontent.com/Eliot-P/public_png/master/Time_of_execution_INGI_Input.png)
![Graph](https://raw.githubusercontent.com/Eliot-P/public_png/master/Memory_usage_INGI_Input.png)

Loi de Amdahl fixe une limite théorique sur le gain que l’on peut obtenir en parallélisant un programme :
- Gain qui peut être obtenu de la parallélisation = 1/[(1-f)+f/N]. Avec f, la fraction du programme qui est parallélisable, et N, le nombre de threads.

La fraction de notre programme qui est parallélisable f est 97%. Celle-ci est déduit en fonction du temps d'exécution du thread de lecture jusqu'au join du thread d'écriture sur le temps d'exécution total du programme.
Attention cette loi est considérée comme un maximum théorique qui est difficile d’atteindre. Elle suppose que la parallélisation est parfaite, c’est-à-dire que la création et la terminaison de threads n’ont pas de coût en terme de performance. En pratique, c'est loin d'être le cas.

![Graph](https://raw.githubusercontent.com/Eliot-P/public_png/master/Amdahl_INGI_Input.png)

En espérant que ce README répondra à vos questions,\
Cordialement,\
Le groupe Q5.


>**Contacts** **:**\
>Constantin van Ypersele\
>Eliot Peeters\
>Henry Verdoodt\
>Louis Collinet\
>Brieuc Pierre