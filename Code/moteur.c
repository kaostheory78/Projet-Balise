
/******************************************************************************/
/************************ Carte Balise - PIC18f4431 ***************************/
/******************************************************************************/
/* Fichier 	: moteur.c
 * Auteur  	: M1E11
 * Revision	: 1.0
 * Date		: 08 Avril 2015, 23:06
 *******************************************************************************
 *
 *
 ******************************************************************************/


#include "system.h"


_position position;
int8_t  OVERFLOW_CODEUR;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


void config_pwm ()
{
    PTCON0bits.PTOPS = 0;       // Postcale = 1
    PTCON0bits.PTCKPS = 0;   // Prescale 1:1
    PTCON0bits.PTMOD  = 0;      // free running mode

    PTCON1bits.PTEN = 1;        // PWM time base is ON

    PWMCON0bits.PMOD0 = 1;      // PWM0 et 1 indépendants
    PWMCON0bits.PWMEN = 0b001;  // Juste PWM1

    PWMCON1bits.UDIS = 0;       // MAJ duty cycle et period enable

    PTPERL = 166;               // F = 23,9 kHz
    PTPERH = 0;

    // Remise à zéro des pwm
    PDC0L = 0;
    PDC0H = 0;
    PDC1L = 0;
    PDC1H = 0;
    PDC2L = 0;
    PDC2H = 0;
    PDC3L = 0;
    PDC3H = 0;
}

void envoit_pwm (double valeur)
{
    double abs_valeur;

    if (valeur < 0)
        abs_valeur = (double) (-valeur);
    else
      abs_valeur = (double) valeur;

    abs_valeur *= (double) PWM_VALEUR_MAX / 100;

    if (abs_valeur > PWM_VALEUR_MAX)
        abs_valeur = PWM_VALEUR_MAX;

    if (valeur < (double) 0.) //>
    {
       SENS_MOTEUR = 1;
    }
    else
    {
        SENS_MOTEUR = 0;
    }

    PDC0L = (uint8_t) ((uint16_t) abs_valeur % 256);
    PDC0H = (uint8_t) ((uint16_t) abs_valeur / 256);

}

void config_QEI ()
{
    CAP1CONbits.CAP1REN = 1;
    ANSEL0 = 0;


    QEICONbits.VELM = 1;        // Velocity mode disable
    QEICONbits.QERR = 0;        // Clear flag overflow
    QEICONbits.QEIM = 0b110;    // QEI en x4, avec reset sur POSCNT = MAXCNT 110
    QEICONbits.PDEC = 0b01;     // velocity pulse reduction de 1:4



    // Noise filter :
    DFLTCONbits.FLT3EN = 1;     // Filtre sur QEB
    DFLTCONbits.FLT2EN = 1;     // Filtre sur QE1
    DFLTCONbits.FLTCK = 0b010;  // Filtre de 1:4

    POSCNTH = 0;
    POSCNTL = 0;

    MAXCNTH = (uint8_t) (NOMBRE_DE_TICKS / 256);
    MAXCNTL = (uint8_t) (NOMBRE_DE_TICKS % 256);

    position.ancien = 0;
    position.ecart = 0;
    position.nouvelle = 0;
}


void get_valeur_codeur (void)
{
    static int32_t res = 0;
    int etat_overflow;

    etat_overflow = OVERFLOW_CODEUR;
    OVERFLOW_CODEUR = PAS_D_OVERFLOW_CODEUR;

    position.nouvelle = - POSCNTH * 256 - POSCNTL;

    res = (int32_t)( (int32_t) position.nouvelle - (int32_t) position.ancien );
    if (etat_overflow != PAS_D_OVERFLOW_CODEUR)
    {
        res+= (int32_t) etat_overflow * (NOMBRE_DE_TICKS);
    }

    position.ecart = res;
    position.ancien = position.nouvelle;
}

void conversion_angle ()
{
    double angle;
    angle  = (double) (position.nouvelle) / 8000.;
    angle *= 360;
    position.angle = angle;
}
