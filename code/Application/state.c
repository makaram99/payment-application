/********************************************************************************
 * @file    state.c
 * @author  Mahmoud Karam Emara (ma.karam272@gmail.com)
 * @brief   This file contains the state machine functions.
 * @version 1.0.0
 * @date    2022-07-28
 * 
 * @copyright Copyright (c) 2022
 * 
 ********************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include "../macros.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Server/server.h"
#include "state.h"


/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                        PRIVATE FUNCTION DECLARATIONS                        */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

/********************************************************************************
 * @brief   This function is used to give the user another try to enter the
 *          correct data. It uses a timeout to limit the number of tries.
 * 
 * @return  TRUE if the user wants to try again, FALSE otherwise.
 *******************************************************************************/
static BOOL_t giveAnotherTry(void);


/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                       PRIVATE VARIABLES AND FUNCTIONS                       */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

/********************************************************************************
 * @brief   Timeout for the user to enter the correct data.
 *******************************************************************************/
static uint8_t timeout = 0;

/********************************************************************************
 * @brief   Maximum number of tries for the user to enter the correct data.
 *******************************************************************************/
#define MAX_TIMEOUT  3


/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                              GLOBAL VARIABLES                               */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

STATE_MACHINE_t stateMachine[]= {
    {.state = STATE_MACHINE_CARD    , .name = "STATE_MACHINE_CARD"      , .func = appCard       },
    {.state = STATE_MACHINE_TERMINAL, .name = "STATE_MACHINE_TERMINAL"  , .func = appTerminal   },
    {.state = STATE_MACHINE_SERVER  , .name = "STATE_MACHINE_SERVER"    , .func = appServer     },
};

uint8_t countStates = sizeof(stateMachine) / sizeof(stateMachine[0]);

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                          PUBLIC FUNCTION DEFINITIONS                        */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

BOOL_t appCard(ST_transaction_t * const transData) {
    EN_cardError_t cardError;

    /*!< Getting Card Holder Name */
    timeout = 0;
    while(1) {
        cardError = getCardHolderName( &(transData->cardHolderData) );

        if(WRONG_NAME != cardError){
            printf("Name: %s\n", transData->cardHolderData.cardHolderName);
            break;
        }

        printf("Invalid name. (Card Error: %d)\n", cardError);
        if(FALSE == giveAnotherTry()) {
            return FALSE;
        }
    }


    /*!< Getting Card Expiry Date */
    timeout = 0;
    while(1) {
        cardError = getCardExpiryDate( &(transData->cardHolderData) );
        if(WRONG_EXP_DATE != cardError) {
            printf("Expiry date: %s\n", transData->cardHolderData.cardExpirationDate);
            break;
        }

        printf("Invalid expiry date. (Card Error: %d)\n", cardError);
        if(FALSE == giveAnotherTry()) {
            return FALSE;
        }
    }

    /*!< Getting Card PAN */
    timeout = 0;
    while(1) {
        cardError = getCardPAN( &(transData->cardHolderData) );
        if(WRONG_PAN != cardError) {
            printf("PAN: %s\n", transData->cardHolderData.primaryAccountNumber);
            break;
        }

        printf("Invalid PAN. (Card Error: %d)\n", cardError);
        if(FALSE == giveAnotherTry()) {
            return FALSE;
        }
    } 

    return TRUE;
}

BOOL_t appTerminal(ST_transaction_t * const transData) {
    EN_terminalError_t termError;

    /*!< Getting Transaction Date */
    timeout = 0;
    while(1) {
        termError = getTransactionDate( &(transData->terminalData) );
        if(WRONG_DATE != termError) {
            printf("Date: %s\n", transData->terminalData.transactionDate);
            break;
        }

        printf("Invalid date. (Terminal Error: %d)\n", termError);
        if(FALSE == giveAnotherTry()) {
            return FALSE;
        }
    } 

    /*!< Checking if the card is expired */
    termError = isCardExpired(transData->cardHolderData, transData->terminalData);
    if(EXPIRED_CARD == termError) {
        printf("Expired card (Terminal Error: %d)\n", termError);
        return FALSE;
    } else {
        printf("Card is not expired\n");
    }

    /*!< Getting Maximum Transaction Amount */
    timeout = 0;
    while(1) {
        termError = setMaxAmount( &(transData->terminalData) );
        if(INVALID_MAX_AMOUNT != termError) {
            printf("Maximum amount: %.0f\n", transData->terminalData.maxTransAmount);
            break;
        }

        printf("Invalid maximum amount. (Terminal Error: %d)\n", termError);
        if(FALSE == giveAnotherTry()) {
            return FALSE;
        }
    }

    /*!< Getting Transaction Amount */
    timeout = 0;
    while(1) {
        termError = getTransactionAmount( &(transData->terminalData) );
        if(INVALID_AMOUNT != termError) {
            printf("Amount: %.0f\n", transData->terminalData.transAmount);

            /* Validating the required amount   */
            termError = isBelowMaxAmount( &(transData->terminalData) );
            if(EXCEED_MAX_AMOUNT == termError) {
                printf("Exceeds maximum amount. (termError %d)\n", termError);
                if(FALSE == giveAnotherTry()) {
                    return FALSE;
                } else {
                    timeout = 0;
                    continue;
                }
            } else {                
                printf("Below max amount\n");
                break;
            }
        } else {    /* Invalid amount */
            printf("Invalid amount. (Terminal Error: %d)\n", termError);
            if(FALSE == giveAnotherTry()) {
                return FALSE;
            }
        }
    } 

    return TRUE;
}

BOOL_t appServer(ST_transaction_t * const transData) {
    EN_transState_t transactionError;

    transactionError = recieveTransactionData(transData);
    if(APPROVED == transactionError) {
        printf("Approved Transaction of: %.0f\n", transData->terminalData.transAmount);
    } else {
        printf("Disapproved transsaction. (Transaction Error %d)\n", transactionError);
        return FALSE;
    }

    return TRUE;
}

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                        PRIVATE FUNCTION DEFINITIONS                         */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

static BOOL_t giveAnotherTry(void) {
    char tryAgain = 1;

    ++timeout;
    if(timeout >= MAX_TIMEOUT) {
        printf("Exceeded number of tries\n");
        timeout = 0;
        return FALSE;
    }    

    printf("\nTry again? (y/n): ");
    scanf(" %c%*c", &tryAgain);
    tryAgain = toupper(tryAgain);

    return ('Y' == tryAgain) ? TRUE : FALSE;
}

