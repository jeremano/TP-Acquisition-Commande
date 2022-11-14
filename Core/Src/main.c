/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define UART_TX_BUFFER_SIZE 64
#define UART_RX_BUFFER_SIZE 1
#define CMD_BUFFER_SIZE 64
#define MAX_ARGS 9
// LF = line feed, saut de ligne
#define ASCII_LF 0x0A
// CR = carriage return, retour chariot
#define ASCII_CR 0x0D
// DEL = delete
#define ASCII_DEL 0x7F


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t prompt[]="user@Nucleo-STM32G431>>";
uint8_t started[]=
		"\r\n*-----------------------------*"
		"\r\n| Welcome on Nucleo-STM32G431 |"
		"\r\n*-----------------------------*"
		"\r\n";
uint8_t newline[]="\r\n";
uint8_t cmdNotFound[]="Command not found\r\n";
uint32_t uartRxReceived;
uint8_t uartRxBuffer[UART_RX_BUFFER_SIZE];
uint8_t uartTxBuffer[UART_TX_BUFFER_SIZE];
// Tableaux comparatifs des commandes
uint8_t helpCMD[] = "help";
uint8_t pinoutCMD[] = "pinout";
uint8_t startCMD[] = "start";
uint8_t stopCMD[] = "stop";
uint8_t alphaCMD[] = "alpha";
uint8_t IsoReset[] = "isoreset";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	char	 	cmdBuffer[CMD_BUFFER_SIZE];
	int 		idx_cmd;
	char* 		argv[MAX_ARGS];
	int		 	argc = 0;
	char*		token;
	int 		newCmdReady = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  memset(argv,NULL,MAX_ARGS*sizeof(char*));
  memset(cmdBuffer,NULL,CMD_BUFFER_SIZE*sizeof(char));
  memset(uartRxBuffer,NULL,UART_RX_BUFFER_SIZE*sizeof(char));
  memset(uartTxBuffer,NULL,UART_TX_BUFFER_SIZE*sizeof(char));

  HAL_UART_Receive_IT(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE);
  HAL_Delay(10);
  HAL_UART_Transmit(&huart2, started, sizeof(started), HAL_MAX_DELAY);
  HAL_UART_Transmit(&huart2, prompt, sizeof(prompt), HAL_MAX_DELAY);



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // uartRxReceived is set to 1 when a new character is received on uart 1
	  	  if(uartRxReceived){
	  		  switch(uartRxBuffer[0]){
	  		  // Nouvelle ligne, instruction à traiter
	  		  case ASCII_CR:
	  			  HAL_UART_Transmit(&huart2, newline, sizeof(newline), HAL_MAX_DELAY);
	  			  cmdBuffer[idx_cmd] = '\0';
	  			  argc = 0;
	  			  token = strtok(cmdBuffer, " ");
	  			  while(token!=NULL){
	  				  argv[argc++] = token;
	  				  token = strtok(NULL, " ");
	  			  }

	  			  idx_cmd = 0;
	  			  newCmdReady = 1;
	  			  break;
	  		  // Suppression du dernier caractère
	  		  case ASCII_DEL:
	  			  cmdBuffer[idx_cmd--] = '\0';
	  			  HAL_UART_Transmit(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE, HAL_MAX_DELAY);
	  			  break;
	  	      // Nouveau caractère
	  		  default:
	  			  cmdBuffer[idx_cmd++] = uartRxBuffer[0];
	  			  HAL_UART_Transmit(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE, HAL_MAX_DELAY);
	  		  }
	  		  uartRxReceived = 0;
	  	  }

	  	  if(newCmdReady){
	  		  if(strcmp(argv[0],"set")==0){
	  			  if(strcmp(argv[1],"PA5")==0){
	  				  HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, atoi(argv[2]));
	  				  sprintf(uartTxBuffer,"Switch on/off led : %d\r\n",atoi(argv[2]));
	  				  HAL_UART_Transmit(&huart2, uartTxBuffer, 32, HAL_MAX_DELAY);
	  			  }
	  			  else{
	  				  HAL_UART_Transmit(&huart2, cmdNotFound, sizeof(cmdNotFound), HAL_MAX_DELAY);
	  			  }
	  		  }
	  		  else if(strcmp(argv[0],"get")==0)
	  		  {
	  			  HAL_UART_Transmit(&huart2, cmdNotFound, sizeof(cmdNotFound), HAL_MAX_DELAY);
	  		  }
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
	  		  }
	  		  else if(strcmp(argv[0],pinoutCMD)==0)
	  		  {
	  			  HAL_UART_Transmit(&huart2, "list of PINs used :\r\n", sizeof("list of PINs used :\r\n"), HAL_MAX_DELAY);
	  			  HAL_UART_Transmit(&huart2, "	PA5  : Switch on/off the LED\r\n", sizeof("	PA5  : Switch on/off the LED\r\n"), HAL_MAX_DELAY);
	  			  HAL_UART_Transmit(&huart2, "	PA8  : PWM 1\r\n", sizeof("	PA8  : PWM 1\r\n"), HAL_MAX_DELAY);
	  			  HAL_UART_Transmit(&huart2, "	PA9  : PWM 2\r\n", sizeof("	PA9  : PWM 2\r\n"), HAL_MAX_DELAY);
	  			  HAL_UART_Transmit(&huart2, "	PA11 : PWM 1N\r\n", sizeof("	PA11 : PWM 1N\r\n"), HAL_MAX_DELAY);
	  			  HAL_UART_Transmit(&huart2, "	PA12 : PWM 2N\r\n", sizeof("	PA12 : PWM 2N\r\n"), HAL_MAX_DELAY);
	  			  HAL_UART_Transmit(&huart2, "	PC3  : IsoReset\r\n", sizeof("	PC3  : IsoReset\r\n"), HAL_MAX_DELAY);
	  		  }
	  		  else if(strcmp(argv[0],alphaCMD)==0)
	  		  {
	  			  TIM1->CCR1 = (int)((5325*(atoi(argv[1]))/100));
	  			  TIM1->CCR2 = (int)((5325*((100 - (atoi(argv[1]))))/100));
	  			  TIM1->CNT=0;

	  		  }
	  		  else if(strcmp(argv[0],startCMD)==0)
	  		  {
	  			  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	  			  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	  			  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	  			  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
	  			  HAL_UART_Transmit(&huart2, "The PWMs have been generated\r\n", sizeof("The PWMs have been generated\r\n"), HAL_MAX_DELAY);
	  		  }
	  		  else if(strcmp(argv[0],stopCMD)==0)
	  		  {
	  			  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	  			  HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
	  			  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	  			  HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
	  			  HAL_UART_Transmit(&huart2, "The PWMs have been stopped\r\n", sizeof("The PWMs have been stopped\r\n"), HAL_MAX_DELAY);
	  		  }
	  		 else if(strcmp(argv[0],IsoReset)==0)
	  		  {
	  			  HAL_UART_Transmit(&huart2, "IsoReset in progress!\r\n", sizeof("IsoReset in progress!\r\n"), HAL_MAX_DELAY);
	  			  HAL_GPIO_WritePin(ISO_RESET_GPIO_Port, ISO_RESET_Pin, 1);
	  			  HAL_Delay(1);
	  			  HAL_GPIO_WritePin(ISO_RESET_GPIO_Port, ISO_RESET_Pin, 0);
	  			  HAL_UART_Transmit(&huart2, "IsoReset done!\r\n", sizeof("IsoReset done!\r\n"), HAL_MAX_DELAY);
	  		  }

	  		  else{
	  			  HAL_UART_Transmit(&huart2, cmdNotFound, sizeof(cmdNotFound), HAL_MAX_DELAY);
	  		  }
	  			  HAL_UART_Transmit(&huart2, prompt, sizeof(prompt), HAL_MAX_DELAY);
	  			  newCmdReady = 0;
	  	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback (UART_HandleTypeDef * huart){
	uartRxReceived = 1;
	HAL_UART_Receive_IT(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
