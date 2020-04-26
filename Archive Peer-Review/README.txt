=========================   README   =========================
Vous trouverez dans ce dossier .zip:

fact.c : le programme C du projet
fact.h : les headers du programme
main.c :  la fonction main permettant de lancer le programme
Makefile : le Makefile contenant les commandes demand�es
test.c :  des tests unitaires

///  Description du programme  \\\

L'architecture employ�e est un double producer - consummer. Les tableaux servant � stocker les lignes du fichier/leur diviseurs premiers sont des structures de types "entrepots" qui contiennent diff�rentes varibales suffisamment explicites. Il est important de remarquer que les tableaux ne sont pas des tableaux de unisgned long long mais bien des tableaux de structure "repertoire". 
3 types de threads sont utilis�s: 

- 1 thread de lecture associ� � la fonction "lecture" qui s'occupe de lire les lignes du fichier et de mettre les r�sultats dans le premier tableau;

- N threads de calculs associ� � la fonction "calcul",  qui prend un nombre dans le premier tableau, calcule ses diviseurs premier et stocke   le r�sultat dans le 2e tableau (toujours sous la forme d'une structure de type "Repertoire_th";

- 1 thread d'�criture associ� � la fonction "�criture" qui se charge de prendre les r�sultats du 2e tableau et de les �crire dans le fichier de   sortie;

De mani�re g�n�rale le code contient des commentaires permettant de mieux comprendre son fonctionnement . Vous verrez de temps en temps certaines erreur qui arrivent lorsqu'on retire certaines lignes de commentaires.

/!\  /!\ /!\ 
Le point qui risque de vous �tre difficile de comprendre est la boucle for (� la fin) dans la fonction ''lecture". Cette boucle permet de signaler au threads de calcul que le fichier est fini ET qu'ils DOIVENT s'arr�ter. En effet, sans cette boucle des exceptions peuvent arriver si, par exemple, on a 	appelle la fonction avec un nombre N sup�rieur aux nombres de lignes pr�sentes dans le fichier. Elle permet d'�viter des deadlocks o� plusieurs threads attendraient que "lecture" ajoute un �l�ment dans le tableau alors que la fin du fichier a �t� atteinte.

En �sp�rant que tout soit clair pour vous,
le groupe Q5. 