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

En espérant que ce README répondra à vos questions,
Cordialement,
Le groupe Q5.


>**Contacts** **:**
>
>Constantin Van Yperzeel
>
>Eliot Peeters