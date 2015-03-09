/******************************************************************************/
/************************ Carte Balise - PIC18f4431 ***************************/
/******************************************************************************/
/* Fichier 	: uart.c
 * Auteur  	: M1E11
 * Revision	: 1.0
 * Date		: 09 Mars 2014, 22:40
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

void config_uart (uint16_t vitesse_com)
{
    //¨Pins initialisé plus tôt

    // ATTENTION : ERRATA
    // IL NE FAUT PAS QUE BRGH = 1, BRG16 = 1 et SPBRG < 4 EN MÊME TEMPS

    // TXSTA
    TXSTAbits.TXEN = 1;     // Activation Transmission
    TXSTAbits.TX9 = 0;      // 8 bits de données, pas de parité
    TXSTAbits.SYNC = 0;     // Transmission Assynchrones
    TXSTAbits.SENDB = 0;    // Break en fin de transmission
    TXSTAbits.BRGH = 1;     // Mode haute vitesse
    TXSTAbits.TX9 = 0;      // 8 bits

    // BAUDCON
    BAUDCONbits.BRG16 = 1;  // 16 bits générateur de bode
    //WUE
    BAUDCONbits.ABDEN = 0;  // Pas d'auto baud

    #ifdef FOSC_40MHz
        SPBRG = 86;         // 115200 bauds : - 0,22 % d'erreur
    #else
        SPBRG = calcul_baud(vitesse_com);             // 115200 bauds : + 0,79 % d'erreur
    #endif

    // RCSTA
    RCSTAbits.SPEN = 1;     // Activation du port série
    RCSTAbits.RX9 = 0;      // Réception 8 bits de donnée, pas de parité
    RCSTAbits.CREN = 1;     // Activation de la réception

}

uint16_t calcul_baud (uint32_t baud)
{
    switch (baud)
    {
        case 115200 :
            return 34;
        default :
            return (uint16_t) (((FCY/baud)/4) - 1);
    }
}

/******************************************************************************/
/*************************** GESTION DE L'UART ********************************/
/******************************************************************************/

/*void vider_buffer_reception_uart (void)
{
    uint8_t buf;
    //while(U2STAbits.URXDA == 1)
    {
        buf = RCREG;
    }
}*/


/******************************************************************************/
/************************* FONCTIONS DE COM UART ******************************/
/******************************************************************************/

void PutcUART (uint8_t octet)
{
    while (PIR1bits.TXIF == 1);
    TXREG = octet;
}

void PutsUART (const char *chaine)
{
    do
    {
        PutcUART (*chaine);
    }
    while (*chaine++);
}

void Puts2UART (uint8_t *chaine, uint16_t taille_chaine)
{
    int i;
    for (i=0; i<taille_chaine; i++)
    {
        PutcUART (chaine[i]);
    }
}


char *IntToC (int16_t nb)
{
    // Delarations locales
    int16_t i[5];
    int j = 0;
    static char out[8];

    // Nombre negatif
    if (nb < 0)
    {
        nb *= (-1);
        out[j++] = '-';
    }

    // Conversion
    //12345 = 1*10000 + 2*1000 + 3*100 + 4*10 + 5
    i[0] = (nb / 10000);
    i[1] = (nb - i[0] * 10000) / 1000;
    i[2] = (nb - i[0] * 10000 - i[1] * 1000) / 100;
    i[3] = (nb - i[0] * 10000 - i[1] * 1000 - i[2] * 100) / 10;
    i[4] = (nb - i[0] * 10000 - i[1] * 1000 - i[2] * 100 - i[3] * 10);

    out[j++] = (unsigned char)(i[0] + 48);
    out[j++] = (unsigned char)(i[1] + 48);
    out[j++] = (unsigned char)(i[2] + 48);
    out[j++] = (unsigned char)(i[3] + 48);
    out[j++] = (unsigned char)(i[4] + 48);
    out[j++] = '\0';

    return out;
}


char *LongToC (int32_t nb)
{
    //4294967296
    //4.294.967.296

    // Declarations locales
    int32_t i[10];
    int j = 0;
    static char out[15];


    // Nombre negatif
    if (nb < 0)
    {
            nb *= (-1);
            out[j++] = '-';
    }

    // Conversion
    //4.294.967.296 = 4*1000000000 + 2*100000000 + 9*10000000 + 4*1000000 + 9*100000 + 6*10000 + 7*1000 + 2*100 + 9*10 + 6

    i[0] = (nb / 1000000000);
    i[1] = (nb - i[0] * 1000000000) / 100000000;
    i[2] = (nb - i[0] * 1000000000 - i[1] * 100000000) / 10000000;
    i[3] = (nb - i[0] * 1000000000 - i[1] * 100000000 - i[2] * 10000000) / 1000000;
    i[4] = (nb - i[0] * 1000000000 - i[1] * 100000000 - i[2] * 10000000 - i[3] * 1000000) / 100000;
    i[5] = (nb - i[0] * 1000000000 - i[1] * 100000000 - i[2] * 10000000 - i[3] * 1000000 - i[4] * 100000) / 10000;
    i[6] = (nb - i[0] * 1000000000 - i[1] * 100000000 - i[2] * 10000000 - i[3] * 1000000 - i[4] * 100000 - i[5] * 10000) / 1000;
    i[7] = (nb - i[0] * 1000000000 - i[1] * 100000000 - i[2] * 10000000 - i[3] * 1000000 - i[4] * 100000 - i[5] * 10000 - i [6] * 1000) / 100;
    i[8] = (nb - i[0] * 1000000000 - i[1] * 100000000 - i[2] * 10000000 - i[3] * 1000000 - i[4] * 100000 - i[5] * 10000 - i [6] * 1000 - i[7] * 100) / 10;
    i[9] = (nb - i[0] * 1000000000 - i[1] * 100000000 - i[2] * 10000000 - i[3] * 1000000 - i[4] * 100000 - i[5] * 10000 - i [6] * 1000 - i[7] * 100 - i[8] * 10);


    out[j++] = (unsigned char)(i[0] + 48);
    out[j++] = (unsigned char)(i[1] + 48);
    out[j++] = (unsigned char)(i[2] + 48);
    out[j++] = (unsigned char)(i[3] + 48);
    out[j++] = (unsigned char)(i[4] + 48);
    out[j++] = (unsigned char)(i[5] + 48);
    out[j++] = (unsigned char)(i[6] + 48);
    out[j++] = (unsigned char)(i[7] + 48);
    out[j++] = (unsigned char)(i[8] + 48);
    out[j++] = (unsigned char)(i[9] + 48);
    out[j++] = '\0';

    return out;
}

void PutIntUART (int16_t nb)
{
    PutsUART (IntToC (nb));
}

void PutLongUART (int32_t nb)
{
    PutsUART (LongToC (nb));
}


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/