/* 
 * File:   interruptions.h
 * Author: Quentin
 *
 * Created on 18 mars 2015, 23:22
 */

#ifndef INTERRUPTIONS_H
#define	INTERRUPTIONS_H

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
     * @brief Fonction qui vérifie toutes les interruptions et si elles sont concernés par la priorité
     * @param priorite : priorité de l'interruption : PRIORITE_HAUTE, PRIORITE_BASSE
     */
    void subroutine_interruptions (bool priorite);


    /**
     * Interruption du timer 0
     */
    void interruption_timer0 ();


    /**
     * Interruption du timer 1
     */
    void interruption_timer1 ();


    /**
     * Interruption du timer 2
     */
    void interruption_timer2 ();


    /**
     * Interruption du timer 5
     */
    void interruption_timer5 ();


    /**
     * Interruption Externe 0
     */
    void interruption_INT0();

    /**
     * Interruption Externe 1
     */
    void interruption_INT1();


    /**
     * Interruption Externe 2
     */
    void interruption_INT2();


    /***** Interruptions QEI */

    /**
     * Interruption des codeurs lors d'un overflow
     */
    void interruption_QEI();


    /**
     * Interruption des codeurs lors d'un changemenr de sens
     */
    void interuption_QEI_sens();


    /***** Interrptions ADC */

    /**
     * Interruption de l'ADC
     */
    void interrupt_ADC ();

    /***** Interrptions Uart */

    /**
     * Interruption sur RX
     */
    void interrupt_RX();

    /**
     * Interruption en TX
     */
    void interrupt_TX();


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


#ifdef	__cplusplus
}
#endif

#endif	/* INTERRUPTIONS_H */

