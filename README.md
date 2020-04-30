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
|test_files.c|code permettant de faire des mesure temporelle sur le Prime.c, Prime_thread.c et le code Python original|
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

## Comment executer le programme ?

Il y a plusieurs manière d'executer le programme