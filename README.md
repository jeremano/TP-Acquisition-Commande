# TP-Acquisition-Commande <br>


![GitHub language count](https://img.shields.io/github/languages/count/jeremano/TP-Acquisition-Commande?style=flat-square) 
![GitHub top language](https://img.shields.io/github/languages/top/jeremano/TP-Acquisition-Commande?color=blue&style=flat-square) 
<br>

![GitHub contributors](https://img.shields.io/github/contributors/jeremano/TP-Acquisition-Commande?style=flat-square) _Boudet - Manologlou_
## Sommaire
[1. Objectifs](#1-objectifs)

[2. Liaison UART](#2-liaison-uart)

[3. Commande du moteur](#3-commande-du-moteur)

[4. Mesure du courant](#4-mesure-du-courant)

[5. Mesure de la vitesse](#5-mesure-de-la-vitesse)

[6. Asservissement](#6-asservissement)

[7. Fonctionnalitées supplémentaires](#7-fonctionnalitées-supplémentaires)

[8. Conclusion](#8-conclusion)

## 1. Objectifs
Le but de ce TP est d'asservir un moteur à courant continu en courant et en vitesse.
On utilise plusieurs outils :
* Un moteur à courant continu 3000RPM / 6A
* Un driver de moteur MC1L 3-Phase Microchip
* Une carte STM32G474RE
* Un PCB d'interfaçage entre la carte et le driver
* Une alimentation réglable allant jusqu'à 48V
<br>

## 2. Liaison UART
La console UART doit répondre aux critères suivants :
* Un echo des caractères envoyés
* Traite les caractères envoyés après réception du caractère "ENTER"
* Peut prendre en compte plusieurs chaines de caractères
* Renvoie le résultat des fonctions appelées

___

Les fonctions codées :
* help
  - Renvoie la liste des commandes disponibles
```
code
```
expliquation
<br><br>

* pinout
  - Renvoie la liste des broches utilisée ainsi que leur fonctionnalité
```
code
```
expliquation
<br><br>

* start
  - Génère les PWM de commande avec un rapport cyclique de 50% (0RPM)
```
code
```
expliquation
<br><br>

* stop
  - Arrête les PWM, ce qui éteint le moteur
```
code
```
expliquation
<br><br>

* alpha [0;100]
  - Change le rapport cyclique des PWM
```
code
```
expliquation
<br><br>

* isoreset
  - Remet à 0 le driver, pour permettre son démarrage
```
code
```
expliquation
<br><br>

* speed [-3000;3000]
  - Change la vitesse de rotation du moteur
```
code
```
expliquation
<br>

## 3. Commande du moteur
Nous allons tout d'abord générer des PWM pour créer la commande complémentaire, afin de faire tourner le moteur dans les deux sens et à une vitesse variable.
<br>

Le cahier des charges nous impose les configurations suivantes :
* Fréquence de la PWM : 16kHz
* Temps mort minimum : 2us
* Résolution minimum : 10bits

___

La complémentaire décallée permeet d'obtenir une tension auw bornes du moteur qui n'est que positive ou négative, contrairement à la commande complémentaire basique qui crée une alternance positive négative qui crée un appel de courant inutile et freine le moteur. 

* Image commande simple
* Image complémentaire décallée

Quand le moteur est à l'arrêt, il reçoit du courant positif et négatif en complémentaire simple. En complémentaire, le moteur est à l'arrêt et ne reçoit pas de tension.

___

Pour générer la commande, nous utilisons le timer 1 avec la configuration suivante :
* Screen config
* Screen paramètres
* Explications (calcul du temps mort)
```
code init
```
* Screen oscillo PWM
* Screen oscillo temps mort
* image branchements
<br>

## 4. Mesure du courant
* Config ADC
* Config DMA
```
Code init
```

___

* Explication calcul moyenne
```
Code calcul moyenne
```
<br>

## 5. Mesure de la vitesse
La mesure de vitesse s'effectue avec le retour d'une roue codeuse. La roue codeuse donne la position, qui est plus précise que la vitesse, et de laquelle nous déduirons la vitesse.
* Principe roue codeuse

___

On utilise 2 horloges :
* En mode lecteur incrémental qui compte le nombre de fronts envoyés par la roue codeuse
* En mode interruption qui permet de vérifier la concordance entre la valeur mesurée de la position et la valeur estimée, à une fréquence régulière

* Config TIM2
* Explication
* Config TIM3
* Explication
```
Code init
```

___

* théorie calcul vitesse
```
Code calcul vitesse
```
* Expliquation
<br>

## 6. Asservissement
<br>

## 7. Fonctionnalitées supplémentaires
Appui bouton bleu

## 8. Conclusion

## Prise en main du moteur
Les caractéristiques du moteur :
* 3000 RPM nominal
* 48 V
* 6 A nominal

Data :

  -Clock speed : 170 MHz
  -Counter période : 5325
  -CCR1,CCR2 2663 (alpha = 50%)
  -Dead Time : 205

How to use it :

  Connect pins PA8,9,11,12 to the Microchip's power module. (PWM)
  Connect pin PC3 to pin 33 of the Microchip's power module. (Iso_Reset)
  Powering the Microchip's power module.
  Use the "isoreset" (shell) command to unlock the Microchip's power module.
  Use the "start" command to start generating PWMs.
  Use the "alpha" command to change the alpha value of PWMs.
  Use the "stop" command to stop the generation of PWMs.
  Use the "speed" command to set a specific speed.
