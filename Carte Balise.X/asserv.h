/* 
 * File:   asserv.h
 * Author: Quentin
 *
 * Created on 08 novembre 2015 15:47
 */

#ifndef ASSERV_H
#define	ASSERV_H


/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "codeurs.h"

/******************************************************************************/
/****************************** DEFINES GLOBALES ******************************/
/******************************************************************************/

#define ON                          1
#define OFF                         2

#define Pi                          3.14159265359

#define MARCHE_AVANT                1
#define MARCHE_ARRIERE              -1

/******************************************************************************/
/****************************** REGLAGE DU PID ********************************/
/******************************************************************************/


#define KP                          _KP
#define KI                          _KI
#define KD                          _KD

#define MAX_ERREUR_INTEGRALLE       _MAX_ERREUR_INTEGRALLE

/******************************************************************************/
/************************* CONSIGNES DIVERSES *********************************/
/******************************************************************************/

#define CONSIGNE_MAX                100L

/******************************************************************************/
/************************ DEFINES DES ERREURS *********************************/
/******************************************************************************/

#define DEPLACEMENT_NORMAL          0
#define BLOCAGE                     1
#define EVITEMENT                   2

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
  
    typedef struct
    {
        double KP;
        double KI;
        double KD;
    }_PID;

    typedef struct
    {
        double consigne;
        double theorique;
        double actuelle;
    }_systeme_asserv;

    typedef struct
    {
        double actuelle;
        double integralle;
        double integralle_precedente;
        double precedente;
        double maximum;
    }_erreur;


/******************************************************************************/
/******************* FONCTIONS ASSERVISSEMENT BRAKE ****************************/
/******************************************************************************/    

/**
 * Asservissement qui permet d'asservir le robot à une position précise
 */
void asserv_brake(void);

/**
 * FOnction qui arrête immédiatement le robot
 */
void brake();

/**
 * Fonction qui permet au robot de reprendre son déplacement (terminer sa fonction de déplcaement) après un brake
 */
void unbrake (void);


/******************************************************************************/
/************************** FONCTIONS ECRETAGES *******************************/
/******************************************************************************/

/**
 *
 * @param type
 */
 void saturation_vitesse_max (unsigned char type);


 /**
  * 
  */
 void saturation_erreur_integralle_vitesse (void);

 /*****************************************************************************/
 /************************* FONCTIONS DE CALCULS ******************************/
 /*****************************************************************************/

 /**
  * Fonction qui calcule la consigne de distance à partir de consigne en X et Y
  */
 void calcul_distance_consigne_XY (void);


 /**
  * Fonction qui calcul la vitesse du robot à atteindre
  * @param pourcentage_vitesse : pourcentage de la vitesse maximum
  */
 void calcul_vitesse_position (double pourcentage_vitesse);


 /**
  * Fonction qui calcul l'accélération pour générer les rampes
  */
 void calcul_acceleration_position (void);


 /**
  *
  * @param pourcentage_vitesse
  */
 void calcul_vitesse_orientation (double pourcentage_vitesse);


 /**
  *
  */
 void calcul_acceleration_orientation (void);

 /*****************************************************************************/
 /**************************** FONCTIONS GETTERS ******************************/
 /*****************************************************************************/

 /**
  * Fonction qui renvoit la coordonnée x du robot en mm
  * @return : x en mm
  */
double get_X (void);


/**
 * Fonction qui renvoit la coordonnée y du robot en mm
 * @return : y en mm
 */
double get_Y (void);


/**
 * Fonction qui renvoit l'orientation du robot en degré
 * @return : angle en degré
 */
double get_orientation (void);

/******************************************************************************/
/*********************** FONCTIONS INIT POSITIONS *****************************/
/******************************************************************************/

/**
 *  Fonction qui permet d'initialiser le robot à une position précise (centre du robot)
 * @param x0 : en mm
 * @param y0 : en mm
 * @param teta0 : en degré
 */
void init_position_robot (double x0, double y0, uint32_t teta0);


/**
 * Permet de réinitialiser la position x du robot (centre du robot)
 * @param x : position x en mm
 */
void init_X (double x);


/**
 * Permet de réinitialiser la position y du robot (centre du robot)
 * @param y : Coordonée y en mm
 */
void init_Y (double y);


/**
 * Permet de réinitialiser l'orientation du robot par rapport au repère de départ
 * \n
 * 0° = longueur du terrain\n
 * -90° = vers la porte du local
 *  90° = de l'autre côté
 * @param teta : angle en °
 */
void init_orientation (double teta);


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/**
 * Fonction qui permet de calculer la position du robot.
 * C'est la fonction ODOMETRIE.
 *
 * Elle est appelée toutes les 5 ms par l'asserv.
 */
void calcul_position_robot (void);

/**
 * Fonction qui permet de finir un déplacement prématurément
 */
void fin_deplacement (void);

/**
 * Fontion qui incrémente l'immobilité en cas de blocage
 */
void detection_blocage (void);

/******************************************************************************/
/************************ FONCTIONS INITS DIVERSES ****************************/
/******************************************************************************/

/**
 * PErmet de réinitialiser les variables de commande moteur
 *
 * Important car on écrase pas la valeur à chaque itération mais ou lui rajoutes la nouvelle
 */
void init_commande_moteur(void);

/**
 * Fonction qui initialise les flag de l'asserv pour démarer l'asserv
 */
void init_flag();

/**
 * Fonction qui écrete les commandes moteurs pour ne pas envoyer plus que la tension nominale des moteurs
 */
void ecretage_consignes(void);

/**
 * A utiliser pour remettre l'asserv ) 0 avant un nouveau déplacement
 */
void reinit_asserv(void);


/******************************************************************************/
/***************************** ASSERVISSEMENT *********************************/
/******************************************************************************/


/**
 * génère les consignes qui permettent de génrer les rampes
 */
void asserv_distance(void);

/**
 * génère les rampes de vitesse : accélération, décélération, phase constante
 */
void asserv_vitesse_distance (void);

/**
 * génère les consignes d'asserv en orientation
 */
void asserv_orientation (void);

/**
 * Génère les rampes de l'asserv en vitesse
 */
void asserv_vitesse_orientation (void);

/**
 * permet de lancer toutes les fonctions d'asservissement !!
 */
void asserv();

/******************************************************************************/
/******************************* FONCTIONS PID ********************************/
/******************************************************************************/

/**
 * Fonction qui calcul le PID
 *
 * Asserv position : renvoit la valeur de l'erreur multiplié par le PID
 *
 * Asserv de vitesse_distance : applique la commande moteur de la roue droite et gauche
 *
 * @param type : #define ASSERV_VITESSE_DISTANCE, ASSERV_POSITION, ASSERV_ORIENTATION
 * @return
 */
double fonction_PID (unsigned char type);


/**
 * inscrit les valeurs des KI, KP, KD dans leurs variables respectives.
 */
void reglage_PID (void);

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#endif	/* ASSERV_H */

