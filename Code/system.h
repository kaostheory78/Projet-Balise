/* 
 * File:   system.h
 * Author: Quentin
 *
 * Created on 9 mars 2015, 17:01
 */

#ifndef SYSTEM_H
#define	SYSTEM_H

#ifdef	__cplusplus
extern "C" {
#endif

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

    #include <p18f4431.h>
    #include <xc.h>
    #include <stdio.h>
    #include <stdint.h>
    #include <stdlib.h>
    #include <math.h>
    #include "uart.h"
    #include <stdbool.h>
    #include "interruptions.h"


/******************************************************************************/
/***************************** Defines ****************************************/
/******************************************************************************/


    //Macro pour les tempos
#define FOSC             16000000LL
#define _XTAL_FREQ       16000000
#define FCY             (FOSC/4)
#define __delay_us(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000000.0)))
#define __delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000.0)))
//#define delay_us(x) __delay32(((x*FCY)/1000000L)) // delays x us
//#define delay_ms(x) __delay32(((x*FCY)/1000L))      // delays x ms


/******************************************************************************/
/********************************* Timers *************************************/
/******************************************************************************/

#define ACTIVE                  0b1
#define DESACTIVE               0b0

#define PRIORITE_HAUTE          0b1
#define PRIORITE_BASSE          0b0

/******************************************************************************/
/********************************  PORT CARTE   *******************************/
/******************************************************************************/

#define TX_LOGICIEL             PORTCbits.RC5

/******************************************************************************/
/******************************* Interruptions  *******************************/
/******************************************************************************/


/******************************************************************************/
/******************************* Interruptions  *******************************/
/******************************************************************************/

    // Niveau de priorité
#define PRIO_INTER_TIMER0               PRIORITE_HAUTE  //
#define PRIO_INTER_TIMER1               PRIORITE_HAUTE  //
#define PRIO_INTER_TIMER2               PRIORITE_BASSE  //
#define PRIO_INTER_TIMER5               PRIORITE_BASSE  //

#define PRIO_INTER_INT0                 PRIORITE_BASSE  //
#define PRIO_INTER_INT1                 PRIORITE_BASSE  //
#define PRIO_INTER_INT2                 PRIORITE_BASSE  //

#define PRIO_INTER_QEI                  PRIORITE_HAUTE  //
#define PRIO_INTER_QEI_SENS             PRIORITE_HAUTE  //

#define PRIO_INTER_ADC                  PRIORITE_BASSE  //

#define PRIO_INTER_RX                   PRIORITE_BASSE
#define PRIO_INTER_TX                   PRIORITE_BASSE

    // ACtivations des interruptions
#define ACTIV_INTER_TIMER0              ACTIVE      //
#define ACTIV_INTER_TIMER1              ACTIVE   //
#define ACTIV_INTER_TIMER2              DESACTIVE   //
#define ACTIV_INTER_TIMER5              DESACTIVE   //

#define ACTIV_INTER_INT0                ACTIVE      //
#define ACTIV_INTER_INT1                DESACTIVE   //
#define ACTIV_INTER_INT2                DESACTIVE   //

#define ACTIV_INTER_QEI                 ACTIVE      //
#define ACTIV_INTER_QEI_SENS            ACTIVE      //

#define ACTIV_INTER_ADC                 DESACTIVE   //

#define ACTIV_INTER_RX                  ACTIVE
#define ACTIV_INTER_TX                  DESACTIVE


    //Flag interruptions
#define FLAG_TIMER0                     INTCONbits.TMR0IF
#define FLAG_TIMER1                     PIR1bits.TMR1IF
#define FLAG_TIMER2                     PIR1bits.TMR2IF
#define FLAG_TIMER5                     PIR3bits.TMR5IF 

#define FLAG_INT0                       INTCONbits.INT0IF
#define FLAG_INT1                       INTCON3bits.INT1IF
#define FLAG_INT2                       INTCON3bits.INT2IF

#define FLAG_QEI                        PIR3bits.IC2QEIF
#define FLAG_QEI_SENS                   PIR3bits.IC3DRIF

#define FLAG_ADC                        PIR1bits.ADIF

#define FLAG_RX                         PIR1bits.RCIF
#define FLAG_TX                         PIR1bits.TXIF


/******************************************************************************/
/*************************** Variables Globales *******************************/
/******************************************************************************/

extern _buf uart_logiciel;

/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

    /**
     * Fonction qui initialise tous les systèmes de la carte
     * Uart, Timer, PWM, mapping ....
     */
    void init_system (void);


    /**
     * Fonction qui rèfle l'horloge à 80Mhz
     */
    void init_clock(void);


    /**
     * Fonction qui configure toutes les interruptions
     * Ainsi que les priorités
     */
    void ConfigInterrupt (void);


    /**
     * Fonction qui configure toures les broches entrées sorties
     */
    void ConfigPorts (void);


    void init_timer_0 ();
    void init_timer_1 ();


    /**************************************************************************/
    /**************************************************************************/
    /**************************************************************************/
    


#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_H */

