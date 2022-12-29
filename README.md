# TP-Acquisition-Commande
###### Boudet - Manologlou

## Sommaire
[Objectifs](#objectifs)

## Objectifs
Le but de ce TP est d'asservir un moteur à courant continu en courant et en vitesse.
On utilise plusieurs outils :
* Un moteur à courant continu
* Un driver de moteur MC1L 3-Phase Microchip
* Une carte STM32G474RE
* Un PCB d'interfaçage entre la carte et le driver
* Une alimentation réglable allant jusqu'à 48 V

## Prise en main
On liste tout d'abord les caractéristiques du moteur :
* 3000 RPM nominal
* 48 V
* 6 A nominal

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
