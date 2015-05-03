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


/******************************************************************************/
/****************************** DEFINES GLOBALES ******************************/
/******************************************************************************/

#define SENS_HORAIRE            1
#define SENS_ANTI_HORAIRE       2

#define OK                      0
#define _ERREUR                 -1


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


void triangulation (double *x, double *y, double angle1, double angle2, double angle3);
double Cot (double angle);
double saturation (double valeur, double maximum);
double conversion_degre_radian (double angle);

double modulo_angle (double angle);
int8_t retrouver_balise_mere ();
int8_t check_divergence_angle ( double ancien_angle, double nouvel_angle );
void inversion_balises(uint8_t id_balise_mere);
void modulo_id_balise_mere ();
check_divergence_position (double ancien_x, double ancien_y, double x, double y);
void affichage_position (double x, double y);

#ifdef	__cplusplus
}
#endif

#endif	/* TRIANGULATION_H */

