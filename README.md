# ft_irc

# TODO List

au lancement du serveur ecouter sur le port passé en argument

a la connexion d'un utilisateur lui attribuer un socket/fd (a determiner) propre

continuer a ecouter sur le port de "connexion" et ecouter les commandes des clients déja connectés

//totor
vérifier que le port est disponible sur le serveur
 
# Liste des commandes

/SERVER [serveur IRC] [port]

/NICK Votre_identifiant

/JOIN #Nom_du_canal

/PART #Nom_du_canal

/PART #Nom_du_canal [fin]

/LIST [#chaine][-min #][-MAX #]


//totor 

OPTI 

-> transformer le sock_pfd du serveyr en pollfd
	-> poll ce pollfd à chaque nouvelle iteration de vie du serveur -> si revent en mode POLLIN, un utilisateur tente de se connecter (évite un appel à accept() qui retourne une erreur

