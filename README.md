# LEPL1503 - PROJET 3 - GROUPE Q5

## Petite explication des differents fichiers

|Fichier|Utilisation|
|---|---|
|Prime.c|traduction en C du code python sans multithreading|
|Prime.h|fichier header de Prime.c|
|Prime_thread.c|Code C avec multithreading|
|Prime_thread.h|fichier header de Prime_thread.c|
|main.c|le fichier main necessaire afin d'exécuter Prime_thread.c|
|test_lib.c|le code des test unitaires|
|test_files.c|le code permettant de faire des mesure temporelle sur le Prime.c, Prime_thread.c et le code Python original|
|Makefile|le makefile afin d executer le code|
|.gitignore|le fichier permettant d'éviter d'upload des fichiers inutiles sur le git|
|README.md|le README en markdown|
|README.txt|le README en txt pour le Peer-Review|
|Archive Peer-Review|l'archive qui a été envoyée pour le Peer-Review|
|Optimisatons.py|le code Python permettant l'optimisation du nombre de thread|
|Input.txt|l'exemple original d'input|
|token_jenkins.txt|le token jenkins|
|Autoprime.sh|un script shell rapide permettant la compilation et l'exécution de Prime_thread.c|
|miniscript.c|un code exécutant des mesures temporelle sur Prime.c et Prime_thread.c|
|minirun.sh|un script shell permettant la compilation et l'exécution de miniscript.c|
|Rapport|le dossier contenant le rapport en Latex|

## Déscription du programme 

Le programme réalisé est un programme écrit en C, avec du multithreading. Celui-ci prend en entrée un fichier contenant une liste d'entier (ligne par ligne), et renvoie dans le fichier de sortie ces entiers suivis de leurs diviseurs premiers (ligne par ligne).
L'architecture employée est un double producer - consummer. Les tableaux servant à stocker les lignes du fichier/leur diviseurs premiers sont des structures de types "entrepots" qui contiennent différentes variables suffisamment explicites. Il est important de remarquer que les tableaux ne sont pas des tableaux de unsigned long long mais bien des tableaux de structure "repertoire".
3 types de threads sont utilisés:

- 1 thread de lecture associé à la fonction "lecture" qui s'occupe de lire les lignes du fichier et de mettre les résultats dans le premier tableau
- N threads de calculs associé à la fonction "calcul", qui prend un nombre dans le premier tableau, calcule ses diviseurs premiers et stocke le résultat dans le 2e tableau (toujours sous la forme d'une structure de type "Repertoire_th")
- 1 thread d'écriture associé à la fonction "écriture" qui se charge de prendre les résultats du 2e tableau et de les écrire dans le fichier de sortie

De manière générale le code contient des commentaires permettant de mieux comprendre son fonctionnement.

## Déscription des fonctions principales

- La fonction is_div : Retourne True si i divise number, sinon False.
- La fonction is_prime : Retourne 1 si number est un nombre premier, retourne 0 si le nombre n'est pas premier et -1 en cas d'erreur.
- La fonction prime_divs : Retourne un pointeur vers une structure Reperetoire_t contenant une liste de tout les diviseurs premiers de number et le nombre de diviseurs ou NULL pour une erreur de malloc.
- La fonction principale : Écrit dans chaque ligne de output_file la liste des diviseurs premiers du int à la ligne correspondante dans input. Si jamais l'élément n'est pas un int, écrit une erreur.
- La fonction lecture : Lit la ligne, et place dans le lect->buffer le résultat.
- La fonction calcul : Calcul les diviseurs premiers d’un entier dans buffer.
- La fonction ecriture : Écrit l’entier suivi de ses diviseurs premiers dans le fichier de sortie.

## Comment executer le programme ?

Il y a plusieurs manière d'executer le programme et plusieurs script shell disponnible afin de rendre cette execution plus simple. Afin d'executer les scripts shell, il est nessecaire de d'abord autoriser ces scripts si il s'agit de leur première execution avec la commande suivante dans le terminal Linux :

``` bash
chmod +x ./nom_du_script.sh
```

Il y a trois scripts shell disponnibles, Auto_prime.sh qui compile le programme, demande le nombre de thread puis affiche l'output dans le terminal. Ce script prendra comme fichier le fichier Input.txt. Ensuite il y a minirun.sh, ce scripts compile et execute miniscript.c puis affiche le temps d'execution de Prime et Prime_thread dans le terminal. Pour finir, Shell_Prime.sh est un scripts proposant plusieurs mode d'execution différents, notament l'execution des test CUnit.

## Le Makefile

Le Makefile propose plusieurs options de compilation, il y a **make fact** qui crée un executable fact de la spécification demandée, puis il y a **make test** qui crée un executable test afin d'effectuer les test unitaires. Pour les clean, il y a **make clean** et **make clean_output** qui supprime respectivement les fichiers executables et les fichiers d'output. Il y a aussi la commande **make push** qui effectue un commit et un push automatiquement. Enfin il y a la commande **make stat** qui permet de créer un fichier*stat.txt* contenant les statistiques de participation des membre du groupe (ce dernier necessite le programme gitinspecor).

## Performance du programme

Nous continuons d'essayer d'améliorer les performances du programme au quotidien, cependant voici un graphique obtenu avec le programme Python disponible sur le git.

![Graph](https://raw.githubusercontent.com/Eliot-P/public_png/master/Graph2.png)

Loi de Amdahl fixe une limite théorique sur le gain que l’on peut obtenir en parallélisant un programme :
- Gain qui peut être obtenu de la parallélisation = 1/[(1-f)+f/N]. Avec f, la fraction du programme qui est parallélisable, et N, le nombre de threads.

La fraction de notre programme qui est parallélisable f est 90%. Celle-ci est déduit en fonction du temps d'exécution du thread de lecture jusqu'au join du thread d'écriture sur le temps d'exécution total du programme.
Attention cette loi est considérée comme un maximum théorique qui est difficile d’atteindre. Elle suppose que la parallélisation est parfaite, c’est-à-dire que la création et la terminaison de threads n’ont pas de coût en terme de performance. En pratique, c'est loin d'être le cas.

En espérant que ce README répondra à vos questions,\
Cordialement,\
Le groupe Q5.


>**Contacts** **:**\
>Constantin Van Yperzeel\
>Eliot Peeters