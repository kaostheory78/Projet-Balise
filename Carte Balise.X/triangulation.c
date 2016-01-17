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

volatile _capteur capteur;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

void triangulation (_coordonees *c, double angle1, double angle2, double angle3)
{
    _triangulation t;

    // Calcul des cotangeantes :
    t.cotan_1_2= cotangeante( angle2 - angle1 ) ;
    t.cotan_2_3 = cotangeante( angle3 - angle2 ) ;

    // Saturation des valeurs pour éviter les overflow
    t.cotan_1_2 = saturation( t.cotan_1_2 , COT_MAX ) ;
    t.cotan_2_3 = saturation( t.cotan_2_3 , COT_MAX ) ;

    t.cotan_3_1 = ( 1.0 - t.cotan_1_2 * t.cotan_2_3 ) / ( t.cotan_1_2 + t.cotan_2_3 ) ;
    t.cotan_3_1 = saturation( t.cotan_3_1 , COT_MAX ) ;

    // Calcul du centre du cercle circonscrit du triangle formé par la balise 1 et 2
    t.cercle._1_2.x = ECART_BALISE_X1_X2 + t.cotan_1_2 * ECART_BALISE_Y1_Y2 ;
    t.cercle._1_2.y = ECART_BALISE_Y1_Y2 - t.cotan_1_2 * ECART_BALISE_X1_X2 ;

    // Calcul du centre du cercle circonscrit du triangle formé par la balise 2 et 3
    t.cercle._2_3.x = ECART_BALISE_X2_X3 - t.cotan_2_3 * ECART_BALISE_Y2_Y3;
    t.cercle._2_3.y = ECART_BALISE_Y2_Y3 + t.cotan_2_3 * ECART_BALISE_X2_X3;

    // Calcul du centre du cercle circonscrit du triangle formé par la balise 3 et 1
    t.cercle._3_1.x = (ECART_BALISE_X2_X3 + ECART_BALISE_X1_X2) + t.cotan_3_1 * (ECART_BALISE_Y2_Y3 - ECART_BALISE_Y1_Y2);
    t.cercle._3_1.y = (ECART_BALISE_Y2_Y3 + ECART_BALISE_Y1_Y2) - t.cotan_3_1 * (ECART_BALISE_X2_X3 - ECART_BALISE_X1_X2);

    t.K = (ECART_BALISE_X2_X3 * ECART_BALISE_X1_X2) + (ECART_BALISE_Y2_Y3 * ECART_BALISE_Y1_Y2) + t.cotan_3_1 * ( (ECART_BALISE_Y2_Y3 * ECART_BALISE_X1_X2) - (ECART_BALISE_X2_X3 * ECART_BALISE_Y1_Y2));

    t.D = (t.cercle._1_2.x - t.cercle._2_3.x) * (t.cercle._2_3.y - t.cercle._3_1.y) - (t.cercle._2_3.x - t.cercle._3_1.x) * (t.cercle._1_2.y - t.cercle._2_3.y);
    t.K /= t.D;

    c->x = t.K * (t.cercle._1_2.y - t.cercle._2_3.y) + X_BALISE_2 ;
    c->y = t.K * (t.cercle._2_3.x - t.cercle._1_2.x) + Y_BALISE_2 ;
}

double cotangeante (double angle)
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


double get_distance (_coordonees ancien, _coordonees actu)
{
    double delta_x, delta_y, distance;

    delta_x = ancien.x - actu.x;
    delta_y = ancien.y - actu.y;
    distance = sqrt (delta_x * delta_x + delta_y * delta_y);

    return distance;
}


void affichage_position ()
{

    int i, j;
    static int compteur = 0;

    if (compteur == 2)
    {
        int _x = (int)(capteur.c.x / 100.);
        int _y = (int)(capteur.c.y / 100.);

        PutsUART(UART_BLUETOOTH, "X ");
        for (i = 0; i < 60; i++)
                PutsUART(UART_BLUETOOTH, "~");
        PutsUART(UART_BLUETOOTH, "\n\r");

        for (i = 0; i < 20; i++)
        {
                PutsUART(UART_BLUETOOTH, " |");
                for (j = 0; j < 30; j++)
                {
                        if (_x == i  && _y == j)
                                PutsUART(UART_BLUETOOTH, "O ");
                        else
                                PutsUART(UART_BLUETOOTH, "- ");
                }
                PutsUART(UART_BLUETOOTH, "|");
                if (i == 10)
                        PutsUART(UART_BLUETOOTH, "X");
                PutsUART(UART_BLUETOOTH, "\n\r");
        }

        PutsUART(UART_BLUETOOTH, "X ");
        for (i = 0; i < 60; i++)
                PutsUART(UART_BLUETOOTH, "~");
        PutsUART(UART_BLUETOOTH, "\n\r");


       PutIntUART(UART_BLUETOOTH, (int16_t) capteur.c.x);
        PutsUART(UART_BLUETOOTH, "\n\r");
        PutIntUART(UART_BLUETOOTH, (int16_t) capteur.c.y);
        PutsUART(UART_BLUETOOTH, "\n\r");

        compteur = 0;
    }

    compteur++;

    
}

double obtention_angle (uint16_t position)
{
    double angle;
    angle  = (double) ((position) / 7999.);
    angle *= 360;
    return angle;
}

void init_capteur()
{
    capteur.angle[0] = 0.;
    capteur.angle[1] = 0.;
    capteur.angle[2] = 0.;
    capteur.flag_capteur = false;
    capteur.indice = 0;
    capteur.position[0] = 0;
    capteur.position[1] = 0;
    capteur.position[2] = 0;
    capteur.synchro_debut_tour = false;
    capteur.tour_en_cours = false;

    //_c.x = 0;
    //_c.y = 0;

}