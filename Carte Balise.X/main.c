/******************************************************************************/
/**************** Carte Balise : DSPIC33FJ128MC804 ****************************/
/******************************************************************************/
/* Fichier 	: main.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 04 novembre 2015, 23:08
 *******************************************************************************
 *
 *
 ******************************************************************************/

#include "system.h"


/******************************************************************************/
/************************ Configurations Bits *********************************/
/******************************************************************************/

// DSPIC33FJ128MC804 Configuration Bit Settings

// FBS
#pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
#pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)
#pragma config RBS = NO_RAM             // Boot Segment RAM Protection (No Boot RAM)

// FSS
#pragma config SWRP = WRPROTECT_OFF     // Secure Segment Program Write Protect (Secure segment may be written)
#pragma config SSS = NO_FLASH           // Secure Segment Program Flash Code Protection (No Secure Segment)
#pragma config RSS = NO_RAM             // Secure Segment Data RAM Protection (No Secure RAM)

// FGS
#pragma config GWRP = OFF               // General Code Segment Write Protect (User program memory is not write-protected)
#pragma config GSS = OFF                // General Segment Code Protection (User program memory is not code-protected)

// FOSCSEL
#pragma config FNOSC = FRCPLL           // Oscillator Mode (Internal Fast RC (FRC) w/ PLL)
#pragma config IESO = OFF //ON                // Internal External Switch Over Mode (Start-up device with FRC, then automatically switch to user-selected oscillator source when ready)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Source (Primary Oscillator Disabled)
#pragma config OSCIOFNC = OFF            // OSC2 Pin Function (OSC2 pin has digital I/O function)
#pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow Multiple Re-configurations)
#pragma config FCKSM = CSECMD  //CSDCMD           // Clock Switching and Monitor (Both Clock Switching and Fail-Safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler (1:32,768)
#pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR128           // POR Timer Value (128ms)
#pragma config ALTI2C = ON              // Alternate I2C  pins (I2C mapped to ASDA1/ASCL1 pins)
#pragma config LPOL = ON                // Motor Control PWM Low Side Polarity bit (PWM module low side output pins have active-high output polarity)
#pragma config HPOL = ON                // Motor Control PWM High Side Polarity bit (PWM module high side output pins have active-high output polarity)
#pragma config PWMPIN = ON              // Motor Control PWM Module Pin Mode bit (PWM module pins controlled by PORT register at device Reset)

// FICD
#pragma config ICS = PGD2               // Comm Channel Select (Communicate on PGC2/EMUC2 and PGD2/EMUD2)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is Disabled)



/******************************************************************************/
/********************* DECLARATION DES VARIABLES GLOBALES *********************/
/******************************************************************************/


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

int main(int argc, char** argv)
{
    
    /**************************************************************************/
    /*************************** INIT ROBOT ***********************************/
    /**************************************************************************/

    uint16_t i;
    uint16_t j=0.;
    init_system();
    
    
    TIMER_5ms = ACTIVE;
    //TIMER_DEBUG = ACTIVE;
    //envoit_pwm(50);
    
    VITESSE.consigne = 200;
    ENABLE_CAPTEUR = 1;
    
    while(1)
    {

        static bool led = true;
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
 
    
//    while (1)
//    {
//        LED9 = 0;
//        LED1 = 1;
//        delay_ms(1000);
//        
//        LED1 = 0;
//        LED2 = 1;
//        delay_ms(1000);
//        
//        LED2 = 0;
//        LED3 = 1;
//        delay_ms(1000);
//        
//        LED3 = 0;
//        LED4 = 1;
//        delay_ms(1000);
//        
//        LED4 = 0;
//        LED5 = 1;
//        delay_ms(1000);
//        
//        LED5 = 0;
//        LED6 = 1;
//        delay_ms(1000);
//        
//        LED6 = 0;
//        LED7 = 1;
//        delay_ms(1000);
//        
//        LED7 = 0;
//        LED8 = 1;
//        delay_ms(1000);
//        
//        LED8 = 0;
//        LED9 = 1;
//        delay_ms(1000);
//    }

    delay_ms(500);


    while(1);
    return (EXIT_SUCCESS);
}

