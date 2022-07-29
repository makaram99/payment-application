/********************************************************************************
 * @file    card.h
 * @author  Mahmoud Karam Emara (ma.karam272@gmail.com)
 * @brief   This file contains the interfaces for the card module \ref card.c
 * @version 1.0.0
 * @date    2022-07-28
 * 
 * @copyright Copyright (c) 2022
 * 
 ********************************************************************************/


#ifndef CARD_H
#define CARD_H


/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                              TYPE DEFINITIONS                               */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

/********************************************************************************
 * @brief   This struct contains the card data.
 ********************************************************************************/
typedef struct ST_cardData_t {
    uint8_t cardHolderName[25];
    uint8_t primaryAccountNumber[20];
    uint8_t cardExpirationDate[6];
} ST_cardData_t;

/********************************************************************************
 * @brief   This enum contains the different card errors.
 ********************************************************************************/
typedef enum EN_cardError_t {
    CARD_OK, 
    WRONG_NAME, 
    WRONG_EXP_DATE, 
    WRONG_PAN
} EN_cardError_t;


/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                           PUBLIC FUNCTION DECLARATIONS                       */
/*                                                                              */
/*------------------------------------------------------------------------------*/

EN_cardError_t getCardHolderName(ST_cardData_t * const cardData);
EN_cardError_t getCardExpiryDate(ST_cardData_t * const cardData);
EN_cardError_t getCardPAN(ST_cardData_t * const cardData);

/********************************************************************************
 * @brief       Get the Card Expiry Year object in the cardData
 * 
 * @param[in]   cardData: Pointer to the cardData structure
 * @return      int8_t: Expiry Year:
 *              -1: Error
 *              xy: 20xy    -> x: 0-9, y: 0-9
 *******************************************************************************/
int8_t getCardExpiryYear(const ST_cardData_t * const cardData);

/********************************************************************************
 * @brief       Get the Card Expiry Month object in the cardData
 * 
 * @param[in]   cardData: Pointer to the cardData structure
 * @return      int8_t: Expiry Month:
 *              -1: Error
 *              xy: xy    -> xy: 1-12
 *******************************************************************************/
int8_t getCardExpiryMonth(const ST_cardData_t * const cardData);

#endif      /* CARD_H */