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

#include <xc.h>
#include "system.h"


/******************************************************************************/
/************************ Configurations Bits *********************************/
/******************************************************************************/

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
#pragma config FNOSC = FRCPLL//PRI//PLL           // Oscillator Mode (Primary Oscillator (XT, HS, EC) w/ PLL)
#pragma config IESO = OFF               // Internal External Switch Over Mode (Start-up device with user-selected oscillator source)

// FOSC
#pragma config POSCMD = NONE//EC              // Primary Oscillator Source (EC Oscillator Mode)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function (OSC2 pin has digital I/O function)
#pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow Multiple Re-configurations)
#pragma config FCKSM = CSECMD//CSECME           // Clock Switching and Monitor (Both Clock Switching and Fail-Safe Clock Monitor are enabled)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler (1:32,768)
#pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF              // Watchdog Timer Enable (Watchdog timer always enabled)

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

    int i;
    double j=0.;
    init_system();
    
    
    while(1)
    {
        for (i = 0 ; i <=2000 ; i++)
        {
            PDC2 = i;
            delay_ms(5);
        }
        for (i = 2000 ; i >=0 ; i--)
        {
            PDC2 = i;
            delay_ms(5);
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

