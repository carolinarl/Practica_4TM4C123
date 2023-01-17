// ESTE ES UN ARCHIVO FUENTE

#include "lib/include.h"
// ESTE PROGRAMA ES DECLARADO EN SU LIBRERÍA COMO PROTOTIPO DE FUNCIÓN

extern void Configurar_UART5(void)
{
    //-------------------------------------CONFIGURACIÓN DEL PIN

    SYSCTL->RCGCUART  = (1<<5);   //Paso 1 (RCGCUART) pag.344 UART/modulo5 0->Disable 1->Enable
    SYSCTL->RCGCGPIO |= (1<<4);     //Paso 2 (RCGCGPIO) pag.340 Enable clock port E
    
    GPIOE->LOCK= 0x4C4F434B;
    GPIOE->CR= 0xF0;
    
    //(GPIOAFSEL) pag.671 Enable alternate function
    //Pág. 895 para conocer los puertos asociados
    GPIOE->AFSEL = (1<<4) | (1<<5); //PE4 y PE5

    //GPIO Port Control (GPIOPCTL) PMC4-> U5Rx PMC5-> U5Tx pag.689
    // (1<<20) | (1<<16); (por el 1 en los pines 4 y 5, vía campo de bits)
    //0x00110000 en hexadecimal
    GPIOE->PCTL = (GPIOE->PCTL&0xFF00FFFF) | 0x00110000;
    
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOE->DEN = (1<<4) | (1<<5);//PE4 PE5 como pines digitales
    

    //----------------------------------HABILITACIÓN Y CONFIG. DEL PROTOCOLO DE COMUNICACIÓN

    //UART0 UART Control (UARTCTL) pag.918 DISABLE!!
    UART5->CTL = (0<<9) | (0<<8) | (0<<0);

    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    /*
    BRD = BRDI + BRDF = UARTSysClk / (ClkDiv * Baud Rate)
    BRD = 33,000,000 / (16 * 19200) = 107.421875
    
    UARTFBRD[DIVFRAC] = integer(BRDF * 64 + 0.5)
    UARTFBRD[DIVFRAC] = integer(0.421875 * 64 + 0.5) = 27.5
    */
    UART5->IBRD = 107;
    UART5->FBRD = 27;
    //  UART Line Control (UARTLCRH) pag.916
    UART5->LCRH = (0x3<<5)|(1<<4);
    //  UART Clock Configuration(UARTCC) pag.939
    UART5->CC =(0<<0);
    //UART0 UART Control (UARTCTL) pag.918 HABILITADO!!
    UART5->CTL = (1<<0) | (1<<8) | (1<<9);
    
    
}

extern char readChar(void)
{
    //UART FR flag pag 911
    //UART DR data 906
    char c;
    while((UART5->FR & (1<<4)) != 0 ); //Evaluación de memoria FIFO
    c = UART5->DR;
    return (unsigned char) c;
}


extern void printChar(char c)
{
    while((UART5->FR & (1<<5)) != 0 );
    UART5->DR = c;
}


extern void printString(char* string)
{
    while(*(string)){
       printChar(*(string++));
    }
}


extern int readString(char delimitador, char *string)
{
   int i = 0;
   char c = readChar();

   while(c != delimitador)
   { 
            string[i] = c; //Guardado de caracter por caracter en la cadena 
            i++;
            c = readChar(); 
   }

   return i;
}


extern void INVERSION(char *string, int i)
{
    char aux; 

    for (int j = 0; j < i/2; j++){
        aux = string[j];            //En la cadena "aux" se guardan los caracteres presentados del final al inicio de su envío
        string[j] = string[i-1- j]; 
        string[i-1-j] = aux; 
    }

}

extern void NUMEROS(char *string, char *string_num, int i)
{
    int l = 0;
    int p = 0; 
    while(l<(i*2)){ //i es el contador del vector cuyos espacios fueron duplicados
        string_num[l] = string[p]; //guardado de caracter por caracter en la nueva cadena
        l++;  //La cuenta para ir agregando los números tiene que ir de 2 en 2 caracteres
        string_num[l] = (unsigned char)(49+p); //inserción de los números
        p++; //Cuenta para agregar el siguiente caracter de la cadena que contiene únicamente letras a la que contiene letras y números
        l++;
    }
    
    string = string_num; // Retorno de una cadena con números y letras llamada "num" a "string", la cadena original
}


//Experimento 2
//El envio es su nombre  (rave) 
// invertirlo y regresarlo con numeros consecutivos
// entre letras (e1v2a3r) 