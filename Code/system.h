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
    #include <stdint.h>
    #include <stdlib.h>
    #include <math.h>


/******************************************************************************/
/***************************** Defines ****************************************/
/******************************************************************************/


    //Macro pour les tempos
#define FOSC             80017142LL      //80000000
#define FCY             (FOSC/2)
#define delay_us(x) __delay32(((x*FCY)/1000000L)) // delays x us
#define delay_ms(x) __delay32(((x*FCY)/1000L))      // delays x ms


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


/******************************************************************************/
/******************************* Interruptions  *******************************/
/******************************************************************************/


/******************************************************************************/
/******************************* Interruptions  *******************************/
/******************************************************************************/

    // Niveau de priorit�
#define PRIO_INTER_TIMER0               PRIORITE_HAUTE  //
#define PRIO_INTER_TIMER1               PRIORITE_BASSE  //
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
#define ACTIV_INTER_TIMER1              DESACTIVE   //
#define ACTIV_INTER_TIMER2              DESACTIVE   //
#define ACTIV_INTER_TIMER5              DESACTIVE   //

#define ACTIV_INTER_INT0                ACTIVE      //
#define ACTIV_INTER_INT1                DESACTIVE   //
#define ACTIV_INTER_INT2                DESACTIVE   //

#define ACTIV_INTER_QEI                 ACTIVE      //
#define ACTIV_INTER_QEI_SENS            ACTIVE      //

#define ACTIV_INTER_ADC                 DESACTIVE   //

#define ACTIV_INTER_RX                  DESACTIVE
#define ACTIV_INTER_TX                  DESACTIVE


/******************************************************************************/
/*************************** Variables Globales *******************************/
/******************************************************************************/


/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

    /**
     * Fonction qui initialise tous les syst�mes de la carte
     * Uart, Timer, PWM, mapping ....
     */
    void init_system (void);


    /**
     * Fonction qui r�fle l'horloge � 80Mhz
     */
    void init_clock(void);


    /**
     * Fonction qui configure toutes les interruptions
     * Ainsi que les priorit�s
     */
    void ConfigInterrupt (void);


    /**
     * Fonction qui configure toures les broches entr�es sorties
     */
    void ConfigPorts (void);
    


#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_H */

