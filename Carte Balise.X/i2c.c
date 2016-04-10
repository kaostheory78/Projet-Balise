/******************************************************************************/
/********************* Carte Balise - DSPIC33FJ128MC804 ***********************/
/******************************************************************************/
/* Fichier 	: I2c.c
 * Auteur  	: Quentin Mercier
 * Revision	: 1.0
 * Date		: 20 Février 2016, 18:14
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************* INCLUDES *************************************/
/******************************************************************************/

#include "system.h"

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


void config_i2c ()
{
    I2C1CONbits.A10M = 0;           // Slave : 7 bit adress
    //I2C1BRG = 187;                  // F = 400 kHz
    I2C1BRG = 392;                  // F = 400 kHz
     
    I2C1CONbits.I2CEN = 1;           //Enable I2C  
}

void i2c_debut_transmission()
{
    I2C1CONbits.SEN = 1;                // Envoi un bit de start
    //while(I2C1CONbits.SEN == 0){
    //    PutcUART(UART_USB, '%');
    //}        // Attend qu'il soit fini
    
    PutsUART(UART_USB, " START 1");
    
    I2C1TRN = 0x3C;                     // Adresse de l'esclave
    while(I2C1STATbits.TRSTAT);         // Attend que l'envoi soit fini TRN + ACK.    Passe à 0 quand fini 
}

void i2c_debut_reception()
{
    I2C1TRN = 0x3D;                     // Demande de reception
    while(I2C1STATbits.TRSTAT);         // Attend que l'envoi soit fini TRN + ACK.    Passe à 0 quand fini 

    I2C1CONbits.RCEN = 1;               //Initie reception de 8 bits
    while(I2C1CONbits.RCEN);
}

void i2c_fin_transmission(bool condition)
{
    if (condition == true)
    {
        I2C1CONbits.PEN = 1;                // Envoi un bit de stop  
        //while(I2C1CONbits.PEN == 0);

    }
    else
    {
        I2C1CONbits.RSEN = 1;               // Envoi un bit de start
        //while(I2C1CONbits.RSEN == 0);       // Attend qu'il soit fini
    }
}

void i2c_ecriture_octet (uint8_t octet)
{
    I2C1TRN = octet;                    // Envoi de la data 
    while(I2C1STATbits.TRSTAT);         // Attend que l'envoi soit fini TRN + ACK.    Passe à 0 quand fini 
    delay_ms(10);
}

void i2c_envoi_octet (uint8_t adresse, uint8_t octet)
{
    //PutsUART(UART_USB, "START");
    i2c_debut_transmission();
    //PutsUART(UART_USB, " Envoit adresse");
    i2c_ecriture_octet(adresse);  
    //PutsUART(UART_USB, " Envoi octet");
    i2c_ecriture_octet(octet);
    i2c_fin_transmission(true);
    //delay_ms(10);
}

void i2c_envoi_multiple_octets(uint8_t adresse, uint8_t *octet)
{
    i2c_debut_transmission();
    i2c_ecriture_octet(adresse);
    i2c_ecriture_octet(*octet);
    
    while (*++octet)
    {
        i2c_fin_transmission(false);
        i2c_ecriture_octet(*octet);
    }
    i2c_fin_transmission(true);
}

int8_t i2c_lecture_octet (bool derniere_valeur)
{
    int8_t valeur;
    //while(I2C1STATbits.RBF == 0)
        {
        PutcUART(UART_USB, '¨');
    }
    valeur = I2C1RCV;
    PutsUART(UART_USB, " Valeur acquise");
    
    
    if (derniere_valeur == true)
    {
        I2C1CONbits.ACKDT = 1;          //Se prépare à un envoyer un NACK qui signifie la fin de la com.
        I2C1CONbits.ACKEN = 1;          //Envoi le NACK
        i2c_fin_transmission(true);
    }
    else
    {
        I2C1CONbits.ACKDT = 0;          //Se prépare à un envoyer un ACK pour continuer la reception.
        I2C1CONbits.ACKEN = 1;          //Envoi le NACK
    }
    return valeur;
    
}

//void i2c_reception_octet

//int16_t receive_i2c(uint8_t adresse)
//{
//    uint8_t value, compteur = 0;
//    
//    //adressage_i2c(adresse);
//    
//    I2C1CONbits.RSEN = 1;               // Restart
//    while(I2C1CONbits.RSEN == 0);       // Attend qu'il soit fini
//    
//    I2C1TRN = 0x3D;                     // Demande de reception
//    while(I2C1STATbits.TRSTAT);         // Attend que l'envoi soit fini TRN + ACK.    Passe à 0 quand fini 
//
//    I2C1CONbits.RCEN = 1;               //Initie reception de 8 bits
//    while(I2C1CONbits.RCEN);
//       
//    while(I2C1STATbits.RBF == 0 && compteur < 20)
//    {
//        delay_ms(10);
//        compteur++;
//    }
//    
//    if (compteur >= 20)
//    {
//         I2C1CONbits.PEN = 1;           //Envoi un bit de stop
//        while(I2C1CONbits.PEN == 0);
//        return -1;
//    }
//    else 
//    {
//        value = I2C1RCV;                //On récupère la donnée
//
//        I2C1CONbits.ACKDT = 1;          //Se prépare à un envoyer un NACK qui signifie la fin de la com.
//        I2C1CONbits.ACKEN = 1;          //Envoi le NACK
//
//        I2C1CONbits.PEN = 1;             // Envoi un bit de stop  
//        while(I2C1CONbits.PEN == 0);
//    
//    }
//    return (int16_t) value; 
//}


void config_boussole()
{
    i2c_envoi_octet(0x00, 0x70);       // 8-average, 15Hz
    i2c_envoi_octet(0x01, 0xA0);       // Gain = 15
    //i2c_envoi_octet(0x02, 0x00);       // Mode continu   
}

void recup_valeur()
{
    int16_t X, Y, Z;
    double angle;
    
    //PutsUART(UART_USB, "\n\rDebut transmission octet");
    i2c_debut_transmission();
    //PutsUART(UART_USB, " START");
    i2c_ecriture_octet(0x03);  
    //PutsUART(UART_USB, " adresse transimise");
    i2c_fin_transmission(false);
    //PutsUART(UART_USB, " Octet Tranmis\n\r");
    
    i2c_debut_reception();
    //PutsUART(UART_USB, "\n\rDebut récetion");
    PutIntUART(UART_USB, i2c_lecture_octet(false));
    PutIntUART(UART_USB, i2c_lecture_octet(true));
    
    //X= i2c_lecture_octet(false)<<8|i2c_lecture_octet(false);
    //Z= i2c_lecture_octet(false)<<8|i2c_lecture_octet(false);
    //Y= i2c_lecture_octet(false)<<8|i2c_lecture_octet(true);

//    angle= atan2(Y,X) * (180 / 3.14159265); // angle en degres
//    if (angle<0) 
//    {
//        angle=angle+360;
//    }
//    
//    PutLongUART(UART_USB, angle);
    PutsUART(UART_USB, "\r\n");
    delay_ms(6);
    
}

void setup_boussole()
{
    ByteWriteI2C(0x1E, 0x00, 0x70);
    ByteWriteI2C(0x1E, 0x01, 0xA0);
}

void get_value()
{
    ByteWriteI2C(0x1E, 0x02, 0x01);
    PutIntUART(UART_USB, ByteReadI2C(0x1E, 0X03));
    
}


void setupI2C(long unsigned int i2cfreq)
{

 TRISBbits.TRISB5=0;
 TRISBbits.TRISB6=0;
 LATBbits.LATB5=0;
 LATBbits.LATB6=0;
    
    //This function will initialize the I2C(1) peripheral.
    //Now we will initialise the I2C peripheral for Master Mode, No Slew Rate
    //Control, SMbus levels, and leave the peripheral switched off.


        I2C1CONbits.I2CEN = 0;			// Enables the I2C module and configures the SDA and SCL pins as serial port pins
	I2C1CONbits.I2CSIDL = 0;		// Continue module operation in idle mode
	I2C1CONbits.SCLREL = 1;			// Release the clock
	I2C1CONbits.IPMIEN = 0;			// Only acknowledge own address
	I2C1CONbits.A10M = 0;			// 7bit slave address
	I2C1CONbits.DISSLW = 1;			// 1=Slew rate control disabled (enable for 400kHz operation!)
	I2C1CONbits.SMEN = 0;			// Disable SMBus Input thresholds (set for 3.3V operation!)
	I2C1CONbits.GCEN = 0;			// Enable interrupt on a general address call
	I2C1CONbits.STREN = 0;			// Enable software or receive clock stretching (important when dealing with interrupts)
	I2C1CONbits.ACKDT = 0;			// Send ACK during acknowledge
	I2C1CONbits.ACKEN = 0;			// Acknowledge sequence not in progress
	I2C1CONbits.RCEN = 0;			// Receive sequence not in progress
	I2C1CONbits.PEN = 0;			// STOP condition not in progress
	I2C1CONbits.RSEN = 0;			// Repeated START condition not in progress
	I2C1CONbits.SEN = 0;


    //Clearing the recieve and transmit buffers
    I2C1RCV = 0x0000;
    I2C1TRN = 0x0000;

        /*Set I2C Baud Rate */
    /*Note: from dsPIC33F Family Reference Manual: I2C, section 19.4.3
Baud rate is set according to the following equation:
I2CBRG = (FCY/FSCL - FCY/10000000)-1 */
    I2C1BRG = (FCY/i2cfreq)-(FCY/10000000)-1;

    //Now we can enable the peripheral
    I2C1CONbits.I2CEN = 1;

    /*  IPC4bits.MI2C1IP = 5;			// set the master interrupt priority
        I2C1STATbits.I2COV = 0;          	// clear Overflow flag
        IPC4bits.MI2C1IP = 2;                   // set the master interrupt priority
        I2C1STATbits.I2COV = 0;                 // clear Overflow flag
        IEC1bits.SI2C1IE = 0;			// disables slave int
	I2C1CONbits.I2CEN = 1;			// Enables the I2C module and configures the SDA and SCL pins as serial port pins*/

    delay_ms(10);

    PutsUART(UART_USB, "\nI2C Setup Complete");
}

unsigned char ByteReadI2C(unsigned char SlaveAddress, unsigned char SlaveRegister) 
{
    unsigned char Data;

    /*begin start condition */
    //IdleI2C1(); //wait for bus to be idle
    IFS1bits.MI2C1IF = 0; //clear interrupt flag
    I2C1CONbits.SEN = 1; //set start event
    while(I2C1CONbits.SEN); //wait for the startup sequence to complete

    /*send device address (r/w cleared) */
    I2C1TRN = SlaveAddress;
    IFS1bits.MI2C1IF = 0;
    while(I2C1STATbits.TBF); //wait for data to clock out
    while(I2C1STATbits.ACKSTAT); //wait for device to acknowledge
    //IdleI2C1();

    /*send slave register address*/
    I2C1TRN = SlaveRegister; //status register of RTC
    IFS1bits.MI2C1IF = 0;
    while(I2C1STATbits.TBF); //wait for data to clock out
    while(I2C1STATbits.ACKSTAT); //wait for device to acknowledge
    //IdleI2C1();

    /*initiate a repeated start*/
    //__delay_us(100);
    while(I2C1CON & 0x001F); //wait for the module to be ready (see 19.5.6 of dsPIC33 reference manual 12C)
    I2C1CONbits.RSEN = 1;
    while(I2C1CONbits.RSEN); //wait for slave to respond
    //__delay_us(100);

    /*send device address (read/write set) */
    I2C1TRN = SlaveAddress | 0x1;
    IFS1bits.MI2C1IF = 0;
    while(I2C1STATbits.TBF); //wait for data to clock out
    while(I2C1STATbits.ACKSTAT); //wait for device to acknowledge
    //IdleI2C1();

    /*receive data from device*/
    I2C1CONbits.RCEN = 1; //receive enable (start clocking for slave transfer)
    while(I2C1CONbits.RCEN); //wait for data (NOTE: can also consider polling RBF bit here)
    Data = I2C1RCV;
    //IdleI2C1();

    //NotAckI2C1();
    while(I2C1CONbits.ACKEN);

    /*generate stop bus event*/
    IFS1bits.MI2C1IF = 0;
    while(I2C1CON & 0x001F); //wait for the module to be ready (see 19.5.5 of dsPIC33 reference manual 12C)
    I2C1CONbits.PEN = 1;
    while(IFS1bits.MI2C1IF == 0);

    /*done!*/
    return Data;
}

void ByteWriteI2C(unsigned char SlaveAddress, unsigned char SlaveRegister, unsigned char Data)
{
   /*begin start condition */
    //while(I2C1STATbits.P == 1); //make sure the bus is idle
    //IdleI2C1();
    IFS1bits.MI2C1IF = 0; //clear interrupt flag
    I2C1CONbits.SEN = 1; //set start event
    while(IFS1bits.MI2C1IF == 0); //wait for interrupt flag to signify end of start condition

    /*send device address (r/w cleared) */
    I2C1TRN = SlaveAddress;
    IFS1bits.MI2C1IF = 0;
    while(I2C1STATbits.TBF); //wait for data to clock out
    while(I2C1STATbits.ACKSTAT); //wait for device to acknowledge
    //IdleI2C1();

    /*send slave register address*/
    I2C1TRN = SlaveRegister; //status register of RTC
    IFS1bits.MI2C1IF = 0;
    while(I2C1STATbits.TBF); //wait for data to clock out
    while(I2C1STATbits.ACKSTAT); //wait for device to acknowledge
    //IdleI2C1();

    /*send data */
    I2C1TRN = Data; //status register of RTC
    IFS1bits.MI2C1IF = 0;
    while(I2C1STATbits.TBF); //wait for data to clock out
    while(I2C1STATbits.ACKSTAT); //wait for device to acknowledge
    //IdleI2C1();

    /*generate stop bus event*/
    IFS1bits.MI2C1IF = 0;
    while(I2C1CON & 0x001F); //wait for the module to be ready (see 19.5.5 of dsPIC33 reference manual 12C)
    I2C1CONbits.PEN = 1;
    while(IFS1bits.MI2C1IF == 0);

    /*done!*/
}
