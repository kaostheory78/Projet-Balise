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

volatile _PID PID;
volatile _systeme_asserv VITESSE;
volatile _erreur ERREUR;
volatile double COMMANDE;
volatile double ORIENTATION;

/******************************************************************************/
/***************************** Fonctions Inits ********************************/
/******************************************************************************/

void init_flag()
{
    reinit_asserv();
    reglage_PID();
}


void init_commande_moteur(void)
{
    COMMANDE = 0;
}

void init_position_codeur (void)
{
    position.ancien = 0;
    position.ecart = 0;
    position.ancien = 0;
    ORIENTATION = 0;
}

void reinit_asserv(void)
{
    ERREUR.actuelle = 0;
    ERREUR.integrale = 0;
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
    //return ROBOT.orientation_degre;
    return 0;
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

void saturation_erreur (void)
{
     if (ERREUR.integrale > MAX_ERREUR_INTEGRALE)
            ERREUR.integrale = MAX_ERREUR_INTEGRALE;
     else if (ERREUR.integrale < - MAX_ERREUR_INTEGRALE)
            ERREUR.integrale = - MAX_ERREUR_INTEGRALE;
}

/*void detection_blocage (void)
{
    if (VITESSE.actuelle < 0.2 * (VITESSE.consigne * FLAG_ASSERV.sens_deplacement))
    {
        if (ERREUR.integrale == MAX_ERREUR_INTEGRALE || ERREUR.integrale == - MAX_ERREUR_INTEGRALE)
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
}*/

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
    //VITESSE.consigne = 0;

    //Fonction d'appel de l'asserv
    //if (0)
    {
           
        //Réinitialisation des commandes moteurs à 0
        init_commande_moteur();

        //Calcul des consignes moteurs gauche et droit, asserv indispendable
        fonction_PID();  
        ecretage_consignes();

        //envoit sur les moteurs
        envoit_pwm(COMMANDE);     
    }
//    else
//    {
//        //Si aucun asserv, on bloque les moteurs à 0;
//        envoit_pwm(0);
//    }     
}


/******************************************************************************/
/************************************ PID *************************************/
/******************************************************************************/

void reglage_PID (void)
{
    
    PID.KP = KP_;
    PID.KI = KI_;
    PID.KD = KD_;
}

double fonction_PID ()
{
    ERREUR.actuelle = VITESSE.consigne - VITESSE.actuelle;
    ERREUR.integrale += ERREUR.actuelle;      

    saturation_erreur();
    //detection_blocage();

    COMMANDE = ERREUR.actuelle * PID.KP + ERREUR.integrale * PID.KI + (ERREUR.actuelle - ERREUR.precedente ) * PID.KD;

    ERREUR.precedente = ERREUR.actuelle;

    return (double) 0;
}


void calcul_orientation (void)
{
    get_valeur_codeur();
    
    VITESSE.actuelle = (double) position.ecart;
    ORIENTATION = (double) position.nouvelle * 360. / MAX_CODEUR;
}

