/*
 * ADC.c
 *
 *  Created on: 04/11/2019
 *      Author: ravenelco
 */
#include "lib/include.h"

extern void Configuracion_ADC(void)
{
    
     //Pag 352 (TIVA CHICA) para inicializar el modulo de reloj del adc RCGCADC
    SYSCTL->RCGCADC = (1<<0) | (1<<1); //Se inicializa el modulo 0 y 1 del ADC 
    //Pag 382 (RGCGPIO) Se habilitan los puertos que corresponden 
    
    
    //b) 2,4,6,7,10,1- 30MHZ -57600 -sec3, sec1
    // AN1 - PIN 7 - PE2
    // AN4 - PIN 64 - PD3
    // AN6 - PIN 62 - PD1
    // AN7 - PIN 61 - PD0
    // AN10 - PIN 58 - PB4
    // AN2 - PIN 8 - PE1
    // PUERTOS B, E, D


    //                     F     E      D       C      B     A
    SYSCTL->RCGCGPIO |= (1<<5)|(1<<4)|(1<<3)|(0<<2)|(1<<1)|(1<<0);
    //Pag 760 (GPIODIR) Habilta los pines como I/O (los ADC SON ENTRADAS - SE PONEN LOS PINES QUE NOS TOCARON)
    //HABILITACIÓN PUERTO A, DEBIDO AL UART
    //HABILITACIÓN PUERTO F, DEBIDO AL USO DE LEDS



    GPIOE_AHB->DIR = (0<<1) | (0<<2); //PIN 1 y 2  - PUERTO E COMO ENTRADAS
    GPIOD_AHB->DIR = (0<<0) | (0<<1) | (0<<3); //PIN 0, 1 y 3 - PUERTO D COMO ENTRADAS
    GPIOB_AHB->DIR = (0<<4); //PIN 4 - PUERTO B COMO ENTRADAS
    // DIR: (0)entrada, (1)salida.
    
    
    //(GPIOAFSEL) pag.672 Habilitar funciones alternativas para que el modulo analógico tenga control de esos pines (PUERTO E, D, B) - SE PONEN EN 1
    GPIOE_AHB->AFSEL =  (1<<1) | (1<<2);
    GPIOD_AHB->AFSEL =  (1<<0) | (1<<1) | (1<<3);
    GPIOB_AHB->AFSEL =  (1<<4);
    // ASFEL

    //(GPIODEN) pag.672 desabilita el modo digital para los puertos E, D y B 
    GPIOE_AHB->DEN = (0<<1) | (0<<2);
    GPIOD_AHB->DEN = (0<<0) | (0<<1) | (0<<3);
    GPIOB_AHB->DEN =  (0<<4);

    //Pag 688 GPIOPCTL registro combinado con el GPIOAFSEL y la tabla pag 1808
    GPIOE_AHB->PCTL = GPIOE_AHB->PCTL & (0xFFFFF00F); 
     GPIOD_AHB->PCTL = GPIOD_AHB->PCTL & (0xFFFF0F00); 
      GPIOB_AHB->PCTL = GPIOB_AHB->PCTL & (0xFFF0FFFF); 

    //(GPIOAMSEL) pag.687 Se habilita función analogica para cada uno de los puertos
    GPIOE_AHB->AMSEL |= (1<<1) | (1<<2);
    GPIOD_AHB->AMSEL |= (1<<0) | (1<<1) | (1<<3);
    GPIOB_AHB->AMSEL |= (1<<4);


    //Pag 891 El registro (ADCPC) establece la velocidad de conversión de un millon por segundo para cada ADC y para cada puerto 
    //1Msps, valor máximo
    ADC0->PC = 0x7;//0x07, 1 Msps
    ADC1->PC = 0x7;//0x07, 1 Msps
    
    //Pag 841 Este registro (ADCSSPRI) configura la prioridad de los secuenciadores
    ADC0->SSPRI = 0x1302;
    //ADC1->SSPRI = 0x0000;   //Se pone por desault así que la prioridad sera 0, 1,2,3
    
    //Pag 841 (ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC0->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0); //SE DESACTIVAN PARA INICIAR LA CONFIGURACIÓN 
    
    
    //----------------------------------------------------------------------------------------------------------------------------
    
    // CONFIGURACIÓN DE SECUENCIADORES
    
    //Pag 833 Este registro (ADCEMUX) selecciona el evento que activa la conversión
    // Por GPIO
    ADC0->EMUX = (0x0<<12) | (0x0<<8) | (0x0<<4);
    ADC1->EMUX = (0x0<<12) | (0x0<<8) | (0x0<<4);
    //ADC1->EMUX  = (0x0000);  //Se eligio trigger

    //Pag 1129 Este registro (ADCSSMUX2) define las entradas analógicas con el canal y secuenciador seleccionado
    // sec1 y sec3
    //izquierda - entradas analógicas
    //derecha - samples
    ADC0->SSMUX1 = (ADC0->SSMUX1 & ~0x0F) | (2<<0) | (4<<4) | (6<<8) | (7<<12);
    ADC0->SSMUX3 = (ADC0->SSMUX3 & ~0x0F) | (10<0);
    ADC1->SSMUX1 = (ADC0->SSMUX1 & ~0x0F) | (2<<0) | (4<<4) | (6<<8) | (7<<12);
    ADC1->SSMUX3 = (ADC0->SSMUX3 & ~0x0F) | (10<0);


    //pag 868 Este registro (ADCSSCTL2), configura el bit de control de muestreo y la interrupción
    // Se interrumpe el muestreo cuando se finalice el muestreo.
    ADC0->SSCTL1 = (1<<2) | (1<<6) | (1<<9) | (1<<13);
    ADC0->SSCTL3 = (1<<2) | (1<<1);
    ADC1->SSCTL1 = (1<<2) | (1<<6) | (1<<9) | (1<<13);
    ADC1->SSCTL3 = (1<<2) | (1<<1);

    /* Enable ADC Interrupt */
    ADC0->IM |= (1<<3) | (1<<1); /* Unmask ADC0 sequence 2 interrupt pag 1082*/
    ADC1->IM |= (1<<3) | (1<<1);

    //NVIC_PRI4_R = (NVIC_PRI4_R & 0xFFFFFF00) | 0x00000020;
    //NVIC_EN0_R = 0x00010000;
    //Pag 1077 (ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC0->ACTSS |= (1<<3) | (1<<1);
    ADC1->ACTSS |= (1<<3) | (1<<1);
    // sec 1 y 3 habilitados


    ADC0->PSSI |= (1<<3) | (1<<1) | (1<<0); //Se inicializa el muestreo en el secuenciador que se va a utilizar
    ADC1->PSSI |= (1<<3) | (1<<1) | (1<<0);

}
// SS1 ---> 4 muestras
// SS3 ---> 1 muestra

extern void ADC0_InSeq1(uint16_t *Result){

    //ADC Processor Sample Sequence Initiate (ADCPSSI)
       ADC0->PSSI = (1<<1);
       while((ADC0->RIS & (1<<1))==0){}; // espera al convertidor
       Result[0] = ADC0->SSFIFO1&0xFFF; //  Leer  el resultado almacenado en la pila2
       Result[1] = ADC0->SSFIFO1&0xFFF;
       Result[2] = ADC0->SSFIFO1&0xFFF;
       Result[3] = ADC0->SSFIFO1&0xFFF;
       //printChar('A');
       ADC0->ISC = 0x0002;  //Conversion finalizada

}

extern void ADC0_InSeq3(uint16_t *Result){

    //ADC Processor Sample Sequence Initiate (ADCPSSI)
       ADC0->PSSI = (1<<3);
       while((ADC0->RIS&(1<<3))==0){}; // espera al convertidor
       Result[4] = ADC0->SSFIFO1&0xFFF;
       //printChar('A');
       ADC0->ISC = 0x0008;  //Conversion finalizada

}

extern void ADC1_InSeq3(uint16_t *Result){

    //ADC Processor Sample Sequence Initiate (ADCPSSI)
       ADC1->PSSI = (1<<3);
       while((ADC1->RIS&(1<<3))==0){}; // espera al convertidor
       Result[5] = ADC1->SSFIFO1&0xFFF;
       //printChar('A');
       ADC1->ISC = 0x0008;  //Conversion finalizada

}

