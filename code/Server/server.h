/********************************************************************************
 * @file    server.h
 * @author  Mahmoud Karam Emara (ma.karam272@gmail.com)
 * @brief   This file contains the interfaces for the server module \ref server.c
 * @version 1.0.0
 * @date    2022-07-28
 * 
 * @copyright Copyright (c) 2022
 * 
 ********************************************************************************/


#ifndef SERVER_H
#define SERVER_H


/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                              TYPE DEFINITIONS                                */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*********************************************************************************
 * @brief   Enum for the different errors of the <b>transaction</b>
 ********************************************************************************/
typedef enum EN_transState_t{
    APPROVED,                       /*!< Transaction approved */
    DECLINED_INSUFFICIENT_FUND,     /*!< Transaction declined due to insufficient fund */
    DECLINED_STOLEN_CARD,           /*!< Transaction declined due to stolen card */
    INTERNAL_SERVER_ERROR           /*!< Transaction declined due to internal server error */
} EN_transState_t;

/*********************************************************************************
 * @brief   Enum for the different errors of the <b>server</b>
 ********************************************************************************/
typedef enum EN_serverError_t{
    SERVER_OK,                      /*!< Server is working fine */
    SAVING_FAILED,                  /*!< Failed saving transaction in the server database */
    TRANSACTION_NOT_FOUND,          /*!< Transaction not found in the server history database */
    ACCOUNT_NOT_FOUND,              /*!< Account not found in the server database */
    LOW_BALANCE                     /*!< Account balance is lower than the transaction amount */
} EN_serverError_t;

/*********************************************************************************
 * @brief   Struct for the transaction data
 ********************************************************************************/
typedef struct ST_transaction_t {
    ST_cardData_t cardHolderData;           /*!< Card holder data */
    ST_terminalData_t terminalData;         /*!< Terminal data */
    EN_transState_t transState;             /*!< Transaction error state */
    uint32_t transactionSequenceNumber;     /*!< Transaction sequence number in the server database */
} ST_transaction_t;

/*********************************************************************************
 * @brief   Struct for the account data
 ********************************************************************************/
typedef struct {
    float balance;                          /*!< Account balance in float */
    uint8_t primaryAccountNumber[20];       /*!< Account primary number */
}ST_accountsDB_t;



/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                           PUBLIC FUNCTION DECLARATIONS                       */
/*                                                                              */
/*------------------------------------------------------------------------------*/

EN_transState_t recieveTransactionData(ST_transaction_t * const transData);
EN_serverError_t isValidAccount(ST_cardData_t * const cardData);
EN_serverError_t isAmountAvailable(ST_terminalData_t * const termData);
EN_serverError_t saveTransaction(ST_transaction_t * const transData);
EN_serverError_t getTransaction(const uint32_t transactionSequenceNumber, ST_transaction_t * const transData);


#endif      /* SERVER_H */