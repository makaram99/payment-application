/********************************************************************************
 * @file    state.h
 * @author  Mahmoud Karam Emara (ma.karam272@gmail.com)
 * @brief   This file contains the state machine interfaces for the application.
 *          The state machine is implemented in the \ref state.c file.
 * @version 1.0.0
 * @date    2022-07-28
 * 
 * @copyright Copyright (c) 2022
 * 
 ********************************************************************************/

#ifndef STATE_H
#define STATE_H

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                              TYPE DEFINITIONS                               */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

/********************************************************************************
 * @brief   Enum for the states in the state machine.
 *******************************************************************************/
typedef enum SYSTEM_STATE_t {
    STATE_MACHINE_CARD,             /*!< State to get the card data. */
    STATE_MACHINE_TERMINAL,         /*!< State to get the terminal data and process it */  
    STATE_MACHINE_SERVER,           /*!< State to get the server data and process it */
} SYSTEM_STATE_t;

/********************************************************************************
 * @brief   Struct for the state machine.
 *******************************************************************************/
typedef struct STATE_MACHINE_t {
    char *name;                     /*!< Name of the state. */
    SYSTEM_STATE_t state;           /*!< State of the state machine. */
    BOOL_t (*func)(ST_transaction_t * const transData);    /*!< Function pointer to the state function. */
} STATE_MACHINE_t;

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                              GLOBAL VARIABLES                               */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

/********************************************************************************
 * @brief   Contains the different state machine for the application.
 *          The order of the states is important as it will be executed in the
 *          same order.
 *******************************************************************************/
extern STATE_MACHINE_t stateMachine[];

/********************************************************************************
 * @brief   Number of states in the state machine.
 *******************************************************************************/
extern uint8_t countStates;

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                          PUBLIC FUNCTION DECLARATIONS                       */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

/********************************************************************************
 * @brief       Function used in state machine to process the card data.
 * 
 * @param[out]  transData: Pointer to the transaction data.
 * @return      BOOL_t: TRUE if the state was executed successfully, 
 *                      FALSE otherwise.
 * @warning     This function must be called from the state machine only.
 *******************************************************************************/
BOOL_t appCard(ST_transaction_t * const transData);

/********************************************************************************
 * @brief       Function used in state machine to process the terminal data.
 * 
 * @param[out]  transData: Pointer to the transaction data.
 * @return      BOOL_t: TRUE if the state was executed successfully, 
 *                      FALSE otherwise.
 * @warning     This function must be called only after the card data has been
 *              processed.
 * @warning     This function must be called from the state machine only.
 *******************************************************************************/
BOOL_t appTerminal(ST_transaction_t * const transData);

/********************************************************************************
 * @brief       Function used in state machine to process the server data.
 * 
 * @param[out]  transData: Pointer to the transaction data.
 * @return      BOOL_t: TRUE if the state was executed successfully, 
 *                      FALSE otherwise.
 * @warning     This function must be called only after the card and terminal
 *              states have been executed.
 * @warning     This function must be called from the state machine only.
 *******************************************************************************/
BOOL_t appServer(ST_transaction_t * const transData);


#endif      /* STATE_H */