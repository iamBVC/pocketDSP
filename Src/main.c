/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "allpass_filter.h"
#include "comb_filter.h"
#include "reverb.h"
#include "FIR_filter.h"
#include "phaser.h"
#include "delay.h"
#include "distorsion.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

DAC_HandleTypeDef hdac1;

TIM_HandleTypeDef htim7;

/* USER CODE BEGIN PV */

FIRFilter lpf;
Delay dly;
Phaser phs;
Reverb rvb;
Distorsion dist;

const float lpf_coeff[] = {0.0000000f,-0.0000572f,-0.0000035f,0.0000505f,-0.0000070f,-0.0000648f,-0.0000105f,0.0000441f,-0.0000141f,-0.0000727f,-0.0000176f,0.0000379f,-0.0000212f,-0.0000808f,-0.0000247f,0.0000319f,-0.0000283f,-0.0000891f,-0.0000318f,0.0000261f,-0.0000353f,-0.0000975f,-0.0000388f,0.0000207f,-0.0000422f,-0.0001059f,-0.0000455f,0.0000157f,-0.0000488f,-0.0001143f,-0.0000521f,0.0000112f,-0.0000552f,-0.0001227f,-0.0000583f,0.0000073f,-0.0000612f,-0.0001308f,-0.0000640f,0.0000041f,-0.0000667f,-0.0001387f,-0.0000692f,0.0000017f,-0.0000715f,-0.0001461f,-0.0000737f,0.0000003f,-0.0000756f,-0.0001530f,-0.0000773f,0.0000000f,-0.0000788f,-0.0001592f,-0.0000800f,0.0000010f,-0.0000809f,-0.0001646f,-0.0000816f,0.0000034f,-0.0000819f,-0.0001690f,-0.0000819f,0.0000073f,-0.0000815f,-0.0001723f,-0.0000808f,0.0000130f,-0.0000796f,-0.0001743f,-0.0000781f,0.0000205f,-0.0000761f,-0.0001750f,-0.0000738f,0.0000300f,-0.0000709f,-0.0001742f,-0.0000676f,0.0000416f,-0.0000639f,-0.0001719f,-0.0000597f,0.0000553f,-0.0000550f,-0.0001679f,-0.0000498f,0.0000713f,-0.0000441f,-0.0001623f,-0.0000379f,0.0000895f,-0.0000313f,-0.0001550f,-0.0000241f,0.0001100f,-0.0000165f,-0.0001462f,-0.0000085f,0.0001326f,0.0000000f,-0.0001359f,0.0000089f,0.0001573f,0.0000182f,-0.0001242f,0.0000279f,0.0001838f,0.0000380f,-0.0001115f,0.0000484f,0.0002120f,0.0000590f,-0.0000978f,0.0000699f,0.0002417f,0.0000810f,-0.0000836f,0.0000923f,0.0002724f,0.0001037f,-0.0000691f,0.0001152f,0.0003039f,0.0001267f,-0.0000549f,0.0001382f,0.0003357f,0.0001495f,-0.0000412f,0.0001608f,0.0003673f,0.0001718f,-0.0000287f,0.0001825f,0.0003984f,0.0001929f,-0.0000177f,0.0002028f,0.0004283f,0.0002123f,-0.0000090f,0.0002213f,0.0004566f,0.0002296f,-0.0000029f,0.0002372f,0.0004827f,0.0002441f,-0.0000001f,0.0002502f,0.0005060f,0.0002554f,-0.0000011f,0.0002597f,0.0005261f,0.0002629f,-0.0000065f,0.0002651f,0.0005425f,0.0002661f,-0.0000167f,0.0002659f,0.0005546f,0.0002646f,-0.0000322f,0.0002619f,0.0005621f,0.0002579f,-0.0000534f,0.0002525f,0.0005647f,0.0002457f,-0.0000805f,0.0002375f,0.0005622f,0.0002278f,-0.0001139f,0.0002167f,0.0005542f,0.0002041f,-0.0001537f,0.0001900f,0.0005408f,0.0001745f,-0.0001998f,0.0001575f,0.0005221f,0.0001390f,-0.0002523f,0.0001191f,0.0004981f,0.0000979f,-0.0003108f,0.0000753f,0.0004693f,0.0000514f,-0.0003752f,0.0000263f,0.0004359f,0.0000000f,-0.0004448f,-0.0000274f,0.0003987f,-0.0000557f,-0.0005192f,-0.0000850f,0.0003581f,-0.0001151f,-0.0005976f,-0.0001458f,0.0003152f,-0.0001772f,-0.0006792f,-0.0002090f,0.0002707f,-0.0002412f,-0.0007629f,-0.0002736f,0.0002258f,-0.0003060f,-0.0008479f,-0.0003383f,0.0001815f,-0.0003705f,-0.0009328f,-0.0004022f,0.0001392f,-0.0004333f,-0.0010165f,-0.0004638f,0.0001001f,-0.0004933f,-0.0010976f,-0.0005219f,0.0000655f,-0.0005492f,-0.0011750f,-0.0005751f,0.0000369f,-0.0005994f,-0.0012472f,-0.0006220f,0.0000156f,-0.0006428f,-0.0013129f,-0.0006614f,0.0000030f,-0.0006779f,-0.0013708f,-0.0006920f,0.0000003f,-0.0007035f,-0.0014198f,-0.0007124f,0.0000089f,-0.0007185f,-0.0014586f,-0.0007216f,0.0000299f,-0.0007217f,-0.0014864f,-0.0007186f,0.0000641f,-0.0007122f,-0.0015021f,-0.0007025f,0.0001127f,-0.0006893f,-0.0015052f,-0.0006726f,0.0001760f,-0.0006523f,-0.0014951f,-0.0006284f,0.0002548f,-0.0006009f,-0.0014716f,-0.0005697f,0.0003491f,-0.0005349f,-0.0014346f,-0.0004965f,0.0004591f,-0.0004545f,-0.0013843f,-0.0004090f,0.0005844f,-0.0003601f,-0.0013213f,-0.0003077f,0.0007245f,-0.0002522f,-0.0012463f,-0.0001934f,0.0008787f,-0.0001317f,-0.0011603f,-0.0000672f,0.0010458f,0.0000000f,-0.0010648f,0.0000697f,0.0012246f,0.0001415f,-0.0009613f,0.0002155f,0.0014134f,0.0002912f,-0.0008518f,0.0003684f,0.0016103f,0.0004468f,-0.0007383f,0.0005262f,0.0018132f,0.0006062f,-0.0006234f,0.0006866f,0.0020198f,0.0007670f,-0.0005097f,0.0008470f,0.0022276f,0.0009263f,-0.0004000f,0.0010045f,0.0024338f,0.0010814f,-0.0002972f,0.0011564f,0.0026357f,0.0012293f,-0.0002046f,0.0012996f,0.0028304f,0.0013669f,-0.0001254f,0.0014309f,0.0030148f,0.0014911f,-0.0000628f,0.0015472f,0.0031861f,0.0015988f,-0.0000202f,0.0016455f,0.0033413f,0.0016870f,-0.0000008f,0.0017227f,0.0034776f,0.0017525f,-0.0000079f,0.0017759f,0.0035924f,0.0017925f,-0.0000444f,0.0018022f,0.0036831f,0.0018044f,-0.0001133f,0.0017991f,0.0037475f,0.0017857f,-0.0002173f,0.0017642f,0.0037838f,0.0017343f,-0.0003588f,0.0016956f,0.0037903f,0.0016481f,-0.0005400f,0.0015916f,0.0037659f,0.0015258f,-0.0007628f,0.0014507f,0.0037097f,0.0013662f,-0.0010287f,0.0012722f,0.0036215f,0.0011686f,-0.0013389f,0.0010555f,0.0035015f,0.0009328f,-0.0016941f,0.0008005f,0.0033505f,0.0006588f,-0.0020948f,0.0005078f,0.0031699f,0.0003475f,-0.0025411f,0.0001782f,0.0029617f,0.0000000f,-0.0030327f,-0.0001868f,0.0027284f,-0.0003821f,-0.0035689f,-0.0005855f,0.0024733f,-0.0007966f,-0.0041489f,-0.0010153f,0.0022006f,-0.0012410f,-0.0047717f,-0.0014734f,0.0019147f,-0.0017122f,-0.0054361f,-0.0019567f,0.0016213f,-0.0022067f,-0.0061407f,-0.0024615f,0.0013267f,-0.0027207f,-0.0068847f,-0.0029837f,0.0010382f,-0.0032500f,-0.0076671f,-0.0035190f,0.0007640f,-0.0037901f,-0.0084878f,-0.0040628f,0.0005137f,-0.0043364f,-0.0093475f,-0.0046102f,0.0002983f,-0.0048838f,-0.0102480f,-0.0051563f,0.0001306f,-0.0054273f,-0.0111932f,-0.0056959f,0.0000260f,-0.0059617f,-0.0121896f,-0.0062240f,0.0000031f,-0.0064820f,-0.0132480f,-0.0067352f,0.0000855f,-0.0069830f,-0.0143846f,-0.0072247f,0.0003036f,-0.0074597f,-0.0156250f,-0.0076875f,0.0006989f,-0.0079074f,-0.0170088f,-0.0081189f,0.0013301f,-0.0083214f,-0.0185986f,-0.0085145f,0.0022859f,-0.0086976f,-0.0204973f,-0.0088703f,0.0037083f,-0.0090321f,-0.0228824f,-0.0091825f,0.0058441f,-0.0093212f,-0.0260848f,-0.0094479f,0.0091688f,-0.0095621f,-0.0307921f,-0.0096636f,0.0147386f,-0.0097521f,-0.0386972f,-0.0098275f,0.0254819f,-0.0098894f,-0.0553680f,-0.0099377f,0.0536910f,-0.0099723f,-0.1160791f,-0.0099931f,0.3083088f,0.4900000f,0.3083088f,-0.0099931f,-0.1160791f,-0.0099723f,0.0536910f,-0.0099377f,-0.0553680f,-0.0098894f,0.0254819f,-0.0098275f,-0.0386972f,-0.0097521f,0.0147386f,-0.0096636f,-0.0307921f,-0.0095621f,0.0091688f,-0.0094479f,-0.0260848f,-0.0093212f,0.0058441f,-0.0091825f,-0.0228824f,-0.0090321f,0.0037083f,-0.0088703f,-0.0204973f,-0.0086976f,0.0022859f,-0.0085145f,-0.0185986f,-0.0083214f,0.0013301f,-0.0081189f,-0.0170088f,-0.0079074f,0.0006989f,-0.0076875f,-0.0156250f,-0.0074597f,0.0003036f,-0.0072247f,-0.0143846f,-0.0069830f,0.0000855f,-0.0067352f,-0.0132480f,-0.0064820f,0.0000031f,-0.0062240f,-0.0121896f,-0.0059617f,0.0000260f,-0.0056959f,-0.0111932f,-0.0054273f,0.0001306f,-0.0051563f,-0.0102480f,-0.0048838f,0.0002983f,-0.0046102f,-0.0093475f,-0.0043364f,0.0005137f,-0.0040628f,-0.0084878f,-0.0037901f,0.0007640f,-0.0035190f,-0.0076671f,-0.0032500f,0.0010382f,-0.0029837f,-0.0068847f,-0.0027207f,0.0013267f,-0.0024615f,-0.0061407f,-0.0022067f,0.0016213f,-0.0019567f,-0.0054361f,-0.0017122f,0.0019147f,-0.0014734f,-0.0047717f,-0.0012410f,0.0022006f,-0.0010153f,-0.0041489f,-0.0007966f,0.0024733f,-0.0005855f,-0.0035689f,-0.0003821f,0.0027284f,-0.0001868f,-0.0030327f,0.0000000f,0.0029617f,0.0001782f,-0.0025411f,0.0003475f,0.0031699f,0.0005078f,-0.0020948f,0.0006588f,0.0033505f,0.0008005f,-0.0016941f,0.0009328f,0.0035015f,0.0010555f,-0.0013389f,0.0011686f,0.0036215f,0.0012722f,-0.0010287f,0.0013662f,0.0037097f,0.0014507f,-0.0007628f,0.0015258f,0.0037659f,0.0015916f,-0.0005400f,0.0016481f,0.0037903f,0.0016956f,-0.0003588f,0.0017343f,0.0037838f,0.0017642f,-0.0002173f,0.0017857f,0.0037475f,0.0017991f,-0.0001133f,0.0018044f,0.0036831f,0.0018022f,-0.0000444f,0.0017925f,0.0035924f,0.0017759f,-0.0000079f,0.0017525f,0.0034776f,0.0017227f,-0.0000008f,0.0016870f,0.0033413f,0.0016455f,-0.0000202f,0.0015988f,0.0031861f,0.0015472f,-0.0000628f,0.0014911f,0.0030148f,0.0014309f,-0.0001254f,0.0013669f,0.0028304f,0.0012996f,-0.0002046f,0.0012293f,0.0026357f,0.0011564f,-0.0002972f,0.0010814f,0.0024338f,0.0010045f,-0.0004000f,0.0009263f,0.0022276f,0.0008470f,-0.0005097f,0.0007670f,0.0020198f,0.0006866f,-0.0006234f,0.0006062f,0.0018132f,0.0005262f,-0.0007383f,0.0004468f,0.0016103f,0.0003684f,-0.0008518f,0.0002912f,0.0014134f,0.0002155f,-0.0009613f,0.0001415f,0.0012246f,0.0000697f,-0.0010648f,0.0000000f,0.0010458f,-0.0000672f,-0.0011603f,-0.0001317f,0.0008787f,-0.0001934f,-0.0012463f,-0.0002522f,0.0007245f,-0.0003077f,-0.0013213f,-0.0003601f,0.0005844f,-0.0004090f,-0.0013843f,-0.0004545f,0.0004591f,-0.0004965f,-0.0014346f,-0.0005349f,0.0003491f,-0.0005697f,-0.0014716f,-0.0006009f,0.0002548f,-0.0006284f,-0.0014951f,-0.0006523f,0.0001760f,-0.0006726f,-0.0015052f,-0.0006893f,0.0001127f,-0.0007025f,-0.0015021f,-0.0007122f,0.0000641f,-0.0007186f,-0.0014864f,-0.0007217f,0.0000299f,-0.0007216f,-0.0014586f,-0.0007185f,0.0000089f,-0.0007124f,-0.0014198f,-0.0007035f,0.0000003f,-0.0006920f,-0.0013708f,-0.0006779f,0.0000030f,-0.0006614f,-0.0013129f,-0.0006428f,0.0000156f,-0.0006220f,-0.0012472f,-0.0005994f,0.0000369f,-0.0005751f,-0.0011750f,-0.0005492f,0.0000655f,-0.0005219f,-0.0010976f,-0.0004933f,0.0001001f,-0.0004638f,-0.0010165f,-0.0004333f,0.0001392f,-0.0004022f,-0.0009328f,-0.0003705f,0.0001815f,-0.0003383f,-0.0008479f,-0.0003060f,0.0002258f,-0.0002736f,-0.0007629f,-0.0002412f,0.0002707f,-0.0002090f,-0.0006792f,-0.0001772f,0.0003152f,-0.0001458f,-0.0005976f,-0.0001151f,0.0003581f,-0.0000850f,-0.0005192f,-0.0000557f,0.0003987f,-0.0000274f,-0.0004448f,0.0000000f,0.0004359f,0.0000263f,-0.0003752f,0.0000514f,0.0004693f,0.0000753f,-0.0003108f,0.0000979f,0.0004981f,0.0001191f,-0.0002523f,0.0001390f,0.0005221f,0.0001575f,-0.0001998f,0.0001745f,0.0005408f,0.0001900f,-0.0001537f,0.0002041f,0.0005542f,0.0002167f,-0.0001139f,0.0002278f,0.0005622f,0.0002375f,-0.0000805f,0.0002457f,0.0005647f,0.0002525f,-0.0000534f,0.0002579f,0.0005621f,0.0002619f,-0.0000322f,0.0002646f,0.0005546f,0.0002659f,-0.0000167f,0.0002661f,0.0005425f,0.0002651f,-0.0000065f,0.0002629f,0.0005261f,0.0002597f,-0.0000011f,0.0002554f,0.0005060f,0.0002502f,-0.0000001f,0.0002441f,0.0004827f,0.0002372f,-0.0000029f,0.0002296f,0.0004566f,0.0002213f,-0.0000090f,0.0002123f,0.0004283f,0.0002028f,-0.0000177f,0.0001929f,0.0003984f,0.0001825f,-0.0000287f,0.0001718f,0.0003673f,0.0001608f,-0.0000412f,0.0001495f,0.0003357f,0.0001382f,-0.0000549f,0.0001267f,0.0003039f,0.0001152f,-0.0000691f,0.0001037f,0.0002724f,0.0000923f,-0.0000836f,0.0000810f,0.0002417f,0.0000699f,-0.0000978f,0.0000590f,0.0002120f,0.0000484f,-0.0001115f,0.0000380f,0.0001838f,0.0000279f,-0.0001242f,0.0000182f,0.0001573f,0.0000089f,-0.0001359f,0.0000000f,0.0001326f,-0.0000085f,-0.0001462f,-0.0000165f,0.0001100f,-0.0000241f,-0.0001550f,-0.0000313f,0.0000895f,-0.0000379f,-0.0001623f,-0.0000441f,0.0000713f,-0.0000498f,-0.0001679f,-0.0000550f,0.0000553f,-0.0000597f,-0.0001719f,-0.0000639f,0.0000416f,-0.0000676f,-0.0001742f,-0.0000709f,0.0000300f,-0.0000738f,-0.0001750f,-0.0000761f,0.0000205f,-0.0000781f,-0.0001743f,-0.0000796f,0.0000130f,-0.0000808f,-0.0001723f,-0.0000815f,0.0000073f,-0.0000819f,-0.0001690f,-0.0000819f,0.0000034f,-0.0000816f,-0.0001646f,-0.0000809f,0.0000010f,-0.0000800f,-0.0001592f,-0.0000788f,0.0000000f,-0.0000773f,-0.0001530f,-0.0000756f,0.0000003f,-0.0000737f,-0.0001461f,-0.0000715f,0.0000017f,-0.0000692f,-0.0001387f,-0.0000667f,0.0000041f,-0.0000640f,-0.0001308f,-0.0000612f,0.0000073f,-0.0000583f,-0.0001227f,-0.0000552f,0.0000112f,-0.0000521f,-0.0001143f,-0.0000488f,0.0000157f,-0.0000455f,-0.0001059f,-0.0000422f,0.0000207f,-0.0000388f,-0.0000975f,-0.0000353f,0.0000261f,-0.0000318f,-0.0000891f,-0.0000283f,0.0000319f,-0.0000247f,-0.0000808f,-0.0000212f,0.0000379f,-0.0000176f,-0.0000727f,-0.0000141f,0.0000441f,-0.0000105f,-0.0000648f,-0.0000070f,0.0000505f,-0.0000035f,-0.0000572f};

uint8_t FX, BTN_S = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_DAC1_Init(void);
static void MX_TIM7_Init(void);
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
	MX_ADC1_Init();
	MX_DAC1_Init();
	MX_TIM7_Init();
	MX_USB_DEVICE_Init();
	/* USER CODE BEGIN 2 */


	HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
	HAL_TIM_Base_Start_IT(&htim7);
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
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

	/** Supply configuration update enable
	 */
	HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
	/** Configure the main internal regulator output voltage
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
	/** Macro to configure the PLL clock source
	 */
	__HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSI);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 50;
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
	RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
	RCC_OscInitStruct.PLL.PLLFRACN = 0;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
			|RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
	RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
		Error_Handler();
	}
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_USB;
	PeriphClkInitStruct.PLL2.PLL2M = 4;
	PeriphClkInitStruct.PLL2.PLL2N = 9;
	PeriphClkInitStruct.PLL2.PLL2P = 5;
	PeriphClkInitStruct.PLL2.PLL2Q = 2;
	PeriphClkInitStruct.PLL2.PLL2R = 2;
	PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_3;
	PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
	PeriphClkInitStruct.PLL2.PLL2FRACN = 3072;
	PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
	PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Enable USB Voltage detector
	 */
	HAL_PWREx_EnableUSBVoltageDetector();
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void)
{

	/* USER CODE BEGIN ADC1_Init 0 */

	/* USER CODE END ADC1_Init 0 */

	ADC_MultiModeTypeDef multimode = {0};
	ADC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE BEGIN ADC1_Init 1 */

	/* USER CODE END ADC1_Init 1 */
	/** Common config
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	hadc1.Init.Resolution = ADC_RESOLUTION_16B;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DiscontinuousConvMode = ENABLE;
	hadc1.Init.NbrOfDiscConversion = 1;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
	hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
	hadc1.Init.OversamplingMode = ENABLE;
	hadc1.Init.Oversampling.Ratio = 16;
	hadc1.Init.Oversampling.RightBitShift = ADC_RIGHTBITSHIFT_4;
	hadc1.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
	hadc1.Init.Oversampling.OversamplingStopReset = ADC_REGOVERSAMPLING_CONTINUED_MODE;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure the ADC multi-mode
	 */
	multimode.Mode = ADC_MODE_INDEPENDENT;
	if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_3;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	sConfig.OffsetSignedSaturation = DISABLE;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN ADC1_Init 2 */
	ADC1->CR &= ~ADC_CR_DEEPPWD;
	ADC1->CR |= ADC_CR_ADVREGEN;
	HAL_Delay(10);
	ADC1->CR &= ~ADC_CR_ADEN;
	ADC1->CR &= ~ADC_CR_ADCALDIF;
	ADC1->CR |= ADC_CR_ADCAL;
	while (ADC1->CR & ADC_CR_ADCAL);
	ADC1->CR |= ADC_CR_ADEN;
	HAL_Delay(10);
	/* USER CODE END ADC1_Init 2 */

}

/**
 * @brief DAC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_DAC1_Init(void)
{

	/* USER CODE BEGIN DAC1_Init 0 */

	/* USER CODE END DAC1_Init 0 */

	DAC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE BEGIN DAC1_Init 1 */

	/* USER CODE END DAC1_Init 1 */
	/** DAC Initialization
	 */
	hdac1.Instance = DAC1;
	if (HAL_DAC_Init(&hdac1) != HAL_OK)
	{
		Error_Handler();
	}
	/** DAC channel OUT1 config
	 */
	sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
	sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
	sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
	if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN DAC1_Init 2 */

	/* USER CODE END DAC1_Init 2 */

}

/**
 * @brief TIM7 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM7_Init(void)
{

	/* USER CODE BEGIN TIM7_Init 0 */

	/* USER CODE END TIM7_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* USER CODE BEGIN TIM7_Init 1 */

	/* USER CODE END TIM7_Init 1 */
	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 0;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 2499;
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM7_Init 2 */

	/* USER CODE END TIM7_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);

	/*Configure GPIO pin : PE3 */
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pin : PA1 */
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : PC5 */
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {


	if (htim == &htim7) {

		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) == 1 && HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == 1 && BTN_S == 1) BTN_S = 0;

		if((HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) == 0 || HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == 0 ) && BTN_S == 0){
			HAL_TIM_Base_Stop_IT(&htim7);
			BTN_S = 1;
			if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == 0) FX--;
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) == 0) FX++;

			switch (FX){
			case 0:
				break;
			case 1:
				FIRFilter_Init(&lpf, lpf_coeff);
				break;
			case 2:
				FIRFilter_Init(&lpf, lpf_coeff);
				Reverb_Init(&rvb, 673, 691, 709, 739, 331, 101, 0.8, 0.8, 0.8, 0.8, 0.7, 0.7);
				break;
			case 3:
				FIRFilter_Init(&lpf, lpf_coeff);
				Delay_Init(&dly, 1300, 1.0, 0.7); // Delay in Samples, Mix Level, Feedback
				break;
			case 4:
				FIRFilter_Init(&lpf, lpf_coeff);
				Phaser_Init(&phs, 5, 0.5, 0.7); // Delay in Samples, Mix Level, Gain
				break;
			case 5:
				FIRFilter_Init(&lpf, lpf_coeff);
				Reverb_Init(&rvb, 673, 691, 709, 739, 331, 101, 0.8, 0.8, 0.8, 0.8, 0.7, 0.7);
				Phaser_Init(&phs, 5, 0.5, 0.7); // Delay in Samples, Mix Level, Gain
				Delay_Init(&dly, 1300, 1.0, 0.7); // Delay in Samples, Mix Level, Feedback
				break;
			case 6:
				FIRFilter_Init(&lpf, lpf_coeff);
				Reverb_Init(&rvb, 283, 293, 307, 311, 73, 29, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5);
				Distorsion_Init(&dist, 80, 5);
				break;

			}

			HAL_TIM_Base_Start_IT(&htim7);

		}


		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
		HAL_ADC_Start(&hadc1);
		while (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) != HAL_OK);
		float signal_in = (HAL_ADC_GetValue(&hadc1) - 32768.0)/8.0;
		float signal_out = 2048.0;

		switch (FX){

		case 0:
			signal_out += signal_in;
			break;

		case 1:
			FIRFilter_Update(&lpf, signal_in);
			signal_out += lpf.out;
			break;

		case 2:
			FIRFilter_Update(&lpf, signal_in);
			Reverb_Update(&rvb, lpf.out);
			signal_out += rvb.out;
			break;

		case 3:
			FIRFilter_Update(&lpf, signal_in);
			Delay_Update(&dly, lpf.out);
			signal_out += dly.out;
			break;

		case 4:
			FIRFilter_Update(&lpf, signal_in);
			Phaser_Update(&phs, lpf.out);
			signal_out += phs.out;
			break;
		case 5:
			FIRFilter_Update(&lpf, signal_in);
			Phaser_Update(&phs, lpf.out);
			Delay_Update(&dly, phs.out);
			Reverb_Update(&rvb, dly.out);
			signal_out += rvb.out;
			break;
		case 6:
			FIRFilter_Update(&lpf, signal_in);
			Distorsion_Update(&dist, lpf.out);
			Reverb_Update(&rvb, dist.out * 4.0);
			signal_out += rvb.out;
			break;

		}

		if (signal_out > 4095.0) signal_out = 4095;
		if (signal_out < 0.0) signal_out = 0;
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint16_t)signal_out);

		//char logBuf[256];
		//sprintf(logBuf, "%.3f,%.3f\r\n", signal_in, rvb.out);
		//CDC_Transmit_FS((uint8_t *) logBuf, strlen(logBuf));

		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);
	}
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
