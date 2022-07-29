/********************************************************************************
 * @file    terminal.h
 * @author  Mahmoud Karam Emara (ma.karam272@gmail.com)
 * @brief   This file contains the interfaces for the terminal module 
 *          \ref terminal.c
 * @version 1.0.0
 * @date    2022-07-28
 * 
 * @copyright Copyright (c) 2022
 * 
 ********************************************************************************/

#ifndef TERMINAL_H
#define TERMINAL_H


/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                              TYPE DEFINITIONS                                */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*********************************************************************************
 * @brief   Struct for the terminal data
 ********************************************************************************/
typedef struct ST_terminalData_t {
    float transAmount;                  /*!< Transaction amount in float */
    float maxTransAmount;               /*!< Maximum transaction amount in float */
    uint8_t transactionDate[11];        /*!< Transaction date DD/MM/YYYY */
} ST_terminalData_t;

/*********************************************************************************
 * @brief   Enum for the different errors of the <b>terminal</b>
 ********************************************************************************/
typedef enum EN_terminalError_t {
    TERMINAL_OK,                        /*!< Terminal is working fine */
    WRONG_DATE,                         /*!< Wrong date */
    EXPIRED_CARD,                       /*!< Card is expired */
    INVALID_CARD,                       /*!< Card is invalid */
    INVALID_AMOUNT,                     /*!< Transaction amount is invalid */
    EXCEED_MAX_AMOUNT,                  /*!< Transaction amount is greater than the maximum amount */
    INVALID_MAX_AMOUNT                  /*!< Maximum transaction amount is invalid */
}EN_terminalError_t ;



/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                           PUBLIC FUNCTION DECLARATIONS                       */
/*                                                                              */
/*------------------------------------------------------------------------------*/

EN_terminalError_t getTransactionDate(ST_terminalData_t * const termData);
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData);
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t * const termData);
EN_terminalError_t isBelowMaxAmount(const ST_terminalData_t * const termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t * const termData);



#endif      /* TERMINAL_H */