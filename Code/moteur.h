/* 
 * File:   moteur.h
 * Author: Quentin
 *
 * Created on 8 avril 2015, 23:07
 */

#ifndef MOTEUR_H
#define	MOTEUR_H

#include "triangulation.h"


#ifdef	__cplusplus
extern "C" {
#endif

/******************************************************************************/
/***************************** Defines ****************************************/
/******************************************************************************/

    #define PWM_VALEUR_MAX                  669
    #define NOMBRE_DE_TICKS                 ((4 * 2000) - 1)

    #define PAS_D_OVERFLOW_CODEUR           0

/******************************************************************************/
/******************************* structures ***********************************/
/******************************************************************************/

//    typedef struct
//    {
//        uint16_t ancien;
//        uint16_t nouvelle;
//        int32_t ecart;
//        double angle;
//    }_position;

    typedef struct
    {
        bool tour_en_cours;
        bool synchro_debut_tour;
        bool flag_capteur;
        double angle[3];
        uint16_t position[3];
        uint8_t indice;
        _coordonees c;
        //int8_t id_balise_mere;
    }_capteur;

/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

    void config_QEI ();
    void get_valeur_codeur (void);
    void conversion_angle ();
    void init_capteur();
    double obtention_angle (uint16_t position);


#ifdef	__cplusplus
}
#endif

#endif	/* MOTEUR_H */

