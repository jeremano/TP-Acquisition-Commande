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
else if(strcmp(argv[0],helpCMD)==0)
{
 HAL_UART_Transmit(&huart2, "List of commands:\r\n", sizeof("List of commands:\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	help\r\n", sizeof("	help\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	-> Displays the list of commands\r\n", sizeof("	-> Displays the list of commands\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	set PA5 (0 or 1)\r\n", sizeof("	set PA5 (0 or 1)\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	-> Turns the LED on or off\r\n", sizeof("	-> Turns the LED on or off\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	pinout\r\n", sizeof("	pinout\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	-> Displays the list of used PINs and their uses\r\n", sizeof("	-> Displays the list of used PINs and their uses\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	start\r\n", sizeof("	start\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	-> Starts the generation of PWMs\r\n", sizeof("	-> Starts the generation of PWMs\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	stop\r\n", sizeof("	stop\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	-> Stops the generation of PWMs\r\n", sizeof("	-> Stops the generation of PWMs\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	alpha [0;1]\r\n", sizeof("	alpha [0;1]\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	-> Changes the value of the Alpha duty cycle between 0 and 1\r\n", sizeof("	-> Changes the value of the Alpha duty cycle between 0 and 1\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	isoreset\r\n", sizeof("	isoreset\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	-> Reset the system\r\n", sizeof("	-> Reset the system\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	speed = XXXX\r\n", sizeof("	speed = XXXX\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	-> Set the speed at XXXX [-3000;3000] RPM\r\n", sizeof("	-> Set the speed at XXXX [-3000;3000] RPM\r\n"), HAL_MAX_DELAY);
}
```
Lorsque les caractères UART reçus sont __"help"__, Le microcontroleur renvoie les différentes commandes à l'aide le la fonction __HAL__ associée. Lenvoi de chaque commande se fait en deux parties :
* Le nom de la commande et sa syntaxe
* Sa description et son utilisation

<br>

* pinout
  - Renvoie la liste des broches utilisée ainsi que leur fonctionnalité
```
else if(strcmp(argv[0],pinoutCMD)==0)
{
 HAL_UART_Transmit(&huart2, "list of PINs used :\r\n", sizeof("list of PINs used :\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	PA5  : Switch on/off the LED\r\n", sizeof("	PA5  : Switch on/off the LED\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	PA8  : PWM 1\r\n", sizeof("	PA8  : PWM 1\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	PA9  : PWM 2\r\n", sizeof("	PA9  : PWM 2\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	PA11 : PWM 1N\r\n", sizeof("	PA11 : PWM 1N\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	PA12 : PWM 2N\r\n", sizeof("	PA12 : PWM 2N\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	PC3  : IsoReset\r\n", sizeof("	PC3  : IsoReset\r\n"), HAL_MAX_DELAY);
 HAL_UART_Transmit(&huart2, "	PA1  : Current measurement (ADC)\r\n", sizeof("	PC3  : Current measurement (ADC)\r\n"), HAL_MAX_DELAY);
}
```
Lorsque les caractères UART reçus sont __"pinout"__, Le microcontroleur renvoie les différents noms de broches et leur utilisation.
<br><br>

* start
  - Génère les PWM de commande avec un rapport cyclique de 50% (0RPM)
```
else if(strcmp(argv[0],startCMD)==0)
{
 Status = 0;
 PWMStartStop();
 HAL_UART_Transmit(&huart2, "The PWMs have been generated\r\n", sizeof("The PWMs have been generated\r\n"), HAL_MAX_DELAY);
}
```
Lorsque les caractères UART reçus sont __"start"__, le __stauts__ est forcé à __0__ la fonction d'initialisation des __PWM__ est appelée. Le microcontroleur renvoie par __UART__ la confiramtion du démarrage.
<br><br>

* stop
  - Arrête les PWM, ce qui éteint le moteur
```
else if(strcmp(argv[0],stopCMD)==0)
{
 Status = 1;
 PWMStartStop();
 HAL_UART_Transmit(&huart2, "The PWMs have been stopped\r\n", sizeof("The PWMs have been stopped\r\n"), HAL_MAX_DELAY);
}
```
Lorsque les caractères UART reçus sont __"stop"__, le __stauts__ est forcé à __1__ et la fonction d'initialisation des __PWM__ est appelée. Le microcontroleur renvoie par __UART__ la confiramtion de l'arrêt.
<br><br>

* alpha [0;100]
  - Change le rapport cyclique des PWM
```
else if(strcmp(argv[0],alphaCMD)==0)
{
 CCRAlpha(atoi(argv[1]));
}
```
Lorsque les caractères __UART__ reçus sont __"alpha"__ ainsi qu'une valeur numérique, la fonction de modification du rapport cyclique est appelée et modifie la valeur _alpha_ en fonction de la valeur numérique reçue. 
<br><br>

* isoreset
  - Initialise le driver, pour permettre son démarrage
```
else if(strcmp(argv[0],IsoReset)==0)
{
 HAL_UART_Transmit(&huart2, "IsoReset in progress!\r\n", sizeof("IsoReset in progress!\r\n"), HAL_MAX_DELAY);
 HAL_GPIO_WritePin(ISO_RESET_GPIO_Port, ISO_RESET_Pin, 1);
 HAL_Delay(1);
 HAL_GPIO_WritePin(ISO_RESET_GPIO_Port, ISO_RESET_Pin, 0);
 HAL_UART_Transmit(&huart2, "IsoReset done!\r\n", sizeof("IsoReset done!\r\n"), HAL_MAX_DELAY);
}
```
Lorsque les caractères __UART__ reçus sont __"isoreset"__, la séquence d'initialisation du driver est générée et un message confirmant la fin de la séquence est transmmis par __UART__.
<br><br>

* speed [-3000;3000]
  - Change la vitesse de rotation du moteur
```
else if(strcmp(argv[0],SpeedCMD)==0)
{
 int speed = atoi(argv[1]);
 if (speed > 0)
 {
	if (speed > 3000)
	{
	 speed = 3000;
	}
 }
 if (speed < 0)
 {
	if (speed < -3000)
	{
	 speed = -3000;
	}
 }
 sprintf(uartTxBuffer,"Speed will be set to %d RPM \r\n",speed);
 HAL_UART_Transmit(&huart2, uartTxBuffer, 64, HAL_MAX_DELAY);
 HAL_Delay(10);
 int NewAlpha = ConvAlpha(speed);
 sprintf(uartTxBuffer,"Alpha = %d\r\n",NewAlpha);
 HAL_UART_Transmit(&huart2, uartTxBuffer, 64, HAL_MAX_DELAY);
}
```
Lorsque les caractères __UART__ reçus sont __"speed"__ ainsi qu'une valeur numérique, la validité de la valeur est vérifiée et la valeur _alpha_ équivalente est calculée. La nouvelle valeur d'_alpha_ est renvoyée par __UART__. 
<br>

## 3. Commande du moteur
Nous allons tout d'abord générer des PWM pour créer la commande, afin de faire tourner le moteur dans les deux sens et à une vitesse variable.
<br>

Le cahier des charges nous impose les configurations suivantes :
* Fréquence de la PWM : 16kHz
* Temps mort minimum : 2us
* Résolution minimum : 10bits
* Commande complémentaire décalée

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
