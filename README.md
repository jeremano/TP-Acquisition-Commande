# TP-Acquisition-Commande

## Boudet - Manologlou

## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Setup](#setup)

## General info
This project is simple Lorem ipsum dolor generator.
	
## Technologies
Project is created with:
* Lorem version: 12.3
* Ipsum version: 2.33
* Ament library version: 999
	
## Setup
To run this project, install it locally using npm:

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
