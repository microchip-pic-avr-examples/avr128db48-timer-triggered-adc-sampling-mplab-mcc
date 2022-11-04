 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
*/

/*
© [2022] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
#include "mcc_generated_files/system/system.h"

/*
    Main application
*/

struct
{
	uint32_t result;
	uint16_t sample;
	uint16_t average_result;
} adc_data;

#define TRUNCATED_SHIFT 4  /* Total number of SAMPLES accumulated are 128. Since this is truncated down to 16 bits, dividing ADC result by 16 or right shifting by 4 will give the average 12-bit ADC result */ 
#define MAX_VOLTAGE	 3.3

#define ADC_RESOLUTION 0x0FFF /* In the test setup, VDD = 3.3V, ADC ref is VDD. 12 bit ADC count is 4095 at 3.3V */
                                                 
float voltage;



/***************************************transmit_to_terminal********************************************
Convert float number (calculated voltage) to string and send to USART3, (PC serial terminal)
**************************************************************************************************/
void transmit_to_terminal(float number)
{
	printf("\nVoltage:");
        printf("%.2f", number);
    printf("V");
  
}
/***************************************MAIN*******************************************************
 * Initialize peripherals.
 * Enable ADC Auto trigger.
 * When ADC result is ready(after accumulating 128 samples), read accumulated result.
 * Calculate average result and voltage at analog pin.
 * Send calculated voltage to serial terminal through USART3. 
**************************************************************************************************/
int main(void)
{
    SYSTEM_Initialize();
    ADC0_EnableAutoTrigger();
   

    while(1)
    {
        if(ADC0_IsConversionDone())
        {
            adc_data.result = ADC0_GetConversionResult();
            adc_data.average_result = (uint16_t)(adc_data.result >> (TRUNCATED_SHIFT));
           	voltage  = (float)(adc_data.average_result * MAX_VOLTAGE) / (ADC_RESOLUTION);
            transmit_to_terminal(voltage);
            
        }    
       
    }    
}