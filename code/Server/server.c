/********************************************************************************
 * @file    server.c
 * @author  Mahmoud Karam Emara (ma.karam272@gmail.com)
 * @brief   This file contains the server module implementation.
 * @version 1.0.0
 * @date    2022-07-28
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
#include "server.h"


/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                              PRIVATE VARIABLES                              */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

/********************************************************************************
 * @brief   Database of valid accounts
 ********************************************************************************/
static ST_accountsDB_t accountsDB[255] = {
    {.balance = 5000    , .primaryAccountNumber = "1111222233334444555"    },
    {.balance = 10000   , .primaryAccountNumber = "1112223334445556667"   },
    {.balance = 3000    , .primaryAccountNumber = "1122334455667788991"    },
    {.balance = 20000   , .primaryAccountNumber = "1234567891234567891"   },
    {.balance = 50000   , .primaryAccountNumber = "9876543219876543219"   },
};

/********************************************************************************
 * @brief   The index of the current account being processed
 ********************************************************************************/
static int16_t accountsDBIndex = 0;

/********************************************************************************
 * @brief Database of transactions history 
 ********************************************************************************/
static ST_transaction_t transactionDB[255] = {0};

/********************************************************************************
 * @brief   The index of the next free memory to save the transaction
 ********************************************************************************/
static uint8_t transDBIndex = 0;

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                      PRIVATE FUNCTION DECLARATIONS                          */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

/********************************************************************************
 * @brief       Get the Account Index In accountsDB array
 * 
 * @param[in]   pan: Pointer to the Primary Account Number
 * @return      int16_t: The index of the account in accountsDB array:
 *              * -1: Account not found
 *              * >=0: Account found
 ********************************************************************************/
static int16_t getAccountIndexInDB(uint8_t * pan);


/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                         PUBLIC FUNCTION DEFINITIONS                         */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

EN_transState_t recieveTransactionData(ST_transaction_t * const transData) {
    EN_serverError_t serverError = SERVER_OK;

    /* Validating the passed address    */
    if(NULL == transData) {
        return INTERNAL_SERVER_ERROR;
    }

    accountsDBIndex = getAccountIndexInDB(transData->cardHolderData.primaryAccountNumber);

    if( ACCOUNT_NOT_FOUND == isValidAccount(&(transData->cardHolderData)) ) {

        transData->transState = DECLINED_STOLEN_CARD;

    } else if( LOW_BALANCE == isAmountAvailable(&(transData->terminalData)) ) {

        transData->transState = DECLINED_INSUFFICIENT_FUND;

    } else {
        transData->transState = APPROVED;
    }

    serverError = saveTransaction(transData);

    /* Validating the passed address    */
    if( (SERVER_OK == serverError) && (APPROVED == transData->transState) ) {
        /* Updating the balance */
        printf("Account balance: %.0f\n", accountsDB[accountsDBIndex].balance);
        accountsDB[accountsDBIndex].balance -= transData->terminalData.transAmount;
        printf("Your new balance: %.0f\n", accountsDB[accountsDBIndex].balance);
    } else {
        if(SERVER_OK != serverError) {
            transData->transState = INTERNAL_SERVER_ERROR;
        }
    }    

    return transData->transState;
}

EN_serverError_t isValidAccount(ST_cardData_t * const cardData) {
    uint8_t i = 0;

    /* Validating the passed address    */
    if(NULL == cardData) {
        return ACCOUNT_NOT_FOUND;
    }

    for(i = 0; i < sizeof(accountsDB->primaryAccountNumber); ++i) {
        if(0 == strcmp((char *)(cardData->primaryAccountNumber), (char *) (accountsDB[i].primaryAccountNumber) )) {
            accountsDBIndex = i;
            return SERVER_OK;
        }
    }

    return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t * const termData) {

    /* Validating the passed address    */
    if(NULL == termData) {
        return LOW_BALANCE;
    }

    if(-1 == accountsDBIndex) {
        return LOW_BALANCE;
    }

    if(accountsDB[accountsDBIndex].balance < termData->transAmount) {       
        return LOW_BALANCE;
    }

    return SERVER_OK;
}

EN_serverError_t saveTransaction(ST_transaction_t * const transData) {

    if(NULL == transData) {
        return SAVING_FAILED;
    }

    transactionDB[transDBIndex] = *transData;
    transactionDB[transDBIndex].transactionSequenceNumber = transDBIndex;

    ++transDBIndex;

    return SERVER_OK;
}

EN_serverError_t getTransaction(const uint32_t transactionSequenceNumber, ST_transaction_t * const transData) {



    return SERVER_OK;
}


/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                             PRIVATE FUNCTION DEFINITIONS                    */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

static int16_t getAccountIndexInDB(uint8_t * pan) {
    uint8_t i = 0;

    for(i = 0; i < sizeof(accountsDB->primaryAccountNumber); ++i) {
        if(0 == strcmp((char *)pan, (char *) (accountsDB[i].primaryAccountNumber) ) ) {
            return i;
        }
    }

    return -1;
}

