/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @author			: C�ssio F. Braga, Nathan E. Nogara
  * @brief          : DSP FIR Rejeita-Faixa
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#define ARM_MATH_CM4  // Define o CPU como ARM Cortex M4
#define __FPU_PRESENT 1  // Dispositivo trabalha com ponto flutuante
#include <arm_math.h>  // Inclui o cabe�alho da CMSIS

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define NDMA 1
#define TEST_LENGTH_SAMPLES 1
#define BLOCK_SIZE 1
#define NUM_TAPS 21

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//Inst�ncia do filtro
arm_fir_instance_f32 instancia;

// ADC e DAC
uint16_t adc[NDMA], dac[NDMA];

// Coeficientes
static float32_t coeficientes[NUM_TAPS] = { 0.024023481183095947,
		 0.036394205001950820,
		 0.045930989967580653,
		 0.047339812446158175,
		 0.036112174194985539,
		 0.010394323894579812,
		-0.027726252230325252,
		-0.072112617369714194,
		-0.113857056797813022,
		-0.143641753182584708,
		 0.845559965451106010,
		-0.143641753182584708,
		-0.113857056797813022,
		-0.072112617369714194,
		-0.027726252230325252,
		 0.010394323894579812,
		 0.036112174194985539,
		 0.047339812446158175,
		 0.045930989967580653,
		 0.036394205001950820,
		 0.024023481183095947};

//Sinal de entrada do filtro em float
float32_t signal_in = 0;

//Sinal de sa�da do filtro em float
float32_t signal_out = 0;

static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1];

//Tamanho do bloco do filtro = 1
uint32_t blockSize = BLOCK_SIZE;

uint32_t numBlocks = TEST_LENGTH_SAMPLES/BLOCK_SIZE;

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
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_DAC_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim2); // Timer da frequ�ncia de amostragem
  HAL_ADC_Start_DMA(&hadc1, adc, NDMA); // Coleta da amostra do sinal de entrada
  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, dac, NDMA, DAC_ALIGN_12B_R); // Emiss�o do resultado do filtro
  arm_fir_init_f32(&instancia, NUM_TAPS, (float32_t *)&coeficientes[0], (float32_t *)&firStateF32[0], blockSize); //Inicializa��o do filtro
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /**Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
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

/*
 * @brief Callback do fim da convers�o do ADC
 * @param Handler do ADC
 * @retval Nada
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    signal_in = (float32_t)adc[0]; // Converte o sinal do adc para float

    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); //Inicia a contagem do tempo de c�lculo

    arm_fir_f32(&instancia, &signal_in, &signal_out, blockSize); //Filtro com os valores atuais

    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); //Termina a contagem do tempo de c�lculo

    dac[0] = (uint32_t)signal_out; //Converte o sinal resultante de float para uint32
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
