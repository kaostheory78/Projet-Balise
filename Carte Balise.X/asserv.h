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
 * Asservissement qui permet d'asservir le robot � une position pr�cise
 */
void asserv_brake(void);

/**
 * FOnction qui arr�te imm�diatement le robot
 */
void brake();

/**
 * Fonction qui permet au robot de reprendre son d�placement (terminer sa fonction de d�plcaement) apr�s un brake
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
  * Fonction qui calcule la consigne de distance � partir de consigne en X et Y
  */
 void calcul_distance_consigne_XY (void);


 /**
  * Fonction qui calcul la vitesse du robot � atteindre
  * @param pourcentage_vitesse : pourcentage de la vitesse maximum
  */
 void calcul_vitesse_position (double pourcentage_vitesse);


 /**
  * Fonction qui calcul l'acc�l�ration pour g�n�rer les rampes
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
  * Fonction qui renvoit la coordonn�e x du robot en mm
  * @return : x en mm
  */
double get_X (void);


/**
 * Fonction qui renvoit la coordonn�e y du robot en mm
 * @return : y en mm
 */
double get_Y (void);


/**
 * Fonction qui renvoit l'orientation du robot en degr�
 * @return : angle en degr�
 */
double get_orientation (void);

/******************************************************************************/
/*********************** FONCTIONS INIT POSITIONS *****************************/
/******************************************************************************/

/**
 *  Fonction qui permet d'initialiser le robot � une position pr�cise (centre du robot)
 * @param x0 : en mm
 * @param y0 : en mm
 * @param teta0 : en degr�
 */
void init_position_robot (double x0, double y0, uint32_t teta0);


/**
 * Permet de r�initialiser la position x du robot (centre du robot)
 * @param x : position x en mm
 */
void init_X (double x);


/**
 * Permet de r�initialiser la position y du robot (centre du robot)
 * @param y : Coordon�e y en mm
 */
void init_Y (double y);


/**
 * Permet de r�initialiser l'orientation du robot par rapport au rep�re de d�part
 * \n
 * 0� = longueur du terrain\n
 * -90� = vers la porte du local
 *  90� = de l'autre c�t�
 * @param teta : angle en �
 */
void init_orientation (double teta);


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/**
 * Fonction qui permet de calculer la position du robot.
 * C'est la fonction ODOMETRIE.
 *
 * Elle est appel�e toutes les 5 ms par l'asserv.
 */
void calcul_position_robot (void);

/**
 * Fonction qui permet de finir un d�placement pr�matur�ment
 */
void fin_deplacement (void);

/**
 * Fontion qui incr�mente l'immobilit� en cas de blocage
 */
void detection_blocage (void);

/******************************************************************************/
/************************ FONCTIONS INITS DIVERSES ****************************/
/******************************************************************************/

/**
 * PErmet de r�initialiser les variables de commande moteur
 *
 * Important car on �crase pas la valeur � chaque it�ration mais ou lui rajoutes la nouvelle
 */
void init_commande_moteur(void);

/**
 * Fonction qui initialise les flag de l'asserv pour d�marer l'asserv
 */
void init_flag();

/**
 * Fonction qui �crete les commandes moteurs pour ne pas envoyer plus que la tension nominale des moteurs
 */
void ecretage_consignes(void);

/**
 * A utiliser pour remettre l'asserv ) 0 avant un nouveau d�placement
 */
void reinit_asserv(void);


/******************************************************************************/
/***************************** ASSERVISSEMENT *********************************/
/******************************************************************************/


/**
 * g�n�re les consignes qui permettent de g�nrer les rampes
 */
void asserv_distance(void);

/**
 * g�n�re les rampes de vitesse : acc�l�ration, d�c�l�ration, phase constante
 */
void asserv_vitesse_distance (void);

/**
 * g�n�re les consignes d'asserv en orientation
 */
void asserv_orientation (void);

/**
 * G�n�re les rampes de l'asserv en vitesse
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
 * Asserv position : renvoit la valeur de l'erreur multipli� par le PID
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

