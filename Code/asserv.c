
/******************************************************************************/
/************************ Carte Balise - PIC18f4431 ***************************/
/******************************************************************************/
/* Fichier 	: asserv.c
 * Auteur  	: M1E11
 * Revision	: 1.0
 * Date		: 12 Avril 2015, 21:55
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************* INCLUDES *************************************/
/******************************************************************************/

#include "system.h"

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

_coef_PID PID;
_systeme_asserv VITESSE;
_erreur ERREUR_VITESSE;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

void init_angle (double angle)
{
    Nop();
}

/******************************************************************************/
/************************** Fonctions Saturations *****************************/
/******************************************************************************/

/**
 * Ecretage des consignes maximum pour respecter la tention max des moteurs
 */
void ecretage_consignes(double *commande)
{
    if (*commande > CONSIGNE_MAX)
        *commande = CONSIGNE_MAX;
    else if (*commande < - CONSIGNE_MAX)
        *commande = - CONSIGNE_MAX;
}

void saturation_erreur_integralle_vitesse (void)
{
     if (ERREUR_VITESSE.integralle > MAX_ERREUR_INTEGRALLE)
            ERREUR_VITESSE.integralle = MAX_ERREUR_INTEGRALLE;
     else if (ERREUR_VITESSE.integralle < - MAX_ERREUR_INTEGRALLE)
            ERREUR_VITESSE.integralle = - MAX_ERREUR_INTEGRALLE;
}

/******************************************************************************/
/************************************ PID *************************************/
/******************************************************************************/

void reglage_PID (void)
{
    //PID VITESSE DISTANCE
    PID.KP = VITESSE_KP;
    PID.KI = VITESSE_KI;
    PID.KD = VITESSE_KD;
}

double fonction_PID (void)
{
    static double commande;
    ERREUR_VITESSE.actuelle = VITESSE.consigne - VITESSE.actuelle;
    ERREUR_VITESSE.integralle += ERREUR_VITESSE.actuelle;

    saturation_erreur_integralle_vitesse();
    //detection_blocage();

    commande  = ERREUR_VITESSE.actuelle * PID.KP + ERREUR_VITESSE.integralle * PID.KI + (ERREUR_VITESSE.actuelle - ERREUR_VITESSE.precedente ) * PID.KD;

    ERREUR_VITESSE.precedente = ERREUR_VITESSE.actuelle;

    return commande;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

void asserv (void)
{
    static double commande = 0;

    VITESSE.consigne = VITESSE_CONSIGNE;

    commande = fonction_PID();
    ecretage_consignes(&commande);
    envoit_pwm(commande);
}


void mesure_vitesse (void)
{
    get_valeur_codeur();

    VITESSE.actuelle = position.ecart;
    VITESSE.actuelle /= (NOMBRE_DE_TICKS ); //Tour par secondes
    VITESSE.actuelle *= 1000; //Tour par secondes
    conversion_angle();
}