#include "lib/include.h"

int main(void)
{
    uint16_t Result[6];
    char ADC00[4];
    char ADC11[4]; 
    char ADC2[4];
    char ADC3[4];
    char ADC4[4];
    char ADC5[4];
    Configurar_PLL(_50MHZ);  //Confiuracion de velocidad de reloj
    Configurar_UART5();// FCLK 33MHZ Baudrate 19200
    Configurar_GPIO();
    Configura_Reg_ADC0();
    Configura_Reg_ADC1();
   
while(1)
    {
        
        ADC0->PSSI = (1<<1);
        while((ADC0->RIS & (1<<1))==0){}; // espera al convertidor
        Result[0] = ADC0->SSFIFO1&0xFFF; //  Leer  el resultado almacenado en la pila2
        Result[1] = ADC0->SSFIFO1&0xFFF;
        Result[2] = ADC0->SSFIFO1&0xFFF;
        //ADC0_InSeq1(Result); //llamada a la conversion por procesador
        utoa(Result[0],ADC00,10);
        utoa(Result[1],ADC11,10);
        utoa (Result[2],ADC2,10);
        printString(ADC00); 
        printString(ADC11); 
        printString(ADC2);
        ADC0->ISC = 0x0002; 

        ADC1->PSSI = (1<<1);
        while((ADC1->RIS&(1<<1))==0){}; // espera al convertidor
        Result[3] = ADC1->SSFIFO1&0xFFF;
        //ADC1_InSeq1(Result);
        utoa(Result[3],ADC3,10);
        printString(ADC3); 
        ADC1->ISC = 0x0002;

        ADC1->PSSI = (1<<3);
        while((ADC1->RIS&(1<<3))==0){}; // espera al convertidor
        Result[4] = ADC1->SSFIFO3&0xFFF;
        //ADC1_InSeq3(Result);
        utoa(Result[4],ADC4,10);
        printString(ADC4); 
        ADC1->ISC = 0x0008;

        ADC1->PSSI = (1<<2);
        while((ADC1->RIS&(1<<2))==0){}; // espera al convertidor
        Result[5] = ADC1->SSFIFO2&0xFFF;
        //ADC1_InSeq2(Result);
        utoa(Result[5],ADC5,10);
        printString(ADC5); 
        ADC1->ISC = 0x0004;

    }
}