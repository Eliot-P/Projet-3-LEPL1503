=========================   README   =========================
Vous trouverez dans ce dossier .zip:

fact.c : le programme C du projet
fact.h : les headers du programme
main.c :  la fonction main permettant de lancer le programme
Makefile : le Makefile contenant les commandes demandées
test.c :  des tests unitaires

///  Description du programme  \\\

L'architecture employée est un double producer - consummer. Les tableaux servant à stocker les lignes du fichier/leur diviseurs premiers sont des structures de types "entrepots" qui contiennent différentes varibales suffisamment explicites. Il est important de remarquer que les tableaux ne sont pas des tableaux de unisgned long long mais bien des tableaux de structure "repertoire". 
3 types de threads sont utilisés: 

- 1 thread de lecture associé à la fonction "lecture" qui s'occupe de lire les lignes du fichier et de mettre les r�sultats dans le premier tableau;

- N threads de calculs associé à la fonction "calcul",  qui prend un nombre dans le premier tableau, calcule ses diviseurs premier et stocke   le résultat dans le 2e tableau (toujours sous la forme d'une structure de type "Repertoire_th";

- 1 thread d'écriture associé à la fonction "écriture" qui se charge de prendre les résultats du 2e tableau et de les écrire dans le fichier de   sortie;

De manière générale le code contient des commentaires permettant de mieux comprendre son fonctionnement . Vous verrez de temps en temps certaines erreur qui arrivent lorsqu'on retire certaines lignes de commentaires.

/!\  /!\ /!\ 
Le point qui risque de vous être difficile de comprendre est la boucle for (à la fin) dans la fonction ''lecture". Cette boucle permet de signaler au threads de calcul que le fichier est fini ET qu'ils DOIVENT s'arrêter. En effet, sans cette boucle des exceptions peuvent arriver si, par exemple, on a 	appelle la fonction avec un nombre N supérieur aux nombres de lignes présentes dans le fichier. Elle permet d'éviter des deadlocks où plusieurs threads attendraient que "lecture" ajoute un élément dans le tableau alors que la fin du fichier a été atteinte.

En éspérant que tout soit clair pour vous,
le groupe Q5. 