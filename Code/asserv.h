/* 
 * File:   asserv.h
 * Author: Quentin
 *
 * Created on 12 avril 2015, 22:10
 */

#ifndef ASSERV_H
#define	ASSERV_H


#ifdef	__cplusplus
extern "C" {
#endif

///******************************************************************************/
///****************************** DEFINES GLOBALES ******************************/
///******************************************************************************/
//
////
////#define Pi                          3.14159265359
////
//#define TENSION_MOTEUR              12.
//#define TENSION_CARTE               12.
//#define CONSIGNE_MAX                (double) (TENSION_MOTEUR * 100. / TENSION_CARTE)
//
//#define MAX_ERREUR_INTEGRALLE       1000.
//
//#define VITESSE_KP                  0.
//#define VITESSE_KI                  10.
//#define VITESSE_KD                  0.
//
//#define VITESSE_CONSIGNE            0.00005
//
///******************************************************************************/
///****************************** REGLAGE DU PID ********************************/
///******************************************************************************/
//
//#define DEGRE_PAR_TICKS             (double) ( 360. / NOMBRE_DE_TICKS )
//#define TICKS_PAR_DEGRE             (double) ( NOMBRE_DE_TICKS / 360. )
//
///******************************************************************************/
///******************************************************************************/
///******************************************************************************/
//
//    typedef struct
//    {
//        double KP;
//        double KI;
//        double KD;
//    }_coef_PID;
//
//    typedef struct
//    {
//        double consigne;
//        double actuelle;
//    }_systeme_asserv;
//
//    typedef struct
//    {
//        double actuelle;
//        double integralle;
//        double precedente;
//    }_erreur;
//
///******************************************************************************/
///******************************************************************************/
///******************************************************************************/
//
//    void mesure_vitesse (void);
//    void asserv (void);
//    double fonction_PID (void);
//    void reglage_PID (void);
//    void saturation_erreur_integralle_vitesse (void);
//    void ecretage_consignes(double *commande);
//
//
///******************************************************************************/
///******************************************************************************/
///******************************************************************************/



#ifdef	__cplusplus
}
#endif

#endif	/* ASSERV_H */

