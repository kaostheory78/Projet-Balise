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
        static double x, y, ancien_x = 0, ancien_y = 0;
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
            inversion_balises(capteur.id_balise_mere);

            // On cherche quel angle et le plus proche du dernier angle de la balise mère
            if (retrouver_balise_mere() == OK)
            {
                triangulation(&x, &y, capteur.angle[0], capteur.angle[1], capteur.angle[2]);
                affichage_position(x, y);
                ancien_x = x;
                ancien_y = y;
            }
            // si on en trouve aucun, alors on refait les calculs de triangulations 
            else
            {
                double x_temp = 0, y_temp = 0;
                PutsUART(UART_BLUETOOTH, "\n\n\n\r BALISE MERE PERDUE !!!!!! \n\r");
                PutsUART(UART_BLUETOOTH, "Ancienne position connue : \n\r");
                affichage_position(ancien_x, ancien_y);

                triangulation(&x_temp, &y_temp, capteur.angle[0], capteur.angle[1], capteur.angle[2]);
                PutsUART(UART_BLUETOOTH, "\n\rTenative de base :  \n\r");
                affichage_position(x_temp, y_temp);

                // Si calcul de trianguation normal donne une position correcte
                if (check_divergence_position(ancien_x, ancien_y, x_temp, y_temp) == OK )
                {
                    PutsUART(UART_BLUETOOTH, "\n\n\n\r BALISE MERE INCHANGEE !!!!!! \n\r");
                    affichage_position(x_temp, y_temp);
                    ancien_x = x_temp;
                    ancien_y = y_temp;
                }
                else // Sinon on teste avec la balise 2 comme balise mère
                {
                    triangulation(&x_temp, &y_temp, capteur.angle[1], capteur.angle[2], capteur.angle[0]);
                    PutsUART(UART_BLUETOOTH, "\n\rTenative 1 :  \n\r");
                    affichage_position(x_temp, y_temp);
                    if (check_divergence_position(ancien_x, ancien_y, x_temp, y_temp) == OK )
                    {
                        PutsUART(UART_BLUETOOTH, "\n\n\n\r BALISE MERE RETROUVEE 1 !!!!!! \n\r");
                        affichage_position(x_temp, y_temp);
                        capteur.id_balise_mere ++;
                        modulo_id_balise_mere();
                        ancien_x = x_temp;
                        ancien_y = y_temp;
                    }
                    else // puis la balise 3
                    {
                        triangulation(&x_temp, &y_temp, capteur.angle[2], capteur.angle[0], capteur.angle[1]);
                        PutsUART(UART_BLUETOOTH, "\n\r Tenative 2 :  \n\r");
                        affichage_position(x_temp, y_temp);
                        if (check_divergence_position(ancien_x, ancien_y, x_temp, y_temp) == OK)
                        {
                            PutsUART(UART_BLUETOOTH, "\n\n\n\r BALISE MERE RETROUVEE 2 !!!!!! \n\r");
                            affichage_position(x_temp, y_temp);
                            capteur.id_balise_mere --;
                            modulo_id_balise_mere();
                            ancien_x = x_temp;
                            ancien_y = y_temp;
                        }
                        else
                            PutsUART(UART_BLUETOOTH, "\n\r/!\\/!\\ BALISE MERE NON RETROUVE  !!!!!! /!\\/!\\\n\r");
                    }

                }
            }
                

            for (i = 0 ; i < 3 ; i++)
                capteur.ancien_angle[i] = capteur.angle[i];
        }
    }

   /* ENABLE_BLUETOOTH = 0;

    PORTCbits.RC2 = 1;

    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    ENABLE_BLUETOOTH = 1;
    while(STATUS_BLUETOOTH == 0);
    LED0 = 0;
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);


    PutsUART(UART_BLUETOOTH, "Bonjour les amis !!! =)");


    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    

    traitement_reception();*/

    //envoit_pwm(50);
    while(1);
    


    return (EXIT_SUCCESS);
}

