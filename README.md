# TP-Acquisition-Commande
###### Boudet - Manologlou

## Sommaire
[Objectifs](#objectifs)

[Liaison UART](#liaison-uart)

[Prise en main du mmoteur](#prise-en-main-du-moteur)

## Objectifs
Le but de ce TP est d'asservir un moteur à courant continu en courant et en vitesse.
On utilise plusieurs outils :
* Un moteur à courant continu
* Un driver de moteur MC1L 3-Phase Microchip
* Une carte STM32G474RE
* Un PCB d'interfaçage entre la carte et le driver
* Une alimentation réglable allant jusqu'à 48 V

## Liaison UART
La console UART doit répondre aux critères suivants :
* Un echo des caractères envoyés
* Traite les caractères envoyés après réception du caractère "ENTER"
* Peut prendre en compte plusieurs chaines de caractères
* Renvoie le résultat des fonctions appelées

Les fonctions codées :
* help
  - Renvoie la liste des commandes disponibles
* pinout
  - Renvoie la liste des broches utilisée ainsi que leur fonctionnalité
* start
  - Génère les PWM de commande avec un rapport cyclique de 50% (0 RPM)
* stop
  - Arrête les PWM, ce qui éteint le moteur
* alpha [0;100]
  - Change le rapport cyclique des PWM
* isoreset
  - Remet à 0 le driver, pour permettre son démarrage
* speed [-3000;3000]
  - Change la vitesse de rotation du moteur

## Prise en main du moteur
Les caractéristiques du moteur :
* 3000 RPM nominal
* 48 V
* 6 A nominal
Nous allons simplement générer des PWM pour créer la commande complémentaire, afin de faire tourner le moteur dans les deux sens et à une vitesse variable.
Pour cela, nous utilisons le timer 1 avec la configuration suivante :
* Screen config
* Screen paramètres
* Explications



```
$ cd ../lorem
$ npm install
$ npm start
```

Functions Done :

  -help
  -pinout
  -alpha [0;100]
  -isoreset
  -start
  -stop
  -speed XXXX [-3000;3000]

Data :

  -Clock speed : 170 MHz
  -Counter période : 5325
  -CCR1,CCR2 2663 (alpha = 50%)
  -Dead Time : 205

Work in progress :

  -ADC (almost finished)
  -Current's enslavement
  -Speed's enslavement

How to use it :

  Connect pins PA8,9,11,12 to the Microchip's power module. (PWM)
  Connect pin PC3 to pin 33 of the Microchip's power module. (Iso_Reset)
  Powering the Microchip's power module.
  Use the "isoreset" (shell) command to unlock the Microchip's power module.
  Use the "start" command to start generating PWMs.
  Use the "alpha" command to change the alpha value of PWMs.
  Use the "stop" command to stop the generation of PWMs.
  Use the "speed" command to set a specific speed.
