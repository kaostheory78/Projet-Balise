/******************************************************************************/
/************************ Carte Balise - PIC18f4431 ***************************/
/******************************************************************************/
/* Fichier 	: triangulation.c
 * Auteur  	: M1E11
 * Revision	: 1.0
 * Date		: 28 Avril 2015, 16:21
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************* INCLUDES *************************************/
/******************************************************************************/

#include "system.h"
#define COT_MAX 100000000

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

void triangulation (double *x, double *y, double angle1, double angle2, double angle3)
{
    const double x1 = 2062., y1 = 3062., x2 = -62., y2 = 3062., x3 = 1000., y3 = -62.;

    double cot_12 = Cot( angle2 - angle1 ) ;
    double cot_23 = Cot( angle3 - angle2 ) ;
    cot_12 = saturation( cot_12 , COT_MAX ) ;
    cot_23 = saturation( cot_23 , COT_MAX ) ;
    double cot_31 = ( 1.0 - cot_12 * cot_23 ) / ( cot_12 + cot_23 ) ;
    cot_31 = saturation( cot_31 , COT_MAX ) ;
    double x1_ = x1 - x2 , y1_ = y1 - y2 , x3_ = x3 - x2 , y3_ = y3 - y2 ;
    double c12x = x1_ + cot_12 * y1_ ;
    double c12y = y1_ - cot_12 * x1_ ;

    double c23x = x3_ - cot_23 * y3_ ;
    double c23y = y3_ + cot_23 * x3_ ;

    double c31x = (x3_ + x1_) + cot_31 * (y3_ - y1_) ;
    double c31y = (y3_ + y1_) - cot_31 * (x3_ - x1_) ;
    double k31 = (x3_ * x1_) + (y3_ * y1_) + cot_31 * ( (y3_ * x1_) - (x3_ * y1_) ) ;

    double D = (c12x - c23x) * (c23y - c31y) - (c23x - c31x) * (c12y - c23y) ;
    double invD = 1.0 / D ;
    double K = k31 * invD ;

    *x = K * (c12y - c23y) + x2 ;
    *y = K * (c23x - c12x) + y2 ;
    //return invD ; /* return 1/D */
}

double Cot (double angle)
{
    angle = conversion_degre_radian(angle);
    return (1 / tan(angle));
}


double saturation (double valeur, double maximum)
{
    if (valeur > maximum)
        valeur = maximum;
    else if (valeur < -maximum)
        valeur = -maximum;
    return valeur;
}

double conversion_degre_radian (double angle)
{
    return ((angle / 180.) * 3.14159265);
}

int8_t check_divergence_angle ( double ancien_angle, double nouvel_angle )
{
    double derive = modulo_angle(nouvel_angle - ancien_angle);

    if (derive < -20. && derive < 0.)
    {
        return -1;
    }
    else if (derive > 20. && derive > 0.)
    {
        return 1;
    }
    else
        return OK;

}

check_divergence_position (double ancien_x, double ancien_y, double x, double y)
{
    double delta_x, delta_y, distance;

    delta_x = ancien_x - x;
    delta_y = ancien_y - y;
    distance = sqrt (delta_x * delta_x + delta_y * delta_y);

    if (distance < 200.)
        return OK;
    else
        return -1;
}

int8_t retrouver_balise_mere ()
{
    if (check_divergence_angle(capteur.ancien_angle[0], capteur.angle[0]) != OK)
    {
        if (check_divergence_angle(capteur.ancien_angle[0], capteur.angle[1]) == OK)
        {
            capteur.id_balise_mere ++;
            modulo_id_balise_mere();
            inversion_balises(1);

            PutsUART(UART_BLUETOOTH, "\n\r ROTATION A GAUCHE \n\r");
            return OK;

        }
        else if (check_divergence_angle(capteur.ancien_angle[0], capteur.angle[2]) == OK)
        {
            capteur.id_balise_mere --;
            modulo_id_balise_mere();
            inversion_balises(2);
            PutsUART(UART_BLUETOOTH, "\n\r ROTATION A DROITE \n\r");
            return OK;
        }
        else
            return _ERREUR;
    }
    else
        return OK;
}

double modulo_angle (double angle)
{
    if (angle < -180.)
        angle+= 360.;
    else if (angle > 180.)
        angle -= 360.;

    return angle;
}

void inversion_balises(uint8_t id_balise_mere)
{
    double temp;
    if (id_balise_mere == 1)
    {
        temp = capteur.angle[0];
        capteur.angle[0] = capteur.angle[1];
        capteur.angle[1] = capteur.angle[2];
        capteur.angle[2] = temp;
    }
    else if (id_balise_mere == 2)
    {
        temp = capteur.angle[0];
        capteur.angle[0] = capteur.angle[2];
        capteur.angle[2] = capteur.angle[1];
        capteur.angle[1] = temp;
    }
}

void modulo_id_balise_mere ()
{
    if (capteur.id_balise_mere == 3)
        capteur.id_balise_mere = 0;
    else if (capteur.id_balise_mere == -1)
        capteur.id_balise_mere = 2;
}

void affichage_position (double x, double y)
{
    PutsUART(UART_BLUETOOTH, "Position : X : ");
    PutIntUART(UART_BLUETOOTH, (int16_t) x);
    PutsUART(UART_BLUETOOTH, " Y : ");
    PutIntUART(UART_BLUETOOTH, (int16_t) y);
    PutsUART(UART_BLUETOOTH, "\r");
}