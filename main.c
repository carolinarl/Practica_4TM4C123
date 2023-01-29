#include "lib/include.h"

int main(void)
{
    uint16_t Result[6];
    char ADC[4];
    Configurar_PLL(_25MHZ);  //Configuración de velocidad de reloj
    Configurar_UART5();// FCLK 30MHZ Baudrate 57600
    Configurar_GPIO();
    Configuracion_ADC();

   
while(1)
    {
        
    ADC0_InSeq1(Result);
    // utoa() — Convert unsigned int into a string
    utoa(Result[0],ADC,10);
    printString(ADC);
    
    ADC0_InSeq1(Result);
    utoa(Result[1],ADC,10);
    printString(ADC);

    ADC0_InSeq1(Result);
    utoa(Result[2],ADC,10);
    printString(ADC);

    ADC0_InSeq1(Result);
    utoa(Result[3],ADC,10);
    printString(ADC);

    //------------------------
    
    ADC0_InSeq3(Result);
    utoa(Result[4],ADC,10);
    printString(ADC);
    
    //------------------------

    ADC1_InSeq3(Result);
    utoa(Result[5],ADC,10);
    printString(ADC);

    }
}
