/******************************************************************************/
/***************** CARTE BALISE : DSPIC33FJ128MC804 ***************************/
/******************************************************************************/
/* Fichier 	: codeurs.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 8 novembre 2015, 15:08
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"


    volatile int8_t OVERFLOW_CODEUR;
    volatile _position position ;


/******************************************************************************/
/*************************** Configurations des QEI ***************************/
/******************************************************************************/

void ConfigQEI (void)
{

	// QEI1CON
	QEI1CONbits.CNTERR	= 0;				// Reset flag erreur sur compteur
	QEI1CONbits.QEISIDL	= 0;				// Mode continu en idle
    QEI1CONbits.QEIM    = 0b111;            // Mode QEI avec X4
	QEI1CONbits.SWPAB	= 0;				// PA et PB non entrelacés
	QEI1CONbits.PCDOUT	= 1;				// Position counter dir. status output active (IO controle par QEI)

	// DFLT1CON
	DFLT1CONbits.IMV1	= 0;				// Etat de PB lors d'un INDEX
	DFLT1CONbits.IMV0	= 0;				// Etat de PA lors d'un INDEX
	DFLT1CONbits.CEID	= 0;				// Interruption sur erreur compteur desactive
	DFLT1CONbits.QEOUT	= 1;				// Activation des filtres
	DFLT1CONbits.QECK	= 0b010;			// Filtre / de 1:4

	MAX1CNT             = MAX_CODEUR;       // Limite du compteur codeur
}




/******************************************************************************/
/*************************** Utilisation des codeurs **************************/
/******************************************************************************/



void get_valeur_codeur ()
{
    static int32_t res = 0;
    static uint16_t max_codeur;
    int8_t etat_overflow, sens_rotation;


    etat_overflow = OVERFLOW_CODEUR;
    OVERFLOW_CODEUR = PAS_D_OVERFLOW_CODEUR;

    position.nouvelle = POS1CNT;
    max_codeur = MAX_CODEUR;
    sens_rotation = SENS_ROT;
    

    res = (int32_t)( (int32_t) position.nouvelle - (int32_t) position.ancien );
    if (etat_overflow != PAS_D_OVERFLOW_CODEUR)
    {
        res+= (int32_t) etat_overflow * max_codeur;
    }

    position.ecart = res * sens_rotation;
    position.ancien = position.nouvelle;
}


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/