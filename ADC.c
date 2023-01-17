/*
 * ADC.c
 *
 *  Created on: 04/11/2019
 *      Author: ravenelco
 */
#include "lib/include.h"

extern void Configura_Reg_ADC0(void)
{
    /*
    Habilitar el modulo 0 del ADC con dos canales analogicos 
    en el puerto E a una velocidad de conversion de 250ksps
    dandole la mayor prioridad al secuenciador 2 con evento
    de procesador 
    */
     //Pag 352 (TIVA CHICA) para inicializar el modulo de reloj del adc RCGCADC
    SYSCTL->RCGCADC = (1<<0) | (1<<1); //Se inicializa el modulo 0 y 1 del ADC 
    //Pag 382 (RGCGPIO) Se habilitan los puertos que corresponden 
    
    
    //d)11,9,6,7,4,2 33MHZ 19200 -sec3,sec0,sec2
    // AN11 - PIN 57 - PB5
    // AN9 - PIN 59 - PE4
    // AN6 - PIN 62 - PD1
    // AN7 - PIN 61 - PD0
    // AN4 - PIN 64 - PD3
    // AN2 - PIN 8 - PE1
    // PUERTOS B, E, D


    //                     F     E      D       C      B     A
    SYSCTL->RCGCGPIO |= (1<<5)|(1<<4)|(1<<3)|(0<<2)|(1<<1)|(1<<0);
    //Pag 760 (GPIODIR) Habilta los pines como I/O (los ADC SON ENTRADAS - SE PONEN LOS PINES QUE NOS TOCARON)
    //HABILITACIÓN PUERTO A, DEBIDO AL UART
    //HABILITACIÓN PUERTO F, DEBIDO AL USO DE LEDS



    GPIOE_AHB->DIR = (0<<1) | (0<<4); //PIN 1 y 4  - PUERTO E COMO ENTRADAS
    GPIOD_AHB->DIR = (0<<0) | (0<<1) | (0<<3); //PIN 0, 1 y 3 - PUERTO D COMO ENTRADAS
    GPIOB_AHB->DIR = (0<<5); //PIN 5 - PUERTO B COMO ENTRADAS
    // DIR: (0)entrada, (1)salida.
    
    
    //(GPIOAFSEL) pag.672 Habilitar funciones alternativas para que el modulo analógico tenga control de esos pines (PUERTO E, D, B) - SE PONEN EN 1
    GPIOE_AHB->AFSEL =  (1<<1) | (1<<4);
    GPIOD_AHB->AFSEL =  (1<<0) | (1<<1) | (1<<3);
    GPIOB_AHB->AFSEL =  (1<<5);
    // ASFEL

    //(GPIODEN) pag.672 desabilita el modo digital para los puertos E, D y B 
    GPIOE_AHB->DEN = (0<<1) | (0<<4);
    GPIOD_AHB->DEN = (0<<0) | (0<<1) | (0<<3);
    GPIOB_AHB->DEN =  (0<<5);

    //Pag 688 GPIOPCTL registro combinado con el GPIOAFSEL y la tabla pag 1808
    GPIOE_AHB->PCTL = GPIOE_AHB->PCTL & (0xFFF0FF0F); 
     GPIOD_AHB->PCTL = GPIOD_AHB->PCTL & (0xFFFF0F00); 
      GPIOB_AHB->PCTL = GPIOB_AHB->PCTL & (0xFF0FFFFF); 

    //(GPIOAMSEL) pag.687 Se habilita función analogica para cada uno de los puertos
    GPIOE_AHB->AMSEL |= (1<<1) | (1<<4);
    GPIOD_AHB->AMSEL |= (1<<0) | (1<<1) | (1<<3);
    GPIOB_AHB->AMSEL |= (1<<5);


    //Pag 891 El registro (ADCPC) establece la velocidad de conversión de un millon por segundo para cada ADC y para cada puerto 
    //1Msps, valor máximo
    ADC0->PC = (0<<3)|(1<<2)|(1<<1)|(1<<0);//0x07, 1 Msps
    ADC1->PC = (0<<3)|(1<<2)|(1<<1)|(1<<0);//0x07, 1 Msps
    
    //Pag 841 Este registro (ADCSSPRI) configura la prioridad de los secuenciadores
    ADC0->SSPRI = 0x0000;
    ADC1->SSPRI = 0x0000;
    //ADC1->SSPRI = 0x0000;   //Se pone por desault así que la prioridad sera 0, 1,2,3
    
    //Pag 841 (ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC0->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0); //SE DESACTIVAN PARA INICIAR LA CONFIGURACIÓN 
    ADC1->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0); //SE DESACTIVAN PARA INICIAR LA CONFIGURACIÓN 
    
    
    //----------------------------------------------------------------------------------------------------------------------------
    
    // CONFIGURACIÓN DE SECUENCIADORES
    
    //Pag 1091 Este registro (ADCEMUX) selecciona el evento que activa la conversión
    // Por GPIO
    ADC0->EMUX  = (0x4<<15) | (0x4<<11); 
    ADC1->EMUX  = (0x4<<3);  
    //ADC1->EMUX  = (0x0000);  //Se eligio trigger

    //Pag 1129 Este registro (ADCSSMUX2) define las entradas analógicas con el canal y secuenciador seleccionado
    // sec3,sec0,sec2
    //izquierda - entradas analógicas
    //derecha - samples
    ADC1->SSMUX0 = (11<<0) | (9<<4) | (6<<8); 
    ADC0->SSMUX2 = (7<<0) | (4<<4) ; 
    ADC0->SSMUX3 = (2<<0);

    //d)11,9,6,7,4,2 33MHZ 19200 -sec3,sec0,sec2
    // AN11 - PIN 57 - PB5
    // AN9 - PIN 59 - PE4
    // AN6 - PIN 62 - PD1
    // AN7 - PIN 61 - PD0
    // AN4 - PIN 64 - PD3
    // AN2 - PIN 8 - PE1
    // PUERTOS B, E, D



    //pag 868 Este registro (ADCSSCTL2), configura el bit de control de muestreo y la interrupción
    ADC1->SSCTL0 = (1<<6) | (1<<5);  // Se interrumpe el muestreo cuando se finalice el muestreo del canal 9.
    ADC0->SSCTL2 = (1<<6) | (1<<5);  // Se interrumpe el muestreo cuando se finalice el muestreo del canal 11.
    ADC0->SSCTL3 = (1<<6) | (1<<5);

    /* Enable ADC Interrupt */
    ADC1->IM |= (1<<1); /* Unmask ADC1 sequence 2 interrupt pag 1082*/
    ADC0->IM |= (1<<2); /* Unmask ADC0 sequence 2 interrupt pag 1082*/

    //NVIC_PRI4_R = (NVIC_PRI4_R & 0xFFFFFF00) | 0x00000020;
    //NVIC_EN0_R = 0x00010000;
    //Pag 1077 (ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC1->ACTSS |= (1<<3) | (1<<2) | (0<<1) | (1<<0);
    ADC0->ACTSS |= (1<<3) | (1<<2) | (0<<1) | (1<<0);
    // sec 0, 2 y 3 habilitados


    ADC0->PSSI |= (1<<3) | (1<<2) | (1<<0); //Se inicializa el muestreo en el secuenciador que se va a utilizar
    ADC1->PSSI |= (1<<3) | (1<<2) | (1<<0);
}





extern void ADC_leer_canal(uint16_t data[])
{
    // --------------------------------------selector 0
    ADC1 -> PSSI |= (1<<0);
    delay_ms(1);

    while (ADC1 -> RIS & 0x01 == 0);
    delay_ms(1);
    while(ADC1 -> SSOP0 & (1<<0) == (1<<0));
    data[0] = ADC1 -> SSFIFO0 & 0xfff;
    delay_ms(1);
    while(ADC1->SSOP0 & (1<<4) == (1<<4));
    data[1] = ADC1->SSFIFO0 & 0xfff;
    delay_ms(1);
    while(ADC1->SSOP0 & (1<<8) == (1<<8));
    data[2] = ADC1->SSFIFO0 & 0xfff;
    delay_ms(1);

    ADC0->ISC |= (1<<0);
    delay_ms(1);

    // --------------------------------selector 2
    ADC0->PSSI |= (1<<2);
    delay_ms(1);

    while (ADC0->RIS & 0x02 == 0);
    delay_ms(1);
    while(ADC0->SSOP2 & (1<<0) == (1<<0));
    data[3] = ADC0->SSFIFO2 & 0xfff;
    delay_ms(1);
    while(ADC0->SSOP2 & (1<<4) == (1<<4));
    data[4] = ADC0->SSFIFO2 & 0xfff;
    delay_ms(1);
    while(ADC0->SSOP2 & (1<<8) == (1<<8));
    data[5] = ADC0->SSFIFO2 & 0xfff;
    delay_ms(1);

    ADC0->ISC |= (1<<2);
    delay_ms(1);

    // --------------------------------selector 3
    ADC0->PSSI |= (1<<3);
    delay_ms(1);

    while (ADC0->RIS & 0x02 == 0);
    delay_ms(1);
    while(ADC0->SSOP3 & (1<<0) == (1<<0));
    data[6] = ADC0->SSFIFO3 & 0xfff;
    delay_ms(1);
    while(ADC0->SSOP3 & (1<<4) == (1<<4));
    data[7] = ADC0->SSFIFO3 & 0xfff;
    delay_ms(1);
    while(ADC0->SSOP3 & (1<<8) == (1<<8));
    data[8] = ADC0->SSFIFO3 & 0xfff;
    delay_ms(1);

    ADC0->ISC |= (1<<2);
    delay_ms(1);
}




//*/



extern void ADC0_InSeq3(uint16_t *Result){

    //ADC Processor Sample Sequence Initiate (ADCPSSI)
       ADC0->PSSI = 0x00000004;
       while((ADC0->RIS&0x04)==0){}; // espera al convertidor
       Result[1] = ADC0->SSFIFO3&0xFFF; //  Leer  el resultado almacenado en la pila2
       Result[0] = ADC0->SSFIFO3&0xFFF;
       printChar('A');
       ADC0->ISC = 0x0004;  //Conversion finalizada

}

extern void ADC0_InSeq2(uint16_t *Result){

    //ADC Processor Sample Sequence Initiate (ADCPSSI)
       ADC0->PSSI = 0x00000004;
       while((ADC0->RIS&0x04)==0){}; // espera al convertidor
       Result[1] = ADC0->SSFIFO2&0xFFF; //  Leer  el resultado almacenado en la pila2
       Result[0] = ADC0->SSFIFO2&0xFFF;
       printChar('A');
       ADC0->ISC = 0x0004;  //Conversion finalizada

}

extern void ADC1_InSeq0(uint16_t *Result){

    //ADC Processor Sample Sequence Initiate (ADCPSSI)
       ADC1->PSSI = 0x00000004;
       while((ADC1->RIS&0x04)==0){}; // espera al convertidor
       Result[1] = ADC1->SSFIFO0&0xFFF; //  Leer  el resultado almacenado en la pila2
       Result[0] = ADC1->SSFIFO0&0xFFF;
       printChar('A');
       ADC1->ISC = 0x0004;  //Conversion finalizada

}