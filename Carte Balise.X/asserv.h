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

#include "system.h"

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


#define KP_                          _KP
#define KI_                          _KI
#define KD_                          _KD

#define MAX_ERREUR_INTEGRALE       _MAX_ERREUR_INTEGRALE

/******************************************************************************/
/************************* CONSIGNES DIVERSES *********************************/
/******************************************************************************/

#define CONSIGNE_MAX                100L

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
        double integrale;
        double integrale_precedente;
        double precedente;
        double maximum;
    }_erreur;


/******************************************************************************/
/************************** FONCTIONS ECRETAGES *******************************/
/******************************************************************************/

    void ecretage_consignes(void);
    
    void saturation_erreur (void);
    
 /*****************************************************************************/
 /**************************** FONCTIONS GETTERS ******************************/
 /*****************************************************************************/

    void calcul_vitesse (double pourcentage_vitesse);
    void calcul_orientation (void); 
    
/**
 * Fonction qui renvoit l'orientation du robot en degré
 * @return : angle en degré
 */
double get_orientation (void);

/******************************************************************************/
/*********************** FONCTIONS INIT POSITIONS *****************************/
/******************************************************************************/
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

void init_position_codeur (void);

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
double fonction_PID ();


/**
 * inscrit les valeurs des KI, KP, KD dans leurs variables respectives.
 */
void reglage_PID (void);

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#endif	/* ASSERV_H */

