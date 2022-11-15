# TP-Acquisition-Commande

Functions Done :

  -help
  -pinout
  -alpha [0;100]
  -isoreset
  -start
  -stop

Data :

  -Clock speed : 170 MHz
  -Counter période : 5325
  -CCR1,CCR2 2663 (alpha = 50%)
  -Dead Time : 205

Work in progress :

  -speed XXXX [-3000;3000]
  -ADC

How to use it :

  Connect pins PA8,9,11,12 to the Microchip's power module. (PWM)
  Connect pin PC3 to pin 33 of the Microchip's power module. (Iso_Reset)
  Powering the Microchip's power module.
  Use the "isoreset" (shell) command to unlock the Microchip's power module.
  Use the "start" command to start generating PWMs.
  Use the "alpha" command to change the alpha value of PWMs.
  Use the "stop" command to stop the generation of PWMs.
