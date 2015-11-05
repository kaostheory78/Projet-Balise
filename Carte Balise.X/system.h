/* 
 * File:   system.h
 * Author: Quentin
 *
 * Created on 04 nobember 2015, 23:01
 */

#ifndef SYSTEM_H
#define	SYSTEM_H

#ifdef	__cplusplus
extern "C" {
#endif

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/
    #include <libpic30.h>
    #include <p33FJ128MC804.h>
    #include <stdint.h>
    #include <stdlib.h>
    #include <math.h>
    
/******************************************************************************/
/***************************** Defines ****************************************/
/******************************************************************************/


    //Macro pour les tempos
#define FOSC        80000000LL     //80017142LL      //80000000
#define FCY             (FOSC/2)
#define delay_us(x) __delay32(((x*FCY)/1000000L))   // delays x us
#define delay_ms(x) __delay32(((x*FCY)/1000L))      // delays x ms


/******************************************************************************/
/********************************* Timers *************************************/
/******************************************************************************/

#define ACTIVE                  0b1
#define DESACTIVE               0b0

#define DIGITAL                 1
#define ANALOGIQUE              0

#define TIMER_5ms               T1CONbits.TON
#define TIMER_10ms              T4CONbits.TON
#define TIMER_90s               T2CONbits.TON
#define TIMER_DEBUG             T5CONbits.TON

#define FLAG_TIMER_5ms          IFS0bits.T1IF
#define FLAG_TIMER_10ms         IFS1bits.T4IF
#define FLAG_TIMER_90s          IFS0bits.T3IF
#define FLAG_TIMER_DEBUG        IFS1bits.T5IF

/******************************************************************************/
/********************************  PORT CARTE   *******************************/
/******************************************************************************/

#define SYS_COULEUR             _SYS_COULEUR
#define SYS_STRAT               _SYS_STRAT

#define SENS_MOTEUR_DROIT   PORTBbits.RB11
#define SENS_MOTEUR_GAUCHE  PORTBbits.RB13

#define XBEE_RESET          PORTAbits.RA7
#define INHIBIT_AX12        PORTAbits.RA10
#define DIR_UART_AX12       PORTBbits.RB7


#define SYS_JACK            PORTBbits.RB4

//Du JACK  vers les codeurs
#define CAPTEUR1            PORTAbits.RA4
#define CAPTEUR2            PORTAbits.RA9
#define CAPTEUR3            PORTCbits.RC3
#define CAPTEUR4            PORTCbits.RC4

//Carte d'extenssion
//De la gauche vers la droite (nappe en bas)
#define CAPTEUR5            PORTAbits.RA8
#define CAPTEUR6            PORTAbits.RA3
#define CAPTEUR7            PORTAbits.RA2
#define CAPTEUR8            PORTCbits.RC1
#define CAPTEUR9            PORTCbits.RC2
#define CAPTEUR10           PORTCbits.RC0

/******************************************************************************/
/******************************* Interruptions  *******************************/
/******************************************************************************/

#define TENSION_MOTEUR_DROIT    _TENSION_MOTEUR_DROIT
#define TENSION_MOTEUR_GAUCHE   _TENSION_MOTEUR_GAUCHE

#define TENSION_SORTIE_PTN      _TENSION_SORTIE_PTN

/******************************************************************************/
/******************************* Interruptions  *******************************/
/******************************************************************************/

#define PRIO_INTER_TIMER1               5   // Timer Assev
#define PRIO_INTER_TIMER2               0   // Timer 2 en mode 32 bits (couplé à T3 donc osef)
#define PRIO_INTER_UART1_RX             1   // Prio XBEE
#define PRIO_INTER_TIMER3               6   // Timer Fin de match
#define PRIO_INTER_I2C_MAITRE           5   // I²C désactivé pour le moment
#define PRIO_INTER_I2C_ESCLAVE          4   // I²C désactivé pour le moment
#define PRIO_INTER_TIMER4               2   // Autom
#define PRIO_INTER_TIMER5               1   //Prio Timer debug
#define PRIO_INTER_UART2_TX             3   // AX12
#define PRIO_INTER_UART2_RX             3   // AX12
#define PRIO_INTER_QEI1                 7   // Codeurs : prio la plus haute
#define PRIO_INTER_QEI2                 7   // Codeurs : prio la plus haute

#define ACTIV_INTER_UART1_RX            0   // Uart XBEE
#define ACTIV_INTER_TIMER3              1   // Timer 90 secondes : fin de match
#define ACTIV_INTER_TIMER2              0   // Osef : TIMER 2 et 3 sur 32 bits
#define ACTIV_INTER_TIMER1              1   // Timer asserv : 5 ms
#define ACTIV_INTER_UART2_TX            1   // UART AX12 : Acquittement trame envoyée
#define ACTIV_INTER_UART2_RX            1   // UART AX12
#define ACTIV_INTER_TIMER4              1   // Timer Autom : 10 ms
#define ACTIV_INTER_TIMER5              1   // Timer debug : 200 ms
#define ACTIV_INTER_I2C_MAITRE          0   // Pas implémenté pour le moment
#define ACTIV_INTER_I2C_ESCLAVE         0   // Pas implémenté pour le moment
#define ACTIV_INTER_QEI1                1   // Codeurs
#define ACTIV_INTER_QEI2                1   // Codeurs


/******************************************************************************/
/*************************** Variables Globales *******************************/
/******************************************************************************/

    

/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

    void ConfigADC (void);

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


    /**
     * Configure le mappage des modules spéciaux : UART, QEI
     */
    void ConfigMapping (void);

    

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_H */

