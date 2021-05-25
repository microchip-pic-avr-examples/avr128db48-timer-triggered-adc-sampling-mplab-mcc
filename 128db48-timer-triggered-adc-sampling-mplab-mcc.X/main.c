/*
Copyright (c) [2012-2020] Microchip Technology Inc.  

    All rights reserved.

    You are permitted to use the accompanying software and its derivatives 
    with Microchip products. See the Microchip license agreement accompanying 
    this software, if any, for additional info regarding your rights and 
    obligations.
    
    MICROCHIP SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT 
    WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
    LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT 
    AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP OR ITS
    LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT 
    LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE 
    THEORY FOR ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT 
    LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, 
    OR OTHER SIMILAR COSTS. 
    
    To the fullest extend allowed by law, Microchip and its licensors 
    liability will not exceed the amount of fees, if any, that you paid 
    directly to Microchip to use this software. 
    
    THIRD PARTY SOFTWARE:  Notwithstanding anything to the contrary, any 
    third party software accompanying this software is subject to the terms 
    and conditions of the third party's license agreement.  To the extent 
    required by third party licenses covering such third party software, 
    the terms of such license will apply in lieu of the terms provided in 
    this notice or applicable license.  To the extent the terms of such 
    third party licenses prohibit any of the restrictions described here, 
    such restrictions will not apply to such third party software.
*/
#include "mcc_generated_files/system/system.h"

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
	char string[8];

	dtostrf(number, 4, 2, string);
	printf("\nVoltage:");
    printf(string);
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