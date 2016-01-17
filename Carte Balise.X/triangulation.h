/* 
 * File:   triangulation.h
 * Author: Quentin
 *
 * Created on 28 avril 2015, 16:59
 */

#ifndef TRIANGULATION_H
#define	TRIANGULATION_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "system.h"

/******************************************************************************/
/****************************** DEFINES GLOBALES ******************************/
/******************************************************************************/

#define SENS_HORAIRE            1
#define SENS_ANTI_HORAIRE       2

#define OK                      0
#define _ERREUR                 -1


    //Coordonées des balises
#define X_BALISE_1              2062.
#define Y_BALISE_1              3062.

#define X_BALISE_2              -62.
#define Y_BALISE_2              3062.

#define X_BALISE_3              1000.
#define Y_BALISE_3              -62.

#define ECART_BALISE_X1_X2      (X_BALISE_1 - X_BALISE_2)
#define ECART_BALISE_Y1_Y2      (Y_BALISE_1 - Y_BALISE_2)

#define ECART_BALISE_X2_X3      (X_BALISE_3 - X_BALISE_2)
#define ECART_BALISE_Y2_Y3      (Y_BALISE_3 - Y_BALISE_2)


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

    typedef struct
{
    double x;
    double y;
}_coordonees;

typedef struct
{
    _coordonees _1_2;
    _coordonees _2_3;
    _coordonees _3_1;
}_cercle;

typedef struct
{
    _cercle cercle;
    double cotan_1_2;
    double cotan_2_3;
    double cotan_3_1;
    double D;
    double K;

}_triangulation;

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
/******************************************************************************/
/******************************************************************************/


void triangulation (_coordonees *c, double angle1, double angle2, double angle3);
double cotangeante (double angle);
double saturation (double valeur, double maximum);
double conversion_degre_radian (double angle);

void affichage_position ();
double get_distance (_coordonees ancien, _coordonees actu);
void init_capteur();
double obtention_angle (uint16_t position);
#ifdef	__cplusplus
}
#endif

#endif	/* TRIANGULATION_H */

