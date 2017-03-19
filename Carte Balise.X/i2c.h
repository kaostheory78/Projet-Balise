/* 
 * File:   i2c.h
 * Author: Quentin
 *
 * Created on 20 février 2016, 18:16
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"
    
/******************************************************************************/
/***************************** Defines ****************************************/
/******************************************************************************/

#define TIMER_I2C_MAX_MS        50
    
    typedef enum
    {
        NO_EVENT,                           // 0
        WAITING_FOR_START,                  // 1
        WAITING_FOR_STOP,                   // 2
        WAITING_FOR_ACK,                    // 3
        WAITING_FOR_NACK,                   // 4
        WAITING_FOR_RECEPTION,
        WAITING_FOR_TRANSMITION,            // 5
        WAITING_FOR_END_TRANSMITION,        // 6
        WAITING_FOR_IDLE,                   // 7
        MAX_WAITING_EVENT                   // 8
    }I2C_CMD_EVENT;
    
    typedef enum
    {
        ACK_RECEIVED        = 0b0000000000000001, 
        NACK_RECEIVED       = 0b0000000000000010,                      
        START_RECEIVED      = 0b0000000000000100,                     
        STOP_RECEIVED       = 0b0000000000001000,                      
        START_TRANSMITION   = 0b0000000000010000,                  
        END_TRANSMISION     = 0b0000000000100000,                    
        DATA_READY          = 0b0000000001000000,                         
        ENTER_IDLE          = 0b0000000010000000,   
        BUS_COLLISION       = 0b0000000100000000,
        WRITE_COLLISION     = 0b0000001000000000,
        RX_BUFFER_FULL      = 0b0000010000000000,
        TX_BUFFER_FULL      = 0b0000100000000000,
        TIMER_TIMEOUT       = 0b0001000000000000,
        ERROR_OCCURED       = 0b1000000000000000,                                        
    }I2C_RECEIVE_EVENT;
    
    typedef struct
    {
        I2C1STATBITS statusRegister;
        I2C_CMD_EVENT cmdEvent;
        I2C_RECEIVE_EVENT receiveEvent;
        bool isTimerStarted;
        int8_t waitingEventTimer;
    }_i2c_cmd;

/******************************************************************************/
/*************************** Variables Globales *******************************/
/******************************************************************************/
    

/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

    
    void config_i2c ();

    void i2c_debut_transmission();

    void i2c_debut_reception();

    void i2c_fin_transmission(bool condition);

    void i2c_ecriture_octet (uint8_t octet);

    void i2c_envoi_octet (uint8_t adresse, uint8_t octet);

    void i2c_envoi_multiple_octets(uint8_t adresse, uint8_t *octet);

    int8_t i2c_lecture_octet (bool derniere_valeur);

    void config_boussole();

    void recup_valeur();     
    
    void setup_boussole();
    
    void get_value();
    
    void setupI2C(long unsigned int i2cfreq);
    
    unsigned char ByteReadI2C(unsigned char SlaveAddress, unsigned char SlaveRegister) ;
    
    void ByteWriteI2C(unsigned char SlaveAddress, unsigned char SlaveRegister, unsigned char Data);
    
    
    void i2c_interupt();
    bool I2C_waitForIdle(bool frameStillOk);
    bool I2C_waitForStart(bool frameStillOk);
    bool I2C_waitForStop(bool frameStillOk);
    bool I2C_waitForAck(bool frameStillOk);
    bool I2C_waitForNack(bool frameStillOk);
    bool I2C_waitForTransmisionReady(bool frameStillOk);
    bool I2C_waitForEndTransmision(bool frameStillOk, uint8_t dataToSend);
    bool I2C_waitForDataReady(bool frameStillOk);
    void I2C_printError();
    
    
#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

