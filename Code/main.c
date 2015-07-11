/******************************************************************************/
/************************ Carte Balise - PIC18f4431 ***************************/
/******************************************************************************/
/* Fichier 	: main.c
 * Auteur  	: M1E11
 * Revision	: 1.0
 * Date		: 09 Mars 2014, 17:02
 *******************************************************************************
 *
 *
 ******************************************************************************/

#include "system.h"
#include "triangulation.h"

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1H
#pragma config OSC = HSPLL      // Oscillator Selection bits (HS oscillator, PLL enabled (clock frequency = 4 x FOSC1))
//#pragma config OSC = XT      // Oscillator Selection bits (HS oscillator, PLL enabled (clock frequency = 4 x FOSC1))
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config IESO = OFF        // Internal External Oscillator Switchover bit (Internal External Switchover mode enabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF       // Brown-out Reset Enable bits (Brown-out Reset enabled)
// BORV = No Setting

// CONFIG2H
#pragma config WDTEN = OFF       // Watchdog Timer Enable bit (WDT enabled)
#pragma config WDPS = 32768     // Watchdog Timer Postscale Select bits (1:32768)
#pragma config WINEN = OFF      // Watchdog Timer Window Enable bit (WDT window disabled)

// CONFIG3L
#pragma config PWMPIN = OFF     // PWM output pins Reset state control (PWM outputs disabled upon Reset (default))
#pragma config LPOL = HIGH      // Low-Side Transistors Polarity (PWM0, 2, 4 and 6 are active-high)
#pragma config HPOL = HIGH      // High-Side Transistors Polarity (PWM1, 3, 5 and 7 are active-high)
#pragma config T1OSCMX = OFF     // Timer1 Oscillator MUX (Low-power Timer1 operation when microcontroller is in Sleep mode)

// CONFIG3H
#pragma config FLTAMX = RC1     // FLTA MUX bit (FLTA input is multiplexed with RC1)
#pragma config SSPMX = RC7      // SSP I/O MUX bit (SCK/SCL clocks and SDA/SDI data are multiplexed with RC5 and RC4, respectively. SDO output is multiplexed with RC7.)
#pragma config PWM4MX = RB5     // PWM4 MUX bit (PWM4 output is multiplexed with RB5)
#pragma config EXCLKMX = RC3    // TMR0/T5CKI External clock MUX bit (TMR0/T5CKI external clock input is multiplexed with RC3)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (Enabled)

// CONFIG4L
#pragma config STVREN = OFF      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Low-Voltage ICSP Enable bit (Low-voltage ICSP enabled)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000200-000FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (001000-001FFF) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (002000-002FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (003000-003FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot Block (000000-0001FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000200-000FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (001000-001FFF) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (002000-002FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (003000-003FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0001FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000200-000FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (001000-001FFF) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (002000-002FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (003000-003FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0001FFh) not protected from table reads executed in other blocks)


int main(int argc, char** argv)
{

    init_system ();
    int i = 0;

    ENABLE_BLUETOOTH = 1;
    LATCbits.LATC2 = 1;
    LED0 = 1;

    PutsUART(UART_BLUETOOTH, "Bonjour les amis !!! =)");
    // Attente de connexion sur le bluetooth
    while(STATUS_BLUETOOTH == 0);
    LED0= 0;
    LED5 =1;
    envoit_pwm(60);

    //attente vitesse constante
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);

    while(1)
    {

        static bool led = true;
        led = !led;
        LED0 = led;
        static _coordonees c[3];
        static double distance[3];
        static bool premiere_fois = true;

        capteur.indice = 0;
        capteur.synchro_debut_tour = true;
        //on attends d'avoir fait un tour complet
        while(capteur.synchro_debut_tour == true);
        while(capteur.tour_en_cours == true);

        if (capteur.indice != 3)
        {
            PutsUART(UART_BLUETOOTH, "\n\nProbleme de detection de balises .... : ");
            PutIntUART(UART_BLUETOOTH, (uint16_t) capteur.indice);
            PutsUART(UART_BLUETOOTH, " Balises detectees \n\n\n\r");
        }
        else
        {
            for (i = 0; i < 3 ; i++)
                    capteur.angle[i] = obtention_angle(capteur.position[i]);

            // On remet les angles dans l'ordre en fonction du n° de l'angle correspondant à la balise mère au derneir calcul
            //inversion_balises(capteur.id_balise_mere);
            
            if (premiere_fois == true)
            {
                triangulation(&c[0], capteur.angle[0], capteur.angle[1], capteur.angle[2]);
                capteur.c.x = c[0].x;
                capteur.c.y = c[0].y;
                premiere_fois = false;
            }
            else
            {
                triangulation(&c[0], capteur.angle[0], capteur.angle[1], capteur.angle[2]);
                triangulation(&c[1], capteur.angle[1], capteur.angle[2], capteur.angle[0]);
                triangulation(&c[2], capteur.angle[2], capteur.angle[0], capteur.angle[1]);

                for (i = 0 ; i < 3 ; i++)
                    distance[i] = get_distance(capteur.c, c[i]);

                if (distance[0] < distance[1])
                {
                    if (distance[0] < distance[2])
                    {
                        capteur.c.x = c[0].x;
                        capteur.c.y = c[0].y;
                        // x[0] et y[0]
                    }
                    else
                    {
                        // x[2] et y [2]
                        capteur.c.x = c[2].x;
                        capteur.c.y = c[2].y;
                    }
                }
                else if (distance[1] < distance[2])
                {
                    // x[1] et y[1]
                    capteur.c.x = c[1].x;
                    capteur.c.y = c[1].y;
                }
                else
                {
                    // x[2] y[2]
                    capteur.c.x = c[2].x;
                    capteur.c.y = c[2].y;
                }

            }
            affichage_position();
        }
    }
    

    //traitement_reception();
    while(1);
    


    return (EXIT_SUCCESS);
}

