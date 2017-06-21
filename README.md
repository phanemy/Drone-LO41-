# Drone-LO41-
Projet de LO41, simulation de livraison par drone

make pour compiler le projet.

Pour le lancer le programme avec les valeurs par defauts, utiliser ./zeppelin

Optionel :
Certaines valeurs peuvent être passées en paramètres :
  Le premier paramètre est le nombre de drônes
  Le second, le nombre de clients
  Le dernier, le nombre de colis
  
exemple :
Avec 6 drônes et le reste avec les valeurs par defaut : ./zeppelin 6

Avec 6 drônes et 10 clients : ./zeppelin 6 10
Avec 6 drônes, 10 clients et 100 colis : ./zeppelin 6 10 100

Le reste des valeurs (comme la vitesse de simulation, le nombres de dock, de slots de recharge) peuvent être modifiées directement dans le fichier "structures.h". Il faudra alors recompiler le projet.
