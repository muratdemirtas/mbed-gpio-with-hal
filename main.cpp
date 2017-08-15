#include "mbed.h"       //include mbed back-end

#ifndef __GNUC__
#error  "this demo requires GNU ARM Compiler Collection, pls change..."
#endif // __GNUC__

#define USE_LIB_MBED

#define GPIO_HIGH_LEVEL 1
#define GPIO_LOW_LEVEL 0
#define GREEN_LED_BLINK_TIMEOUT 50



#ifdef USE_HAL_LIB_MBED

#include "stm32f4xx.h"
#include "system_stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

#if defined(TARGET_CORTEX_M) || (TARGET_NUCLEO_F401RE)
#else#error  "TARGET_NUCLEO_F401RE!"
#endif

#define GREEN_LED_HAL_PORT      GPIOA
#define GREEN_LED_HAL_PIN       GPIO_PIN_5

class ledDriver
{
public:
	
	ledDriver(int  _timeout, GPIO_TypeDef *_port, uint16_t _pin) :
		timeout(_timeout), 
		port(_port), 
		pin(_pin) 
	{
		initGPIO();
	}
	
	void initGPIO()
	{
		__GPIOA_CLK_ENABLE();
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Pin = pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
		HAL_GPIO_Init(port, &GPIO_InitStruct);
	}
	
	void toogleLED()
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		HAL_Delay(timeout);
	}
	
private :
	long int timeout;
	GPIO_TypeDef *port;
	uint16_t      pin;
	
};

ledDriver driver(GREEN_LED_BLINK_TIMEOUT,
	(GPIO_TypeDef*) GREEN_LED_HAL_PORT, 
							GREEN_LED_HAL_PIN);

#else  

//green led gpio pin direction will be output with mbed driver
DigitalOut greenled(LED1); //user led on the st nucleo board

#endif // USE_HAL_LIB_MBED

//main function of this application
int main()
{
	
	//forever loop
	while (1)
	{
		#ifdef  USE_HAL_LIB_MBED
				driver.toogleLED();
        
		#else 
		greenled = GPIO_HIGH_LEVEL; // LED is ON, GPIO ON HIGH LEVEL VOLTAGE(+3.3V)
		wait(GREEN_LED_BLINK_TIMEOUT / 50); // Wait 1 second
		greenled = GPIO_LOW_LEVEL; // LED is OFF, GPIO ON LOW LEVEL VOLTAGE(0.0V)
		wait(GREEN_LED_BLINK_TIMEOUT / 50); // Wait 1 second
	
		#endif // USE_HAL_LIB_MBED

		
	}
}

