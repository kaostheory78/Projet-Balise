/******************************************************************************/
/***************** CARTE BALISE : DSPIC33FJ128MC804 ***************************/
/******************************************************************************/
/* Fichier 	: pwm.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 08 novembre 2015, 15:30
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"

/******************************************************************************/
/**************************** Configurations Des PWM **************************/
/******************************************************************************/


void ConfigPWM (void)
{
	//****************
	//PWM
	//****************
	P1TCONbits.PTEN		= 1;		//PWM1 ON
    P2TCONbits.PTEN     = 0;        //PWM2 OFF

    P1TCONbits.PTSIDL   = 1;        //continu en idle
    P1TCONbits.PTOPS    = 0;        //Postscaller 1 : 1
    P1TCONbits.PTCKPS   = 0;        //Prescaller  1 : 1

	P1TCONbits.PTMOD	= 0;		//Base de temps en free running mode (11 bits vmax = 2048)

    P1TPER              = 1999;		//F=20kHz 11 bits

	PWM1CON1bits.PMOD1	= 1;		//Mode indépendant             //Mode complementaire sur pwm1L1
	PWM1CON1bits.PMOD2	= 1;		//Mode indépendant             //Mode complementaire sur pwm1L2
    PWM1CON1bits.PMOD3  = 1;        //Mode indépendant         

	PWM1CON1bits.PEN1L	= 1;		//PWM1L1 pour PWM moteur
	PWM1CON1bits.PEN1H	= 0;		//PWM1H1 inactif => I/O
#ifdef LED1_PWM
	PWM1CON1bits.PEN2H	= 1;		//PWM1H2 pour LED1
#else
    PWM1CON1bits.PEN2H	= 0;		//PWM1H2 inactif => I/O 
#endif
	PWM1CON1bits.PEN2L	= 0;		//PWM1L2 inactif => I/O
	PWM1CON1bits.PEN3H	= 0;		//PWM1H3 inactif => I/O
	PWM1CON1bits.PEN3L	= 0;		//PWM1L3 inactif => I/O

	PWM2CON1bits.PEN1H	= 0;		//PWM2H1 inactif => I/O
	PWM2CON1bits.PEN1L	= 0;		//PWM2L1 inactif => I/O


	// Mise a zero des PWM
    PDC1 = 0;
    PDC2 = 0;
}


/******************************************************************************/
/******************************** MODFIFS PWM *********************************/
/******************************************************************************/

void envoit_pwm (double valeur)
{
    double abs_valeur;

    if (valeur < 0.)
        abs_valeur = (double) (-valeur);
    else
      abs_valeur = (double) valeur;

    abs_valeur *= PWM_VALEUR_MAX / 100;
  
    if (abs_valeur > PWM_VALEUR_MAX)
        abs_valeur = PWM_VALEUR_MAX;

    if (valeur < (double) 0.) //>
    {
       SENS_MOTEUR = AVANCER_MOTEUR;
    }
    else
    {
        SENS_MOTEUR = RECULER_MOTEUR;
    } 

    PDC1 = (uint16_t) abs_valeur;
}

void controle_LED1 (uint16_t pourcentage)
{
    if (pourcentage > 100)
        pourcentage = 100;
    
    double ratio =  ( (double) pourcentage * (double) PWM_MAX_VALUE);
    ratio /= 100.;
    PDC2 = (uint16_t) ratio;
    //PDC2 = (uint16_t) pourcentage;
}


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
