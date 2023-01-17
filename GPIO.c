#include "lib/include.h"
extern void Configurar_GPIO(void)
{
  // MODIFICACIÓN POR ESTRUCTURA DE CMSIS (Estándar de interfaz de software del microcontrolador ARM-Cortex)
    SYSCTL->RCGCGPIO |=(1<<5); // 1) Activación del reloj
    GPIOF->LOCK =  0x4C4F434B; // 2) En la tiva chica, se tienen que desbloquear los puertos F y D
    GPIOF->CR = (1<<4) | (1<<3) | (1<<2) | (1<<1) | (1<<0); 
    // GPIOF->CR = 0x1F; // Permite cambios del puerto PF4 al 0.
    GPIOF->AMSEL = 0x00;        // 3) disable analog on PF
    GPIOF->PCTL = 0x00000000;   // 4) PCTL GPIO on PF4-0
    GPIOF->DIR = 0x0E; // 5) PF4,PF0 in, PF3-1 out
    GPIOF->AFSEL = 0x00; // 6) Deshabilitación de funciones alternativas en PF7-0
    // Se deshabilita el pin como cualquier otro módulo (ej. PWM, analógico, UART, transferencia de datos SPI, etc.)
    // El pin únicamente se trabaja como digital.
    GPIOF->PUR = 0x11; // enable pull-up on PF0 and PF4
    GPIOF->DEN = 0x1F;  // 7) enable digital I/O on PF4-0

    //DIR Y DEN para entradas y salidas.
}
