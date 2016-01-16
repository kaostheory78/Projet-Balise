/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: asserv.c
 * Auteur  	: Quentin
 * Revision	: 1.2
 * Date		: 08/11/2014
 *******************************************************************************
 *
 *
 ******************************************************************************/

#include "system.h"


/******************************************************************************/
/*************************** Variables Globales *******************************/
/******************************************************************************/

_robot ROBOT;

_PID PID;
double KP_hybride;
double VITESSE_MAX_ORIENTATION;
double VITESSE_MAX_POSITION;

_systeme_asserv X, Y;
_systeme_asserv BRAKE[2];
_systeme_asserv DISTANCE;
_systeme_asserv ORIENTATION;
_systeme_asserv VITESSE_ORIENTATION[3];
_systeme_asserv VITESSE[3];

_acc acc;

_erreur ERREUR_DISTANCE;
_erreur ERREUR_VITESSE[2];
_erreur ERREUR_ORIENTATION;
_erreur ERREUR_BRAKE[2];

_commande_moteur COMMANDE;

_flag_asserv FLAG_ASSERV;

uint8_t TYPE_CONSIGNE;

/******************************************************************************/
/***************************** Fonctions Inits ********************************/
/******************************************************************************/

void init_flag()
{
    FLAG_ASSERV.position = OFF;
    FLAG_ASSERV.vitesse = OFF;
    FLAG_ASSERV.orientation = OFF;
    FLAG_ASSERV.fin_deplacement = FIN_DEPLACEMENT;
    FLAG_ASSERV.brake = OFF;
    FLAG_ASSERV.erreur = DEPLACEMENT_NORMAL;
    
    FLAG_ASSERV.vitesse_fin_nulle = ON;
    FLAG_ASSERV.sens_deplacement = MARCHE_AVANT;

    reinit_asserv();
    reglage_PID();

    PORTCbits.RC5 = 1;
    FLAG_ASSERV.totale = ON;
}


void init_commande_moteur(void)
{
    COMMANDE = 0;
}

void reinit_asserv(void)
{
    ERREUR.actuelle = 0;
    ERREUR.integralle = 0;
    ERREUR.precedente = 0;

    VITESSE.actuelle = 0;
    VITESSE.consigne = 0;
    VITESSE.theorique = 0;

}

/******************************************************************************/
/***************************  Fonctions Getters  ******************************/
/******************************************************************************/

double get_orientation (void)
{
    return inversion_couleur(ROBOT.orientation_degre);
}



/******************************************************************************/
/************************** Fonctions Saturations *****************************/
/******************************************************************************/

/**
 * Ecretage des consignes maximum pour respecter la tention max des moteurs 
 */
void ecretage_consignes(void)
{
    if (COMMANDE > CONSIGNE_MAX)
        COMMANDE = CONSIGNE_MAX;
    else if (COMMANDE < - CONSIGNE_MAX)
        COMMANDE = -CONSIGNE_MAX;

}

void saturation_vitesse_max (unsigned char type)
{
    
        //Pour éviter d'osciller autour d'une valeur consigne en haut du trapèze
        //on écrète à la valeur max
        if (VITESSE[SYS_ROBOT].theorique > VITESSE_MAX_POSITION)
            VITESSE[SYS_ROBOT].theorique = VITESSE_MAX_POSITION;
        else if (VITESSE[SYS_ROBOT].theorique < - VITESSE_MAX_POSITION)
            VITESSE[SYS_ROBOT].theorique = - VITESSE_MAX_POSITION;

        //Pour ne pas osciller autour d'une consigne à 0,
        //On check, lors de la génération de la courbe de freinage,
        //Que si on va passer négatif alors on écrette à 0.
        else if (VITESSE[SYS_ROBOT].consigne == 0)
            if (abs(VITESSE[SYS_ROBOT].theorique) < acc.deceleration.position )
                VITESSE[SYS_ROBOT].theorique = 0;

}

void saturation_erreur (void)
{
     if (ERREUR.integralle > MAX_ERREUR_INTEGRALLE)
            ERREUR.integralle = MAX_ERREUR_INTEGRALLE;
     else if (ERREUR.integralle < - MAX_ERREUR_INTEGRALLE)
            ERREUR.integralle = - MAX_ERREUR_INTEGRALLE;
}

void detection_blocage (void)
{
    if (VITESSE[ROUE_DROITE].actuelle < 0.2 * (VITESSE[ROUE_DROITE].consigne * FLAG_ASSERV.sens_deplacement))
    {
        if (ERREUR_VITESSE[ROUE_DROITE].integralle == MAX_ERREUR_INTEGRALLE_V || ERREUR_VITESSE[ROUE_DROITE].integralle == - MAX_ERREUR_INTEGRALLE_V)
        {
            FLAG_ASSERV.immobilite++;
            FLAG_ASSERV.erreur = BLOCAGE;
        }
    }
    else
    {
        FLAG_ASSERV.immobilite = 0;
        FLAG_ASSERV.erreur = DEPLACEMENT_NORMAL;
    }
}

void calcul_vitesse (double pourcentage_vitesse)
{
    
}




/******************************************************************************/
/**************************** Fonctions Asserv ********************************/
/******************************************************************************/


//Fonction principale de l'asserv, qui permet d'activer les différents asserv
//D'envoyer la commande aux moteurs, et de sortir des fonctions de déplacements
void asserv()
{
    VITESSE.consigne = 0;

    //Fonction d'appel de l'asserv
    if ()
    {
           
        //Réinitialisation des commandes moteurs à 0
        init_commande_moteur();

        //Calcul des consignes moteurs gauche et droit, asserv indispendable
        fonction_PID(ASSERV_VITESSE_DISTANCE);

        
        ecretage_consignes();

        //envoit sur les moteurs
        envoit_pwm(COMMANDE);     
    }
    else
    {
        //Si aucun asserv, on bloque les moteurs à 0;
        envoit_pwm(0);
    }     
}


/******************************************************************************/
/************************************ PID *************************************/
/******************************************************************************/

void reglage_PID (void)
{
    
    PID.KP = _KP;
    PID.KI = _KI;
    PID.KD = _KD;
}

double fonction_PID (unsigned char type)
{
    ERREUR.actuelle = VITESSE.consigne - VITESSE.actuelle;
    ERREUR.integralle += ERREUR.actuelle;      

    saturation_erreur_integralle_vitesse();
    detection_blocage();

    COMMANDE.droit  = ERREUR.actuelle * PID.KP + ERREUR.integralle * PID.KI + (ERREUR.actuelle - ERREUR.precedente ) * PID.KD;

    ERREUR.precedente = ERREUR.actuelle;

    return (double) 0;
}

/******************************************************************************/
/****************************** ODOMETRIE *************************************/
/******************************************************************************/

void calcul_position_robot (void)
{
    static int32_t delta_o, delta_d;
    static double d_X = 0, d_Y = 0;

    get_valeur_codeur ();

}


