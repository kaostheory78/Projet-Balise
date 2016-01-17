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

#define PWM_MAX_VALUE           4000u

#define TENSION_MOTEUR          _TENSION_MOTEUR
#define TENSION_ALIM            _TENSION_ALIM

#define PWM_VALEUR_MAX          (uint16_t) ( PWM_MAX_VALUE * TENSION_MOTEUR / TENSION_ALIM )

#define AVANCER_MOTEUR          _AVANCER_MOTEUR
#define RECULER_MOTEUR          _RECULER_MOTEUR

/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/


    void ConfigPWM (void);
    void envoit_pwm (double valeur);
    void controle_LED1 (uint16_t pourcentage);

#ifdef CARTE_V2
    void alimenter_moteur_Y (uint8_t alim, int8_t sens_marche);
#endif


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

