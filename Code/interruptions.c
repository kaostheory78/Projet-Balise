/******************************************************************************/
/************************ Carte Balise - PIC18f4431 ***************************/
/******************************************************************************/
/* Fichier 	: interruptions.c
 * Auteur  	: M1E11
 * Revision	: 1.0
 * Date		: 18 Mars 2015, 22:09
 *******************************************************************************
 *
 *  
 ******************************************************************************/


/******************************************************************************/
/******************************* INCLUDES *************************************/
/******************************************************************************/

#include "system.h"


/******************************************************************************/
/*********************** Vecteurs interruptions L et H ************************/
/******************************************************************************/


//Interruptions de priorités hautes
void interrupt high_isr(void)
{
    subroutine_interruptions(PRIORITE_HAUTE);
}

//Interruptions de priorités basses
void low_priority interrupt low_isr(void)
{
    subroutine_interruptions(PRIORITE_BASSE);
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


void subroutine_interruptions (bool priorite)
{
    if ( FLAG_QEI == true && ACTIV_INTER_QEI == true && priorite == PRIO_INTER_QEI)
    {
        interruption_QEI();
    }
    else if ( FLAG_QEI_SENS == true && ACTIV_INTER_QEI_SENS == true && priorite == PRIO_INTER_QEI_SENS)
    {
        interuption_QEI_sens();
    }
    else if ( FLAG_TIMER0 == true && ACTIV_INTER_TIMER0 == true && priorite == PRIO_INTER_TIMER0)
    {
        interruption_timer0();
    }
    else if ( FLAG_TIMER1 == true && ACTIV_INTER_TIMER1 == true && priorite == PRIO_INTER_TIMER1)
    {
        interruption_timer1();
    }
    else if ( FLAG_TIMER2 == true && ACTIV_INTER_TIMER2 == true && priorite == PRIO_INTER_TIMER2)
    {
        interruption_timer2();
    }
    else if (FLAG_INT0 == true && ACTIV_INTER_INT0 == true && priorite == PRIO_INTER_INT0)
    {
        interruption_INT0();
    }
//    else if (FLAG_INT1 == true && ACTIV_INTER_INT1 == true && priorite == PRIO_INTER_INT1)
//    {
//        interruption_INT1();
//    }
//    else if (FLAG_INT2 == true && ACTIV_INTER_INT2 == true && priorite == PRIO_INTER_INT2)
//    {
//        interruption_INT2();
//    }
//    else if ( FLAG_TIMER5 == true && ACTIV_INTER_TIMER5 == true && priorite == PRIO_INTER_TIMER5)
//    {
//        interruption_timer5();
//    }
//    else if ( FLAG_ADC == true && ACTIV_INTER_ADC == true && priorite == PRIO_INTER_ADC)
//    {
//        interrupt_ADC();
//    }
    else if ( FLAG_RX == true && ACTIV_INTER_RX == true && priorite == PRIO_INTER_RX)
    {
        interrupt_RX();
    }
    else if (FLAG_TX == true && ACTIV_INTER_TX == true && priorite == PRIO_INTER_TX)
    {
        interrupt_TX();
    }

}



/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/***** Interruptions Timers */

void interruption_timer0 ()
{
    FLAG_TIMER0 = false;
    TMR0L = 131;

    asserv();
}

void interruption_timer1()
{
    FLAG_TIMER1 = false;
    TMR1 =  65535  - 810; 
        
    if (uart_logiciel.transmission_en_cours == true)
    {
        TX_LOGICIEL = uart_logiciel.buffer_uart[uart_logiciel.indice];
        uart_logiciel.indice++;

        if (uart_logiciel.indice > 10)
            uart_logiciel.transmission_en_cours = false;
    }
    else
        TX_LOGICIEL = 1;
}

void interruption_timer2()
{
    FLAG_TIMER2 = false;
}

void interruption_timer5()
{
    FLAG_TIMER5 = false;
}

/***** Interruptions Externes */

void interruption_INT0()
{
    FLAG_INT0 = false;
}

void interruption_INT1()
{
    FLAG_INT1 = false;
}

void interruption_INT2()
{
    FLAG_INT2 = false;
}

/***** Interruptions QEI */

void interruption_QEI()
{
    FLAG_QEI = false;
    QEICONbits.QERR = 0;

    if (QEICONbits.UPDOWN == 1)
    {
        OVERFLOW_CODEUR++;
    }
    else
    {
        OVERFLOW_CODEUR--;
    }

}

void interuption_QEI_sens()
{
    FLAG_QEI_SENS = false;
}

/***** Interrptions ADC */

void interrupt_ADC ()
{
    FLAG_ADC = false;
}

/***** Interrptions Uart */

void interrupt_RX()
{
    FLAG_RX = false;
    LED0 = 1;
    
    if (uart_reception.indice < 100)
    {
        uart_reception.buffer_reeption[uart_reception.indice] = RCREG;
        uart_reception.buffer_vide = false;
        uart_reception.indice++;
    }
    else
    {
        uart_reception.buffer_plein = true;
        uint8_t temp = RCREG;
    }
    //CREN = 1;
}

void interrupt_TX()
{
    FLAG_TX = false;
}