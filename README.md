#Subductor

Une machine pour le laboratoire de sciences naturelles de L'université de Lille (Cité scientifique) SN5

## Objectif

Le subductor a pour objectif de subducter ... Bon c'est pas super clair comme ça donc je vais faire mieux:
L'objectif est de tirer un bloc de sable avec un tissus (ici un morceau de voile de bateau) par le dessous afin de simuler des mouvements tectoniques et leurs effets sur des blocs de sable.

### Matériel

Pour se faire est mis à la disposition de l'utilisateur un boitier possédant un écran lcd 4 lignes 20 caractères par lignes ainsi qu'un encodeur rotatif avec un bouton inclus.

### Préparation

Au démarrage de la machine il faut avec le mode Incrémental faire deux tours de aux tapis autour de la tige d'enroulement puis redemarrer la machine pour commencer à l'utiliser.
Le nombre d'enroulement du tapis n'étant pas mesurer : il s'agit de la seule manière de connaître sa position eu démarrage puis de la calculer au cours du fonctionnement de la machine.

### Mode

Dans un premier temps il faut choisir un mode entre:
-Continu : Sélection de la vitesse de déplacement du tapis.
-Incrémental : Sélection de la position voulue du tapis puis déplacement de celui-ci.

Pour passer d'un mode à l'autre il faut éteindre puis allumer la machine (Choix seulement au démarrage).

#### Continu

Choix de la vitesse avec l'encodeur puis appuie sur le bouton 2 secondes pour mettre en mouvement la machine.
Lors d'un changement de vitesse en bougeant l'encodeur il faut appuyer sur le bouton pour confirmer la modification et qu'elle devienne effective.

#### Incrémental

Choix de la position avec l'encodeur puis appuie sur le bouton 2 secondes pour mettre en mouvement la machine.
A chaques changement de position il faut appuyer à nouveau sur le bouton pendant 2 secondes sur le bouton de l'encodeur pour que le changement soit pris en compte.

## Electronique

Du point de vue électronique une carte permet de connecter chaques composants via des connecteurs JST-XH afin de limiter au maximum la possibilité de déconnexion dû aux vibrations ou au temps. Les composants sont tous connecter à un atmega328p cadencé à 16MHz. L'écran en I2C et l'encodeur sur des pin digital. Le driver de moteur pas à pas sur des pins digital lui aussi.

## Programmation

Pour programmer il faut utiliser un programmateur spécifique et le connecteur correspondant sur la carte. Le programmateur est le même que le programmateur du projet Slicor. Une petite fléche permet sur la carte et le connecteur du programmateur permet de détromper le sens de connexion. Ensuite via l'IDE arduino il est possible de mettre le bootloader ("carte : arduino uno", "programmateur : TINYasISP" puis "burn bootloader") il est nécessaire de mettre le bootloader si la puce n'en a pas déjà. Ensuite avec l'option "programmer via un programmateur" il est possible de téléverser le programmer dans le microcontrôleur. 

ATTENTION: Avec le programmeteur mis à dispositon une erreur survient dans certaine condition. Si un timeout est émis par le programmateur une solution est d'enlever ou d'ajouter des delay(1) dans la partie setup du programme. (Si une meilleure solution existe je suis preneur). 
