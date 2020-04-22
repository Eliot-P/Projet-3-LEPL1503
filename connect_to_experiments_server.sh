# $1 = username@server_name
# $2 = path to the id_rsa key

if [ ! -f "$2" ]; then
	echo "Le fichier \"$2\" n'existe pas !"
	exit -1
fi

chmod 600 $2



USERNAME=$(echo $1 | cut -d @ -f 1)
SERVERNAME=$(echo $1 | cut -d @ -f 2)

if [ -z "$SERVERNAME" ]; then
	echo "l'identifiant d'utilisateur SSH est invalide. Il doit être de la forme username@servername"
	exit -1
fi

if [ "$SERVERNAME" != "studsrv2.info.ucl.ac.be" -a "$SERVERNAME" != "studsrv3.info.ucl.ac.be" -a "$SERVERNAME" != "studsrv4.info.ucl.ac.be" ]; then
	echo "Le nom du serveur sur lequel se connecter doit se trouver parmi studsrv2.info.ucl.ac.be, studsrv3.info.ucl.ac.be ou studsrv4.info.ucl.ac.be"
	exit -1
fi

ssh -o IdentitiesOnly=yes -i $2 -o ProxyCommand="ssh -o IdentitiesOnly=yes -i $2 -W %h:%p ${USERNAME}@studssh.info.ucl.ac.be" $1
