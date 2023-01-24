# TP-Acquisition-Commande 
<br>

![GitHub language count](https://img.shields.io/github/languages/count/jeremano/TP-Acquisition-Commande?style=flat-square) 
![GitHub top language](https://img.shields.io/github/languages/top/jeremano/TP-Acquisition-Commande?color=blue&style=flat-square) 
<br><br>

![GitHub contributors](https://img.shields.io/github/contributors/jeremano/TP-Acquisition-Commande?style=flat-square)<br>
[_Anthony BOUDET_](https://github.com/AnthonyBOUDET) - [_Jeremy MANOLOGLOU_](https://github.com/jeremano)
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

Les caractères qui servent de comparaison sont enregistrés en variables globales, et les fonctions _else if_ servent d'aiguillage en comparant les caractères reçu aux caractères préenregistrés.
```
uint8_t helpCMD[] = "help";
uint8_t pinoutCMD[] = "pinout";
uint8_t startCMD[] = "start";
uint8_t stopCMD[] = "stop";
uint8_t alphaCMD[] = "alpha";
uint8_t IsoReset[] = "isoreset";
uint8_t SpeedCMD[] = "speed";
```

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

![alt text](https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/Commande-Complémentaire.jpg)
<p align="center">
Commande Complémentaire Simple
</p>
<br><br>

![alt text](https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/Commande-Compl%C3%A9mentaire-D%C3%A9cal%C3%A9e.jpg)
<p align="center">
Commande Complémentaire Décalée
</p>
<br><br>

Quand le moteur est à l'arrêt, il reçoit du courant positif et négatif en complémentaire simple. En complémentaire, le moteur est à l'arrêt et ne reçoit pas de tension.

___

Pour générer la commande, nous utilisons le timer 1 avec la configuration suivante :

<p align="center">
<img src="https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/TIM1-Mode.png" width="300" /> <img src="https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/TIM1-PWM.png" width="275" /> <img src="https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/TIM1-Counter-settings.png" width="375" />
</p>

![alt text](https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/TIM1-Dead-Time.png)

On souhaite un temps mort d'au moins 2µs. En effectuant les calculs avec DTG[7:0] = 1100101 = 203 on obtient :
DTG[7:5] = 110 donc DT = (32 + DTG[4:0])x8Tdts avec DTG[4:0] = 01011
Le timer 1 étant cadensé à 170 MHz, on a Tdts = 1/(170E-9)= 5.9ns
Donc DT = (32 + 11)x8Tdts = 2.02 µs
DTG doit donc avoir une valeur décimal supérieur ou égale à 203.
On a donc choisi un DTG égale a 205 pour avoir une marge de sécurité en plus.
<br>

```
void PWMStartStop(void)
{
 if(Status == 0)
 {
  CCRAlpha(50);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
  Status = 1;
 }
 else
 {
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
  Status = 0;
 }
}
```

<br>

<p align="center">
<img src="https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/Screen-PWM.png" "width=500" /><br>
<i>
Visualisation des Signaux Générés
</i>
</p>
<br>

<p align="center">
<img src="https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/Screen-temps-mort.png" /><br>
<i>
Visualisation du Temps Mort
</i>
</p>
<br>

## 4. Mesure du courant

![alt text](https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/ADC-Config.png) ![alt text](https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/DMA-Config.png)
```
if(HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED)!=HAL_OK)
{
 Error_Handler();
}
if(HAL_ADC_Start_DMA(&hadc2, DAT, NbMoy)!=HAL_OK)
{
 Error_Handler();
}
```

___

La mesure du courant étant faite lorsque le timer 1 atteint les extremums, et que ces extemums sont placés au milieu des des alternances hautes et basses des PWM, la mesure de l'ADC est faite hors des périodes de transition et pile au milieu de la pente de courant. Ceci permet d'avoir une seule valeur à mesurer par période, et que celle-ci soit exactement la valeur moyenne du courant envoyée dans le bras de pont.
```
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
 NbConv++;
 float CMoy = 0;
 for (int i = 0; i < NbMoy; ++i) 
 {
  CMoy = CMoy + (float)DAT[i];
 }
 CMoy = CMoy/NbMoy;
 courant = ((CMoy-3100)/4096)*(12*3.3)-0.3;
}
```
<br>

## 5. Mesure de la vitesse
La mesure de vitesse s'effectue avec le retour d'une roue codeuse. La roue codeuse donne la position, qui est plus précise que la vitesse, et de laquelle nous déduirons la vitesse.

<p align="center">
<img src="https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/Codeur-rotatif.jpg" /><br>
<i>
Fonctionnement Encodeur Rotatif
</i>
</p>

L'ordre des fronts présents sur les deux broches permettent de déterminer le sens de rotation.

___

On utilise 2 horloges :
* En mode lecteur incrémental qui compte le nombre de fronts envoyés par la roue codeuse
* En mode interruption qui permet de vérifier la concordance entre la valeur mesurée de la position et la valeur estimée, à une fréquence régulière

![alt text](https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/TIM2.png) ![alt text](https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/TIM2-config.png)

Le registre ARR est laissé au maximum pour permettre le comptage des fronts sur la plus grande plage possible.

![alt text](https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/TIM3.png) ![alt text](https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/TIM3-config.png)
![alt text](https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/TIM3-NVIC.png)

```
HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
HAL_TIM_Base_Start_IT(&htim3);
```

___

```
void TIMIRQ(void)
{
 HAL_GPIO_TogglePin(Rotary_GPIO_Port, Rotary_Pin);
 HAL_GPIO_TogglePin(Rotary_GPIO_Port, Rotary_Pin);
 Ticks = ((((TIM2->CNT)-32767)/0.05)/4096);
 TIM2->CNT = 32767;
}
```
<br>

## 6. Asservissement

Pour notre moteur nous avons eu besoin de réaliser deux asservissement en cascade. Tout d'abord, nous avons eu besoin de réaliser un asservissement en courant qui est obligatoire si nous voulons protéger notre moteur de potentielles surintensités. Cet asservissement devra par la suite être couplé à un asservissement en Vitesse qui correspondra à la consigne initiale du moteur. 
	
L'objectif est donc de fournir une consigne en vitesse au moteur, que le moteur atteigne cette vitesse le plus rapidement possible tout en restant dans les limites d'utilisations du moteur en terme d'intensité.
	
[alt text](https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/Asservissement-Global.png)
	
Ces deux asservissements sont composées de saturateurs et de correcteurs de type Propotionnel Intégral (PI). 
	
![alt text](https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/Correcteur-PI.png)
	
Ces deux correcteurs possèdent des paramètre K et Ki qui peuvent être directement déterminer grâce à une simulation sur MatLab. Une fois ces paramètres déterminés, nous pouvons nous occupé de l'intégration de ceux-ci dans notre code. Mais pour cela nous devons utilisé la transformée en Z pour passé du domaine de la place en indiciel pour déterminer l'équation de récurrence régissant la variable de sortie S
	
![alt text](https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/Calcul-Correcteur.jpg)
	
En intégrant notre correcteur PI, pour notre asservissement en courant, dans notre code, nous avons pu effectuer quelques tests afin de vérifier si la commande en courant était belle est bien respectée, et surtout qu'il n'y ait aucun dépassement. Pour cela, nous avons rajouter un effet d'anti-Windup afin d'arrêter d'intégrer quand la différence entre la consigne et la sortie est négative. Ce qui permet à l'intégrateur de ne pas générer de dépassement.
	
![alt text](https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/Code-Asserv-Anti-Windup-(légende).png)
		
![alt text](https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/tek00007.png)

![alt text](https://github.com/jeremano/TP-Acquisition-Commande/blob/main/Medias/tek00004.png)
	
Malheureusement nous n'avons pas eu le temps d'aller plus loin et donc nous n'avons pas pu réaliser l'asservissement en vitesse.
<br>

## 7. Fonctionnalitées supplémentaires

Pour simplifier l'utilisation du système, nous avons programmé le bouton bleu pour démarrer les PWM avec un rapport cyclique de 50%. Mais nous avons aussi ajouté la possibilité d'éteindre les PWM avec ce même bouton, lorsque celles-ci sont déjà lancées!
<br>

## 8. Conclusion

Durant ces séances, nous avons pu apprendre à utiliser une STM32 afin de piloter un moteur et son driver. Nous avons aussi expérimenté l'asservissement en courant et vu beaucoup de nouvelles fonctionnalités.<br>
Malheureusement nous nous sommes heurtés à certaines difficultées, telles que le disfonctionnement de l'ADC sur certaines cartes. Cela nous a empéchés d'aller jusqu'au bout du sujet. Cependant, la partie étudiée (la grande majorité du sujet) à été bien comprise grâce à la tournure ludique du projet!
