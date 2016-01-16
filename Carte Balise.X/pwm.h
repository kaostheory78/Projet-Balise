/* 
 * File:   pwm.h
 * Author: Quentin
 *
 * Created on 08 novembre 2015, 15:22
 */

#ifndef PWM_H
#define	PWM_H

#include "system.h"


#ifdef	__cplusplus
extern "C" {
#endif

    
/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "Config_robots.h"


/******************************************************************************/
/***************************** Defines ****************************************/
/******************************************************************************/

#define PWM_MAX_VALUE           2046

#define TENSION_MOTEUR          _TENSION_MOTEUR

#define PWM_VALEUR_MAX          (uint16_t) ( PWM_MAX_VALUE * TENSION_MOTEUR / TENSION_ALIM )

#define AVANCER_MOTEUR          _AVANCER_MOTEUR_D
#define RECULER_MOTEUR          _RECULER_MOTEUR_D

/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/


    void ConfigPWM (void);
    void envoit_pwm (char moteur, double valeur);

#ifdef CARTE_V2
    void alimenter_moteur_Y (uint8_t alim, int8_t sens_marche);
#endif


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

