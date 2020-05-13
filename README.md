Bienvenue sur mon projet de gomoku,
Le gomoku est un jeu de plateau où le but est d'aligner 5 pions de sa couleur pour gagner.
Ce projet a été réalisé en C et utilise la bibliothèque graphique SDL.
Il implémente une intelligence artificielle utilisant une approche de Monte Carlo et une autre utilisant l'algorithme de Monte Carlo Tree Search

# présentation du projet

<p align="justify">    Le jeu de Gomoku est un jeu traditionnel. Il s’agit d’aligner 5 pions de la même couleur pour gagner. J’ai pris sur mes loisirs cette année, pour en créer une version avec le langage C.</p>

<p align="justify">  <br />  Dans un premier temps, j’ai créé <b>une version du jeu fonctionnelle pour deux joueurs</b> dans la console. Cette partie du développement a été faite rapidement. En effet, j’avais déjà réalisé d’autres projets du même genre, comme un démineur par exemple. Les règles ne sont pas très complexes à implémenter, en effet, il suffit d’être familier avec les tableaux multi-dimensionnels, en affectant la valeur de 0 pour une case vide et 1 ou 2 selon le joueur ayant placé son pion sur une case. Après chaque coup, il faut vérifier s’il s’agit ou non d’un coup gagnant. J’en ai profité pour utiliser une énumération pour améliorer la lisibilité du code, ainsi qu’une structure « Coup » regroupant la ligne et la colonne du coup joué.
Je n’ai évidement pas chronométré le temps passé sur ce projet mais j’estime avoir passé  8 heures sur cette première partie.
</p>

</p align="justify"> <br />Dans un deuxième temps, j’ai poursuivi mon projet en développant une interface graphiqueqe à l’aide de la bibliothèque SDL (Simple Direct Media Layer). C’est ma première expérience d’interface graphique en C (j’avais déjà réalisé une interface avec JavaScript  où des fonctionnalités de dessin sont implémentées de base dans le langage). Il s’agissait d’une démarche personnelle d’approfondissement même si j’ai échangé à plusieurs reprises avec mon professeur d’informatique pour obtenir des conseils. J’ai pris beaucoup de temps à chercher dans la documentation officielle, et dans de nombreux tutoriels faits par d’autres utilisateurs, pour apprendre à utiliser la bibliothèque. 
</p>

<p align="justify">Aussi, j’ai créé <b>une dizaine d’images avec le logiciel Gimp</b>, qui vont du pion élémentaire blanc ou noir aux écrans types menu principal, règles ou choix du mode de jeu. L’affichage fonctionne grâce à une boucle de jeu en trois étapes : récupération des entrées venant des utilisateurs, puis, mise à jour des données du programme en fonction de l’action récupérée, enfin, mise à jour de l’affichage pour refléter les changements internes. </p>

<p align="justify">Cette partie a été relativement longue (environ 60 heures) correspondant au temps de découverte et d’apprentissage de la bibliothèque. La mise en œuvre n’est pas très compliquée mais c’est elle qui nécessite le plus de code.</p>

<p align="justify">  <br />  Dans un troisième temps, je me suis lancé le défi d’ajouter un <b>mode de jeu seul contre l’ordinateur</b>. La première étape a été de faire jouer des coups aléatoires à l’ordinateur. </p>

<p align="justify">Puis, comme cette méthode ne permet pas de jouer des parties intéressantes, je me suis intéressé à une approche par la <b>méthode de Monte Carlo</b>. Le but de cette méthode est de réaliser un grand nombre de simulations de parties sur chaque coup possible puis de sélectionner le coup où l’intelligence artificielle gagne le plus souvent. Pour réaliser une simulation, on fait jouer tour à tour un coup aléatoire à chaque camp jusqu’à arriver à une fin de partie.</p>

<p align="justify">Le problème de cette méthode est que, pour qu’elle soit efficace, il faut réaliser un maximum de simulations sur chaque coup possible pour éviter les anomalies statistiques qui pourraient laisser penser qu’un coup est intéressant car il a conduit à plusieurs victoires par pur hasard. Ainsi, même en se limitant aux coups possibles dans le carré de 13 de coté centré sur le dernier coup joué, et à 1000 simulations par coup - soit un maximum de 168 000 simulations - cela demande un long temps de calcul (près de 5 secondes sur mon ordinateur doté d’un processeur Intel core I7-770HQ cadencé à 2,8GHz de fréquence). </p>

<p align="justify">Pour autant ce mode de jeu m’a apporté une grande satisfaction : l’ordinateur m’a battu dans une partie que je jouais réellement.
	J’ai décidé de poursuivre pour <b>réduire la puissance de calcul demandée</b> en utilisant l’algorithme de MCTS (Monte Carlo Tree Search), se basant sur le même principe de fonctionnement mais choisissant <b>les coups à approfondir à l’aide de la valeur UCT</b> (Upper Confidence bound applied to Trees) de ses fils. Cette valeur UCT permet de s’intéresser principalement aux coups qui paraissent prometteurs, sans pour autant complètement oublier ceux qui n’ont pas été essayés récemment. En utilisant cet algorithme, le nombre de simulations peut être drastiquement réduit sans pour autant perdre en correction du résultat.</p>	

<p align="justify">L’algorithme se décompose en quatre phases :
	<ul>
	<li>Une phase de sélection. On parcourt chaque enfant du nœud sur lequel on se trouve et on 	choisi celui dont la valeur UCT est maximale, on répète ce processus jusqu’à arriver sur une 	feuille de l’arbre de jeu.</li>
	<li>Une phase d’expansion si on a déjà fait une simulation sur cette feuille (Ajout d’un  fils 	par coup possible à partir de l’état du jeu puis sélection d’un fils).</li>
	<li>Une phase de simulation avec des coups aléatoires depuis l’état actuel du jeu jusqu’à une 	fin possible </li>
	<li> Une phase de rétro-propagation du  résultat de la simulation à tous les parents de ce nœud 	jusqu’à la racine (incrémentation de nombre de simulations et augmentation du score du 	nœud 	avec le résultat).</li>
	</ul>	
	
Ces phases sont répétées autant de fois que souhaité (le coup sélectionné est celui ayant le meilleur score moyen). 
Pour, créer l’arbre de jeu, on a besoin de <b>l’allocation dynamique de la mémoire à l’aide de la fonction malloc</b>. Initialement, j’ai demandé l’allocation mémoire d’un bloc de la taille d’un nœud à chaque expansion de l’arbre. Mais, puisque la fonction malloc ne réussit pas forcément à allouer le bloc mémoire demandé, il est intéressant de minimiser le nombre d’appels. C’est pourquoi, j’ai défini l’arbre comme un tableau de nœuds dont le premier élément correspond à la racine et les positions des fils d’un nœud sont stockés dans un tableau des indices. Ainsi, il suffit d’un seul appel à la fonction malloc et d’une seule libération de la mémoire par tour de l’ordinateur.
Jusqu’à présent j’estime avoir passé une centaine d’heures pour programmer l’intelligence artificielle.
</p>

<p align="justify">   <br /> Je suis satisfait de l’avancement de ce projet très formateur qui m’a initié à l’utilisation d’une bibliothèques non fournie avec le langage et m’a donné une première expérience de création d’une intelligence artificielle. J’ai toutefois l’intention de continuer ce projet en y ajoutant notamment un système de sauvegarde qui permettra d’interrompre la partie et de la continuer ultérieurement. </p>
