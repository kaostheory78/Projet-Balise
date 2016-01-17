/******************************************************************************/
/***************** CARTE BALISE : DSPIC33FJ128MC804 ***************************/
/******************************************************************************/
/* Fichier 	: interruptions.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 08/11/2015
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"


/******************************************************************************/
/*********************** INTERRUPTION DES TIMERS ******************************/
/******************************************************************************/


/**
 *  Timer 5 ms : Asserv
 */
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
    FLAG_TIMER_5ms = 0;
    calcul_orientation();
    //PutIntUART(UART_USB, position.ecart);
    //PutcUART(UART_USB, '\r');
    asserv();
}

/**
 * Timer fin de match : 90 secondes
 */
void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void)
{
    FLAG_TIMER_90s = 0;        //On clear le flag d'interruption du timer
}

/**
 * Timer 10 ms : Autom et capteurs
 */
void __attribute__((__interrupt__, no_auto_psv)) _T4Interrupt(void)
{
    TIMER_10ms = DESACTIVE;

    TMR4 = 0;
    FLAG_TIMER_10ms = 0;        //On clear le flag d'interruption du timer
    TIMER_10ms = ACTIVE;
}

void __attribute__((__interrupt__, no_auto_psv)) _T5Interrupt(void)
{
    TIMER_DEBUG = DESACTIVE;
    
    
    PutIntUART(UART_USB, VITESSE.consigne);
    PutcUART(UART_USB, '\t');
    PutIntUART(UART_USB, VITESSE.actuelle);
    PutcUART(UART_USB, '\t');
    PutIntUART(UART_USB, ERREUR.actuelle);
    PutcUART(UART_USB, '\t');
    PutIntUART(UART_USB, ERREUR.integrale);
    PutcUART(UART_USB, '\t');
    PutIntUART(UART_USB, COMMANDE);
    PutcUART(UART_USB, '\n');
    PutcUART(UART_USB, '\r');
    
    
    
    

    FLAG_TIMER_DEBUG = 0;        //On clear le flag d'interruption du timer
    TIMER_DEBUG = ACTIVE;
}

/******************************************************************************/
/************************** INTERRUPTION DES QEI ******************************/
/******************************************************************************/

/**
 * Interruption sur codeur 
 */
void __attribute__((__interrupt__, no_auto_psv)) _QEI1Interrupt(void)
{
    IFS3bits.QEI1IF = 0;            //Clear du flag de l'event

    if (QEI1CONbits.UPDN == 1)      //codeur croit
        OVERFLOW_CODEUR++;
    else                            // codeur decroit
        OVERFLOW_CODEUR--;
    
    if (capteur.synchro_debut_tour == true)
    {
        capteur.tour_en_cours = true;
        capteur.synchro_debut_tour = false;
    }
    else
        capteur.tour_en_cours = false;

}


/******************************************************************************/
/************************** INTERRUPTION DES INT ******************************/
/******************************************************************************/

/**
 * Interruption sur capteur 
 */
void __attribute__((__interrupt__, no_auto_psv)) _INT1Interrupt(void)
{
    if (capteur.tour_en_cours == true)
    {
        if (capteur.indice < 3)
        {
            capteur.position[capteur.indice++] = POS1CNT;
        }
        else
            capteur.indice++;
    }
    IFS1bits.INT1IF = 0;            //Clear du flag de l'event;
}


/******************************************************************************/
/***************************** INTERRUPTIONS UART *****************************/
/******************************************************************************/

void __attribute__ ((interrupt, no_auto_psv)) 	_U1RXInterrupt (void)
{
	// Desactivation de l'interruption
	IEC0bits.U1RXIE	= 0;
	IFS0bits.U1RXIF = 0;

        uint8_t buf;
        static uint8_t value = 0;
        buf= U1RXREG;

        //PORTCbits.RC5 = value;

        if (value == 0)
            value = 1;
        else
            value = 0;
        
	// Traitement
	//char buff = U1RXREG;

	// Activation de l'interruption
	IEC0bits.U1RXIE	= 1;
}



void __attribute__ ((interrupt, no_auto_psv)) 	_U2RXInterrupt (void)
{
    IEC1bits.U2RXIE = 0;
    IFS1bits.U2RXIF = 0;

    IEC1bits.U2RXIE = 1;
}

void __attribute__ ((interrupt, no_auto_psv)) 	_U2TXInterrupt (void)
{
    IFS1bits.U2TXIF = 0; //remise du flag à 0 quand le buffer de transmission est vide
}




/******************************************************************************/
/******************************************************************************/
/******************************************************************************/