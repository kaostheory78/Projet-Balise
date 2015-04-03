
/******************************************************************************/
/************************ Carte Balise - PIC18f4431 ***************************/
/******************************************************************************/
/* Fichier 	: system.c
 * Auteur  	: M1E11
 * Revision	: 1.0
 * Date		: 09 Mars 2015, 17:02
 *******************************************************************************
 *
 *
 ******************************************************************************/


#include "system.h"


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

void init_system (void)
{
    //init_clock();
    ConfigPorts();
    ConfigInterrupt();
    init_timer_0();
    init_uart_logiciel();
    init_uart_reception();
}



/******************************************************************************/
/***************************** Configurations Timers **************************/
/******************************************************************************/


void init_timer_0 () //1ms
{
    T0CONbits.T016BIT = 1;      // Timer 8 bits
    T0CONbits.T0PS = 0b100;     // Prescaler 1:32
    T0CONbits.T0CS = 0;         // INternal Clock FOSC/4 = 4 Mhz
    T0CONbits.PSA = 0;          // Prescaler actif

    TMR0L = 131;                  //32 * 125 * 1/4Mhz = 1 ms

    TMR0ON = 1;                 //TIMER 0 activé
}

void init_timer_1 ()
{
    T1CONbits.RD16 = 1; //entrée 8 bits
    T1CONbits.T1RUN = 0; //Clock != timer1
    T1CONbits.TMR1CS = 0; //Fin = FOSC/4
    T1CONbits.T1OSCEN = 1;
    T1CONbits.T1CKPS0 = 0; //prescaler
    T1CONbits.T1CKPS1 = 0; //prescaler

    TMR1 = 65519;

    //enable timer
    TMR1ON = 1;


}

/******************************************************************************/
/********************************** Init Clock ********************************/
/******************************************************************************/

void init_clock(void)
{
    //Oscillateur externe sur osc1 et 2 avec PLL
    OSCCONbits.SCS = 0; //Primary Oscillator
    //while(OSCCONbits.IOFS == 0); //on attends que la pll soit stable
}

/******************************************************************************/
/************************ Mapping des pins de la carte ************************/
/******************************************************************************/


/*********************************************************/
void ConfigPorts (void)
{
        // PORT A
        TRISAbits.RA0       = 0;        // NC
        TRISAbits.RA1       = 0;        // NC
        TRISAbits.RA2       = 0;        // NC
        TRISAbits.RA3       = 1;        // INPUT : CODEURS QEA
        TRISAbits.RA4       = 1;        // INPUT : CODEURS QEB
        TRISAbits.RA5       = 0;        // NC
        TRISAbits.RA6       = 1;        // OSC2
        TRISAbits.RA7       = 1;        // OSC1

        // PORT B
        TRISBbits.RB0       = 0;        // OUTPUT DIGIT : LED1
        TRISBbits.RB1       = 0;        // OUTPUT DIGIT : LED2
        TRISBbits.RB2       = 0;        //
        TRISBbits.RB3       = 0;        //
        TRISBbits.RB4       = 0;        //
        TRISBbits.RB5       = 0;        //
        TRISBbits.RB6       = 0;        //  PGC
        TRISBbits.RB7       = 0;        //  PGC

        // PORT C
        TRISCbits.RC0       = 0;        // OUTPUT DIGIT : RESET BLUETOOTH
        TRISCbits.RC1       = 0;        //
        TRISCbits.RC2       = 0;        // OUTPUT DIGIT : ENABLE CAPTEUR
        TRISCbits.RC3       = 1;        // INPUT  DIGIT : CAPTEUR 1
        TRISCbits.RC4       = 0;        //
        TRISCbits.RC5       = 0;        //  UART Logiciel
        TRISCbits.RC6       = 1;        //  OUTPUT UART : TX (déclaré input pour configuration uart -> datasheet)
        TRISCbits.RC7       = 1;        //  INPUT  UART : RX

        // PORT D
        TRISDbits.RD0       = 0;        //
        TRISDbits.RD1       = 0;        //
        TRISDbits.RD2       = 1;        // I²C : SDA
        TRISDbits.RD3       = 0;        // I²C : SCL
        TRISDbits.RD4       = 0;        // OUTPUT DIGIT : SENS MOTEUR
        TRISDbits.RD5       = 0;        // OUTPUT DIGIT : BREAK MOTEUR
        TRISDbits.RD6       = 0;        // OUPUT  PWM   : PWM MOTEUR
        TRISDbits.RD7       = 0;        //

        // PORT E
        TRISEbits.RE0       = 0;        //
        TRISEbits.RE1       = 0;        //
        TRISEbits.RE2       = 0;        //


	//****************
	// INITIALISATION
	//****************

	// INIT PORT A
	LATA0           = 0;	// Valeur initiale a  l'etat bas
	LATA1           = 0;	// Valeur initiale a  l'etat bas
	LATA2           = 0;	// Valeur initiale a  l'etat bas
	LATA3           = 0;	// Valeur initiale a  l'etat bas
	LATA4           = 0;	// Valeur initiale a  l'etat bas
	LATA5           = 0;	// Valeur initiale a  l'etat bas
	LATA6           = 0;	// Valeur initiale a  l'etat bas
	LATA7           = 0;	// Valeur initiale a  l'etat bas

	// INIT PORT B
	LATB0           = 0;	// Valeur initiale a  l'etat bas
	LATB1           = 0;	// Valeur initiale a  l'etat bas
	LATB2           = 0;	// Valeur initiale a  l'etat bas
	LATB3           = 0;	// Valeur initiale a  l'etat bas
	LATB4           = 0;	// Valeur initiale a  l'etat bas
	LATB5           = 0;	// Valeur initiale a  l'etat bas
	LATB6           = 0;	// Valeur initiale a  l'etat bas
	LATB7           = 0;	// Valeur initiale a  l'etat bas

	// INIT PORT C
	LATC0           = 0;	// Valeur initiale a  l'etat bas
	LATC1           = 0;	// Valeur initiale a  l'etat bas
	LATC2           = 0;	// Valeur initiale a  l'etat bas
	LATC3           = 0;	// Valeur initiale a  l'etat bas
	LATC4           = 0;	// Valeur initiale a  l'etat bas
	LATC5           = 0;	// Valeur initiale a  l'etat bas
	LATC6           = 0;	// Valeur initiale a  l'etat bas
	LATC7           = 0;	// Valeur initiale a  l'etat bas

	// INIT PORT D
	LATD0           = 0;	// Valeur initiale a  l'etat bas
	LATD1           = 0;	// Valeur initiale a  l'etat bas
	LATD2           = 0;	// Valeur initiale a  l'etat bas
	LATD3           = 0;	// Valeur initiale a  l'etat bas
	LATD4           = 0;	// Valeur initiale a  l'etat bas
	LATD5           = 0;	// Valeur initiale a  l'etat bas
	LATD6           = 0;	// Valeur initiale a  l'etat bas
	LATD7           = 0;	// Valeur initiale a  l'etat bas

	// INIT PORT E
	LATE0           = 0;	// Valeur initiale a  l'etat bas
	LATE1           = 0;	// Valeur initiale a  l'etat bas
	LATE2           = 0;	// Valeur initiale a  l'etat bas
}

/******************************************************************************/
/********************* Configurations Des Interruptions ***********************/
/******************************************************************************/


void ConfigInterrupt (void)
{
    // INTCON
    INTCONbits.GIEH         = 1;                    // Activation des interruptions de priorités hautes
    INTCONbits.GIEL         = 1;                    // Activation des interruptions de priorités basses
    INTCONbits.TMR0IE       = ACTIV_INTER_TIMER0;   // Activation du TIMER 0
    INTCONbits.INT0IE       = ACTIV_INTER_INT0;     // INTO : ACTIVATION
    INTCONbits.RBIE         = 0;                    //
    FLAG_TIMER0             = 0;                    // FLAG TIMER0
    FLAG_INT0               = 0;                    // INT0 : FLAG
    INTCONbits.RBIF         = 0;                    //


    // INTCON2
    INTCON2bits.INTEDG0     = 0;                    // INT0 : rising edge
    INTCON2bits.INTEDG1     = 0;                    // INT1 : rising edge
    INTCON2bits.INTEDG2     = 0;                    // INT2 : rising edge
    INTCON2bits.TMR0IP      = PRIO_INTER_TIMER0;          // TIMER0

    // INTCON3
    INTCON3bits.INT2IP      = PRIO_INTER_INT2 ;     // INT2 : PRIORItE INTERRUPTION
    INTCON3bits.INT1IP      = PRIO_INTER_INT1;      // INT1 : PRIORITE INTERRUPTION
    INTCON3bits.INT2IE      = ACTIV_INTER_INT2;     // INT2 : ACTIVATION
    INTCON3bits.INT2IP      = ACTIV_INTER_INT1;     // INT1 : ACTIVATION
    FLAG_INT2               = 0;                    // INT2 : FLAG
    FLAG_INT1               = 0;                    // INT1 : FLAG


    //****************
    // FLAG
    //****************
    // PIR1 : Remise à zéro des Flags
    FLAG_ADC                = 0;                    // A/D : FLAG
    FLAG_RX                 = 0;                    // UART RX : FLAG
    FLAG_TX                 = 0;                    // UART TX : FLAG
    PIR1bits.SSPIF          = 0;                    //
    PIR1bits.CCP1IF         = 0;                    //
    FLAG_TIMER2             = 0;                    // TMR2 : FLAG
    FLAG_TIMER1             = 0;                    // TMR1 : FLAG

    // PIR2 : remise à zéro des flags
    PIR2bits.OSFIF          = 0;                    //
    PIR2bits.EEIF           = 0;                    //
    PIR2bits.LVDIF          = 0;                    //
    PIR2bits.CCP2IF         = 0;                    //

    // PIR3 : remise à zéro des flags
    PIR3bits.PTIF           = 0;                    //
    FLAG_QEI_SENS           = 0;                    // QEI : FLAG -> changement sens rotation
    FLAG_QEI                = 0;                    // QEI : FLAG -> Overflow
    PIR3bits.IC1IF          = 0;                    // QEI : FLAG -> ...
    FLAG_TIMER5             = 0;                    // TMR5: FLAG


    //****************
    // Priorites
    //****************
    // RCON
    RCONbits.IPEN           = 1;                    // Activation des priorités d'interruptions

    // IPR1
    IPR1bits.ADIP           = PRIO_INTER_ADC;       //
    IPR1bits.RC1IP          = PRIO_INTER_RX;                    //
    IPR1bits.TX1IP          = PRIO_INTER_TX;                    //
    IPR1bits.SSPIP          = 0;                    //
    IPR1bits.CCP1IP         = 0;                    //
    IPR1bits.TMR2IP         = PRIO_INTER_TIMER2;    //
    IPR1bits.TMR1IP         = PRIO_INTER_TIMER1;    //

    // IPR2
    IPR2bits.OSFIP          = 0;                    //
    IPR2bits.EEIP           = 1;                    // interrupt priority bit
    IPR2bits.LVDIP          = 0;                    //
    IPR2bits.CCP2IP         = 0;                    //

    // IPR3
    IPR3bits.PTIP           = 0;                    // pwm time base
    IPR3bits.IC3DRIP        = PRIO_INTER_QEI_SENS;  // QEI : Priorité changement de sens
    IPR3bits.IC2QEIP        = PRIO_INTER_QEI;       // QEI :
    IPR3bits.IC1IP          = 0;                    // IC1 : priorité
    IPR3bits.TMR5IP         = PRIO_INTER_TIMER5;    //


    //****************
    // Activation
    //****************
    // PIE1
    PIE1bits.ADIE           = ACTIV_INTER_ADC;      // ADC
    PIE1bits.RC1IE          = ACTIV_INTER_RX;                    //
    PIE1bits.TXIE           = ACTIV_INTER_TX;                    //
    PIE1bits.SSPIE          = 0;                    //
    PIE1bits.CCP1IE         = 0;                    //
    PIE1bits.TMR2IE         = ACTIV_INTER_TIMER2;   //
    PIE1bits.TMR1IE         = ACTIV_INTER_TIMER1;   //

    // PIE2
    PIE2bits.OSFIE          = 0;                    // Oscillateur fail interrupt
    PIE2bits.EEIE           = 1;                    // Activation des interruptions
    PIE2bits.LVDIE          = 0;                    //
    PIE2bits.CCP2IE         = 0;                    //

    // PIE3
    PIE3bits.PTIE           = 0;                    // PWM TIME BASE INTERRUPT
    PIE3bits.IC3DRIE        = ACTIV_INTER_QEI_SENS; //
    PIE3bits.IC2QEIE        = ACTIV_INTER_QEI;      //
    PIE3bits.IC1IE          = 0;                    // IC1 interrupt
    PIE3bits.TMR5IE         = ACTIV_INTER_TIMER5;   //
}

/******************************************************************************/
/**************************** Configurations   ADC ****************************/
/******************************************************************************/


void ConfigADC (void)
{
	// #if CONFIG_CAPTEUR_1 == CAPTEUR_ANALOGIQUE

	NOP ();
}

void config_pwm ()
{
    PTCON0bits.PTOPS = 2;       // Postcale = 3
    PTCON0bits.PTCKPS = 0b11;   // Prescale 1:64
    PTCON0bits.PTMOD  = 0;      // free running mode

    PTCON1bits.PTEN = 1;        // PWM time base is ON
    PTCON1bits.PTDIR = 0;       // Compteur

    PWMCON0bits.PMOD3 = 1;      // PWM6 et 7 indépendants
    PWMCON0bits.PWMEN = 0b111;  // Tous les pwm impaires

    PWMCON1bits.UDIS = 0;       // MAJ duty cycle et period enable
    
}