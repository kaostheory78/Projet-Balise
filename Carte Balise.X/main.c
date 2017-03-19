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

    typedef enum 
    {
        // /!\ ENUM CODEE SUR 2 BIT !!! /!\ //
        // /!\      VALEUR MIN : -2     /!\ //
        // /!\      VALEUR MAX :  1     /!\ //
        SENS_INDIRECT   = -1,
        ROT_EN_BAS      = -1,
        SENS_DIRECT     =  1,
        ROT_EN_HAUT     =  1
    }_enum_sens_rotation;

    typedef struct decal
    {
        // /!\ suivant et symétrique limité de -32 à + 31 /!\ //
        int16_t angle                       : 16;   // 16 : add 1
        uint16_t position                   : 16;   // 16 : add 2 
        _Bool etat               : 1;    //  1 : add 3
        int8_t suivant                      : 6;    //  7 : add 3
        _enum_sens_rotation sens_rotation   : 2;    //  9 : add 3
        int8_t symetrique                   : 6;    // 15 : add 3
    }decal;
   
typedef enum
{
    VRAI = true,
    FAUX = false
}_enum_vrai_faux;

typedef enum
{
    NB_1,
    NB_2 = NB_1,
    NB_3,
    NB_4,
    NB_5,
    NB_6 = NB_5,
    NB_7,
    NB_8,
    NB_9
}_enum_nombre;

typedef struct
{
    _enum_vrai_faux boolen_1 :1;
    _enum_vrai_faux boolen_2 :1;
    _enum_vrai_faux boolen_3 :1;
    _enum_vrai_faux boolen_4 :1;
    _enum_vrai_faux boolen_5 :1;
    _enum_vrai_faux boolen_6 :1;
    _enum_nombre nb_1        :4;
    _enum_nombre nb_2        :4;
}_test;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

int main(int argc, char** argv)
{
    
    /**************************************************************************/
    /*************************** INIT ROBOT ***********************************/
    /**************************************************************************/

    uint16_t i;
    uint16_t j=0;
    init_system();
     
    
    TIMER_5ms = ACTIVE;
    TIMER_10ms = ACTIVE;
    //TIMER_DEBUG = ACTIVE;
    //envoit_pwm(50);
    
    VITESSE.consigne = 200;
    ENABLE_CAPTEUR = 0;
    
    delay_ms(1000);
    
    PutsUART(UART_USB, "\n\n\rBonjour\n\r");
    delay_ms(100);
    
    // config hmc5883L
    ByteWriteI2C(0x3C, 0x00, 0x70);  
    ByteWriteI2C(0x3C, 0x01, 0xA0);  
    
    //config_boussole();
    while(1)
    {
//        //PutsUART(UART_USB, "Envoi\n\r");
//         //envoiInformationI2C(0x1E, 0x02);
//        //i2c_envoi_octet(0x02, 0x00);
//        //recup_valeur();
//        
//        get_value();
//        delay_ms(100);
        uint8_t i = 0;
        
        for (i = 0 ; i <=12 ; i++)
        {
            ByteReadI2C(0x3C, i);
            delay_ms(50);
        }
        delay_ms(2000);
    }
    
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

