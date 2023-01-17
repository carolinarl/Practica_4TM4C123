#ifndef UART_UART_H_
#define UART_UART_H_

extern void Configurar_UART5(void); //LLAMADA A PROGRAMA FUENTE. MÉTODO O FUNCIÓN DE RETARDO.
extern char readChar(void);
extern void printChar(char c);
extern void printString(char* string);
extern int readString(char delimitador, char *string);
extern void INVERSION(char *string, int i);
extern void NUMEROS(char *string, char *string_num, int i);


#endif /* UART_UART_H_ */