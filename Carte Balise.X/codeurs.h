/* 
 * File:   codeurs.h
 * Author: Quentin
 *
 * Created on 8 novembre 2015, 15:09
 */

#ifndef CODEURS_H
#define	CODEURS_H

#ifdef	__cplusplus
extern "C" {
#endif

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"

/******************************************************************************/
/***************************** Defines ****************************************/
/******************************************************************************/


#define SENS_ROT                        _SENS_ROT
#define CODEUR_NB_IMP                   _CODEUR_NB_IMP
#define RAPPORT_REDUCTION               _RAPPORT_REDUCTION
    
#define MAX_CODEUR                      (4 * CODEUR_NB_IMP * RAPPORT_REDUCTION - 1)

#define PAS_D_OVERFLOW_CODEUR           0

#define STANDART                        0
#define INTER_CODEUR_OVERFLOW           1
#define INTER_CODEUR_UNDERFLOW          2

    typedef struct _codeur
    {
        uint16_t ancien;
        uint16_t nouvelle;
        int32_t ecart;
    }_position;


    
/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

    /**
     * Configuration des modules codeurs de la carte
     */
     void ConfigQEI (void);


     /**
      *
      * Fonction qui renvoit la valeur totale des codeurs
      *
      * @param codeur : codeur à étudier
      * @return : valeur totale des codeurs
      */
     void get_valeur_codeur (void);


     void debug(void);

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


#ifdef	__cplusplus
}
#endif

#endif	/* CODEURS_H */

