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
#include <i2c.h>

volatile _i2c_cmd i2c_cmd;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


// Gyro : adresse 7 bits : 0x1E

void i2c_interupt()
{
    if (i2c_cmd.isTimerStarted)
    {
        bool registerHasChanged = false;
        I2C1STATBITS statusRegister = I2C1STATbits;
        
        if (statusRegister.ACKSTAT != i2c_cmd.statusRegister.ACKSTAT)
        {
            registerHasChanged = true;
            if (statusRegister.ACKSTAT == 0)
            {
                printf("\n\ri2c_interupt : ACK received");
                i2c_cmd.receiveEvent |= ACK_RECEIVED;

                if (i2c_cmd.cmdEvent == WAITING_FOR_ACK)
                    i2c_cmd.isTimerStarted = false;
            }
            else
            {
                printf("\n\ri2c_interupt : NACK received");
                i2c_cmd.receiveEvent |= NACK_RECEIVED;

                if (i2c_cmd.cmdEvent == WAITING_FOR_NACK)
                    i2c_cmd.isTimerStarted = false;
            }
        }

        if (statusRegister.TRSTAT != i2c_cmd.statusRegister.TRSTAT)
        {
            registerHasChanged = true;
            if (statusRegister.TRSTAT == 1)
            {
                printf("\n\ri2c_interupt : transmit in progress");
            }
            else
            {
                printf("\n\ri2c_interupt : transmit terminated + ACK");
                if (i2c_cmd.cmdEvent == WAITING_FOR_END_TRANSMITION)
                {
                    i2c_cmd.receiveEvent |= END_TRANSMISION;
                    i2c_cmd.isTimerStarted = false;
                }

                if (i2c_cmd.cmdEvent == WAITING_FOR_ACK)
                {
                    i2c_cmd.receiveEvent |= ACK_RECEIVED;
                    i2c_cmd.isTimerStarted = false;  
                }
                    
            }
        }

        if (statusRegister.BCL != i2c_cmd.statusRegister.BCL)
        {
            registerHasChanged = true;
            if (statusRegister.BCL == 1)
            {
                printf("\n\ri2c_interupt bus collision");
                i2c_cmd.receiveEvent |= ERROR_OCCURED;
                i2c_cmd.receiveEvent |= BUS_COLLISION;
                statusRegister.BCL = 0;
                i2c_cmd.statusRegister.BCL = 0;
                i2c_cmd.isTimerStarted = false;
            }
            else
            {
                printf("\n\ri2c_interupt buc collision cleared");
            }
        }

        if (statusRegister.IWCOL != i2c_cmd.statusRegister.IWCOL)
        {
            registerHasChanged = true;
            if (statusRegister.IWCOL == 1)
            {
                printf("\n\ri2c_interupt : write colision");
                i2c_cmd.receiveEvent |= ERROR_OCCURED;
                i2c_cmd.receiveEvent |= WRITE_COLLISION;
                statusRegister.IWCOL = 0;
                i2c_cmd.statusRegister.IWCOL = 0;
                i2c_cmd.isTimerStarted = false;
            }
            else
            {
                printf("\n\ri2c_interupt : write colision cleared");
            }
        }

        if (statusRegister.I2COV != i2c_cmd.statusRegister.I2COV)
        {
            registerHasChanged = true;
            if (statusRegister.I2COV == 1)
            {
                printf("\n\ri2c_interupt : overflow in receive register");
                i2c_cmd.receiveEvent |= ERROR_OCCURED;
                i2c_cmd.receiveEvent |= RX_BUFFER_FULL;
                statusRegister.I2COV = 0;
                i2c_cmd.statusRegister.I2COV = 0;
                i2c_cmd.isTimerStarted = false;
            }
            else
            {
                printf("\n\ri2c_interupt : overflow cleared");
            }
        }

        if (statusRegister.P != i2c_cmd.statusRegister.P)
        {
            registerHasChanged = true;
            if (statusRegister.P == 1)
            {
                printf("\n\ri2c_interupt : stop bit detected");
                i2c_cmd.receiveEvent |= STOP_RECEIVED;

                if (i2c_cmd.cmdEvent == WAITING_FOR_STOP)
                    i2c_cmd.isTimerStarted = false;
            }
            else
            {
                printf("\n\ri2c_interupt : new bit received since the last stop");
            }
        }

        if (statusRegister.S != i2c_cmd.statusRegister.S)
        {
            registerHasChanged = true;
            if (statusRegister.S == 1)
            {
                printf("\n\ri2c_interupt : start bit detected");
                i2c_cmd.receiveEvent |= START_RECEIVED;

                if (i2c_cmd.cmdEvent == WAITING_FOR_START)
                    i2c_cmd.isTimerStarted = false;
            }
            else
            {
                printf("\n\ri2c_interupt : =new bit detected since last start");
            }
        }

        if (statusRegister.RBF != i2c_cmd.statusRegister.RBF)
        {
            registerHasChanged = true;
            if (statusRegister.RBF == 1)
            {
                printf("\n\ri2c_interupt : data ready in receive buffer");
                i2c_cmd.receiveEvent |= DATA_READY;

                if (i2c_cmd.cmdEvent == WAITING_FOR_RECEPTION)
                    i2c_cmd.isTimerStarted = false;
            }
            else
            {
                printf("\n\ri2c_interupt : waiting for data in receive buffer");
            }
        }

        if (statusRegister.TBF != i2c_cmd.statusRegister.TBF)
        {
            registerHasChanged = true;
            if (statusRegister.TBF == 1)
            {
                printf("\n\ri2c_interupt : transmit in progress register is full");
                if (i2c_cmd.cmdEvent == WAITING_FOR_TRANSMITION)
                    i2c_cmd.isTimerStarted = false;
            }
            else
            {
                printf("\n\ri2c_interupt : transmit terminated (register empty");
                if (i2c_cmd.cmdEvent == WAITING_FOR_END_TRANSMITION)
                    i2c_cmd.isTimerStarted = false;
            }
        }

        if (statusRegister.GCSTAT != i2c_cmd.statusRegister.GCSTAT)
        {
            registerHasChanged = true;
            printf("\n\ri2c_interupt : unexpected event GCSTAT = %d", statusRegister.GCSTAT);
        }

        if (statusRegister.ADD10 != i2c_cmd.statusRegister.ADD10)
        {
            registerHasChanged = true;
            printf("\n\ri2c_interupt : unexpected event ADD10 = %d", statusRegister.ADD10);
        }

        if (statusRegister.D_A != i2c_cmd.statusRegister.D_A)
        {
            registerHasChanged = true;
            printf("\n\ri2c_interupt : unexpected event D_A = %d", statusRegister.D_A);
        }

        if (statusRegister.R_W != i2c_cmd.statusRegister.R_W)
        {
            registerHasChanged = true;
            printf("\n\ri2c_interupt : unexpected event R/W = %d", statusRegister.R_W);
        }    
        
        if (registerHasChanged == false)
        {
            printf("\n\ri2c_interupt : event received is masked (same as before)");
            
            switch(i2c_cmd.cmdEvent)
            {    
                case WAITING_FOR_START :
                    if (statusRegister.S == 1)
                    {
                        printf("\n\ri2c_interupt : start bit potentially detected");
                        i2c_cmd.receiveEvent |= START_RECEIVED;
                        i2c_cmd.isTimerStarted = false;
                    }
                    break;
                case WAITING_FOR_STOP :
                    if (statusRegister.P == 1)
                    {
                        printf("\n\ri2c_interupt : stop bit potentially detected");
                        i2c_cmd.receiveEvent |= STOP_RECEIVED;
                        i2c_cmd.isTimerStarted = false;
                    }
                    break;
                case WAITING_FOR_ACK :
                    if (statusRegister.ACKSTAT == 0)
                    {
                        printf("\n\ri2c_interupt : ACK potentially detected");
                        i2c_cmd.receiveEvent |= ACK_RECEIVED;
                        i2c_cmd.isTimerStarted = false;
                    }
                    break;
                case WAITING_FOR_NACK :
                    if (statusRegister.ACKSTAT == 1)
                    {
                        printf("\n\ri2c_interupt : NACK potentially detected");
                        i2c_cmd.receiveEvent |= NACK_RECEIVED;
                        i2c_cmd.isTimerStarted = false;
                    }
                    break;
                case WAITING_FOR_TRANSMITION :
                    if (statusRegister.TBF == 1 || statusRegister.TRSTAT == 0)
                    {
                        printf("\n\ri2c_interupt : begin transmition potentially detected");
                        i2c_cmd.receiveEvent |= START_TRANSMITION;
                        i2c_cmd.isTimerStarted = false;
                    }
                    break;
                case WAITING_FOR_END_TRANSMITION :
                    if (statusRegister.TBF == 0 || statusRegister.TRSTAT == 1)
                    {
                        printf("\n\ri2c_interupt : end transmition potentially detected");
                        i2c_cmd.receiveEvent |= END_TRANSMISION;
                        i2c_cmd.isTimerStarted = false;
                    }
                    break;
                case WAITING_FOR_IDLE :
                    if (statusRegister.TBF == 0 || statusRegister.TRSTAT == 1)
                    {
                        // TODO
                        printf("\n\ri2c_interupt : end transmition potentially detected");
                        i2c_cmd.receiveEvent |= END_TRANSMISION;
                        i2c_cmd.isTimerStarted = false;
                    }
                    break;
                default :
                        printf("\n\ri2c_interupt : no event expected");
            }
        }
    }
    else
    {
        printf("\n\ri2c_interuptEvent : received while software not listening for it");
    }
}

bool I2C_waitForIdle(bool frameStillOk)
{
    bool result = false;
    // Check if already idle
    
//    i2c_cmd.statusRegister = I2C1STATbits;
//    i2c_cmd.cmdEvent       = WAITING_FOR_IDLE;
//    i2c_cmd.receiveEvent   = NO_EVENT;
//    
//    // start Timer
//    i2c_cmd.waitingEventTimer = TIMER_I2C_MAX_MS;
//    i2c_cmd.isTimerStarted = true;
//    
//    // Wait for event or timeout
//    while(i2c_cmd.receiveEvent != NO_EVENT);
// 
//    if ( (i2c_cmd.receiveEvent & ERROR_OCCURED) !=  0)
//    {
//        // print Event
//        result = false;
//    }
//    else
//    {
//        
//    }
    return result;
}

bool I2C_waitForStart(bool frameStillOk)
{
    bool result = false;
    
    if (frameStillOk)
    {
        printf("\n\rI2C_waitForStart : enter");
        
        i2c_cmd.statusRegister = I2C1STATbits;
        i2c_cmd.cmdEvent       = WAITING_FOR_START;
        i2c_cmd.receiveEvent   = NO_EVENT;

        // start Timer
        i2c_cmd.waitingEventTimer = TIMER_I2C_MAX_MS;
        i2c_cmd.isTimerStarted = true;

        // launch event here
        I2C1CONbits.SEN = 1; //set start event
        
        // Wait for event or timeout
        while(i2c_cmd.isTimerStarted == true);

        if ( (i2c_cmd.receiveEvent & ERROR_OCCURED) !=  0)
        {
            // print error
            I2C_printError();
            result = false;
        }
        else if ( (i2c_cmd.receiveEvent & START_RECEIVED) != 0)
        {
            result = true;
        }
        else
        {
            result = false;
            printf("\n\rI2C_waitForStart : other event received");
        }
    }
    else
    {
        printf("\n\rI2C_waitForStart : frame already corrupted");
    }
    
    return result;
}

bool I2C_waitForStop(bool frameStillOk)
{
    bool result = false;
    
    if (frameStillOk)
    {
        printf("\n\rI2C_waitForStop : enter");
        // Check if already idle
        i2c_cmd.statusRegister = I2C1STATbits;
        i2c_cmd.cmdEvent       = WAITING_FOR_STOP;
        i2c_cmd.receiveEvent   = NO_EVENT;

        // start Timer
        i2c_cmd.waitingEventTimer = TIMER_I2C_MAX_MS;
        i2c_cmd.isTimerStarted = true;

        I2C1CONbits.PEN = 1;
        
        // Wait for event or timeout
        while(i2c_cmd.isTimerStarted == true);

        if ( (i2c_cmd.receiveEvent & ERROR_OCCURED) !=  0)
        {
            // print error
            I2C_printError();
            result = false;
        }
        else if ( (i2c_cmd.receiveEvent & STOP_RECEIVED) != 0)
        {
            result = true;
        }
        else
        {
            result = false;
            printf("\n\rI2C_waitForStop : other event received");
        }
    }
    else
    {
        printf("\n\rI2C_waitForStop : frame already corrupted");
    }
      
    return result;
}

bool I2C_waitForAck(bool frameStillOk)
{
    bool result = false;
    
    if (frameStillOk)
    {      
        printf("\n\rI2C_waitForAck : enter");
        // Check if already idle

        i2c_cmd.statusRegister = I2C1STATbits;
        i2c_cmd.cmdEvent       = WAITING_FOR_ACK;
        i2c_cmd.receiveEvent   = NO_EVENT;

        // start Timer
        i2c_cmd.waitingEventTimer = TIMER_I2C_MAX_MS;
        i2c_cmd.isTimerStarted = true;

        // Wait for event or timeout
        while(i2c_cmd.isTimerStarted == true);

        if ( (i2c_cmd.receiveEvent & ERROR_OCCURED) !=  0)
        {
            // print error
            I2C_printError();
            result = false;
        }
        else if ( (i2c_cmd.receiveEvent & ACK_RECEIVED) != 0)
        {
            result = true;
        }
        else
        {
            result = false;
            printf("\n\rI2C_waitForAck : other event received");
        }
    }
    else
    {
        printf("\n\rI2C_waitForAck : frame already corrupted");
    }
    
    return result;
}

bool I2C_waitForNack(bool frameStillOk)
{
    bool result = false;
    
    if (frameStillOk)
    {
        printf("\n\rI2C_waitForNack : enter");
        // Check if already idle
    
        i2c_cmd.statusRegister = I2C1STATbits;
        i2c_cmd.cmdEvent       = WAITING_FOR_NACK;
        i2c_cmd.receiveEvent   = NO_EVENT;

        // start Timer
        i2c_cmd.waitingEventTimer = TIMER_I2C_MAX_MS;
        i2c_cmd.isTimerStarted = true;

        // Wait for event or timeout
        while(i2c_cmd.isTimerStarted == true);

        if ( (i2c_cmd.receiveEvent & ERROR_OCCURED) !=  0)
        {
            // print error
            I2C_printError();
            result = false;
        }
        else if ( (i2c_cmd.receiveEvent & NACK_RECEIVED) != 0)
        {
            result = true;
        }
        else
        {
            result = false;
            printf("\n\rI2C_waitForNack : other event received");
        }
    }
    else
    {
        printf("\n\rI2C_waitForNack : frame already corrupted");
    }
    
    return result;
}

bool I2C_waitForTransmisionReady(bool frameStillOk)
{
    bool result = false;
    
    if (frameStillOk)
    {
        printf("\n\rI2C_waitForTransmisionReady : enter");
        // Check if already idle
    
        i2c_cmd.statusRegister = I2C1STATbits;
        i2c_cmd.cmdEvent       = WAITING_FOR_TRANSMITION;
        i2c_cmd.receiveEvent   = NO_EVENT;

        // start Timer
        i2c_cmd.waitingEventTimer = TIMER_I2C_MAX_MS;
        i2c_cmd.isTimerStarted = true;

        // Wait for event or timeout
        while(i2c_cmd.isTimerStarted == true);

        if ( (i2c_cmd.receiveEvent & ERROR_OCCURED) !=  0)
        {
            // print error
            I2C_printError();
            result = false;
        }
        else if ( (i2c_cmd.receiveEvent & START_TRANSMITION) != 0)
        {
            result = true;
        }
        else
        {
            result = false;
            printf("\n\rI2C_waitForTransmisionReady : other event received");
        }
    }
    else
    {
        printf("\n\rI2C_waitForTransmisionReady : frame already corrupted");
    }
    
    return result;
}

bool I2C_waitForEndTransmision(bool frameStillOk, uint8_t dataToSend)
{
    bool result = false;
    
    if (frameStillOk)
    {
        printf("\n\rI2C_waitForEndTransmision : enter");
        // Check if already idle
        
        i2c_cmd.statusRegister = I2C1STATbits;
        i2c_cmd.cmdEvent       = WAITING_FOR_END_TRANSMITION;
        i2c_cmd.receiveEvent   = NO_EVENT;

        // start Timer
        i2c_cmd.waitingEventTimer = TIMER_I2C_MAX_MS;
        i2c_cmd.isTimerStarted = true;

        I2C1TRN = dataToSend;
        
        // Wait for event or timeout
        while(i2c_cmd.isTimerStarted == true);

        if ( (i2c_cmd.receiveEvent & ERROR_OCCURED) !=  0)
        {
            printf("\n\rI2C_waitForEndTransmision : error received");
            I2C_printError();
            result = false;
        }
        else if ( (i2c_cmd.receiveEvent & END_TRANSMISION) != 0)
        {
            result = true;
        }
        else
        {
            result = false;
            printf("\n\rI2C_waitForEndTransmision : other event received");
        }
    }
    else
    {
        printf("\n\rI2C_waitForEndTransmision : frame already corrupted");
    }
    
    return result;
}

bool I2C_waitForDataReady(bool frameStillOk)
{
    bool result = false;
    
    if (frameStillOk)
    {
        printf("\n\rI2C_waitForDataReady : enter");
        // Check if already idle
        
        i2c_cmd.statusRegister = I2C1STATbits;
        i2c_cmd.cmdEvent       = WAITING_FOR_RECEPTION;
        i2c_cmd.receiveEvent   = NO_EVENT;

        // start Timer
        i2c_cmd.waitingEventTimer = TIMER_I2C_MAX_MS;
        i2c_cmd.isTimerStarted = true;

        I2C1CONbits.RCEN = 1;
        
        // Wait for event or timeout
        while(i2c_cmd.isTimerStarted == true);

        if ( (i2c_cmd.receiveEvent & ERROR_OCCURED) !=  0)
        {
            // print error
            I2C_printError();
            result = false;
        }
        else if ( (i2c_cmd.receiveEvent & END_TRANSMISION) != 0)
        {
            result = true;
        }
        else
        {
            result = false;
            printf("\n\rI2C_waitForDataReady : other event received");
        }
    }
    else
    {
        printf("\n\rI2C_waitForDataReady : frame already corrupted");
    }
    
    return result;
}

void I2C_printError()
{
    if ( (i2c_cmd.receiveEvent & TIMER_TIMEOUT) != 0)
    {
        printf("\n\rI2C_printError : timer timeout");
    }
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
    I2C1BRG = 0x188;//(FCY/i2cfreq)-(FCY/10000000)-1;

    //Now we can enable the peripheral
    I2C1CONbits.I2CEN = 1;

    /*  IPC4bits.MI2C1IP = 5;			// set the master interrupt priority
        I2C1STATbits.I2COV = 0;          	// clear Overflow flag
        IPC4bits.MI2C1IP = 2;                   // set the master interrupt priority
        I2C1STATbits.I2COV = 0;                 // clear Overflow flag
        IEC1bits.SI2C1IE = 0;			// disables slave int
	I2C1CONbits.I2CEN = 1;			// Enables the I2C module and configures the SDA and SCL pins as serial port pins*/

    delay_ms(10);

    PutsUART(UART_USB, "\n\rI2C Setup Complete");
}

unsigned char ByteReadI2C(unsigned char SlaveAddress, unsigned char SlaveRegister) 
{
    unsigned char Data;
    bool result = true;

    /*begin start condition */
    //IdleI2C1(); //wait for bus to be idle
    printf("\n\n\rDEBUT TRANSMISION I2C (read adress %x of slave %x", SlaveRegister, SlaveAddress);
    
    result = I2C_waitForStop(result);
    
    I2C1CONbits.RSEN = 0; //write
    result = I2C_waitForStart(result);

    /*send device address (r/w cleared) */
//    I2C1STATbits.ACKSTAT = 0;
    result = I2C_waitForEndTransmision(result, SlaveAddress);
    result = I2C_waitForAck(result);

    /*send slave register address*/
    // Clear ackstat
//    I2C1STATbits.ACKSTAT = 0;
    result = I2C_waitForEndTransmision(result, SlaveRegister);
    result = I2C_waitForAck(result);
    //IdleI2C1();

    /*initiate a repeated start*/
    //__delay_us(100);
    while(I2C1CON & 0x001F); //wait for the module to be ready (see 19.5.6 of dsPIC33 reference manual 12C)
    I2C1CONbits.RSEN = 1;
    while(I2C1CONbits.RSEN); //wait for slave to respond
    //__delay_us(100);

    /*send device address (read/write set) */
    result = I2C_waitForEndTransmision(result, SlaveAddress | 0x1);
    result = I2C_waitForAck(result);

    /*receive data from device*/
    result = I2C_waitForDataReady(result);
    Data = I2C1RCV;

    //NotAckI2C1();
    I2C1CONbits.ACKDT = 1; // on veut un NACK
    I2C1CONbits.ACKEN = 1; // on envoit le NACK
    while(I2C1CONbits.ACKEN);
    printf("\n\renvoit nack OK");

    /*generate stop bus event*/
    while(I2C1CON & 0x001F); //wait for the module to be ready (see 19.5.5 of dsPIC33 reference manual 12C)
    result = I2C_waitForStop(result);
    printf("\n\renvoit stop OK");

    /*done!*/
        
    if (result == true)
    {
        printf("\n\rByteReadI2C : read operation succeed, Data = %d", Data);
    }
    else
    {
         printf("\n\rByteReadI2C : read operation failed");
    }
    
    return Data;
}

void ByteWriteI2C(unsigned char SlaveAddress, unsigned char SlaveRegister, unsigned char Data)
{
    bool result = true;
    
    printf("\n\n\n\rByteWriteI2C : send data 0x%x at 0x%x for slave 0x%x", Data, SlaveRegister, SlaveAddress);
    
   /*begin start condition */
    result = I2C_waitForStart(result);

    /*send device address (r/w cleared) */
    result = I2C_waitForEndTransmision(result, SlaveAddress);
    result = I2C_waitForAck(result);
    //IdleI2C1();

    /*send slave register address*/
    result = I2C_waitForEndTransmision(result, SlaveRegister);
    result = I2C_waitForAck(result);
    //IdleI2C1();

    /*send data */
    result = I2C_waitForEndTransmision(result, Data);
    result = I2C_waitForAck(result);

    /*generate stop bus event*/
    while(I2C1CON & 0x001F); //wait for the module to be ready (see 19.5.5 of dsPIC33 reference manual 12C)
    result = I2C_waitForStop(result);
    
    if (result == true)
    {
        printf("\n\rByteWriteI2C : write operation succeed");
    }
    else
    {
         printf("\n\rByteWriteI2C : write operation failed");
    }

    /*done!*/
}

//unsigned char ByteReadI2C(unsigned char SlaveAddress, unsigned char SlaveRegister) 
//{
//    unsigned char Data;
//
//    /*begin start condition */
//    //IdleI2C1(); //wait for bus to be idle
//    printf("\n\rDEBUT TRANSMISION I2C (read adress %x of slave %x", SlaveRegister, SlaveAddress);
//    
//    while(I2CSTATbits.P != 1)
//    {
//        printf("\n\rwait for idle");
//    }
//    IFS1bits.MI2C1IF = 0; //clear interrupt flag
//    I2C1CONbits.RSEN = 0; //write
//    I2C1CONbits.SEN = 1; //set start event
//    while(I2C1CONbits.SEN); //wait for the startup sequence to complete
//
//    /*send device address (r/w cleared) */
//    I2C1TRN = SlaveAddress;
//    IFS1bits.MI2C1IF = 0;
//    I2C1STATbits.ACKSTAT = 0;
//    while(I2C1STATbits.TBF); //wait for data to clock out
//    while(I2C1STATbits.ACKSTAT); //wait for device to acknowledge
//    //IdleI2C1();
//    while(I2C1STATbits.TRSTAT == 1);
//
//    /*send slave register address*/
//    I2C1TRN = SlaveRegister; //status register of RTC
//    // Clear ackstat
//    I2C1STATbits.ACKSTAT = 0;
//    while(I2C1STATbits.TBF == 1); //wait for data to clock out
//    while(I2C1STATbits.ACKSTAT); //wait for device to acknowledge
//    printf("\n\rack received");
//    while(I2C1STATbits.TRSTAT == 1); //IdleI2C1();
//    printf("\n\rsend register adress OK");
//
//    /*initiate a repeated start*/
//    //__delay_us(100);
//    while(I2C1CON & 0x001F); //wait for the module to be ready (see 19.5.6 of dsPIC33 reference manual 12C)
//    I2C1CONbits.RSEN = 1;
//    while(I2C1CONbits.RSEN); //wait for slave to respond
//    //__delay_us(100);
//    printf("\n\rrestart OK");
//
//    /*send device address (read/write set) */
//    I2C1TRN = SlaveAddress | 0x1;
//    IFS1bits.MI2C1IF = 0;
//    while(I2C1STATbits.TBF); //wait for data to clock out
//    while(I2C1STATbits.ACKSTAT); //wait for device to acknowledge
//    while(I2C1STATbits.TRSTAT == 1); //IdleI2C1();
//    printf("\n\rsend slave adress OK");
//
//    /*receive data from device*/
//    I2C1CONbits.RCEN = 1; //receive enable (start clocking for slave transfer)
//    while(I2C1CONbits.RCEN); //wait for data (NOTE: can also consider polling RBF bit here)
//    Data = I2C1RCV;
//    while(I2C1STATbits.TRSTAT == 1); //IdleI2C1();
//    printf("\n\rreceive data OK");
//
//    //NotAckI2C1();
//    I2C1CONbits.ACKDT = 1; // on veut un NACK
//    I2C1CONbits.ACKEN = 1; // on envoit le NACK
//    while(I2C1CONbits.ACKEN);
//    printf("\n\renvoit nack OK");
//
//    /*generate stop bus event*/
//    IFS1bits.MI2C1IF = 0;
//    while(I2C1CON & 0x001F); //wait for the module to be ready (see 19.5.5 of dsPIC33 reference manual 12C)
//    I2C1CONbits.PEN = 1;
//    while(IFS1bits.MI2C1IF == 0);
//    printf("\n\renvoit stop OK");
//
//    /*done!*/
//    printf("\n\rData received : %d", Data);
//    return Data;
//}

//void ByteWriteI2C(unsigned char SlaveAddress, unsigned char SlaveRegister, unsigned char Data)
//{
//   /*begin start condition */
//    //while(I2C1STATbits.P == 1); //make sure the bus is idle
//    //IdleI2C1();
//    IFS1bits.MI2C1IF = 0; //clear interrupt flag
//    I2C1CONbits.SEN = 1; //set start event
//    while(IFS1bits.MI2C1IF == 0); //wait for interrupt flag to signify end of start condition
//
//    /*send device address (r/w cleared) */
//    I2C1TRN = SlaveAddress;
//    IFS1bits.MI2C1IF = 0;
//    while(I2C1STATbits.TBF); //wait for data to clock out
//    while(I2C1STATbits.ACKSTAT); //wait for device to acknowledge
//    //IdleI2C1();
//
//    /*send slave register address*/
//    I2C1TRN = SlaveRegister; //status register of RTC
//    IFS1bits.MI2C1IF = 0;
//    while(I2C1STATbits.TBF); //wait for data to clock out
//    while(I2C1STATbits.ACKSTAT); //wait for device to acknowledge
//    //IdleI2C1();
//
//    /*send data */
//    I2C1TRN = Data; //status register of RTC
//    IFS1bits.MI2C1IF = 0;
//    while(I2C1STATbits.TBF); //wait for data to clock out
//    while(I2C1STATbits.ACKSTAT); //wait for device to acknowledge
//    //IdleI2C1();
//
//    /*generate stop bus event*/
//    IFS1bits.MI2C1IF = 0;
//    while(I2C1CON & 0x001F); //wait for the module to be ready (see 19.5.5 of dsPIC33 reference manual 12C)
//    I2C1CONbits.PEN = 1;
//    while(IFS1bits.MI2C1IF == 0);
//
//    /*done!*/
//}

//void config_i2c ()
//{
//    I2C1CONbits.A10M = 0;           // Slave : 7 bit adress
//    //I2C1BRG = 187;                  // F = 400 kHz
//    I2C1BRG = 0x188;                  // F = 400 kHz
//     
//    I2C1CONbits.I2CEN = 1;           //Enable I2C  
//}
//
//void i2c_debut_transmission()
//{
//    I2C1CONbits.SEN = 1;                // Envoi un bit de start
//    //while(I2C1CONbits.SEN == 0){
//    //    PutcUART(UART_USB, '%');
//    //}        // Attend qu'il soit fini
//    
//    PutsUART(UART_USB, " START 1");
//    
//    I2C1TRN = 0x3C;                     // Adresse de l'esclave
//    while(I2C1STATbits.TRSTAT);         // Attend que l'envoi soit fini TRN + ACK.    Passe à 0 quand fini 
//}
//
//void i2c_debut_reception()
//{
//    I2C1TRN = 0x3D;                     // Demande de reception
//    while(I2C1STATbits.TRSTAT);         // Attend que l'envoi soit fini TRN + ACK.    Passe à 0 quand fini 
//
//    I2C1CONbits.RCEN = 1;               //Initie reception de 8 bits
//    while(I2C1CONbits.RCEN);
//}
//
//void i2c_fin_transmission(bool condition)
//{
//    if (condition == true)
//    {
//        I2C1CONbits.PEN = 1;                // Envoi un bit de stop  
//        //while(I2C1CONbits.PEN == 0);
//
//    }
//    else
//    {
//        I2C1CONbits.RSEN = 1;               // Envoi un bit de start
//        //while(I2C1CONbits.RSEN == 0);       // Attend qu'il soit fini
//    }
//}
//
//void i2c_ecriture_octet (uint8_t octet)
//{
//    I2C1TRN = octet;                    // Envoi de la data 
//    while(I2C1STATbits.TRSTAT);         // Attend que l'envoi soit fini TRN + ACK.    Passe à 0 quand fini 
//    delay_ms(10);
//}
//
//void i2c_envoi_octet (uint8_t adresse, uint8_t octet)
//{
//    //PutsUART(UART_USB, "START");
//    i2c_debut_transmission();
//    //PutsUART(UART_USB, " Envoit adresse");
//    i2c_ecriture_octet(adresse);  
//    //PutsUART(UART_USB, " Envoi octet");
//    i2c_ecriture_octet(octet);
//    i2c_fin_transmission(true);
//    //delay_ms(10);
//}
//
//void i2c_envoi_multiple_octets(uint8_t adresse, uint8_t *octet)
//{
//    i2c_debut_transmission();
//    i2c_ecriture_octet(adresse);
//    i2c_ecriture_octet(*octet);
//    
//    while (*++octet)
//    {
//        i2c_fin_transmission(false);
//        i2c_ecriture_octet(*octet);
//    }
//    i2c_fin_transmission(true);
//}
//
//int8_t i2c_lecture_octet (bool derniere_valeur)
//{
//    int8_t valeur;
//    //while(I2C1STATbits.RBF == 0)
//        {
//        PutcUART(UART_USB, '¨');
//    }
//    valeur = I2C1RCV;
//    PutsUART(UART_USB, " Valeur acquise");
//    
//    
//    if (derniere_valeur == true)
//    {
//        I2C1CONbits.ACKDT = 1;          //Se prépare à un envoyer un NACK qui signifie la fin de la com.
//        I2C1CONbits.ACKEN = 1;          //Envoi le NACK
//        i2c_fin_transmission(true);
//    }
//    else
//    {
//        I2C1CONbits.ACKDT = 0;          //Se prépare à un envoyer un ACK pour continuer la reception.
//        I2C1CONbits.ACKEN = 1;          //Envoi le NACK
//    }
//    return valeur;
//    
//}
//
////void i2c_reception_octet
//
////int16_t receive_i2c(uint8_t adresse)
////{
////    uint8_t value, compteur = 0;
////    
////    //adressage_i2c(adresse);
////    
////    I2C1CONbits.RSEN = 1;               // Restart
////    while(I2C1CONbits.RSEN == 0);       // Attend qu'il soit fini
////    
////    I2C1TRN = 0x3D;                     // Demande de reception
////    while(I2C1STATbits.TRSTAT);         // Attend que l'envoi soit fini TRN + ACK.    Passe à 0 quand fini 
////
////    I2C1CONbits.RCEN = 1;               //Initie reception de 8 bits
////    while(I2C1CONbits.RCEN);
////       
////    while(I2C1STATbits.RBF == 0 && compteur < 20)
////    {
////        delay_ms(10);
////        compteur++;
////    }
////    
////    if (compteur >= 20)
////    {
////         I2C1CONbits.PEN = 1;           //Envoi un bit de stop
////        while(I2C1CONbits.PEN == 0);
////        return -1;
////    }
////    else 
////    {
////        value = I2C1RCV;                //On récupère la donnée
////
////        I2C1CONbits.ACKDT = 1;          //Se prépare à un envoyer un NACK qui signifie la fin de la com.
////        I2C1CONbits.ACKEN = 1;          //Envoi le NACK
////
////        I2C1CONbits.PEN = 1;             // Envoi un bit de stop  
////        while(I2C1CONbits.PEN == 0);
////    
////    }
////    return (int16_t) value; 
////}
//
//
//void config_boussole()
//{
//    i2c_envoi_octet(0x00, 0x70);       // 8-average, 15Hz
//    i2c_envoi_octet(0x01, 0xA0);       // Gain = 15
//    //i2c_envoi_octet(0x02, 0x00);       // Mode continu   
//}
//
//void recup_valeur()
//{
//    int16_t X, Y, Z;
//    double angle;
//    
//    //PutsUART(UART_USB, "\n\rDebut transmission octet");
//    i2c_debut_transmission();
//    //PutsUART(UART_USB, " START");
//    i2c_ecriture_octet(0x03);  
//    //PutsUART(UART_USB, " adresse transimise");
//    i2c_fin_transmission(false);
//    //PutsUART(UART_USB, " Octet Tranmis\n\r");
//    
//    i2c_debut_reception();
//    //PutsUART(UART_USB, "\n\rDebut récetion");
//    PutIntUART(UART_USB, i2c_lecture_octet(false));
//    PutIntUART(UART_USB, i2c_lecture_octet(true));
//    
//    //X= i2c_lecture_octet(false)<<8|i2c_lecture_octet(false);
//    //Z= i2c_lecture_octet(false)<<8|i2c_lecture_octet(false);
//    //Y= i2c_lecture_octet(false)<<8|i2c_lecture_octet(true);
//
////    angle= atan2(Y,X) * (180 / 3.14159265); // angle en degres
////    if (angle<0) 
////    {
////        angle=angle+360;
////    }
////    
////    PutLongUART(UART_USB, angle);
//    PutsUART(UART_USB, "\r\n");
//    delay_ms(6);
//    
//}
//
//void setup_boussole()
//{
//    ByteWriteI2C(0x1E, 0x00, 0x70);
//    ByteWriteI2C(0x1E, 0x01, 0xA0);
//}
//
//void get_value()
//{
//    ByteWriteI2C(0x1E, 0x02, 0x01);
//    PutIntUART(UART_USB, ByteReadI2C(0x1E, 0X03));
//    
//}



