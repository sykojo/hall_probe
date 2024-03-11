/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "spi_handler.h"
#include "sensors.h"
#include "uart_bridge.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NUM_OF_SENSORS 8
#define NUM_OF_AXIS 2

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */




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
	//int16_t x_ch_result=0;

	//int32_t x_B=0;
	//uint16_t conv_status=0;
	//uint8_t rdy=0;
	//uint8_t xIsCurrent=0;

	//uint8_t message[2] = {0};
	//int i =0;


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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */

  //Sensor sen1 = sensor_init(8);
 // if(sen1.ok){
//	  while(1)
//	  {
//		  HAL_UART_Receive(&huart4, &flag, 1, HAL_MAX_DELAY);
//
//		  while(1)
//		  {
//			  conv_status = read_register(&sen1, CONV_STATUS);
//			  rdy = (conv_status&(1<<13)) ? 1:0;
//			  xIsCurrent = (conv_status&(1<<8)) ? 1:0;
//			  if(rdy&&xIsCurrent)
//			  {
//				  break;
//			  }
//		  }
//
//		  if(rdy&&xIsCurrent)
//		  {
//			  measuringLED();
//			  x_ch_result = read_register(&sen1, X_CH_RESULT);
//			  x_B = ((int32_t)x_ch_result)*lsb;
//
//			  message[0] = (x_B >> 24) & 0xFF;
//			  message[1] = (x_B >> 16) & 0xFF;
//			  message[2] = (x_B >> 8) & 0xFF;
//			  message[3] = x_B & 0xFF;
//
//			  HAL_UART_Transmit(&huart4, message, 4, 10);
//			  //HAL_Delay(10);
//			  i++;
//		  }
//
//	  }
  	  uint16_t conv_status = 0;
  	  uint8_t rdy = 0;
  	  uint8_t dataAreCurrent= 0;
  	  uint8_t num_of_sensors = 8;
  	  Sensor* sensors = (Sensor*)malloc(sizeof(Sensor)*num_of_sensors);

  	  sensor_power_on();
  	  HAL_Delay(250);

  	  deactivateSCLR();
  	  HAL_Delay(250);

  	  for(int i = 0; i<(num_of_sensors); i++)
  	  {
  		  if(i==3)continue;

  		  sensors[i] = sensor_init(i+1);
  		  if(!(sensors[i].ok))
  		  {
  			  while(1)
  			  {
  				  errorLED();
  				  HAL_Delay(200);
  			  }
  		  }
  	  }

	  while(1)
	  {
		  uint8_t flag=0;

		  for(int i=0;i<(num_of_sensors);i++)
		  {
			  HAL_UART_Receive(&huart4, &flag, 1, HAL_MAX_DELAY);
			  //if(i==3)continue;
			  if(sensors[6].ok)
			  {
				  do{
					  conv_status = read_register(&sensors[6], CONV_STATUS);
					  rdy = conv_status&(1<<13) ? 1:0;
					  dataAreCurrent= conv_status&(0x07<<8) ? 1:0;
				  }while(!(dataAreCurrent&&rdy));

				  int32_t lsb = (2*RANGE*1000000)/(1<<16); //LSB in nT
				  measuringLED();

				  int16_t x_result = read_register(&sensors[6],X_CH_RESULT);
				  int16_t y_result = read_register(&sensors[6],Y_CH_RESULT);
				  int16_t z_result = read_register(&sensors[6],Z_CH_RESULT);

				  sensors[6].measured_data.x_B = ((int32_t)x_result)*lsb;
				  sensors[6].measured_data.y_B = ((int32_t)y_result)*lsb;
				  sensors[6].measured_data.z_B = ((int32_t)z_result)*lsb;

//				  sensors[7].measured_data.x_B = (int32_t)(51);
//				  sensors[7].measured_data.y_B = (int32_t)(52);
//				  sensors[7].measured_data.z_B = (int32_t)(53);

				  uint8_t* message = (uint8_t*)malloc(sizeof(MeasuredData));
				  if(message==NULL) errorLED();
				  memcpy(message,&(sensors[6].measured_data),sizeof(MeasuredData));
				  HAL_UART_Transmit(&huart4, message, sizeof(MeasuredData), HAL_MAX_DELAY);
				  free(message);
			  }
			  else
			  {
				  while(1)
				  {
					  errorLED();
				  }
			  }
		  }
	  }
  	  free(sensors);

// -----Konzultace s Lufinkou----------------
/*----------Select Sensor-------------*/
  /*
  sensor_power_on();
  set_sh_reg_OE(1);
  sen1.adr = 1;
  select_sensor(sen1.adr);
  HAL_Delay(1);
  set_sh_reg_OE(0);
*/
  //sen1 = sensor_init(0x02);
  //HAL_SPI_Transmit(&hspi1, crc_disable, 4, 50);
  //crc_disable[0] = 0x8F;
  //uint8_t crc = calc_crc(0x0F, 0);
  //uint8_t adress[4] = {0x8F,0,0,crc};

  /*-----Disable CRC ----------------------------
  uint8_t crc_disable[4] = {0x0F,0x00,0x04,0x07};
  HAL_SPI_TransmitReceive(&hspi1, crc_disable, reg_data, 4, 50);
  HAL_Delay(1);
----------------------------------------------------------*/



  /*---------Select another sensor--------------
  clear_sh_reg();
  set_sh_reg_OE(1);
  sen1.adr = 0;
  select_sensor(sen1.adr);
  HAL_Delay(1);
  set_sh_reg_OE(0);

  clear_sh_reg();
  set_sh_reg_OE(1);
  sen1.adr = 8;
  select_sensor(sen1.adr);
  HAL_Delay(1);
  set_sh_reg_OE(0);

  ----------------------------------------------*/



  /*----------------Read Sensor----------------
  crc_disable[0] = 0x8F;
  crc_disable[1] = 0;
  crc_disable[2] = 0;
  crc_disable[3] = 0;
  HAL_SPI_TransmitReceive(&hspi1,crc_disable ,reg_data , 4, 50);
  HAL_Delay(1);


  clear_sh_reg();
  set_sh_reg_OE(1);
  sen1.adr = 0;
  select_sensor(sen1.adr);
  HAL_Delay(1);
  set_sh_reg_OE(0);
---------------------------------------------------*/


  //uint16_t result = reg_data[1] + reg_data[2];
  //adress[0] = 0x0F;
  //crc_disable[3] = calc_crc(adress[0], &crc_disable[2]);
//  HAL_SPI_Transmit(&hspi1,crc_disable, 4, 50);
//  adress[0] = 0x8F;
//  HAL_SPI_TransmitReceive(&hspi1, adress, reg_data, 4, 50);
//  result = reg_data[1] + reg_data[2];

  //reg_data = spi_read_reg(sen1.DEVICE_CONFIG.adr, &stat);
  //displayTempData(reg_data);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_GPIO_TogglePin(LED_MEASURING_GPIO_Port, LED_MEASURING_Pin);
	  HAL_Delay(500);

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
