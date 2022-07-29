/*********************************************************************************
 * @file    appTest.c
 * @author  Mahmoud Karam Emara (ma.karam272@gmail.com)
 * @brief   This file contains the application test module implementation.
 * @details This module is used to test the application module. 
 *          It is not part of the application.
 *          To test application units, uncomment the corresponding line in the
 *          main function. UNCOMMENT ONLY ONE LINE AT A TIME.
 * 
 * @version 1.0.0
 * @date    2022-07-29
 * 
 * @copyright Copyright (c) 2022
 * 
 ********************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "../macros.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Server/server.h"
#include "app.h"


#define MAX_TIMEOUT   3


/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                            TEST FUNCTION PROTOTYPES                         */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

BOOL_t testGetCardHolderName(ST_cardData_t * const cardData);
BOOL_t testGetCardExpiryDate(ST_cardData_t * const cardData);
BOOL_t testGetCardPan(ST_cardData_t * const cardData);
BOOL_t testGetTransactionDate(ST_terminalData_t * const termData);
BOOL_t testIsExpiredCard(ST_cardData_t * const cardData, ST_terminalData_t * const termData);
BOOL_t testGetTransactionAmount(ST_terminalData_t * const termData);
BOOL_t testSetMaxAmount(ST_terminalData_t * const termData);
BOOL_t testIsBelowMaxAmount(ST_terminalData_t * const termData);
BOOL_t testIsValidAccount(ST_transaction_t * const transData);
BOOL_t testIsAmountAvailable(ST_transaction_t * const transData);
BOOL_t testSaveTransaction(ST_transaction_t * const transData);
BOOL_t testRecieveTransactionData(ST_transaction_t * const transData);

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                                     MAIN                                    */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

int main(void) {
    ST_transaction_t transData = {0};
    uint8_t tryAgain = 0;

    do {
        // printf("Test: %s\n", testGetCardHolderName( &(transData.cardHolderData) ) ? "Passed" : "Failed");
        // printf("Test: %s\n", testGetCardExpiryDate( &(transData.cardHolderData) ) ? "Passed" : "Failed");
        // printf("Test: %s\n", testGetCardPan( &(transData.cardHolderData) ) ? "Passed" : "Failed");
        // printf("Test: %s\n", testGetTransactionDate( &(transData.terminalData) ) ? "Passed" : "Failed");
        // printf("Test: %s\n", testIsExpiredCard( &(transData.cardHolderData), &(transData.terminalData) )? "Passed" : "Failed");
        // printf("Test: %s\n", testGetTransactionAmount( &(transData.terminalData) ) ? "Passed" : "Failed");
        // printf("Test: %s\n", testSetMaxAmount( &(transData.terminalData) ) ? "Passed" : "Failed");
        // printf("Test: %s\n", testIsBelowMaxAmount( &(transData.terminalData) ) ? "Passed" : "Failed");
        // printf("Test: %s\n", testRecieveTransactionData( &transData ) ? "Passed" : "Failed");
        // printf("Test: %s\n", testIsValidAccount( &transData ) ? "Passed" : "Failed");
        // printf("Test: %s\n", testIsAmountAvailable( &transData ) ? "Passed" : "Failed");
        // printf("Test: %s\n", testSaveTransaction( &transData ) ? "Passed" : "Failed");

        printf("Try again? (y/n): ");
        scanf(" %c%*c", &tryAgain);
        tryAgain = toupper(tryAgain);
    } while('Y' == tryAgain);

    return 0;
}


/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                            TEST FUNCTION DEFINITIONS                        */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

BOOL_t testGetCardHolderName(ST_cardData_t * const cardData) {
    EN_cardError_t cardError;
    BOOL_t result = FALSE;

    cardError = getCardHolderName(cardData);

    if(CARD_OK == cardError) {
        printf("Name: %s\n", cardData->cardHolderName);
        result = TRUE;
    } else {
        printf("Invalid Name (Card Error: %d)\n", cardError);
        result = FALSE;
    }

    return result;
}

BOOL_t testGetCardExpiryDate(ST_cardData_t * const cardData) {
    EN_cardError_t cardError;
    BOOL_t result = FALSE;

    cardError = getCardExpiryDate(cardData);

    if(CARD_OK == cardError) {
        printf("Expiry date: %s\n", cardData->cardExpirationDate);
        result = TRUE;
    } else {
        printf("Invalid Expiry Date. (Card Error: %d)\n", cardError);
        result = FALSE;
    }

    return result;
}

BOOL_t testGetCardPan(ST_cardData_t * const cardData) {
    EN_cardError_t cardError;
    BOOL_t result = FALSE;

    cardError = getCardPAN(cardData);

    if(CARD_OK == cardError) {
        printf("Card number: %s\n", cardData->primaryAccountNumber);
        result = TRUE;
    } else {
        printf("Invaild Card Number. (Card Error: %d)\n", cardError);
        result = FALSE;
    }

    return result;
}

BOOL_t testGetTransactionDate(ST_terminalData_t * const termData) {
    EN_terminalError_t termError;
    BOOL_t result = FALSE;

    termError = getTransactionDate(termData);

    if(TERMINAL_OK == termError) {
        printf("Current Date: %s\n", termData->transactionDate);
        result = TRUE;
    } else {
        printf("Invalid Current Date. (Terminal Error: %d)\n", termError);
        result = FALSE;
    }

    return result;
}

BOOL_t testIsExpiredCard(ST_cardData_t * const cardData, ST_terminalData_t * const termData) {
    EN_terminalError_t termError;
    BOOL_t result = FALSE;

    if(testGetCardExpiryDate(cardData) && testGetTransactionDate(termData)) {
        termError = isCardExpired(*cardData, *termData);
        if(TERMINAL_OK == termError) {
            printf("Card is not expired\n");
            result = TRUE;
        } else {
            printf("Card is expired. (Terminal Error: %d)\n", termError);
            result = FALSE;
        }
    } else {
        return FALSE;
    }

    return result;
}

BOOL_t testGetTransactionAmount(ST_terminalData_t * const termData) {
    EN_terminalError_t termError;
    BOOL_t result = FALSE;

    termError = getTransactionAmount(termData);
    
    if(TERMINAL_OK == termError) {
        printf("Amount: %.0f\n", termData->transAmount);
        result = TRUE;
    } else {
        printf("Invalid amount. (termError %d)\n", termError);
        result = FALSE;
    }

    return result;
}

BOOL_t testSetMaxAmount(ST_terminalData_t * const termData) {
    EN_terminalError_t termError;
    BOOL_t result = FALSE;

    termError = setMaxAmount(termData);

    if(TERMINAL_OK == termError) {
        printf("Max amount: %.0f\n", termData->maxTransAmount);
        result = TRUE;
    } else {
        printf("Invalid max amount. (termError %d)\n", termError);
        result = FALSE;
    }

    return result;
}

BOOL_t testIsBelowMaxAmount(ST_terminalData_t * const termData) {
    EN_terminalError_t termError;
    BOOL_t result = FALSE;

    if(testGetTransactionAmount(termData) && testSetMaxAmount(termData)) {
        termError = isBelowMaxAmount(termData);
        if(TERMINAL_OK == termError) {
            printf("Max amount: %.0f\n", termData->maxTransAmount);
            printf("Amount is below max amount\n");
            result = TRUE;
        } else {
            printf("Exceeds maximum amount. (Terminal Error %d)\n", termError);
            result = FALSE;
        }
    } else {
        return FALSE;
    }

    return result;
}

BOOL_t testIsValidAccount(ST_transaction_t * const transData) {
    EN_serverError_t serverError;
    BOOL_t result = FALSE;

    if( testGetCardPan( &(transData->cardHolderData) )) {

        serverError = isValidAccount( &(transData->cardHolderData) );
        if(SERVER_OK == serverError) {
            printf("Valid Account: %s\n", transData->cardHolderData.primaryAccountNumber);
            result = TRUE;
        } else {
            printf("Invalid Account. (Server Error %d)\n", serverError);
            result = FALSE;
        }
    } else {
        result = FALSE;
    }

    return result;
}

BOOL_t testIsAmountAvailable(ST_transaction_t * const transData) {
    EN_serverError_t serverError;
    BOOL_t result = FALSE;

    if( testIsValidAccount(transData) && testIsBelowMaxAmount( &(transData->terminalData) ) ) {

        serverError = isAmountAvailable( &(transData->terminalData) );
        if(SERVER_OK == serverError) {
            printf("Valid Amount.\n");
            result = TRUE;
        } else {
            printf("Invalid Amount. (Server Error %d)\n", serverError);
            result = FALSE;
        }
    } else {
        result = FALSE;
    }

    return result;
}

BOOL_t testSaveTransaction(ST_transaction_t * const transData) {
    EN_serverError_t serverError;
    BOOL_t result = FALSE;

    if( testGetCardHolderName( &(transData->cardHolderData) )                           && 
        testIsExpiredCard( &(transData->cardHolderData), &(transData->terminalData) )
         ) {
        
        testIsAmountAvailable( transData );

        serverError = saveTransaction( transData );
        if(SERVER_OK == serverError) {
            printf("Data saved successfully.\n");
            result = TRUE;
        } else {
            printf("Failed to save data. (Server Error %d)\n", serverError);
            result = FALSE;
        }
    } else {
        result = FALSE;
    }

    return result;
}

BOOL_t testRecieveTransactionData(ST_transaction_t * const transData) {
    EN_transState_t transError;
    BOOL_t result = FALSE;

    if(
        testGetCardHolderName( &(transData->cardHolderData) )                              &&
        testGetCardPan( &(transData->cardHolderData) )                                  &&
        testIsExpiredCard( &(transData->cardHolderData), &(transData->terminalData) )   &&
        testIsBelowMaxAmount( &(transData->terminalData) )
      ) {

        transError = recieveTransactionData(transData);
        if(APPROVED == transError) {
            printf("Approved Transaction of: %.0f\n", transData->terminalData.transAmount);
            result = TRUE;
        } else {
            printf("Disapproved transsaction. (Transaction Error %d)\n", transError);
            result = FALSE;
        }
    } else {
        result = FALSE;
    }

    return result;
}


