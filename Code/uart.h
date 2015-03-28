/* 
 * File:   uart.h
 * Author: Quentin
 *
 * Created on 18 mars 2015, 21:30
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

/******************************************************************************/
/***************************** Defines ****************************************/
/******************************************************************************/


/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

/**
 * @brief	: Fonction permettant d'initialiser les UART
 * @param	: channel	: channel a initialiser (UART_XBEE | UART_AX12)
 * @param	: baud		: vitesse de communication
 */
void config_uart (uint16_t vitesse_com);


/**
 * @ brief      : Fonction qui permet de calculer la veleur � mettre dans le registre
 *                pour changer le baud rate de l'uart
 *
 * @param       : baud          : valeur souhait�e
 * @return      : valeur � mettre dans le registre
 */
uint16_t calcul_baud (uint32_t baud);

/**
 * @brief	: Fonction permettant d'envoyer un caractere sur la liaison serie
 * @param	: channel	: channel a initialiser (UART_XBEE | UART_AX12)
 * @param	: octet		: octet a envoyer
 */
void PutcUART (uint8_t octet);


/**
 * @brief	: Fonction permettant d'envoyer une chaine de caractere sur la liaison serie
 * @param	: chaine	: chaine a envoyer
 */
void PutsUART (const char *chaine);


/**
 * @brief	: Fonction permettant d'envoyer une chaine de caractere ou une partie sur la liaison serie (utile pour les AX12 par exemple)
 * @param	: chaine		: chaine a envoyer
 * @param	: taille_chaine	: taille de la chaine ou nombre d'octet a envoyer
 */
void Puts2UART (uint8_t *chaine, uint16_t taille_chaine);


/**
 * @brief	: Fonction permettant de convertir un int en chaine de caractere
 * @param	: nb	: nombre a convertir
 * @return	: une chaine contenant le nombre
 */
char *IntToC (int16_t nb);


/**
 * @brief	: Fonction permettant de convertir un long en chaine de caractere
 * @param	: nb	: nombre a convertir
 * @return	: une chaine contenant le nombre
 */
char *LongToC (int32_t nb);


/**
 * @brief	: Fonction permettant d'afficher un int sur la console
 * @param	: nb	: nombre a afficher
 */
void PutIntUART (int16_t nb);


/**
 * @brief	: Fonction permettant d'afficher un long sur la console
 * @param	: nb	: nombre a afficher
 */
void PutLongUART (int32_t nb);


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */
