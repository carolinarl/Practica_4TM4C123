/*
 * ADC.h
 *
 *  Created on: 04/11/2019
 *      Author: ravenelco
 */

#ifndef ADC_ADC_H_
#define ADC_ADC_H_

extern void Configuracion_ADC0(void);
extern void ADC0_InSeq1(uint16_t *Result);
extern void ADC0_InSeq3(uint16_t *Result);
extern void ADC1_InSeq3(uint16_t *Result);

#endif /* ADC_ADC_H_ */
