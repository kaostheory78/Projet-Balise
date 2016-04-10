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
#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

