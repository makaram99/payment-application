/********************************************************************************
 * @file    card.c
 * @author  Mahmoud Karam Emara (ma.karam272@gmail.com)
 * @brief   This file contains the card module implementation.
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
#include "card.h"

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                      PRIVATE FUNCTION DECLARATIONS                          */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

static BOOL_t isPanValid(const ST_cardData_t * const cardData);
static BOOL_t isValidExpirationFormat(const ST_cardData_t * const cardData);
static BOOL_t isValidExpirationDate(const ST_cardData_t * const cardData);

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                         PUBLIC FUNCTION DEFINITIONS                         */
/*                                                                             */
/*-----------------------------------------------------------------------------*/


EN_cardError_t getCardHolderName(ST_cardData_t * const cardData) {
    uint8_t length = 0;
    BOOL_t  isValidLength = TRUE;

    if(NULL == cardData) {
        return WRONG_NAME;
    }

    /* Getting Name  */
    printf("\nEnter your name (between 20 & 24 characters): ");
    fgets((char *) (cardData->cardHolderName), sizeof(cardData->cardHolderName), stdin);

    length = strlen((char *) (cardData->cardHolderName) );
    if( length && ('\n' == cardData->cardHolderName[length - 1]) ) {
        length--;
        cardData->cardHolderName[length] = '\0';
    } else if(length && ('\n' != getchar()) ) {
        /* Flush stdin */
        while('\n' != getchar());   

        /* Setting the first character to '\0' */
        cardData->cardHolderName[0] = '\0';

        /* Returning error */
        return WRONG_NAME;
    } else {
        /* Do nothing */
    }

    /* Checking the length */
    isValidLength = (length >= 20) && (length <= 24);
    if(!isValidLength) {
        /* Setting the first character to '\0' */
        cardData->cardHolderName[0] = '\0';
        return WRONG_NAME;
    }

    /* Checking if the name is all alphabetic */
    while(length) {
        if( (!isalpha(cardData->cardHolderName[length - 1])) && (!isspace(cardData->cardHolderName[length - 1])) ) {
            return WRONG_NAME;
        }

        cardData->cardHolderName[length - 1] = toupper(cardData->cardHolderName[length - 1]);
        length--;
    }

    return CARD_OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t * const cardData) {
    uint8_t length = 0;
    BOOL_t isValidLength = TRUE;

    if(NULL == cardData) {
        return WRONG_EXP_DATE;
    }

    /* Getting Expiration date  */
    printf("\nEnter card expiry date (5 characters): ");
    fgets((char *) (cardData->cardExpirationDate), sizeof(cardData->cardExpirationDate), stdin);    
    length = strlen((char *) (cardData->cardExpirationDate) );
    isValidLength = ( (sizeof(cardData->cardExpirationDate) - 1) == length);

    if(length && ('\n' == cardData->cardExpirationDate[length - 1]) ) {
        isValidLength = FALSE;
    } else {
        if(isValidLength && ('\n' == getchar()) ) {
            isValidLength = TRUE;
        } else {
            isValidLength = FALSE;

            /* Flush stdin */
            while('\n' != getchar());  

            /* Setting the first character to '\0' */
            cardData->cardExpirationDate[0] = '\0';
        }
    }

    if( !isValidLength ) {
        return WRONG_EXP_DATE;
    }
    
    /* Validate the format */
    if( !isValidExpirationDate(cardData) ) {
        printf("InValid Expiry Date\n");
        return WRONG_EXP_DATE;
    }

    return CARD_OK;
}

EN_cardError_t getCardPAN(ST_cardData_t * const cardData) {
    uint8_t length = 0;

    if(NULL == cardData) {
        return WRONG_PAN;
    }

    /* Getting Name  */
    printf("\nEnter your PAN (between 16 & 19 digits): ");
    fgets((char *) (cardData->primaryAccountNumber), sizeof(cardData->primaryAccountNumber), stdin);    
    
    length = strlen((char *) (cardData->primaryAccountNumber) );
    if(length) {
        /* Removing Newline */
        if('\n' == cardData->primaryAccountNumber[length - 1]) {
            length--;
            cardData->primaryAccountNumber[length] = '\0';
        } else if('\n' != getchar()) {
            /* Flush stdin */
            while('\n' != getchar());   

            /* Setting the first character to '\0' */
            cardData->primaryAccountNumber[0] = '\0';

            /* Returning error */
            return WRONG_PAN;
        } else {
            /* Do nothing   */
        }
        
    }

    if(!isPanValid(cardData)) {
        return WRONG_PAN;
    }

    return CARD_OK;
}

int8_t getCardExpiryYear(const ST_cardData_t * const cardData) {
    uint8_t year = 0;

    if(NULL == cardData) {
        return -1;
    }

    if( !isValidExpirationFormat(cardData) ) {
        return -1;
    }

    /* Getting Year */
    year = (cardData->cardExpirationDate[3] - '0') * 10 + 
           (cardData->cardExpirationDate[4] - '0');

    return year;
}

int8_t getCardExpiryMonth(const ST_cardData_t * const cardData) {
    uint8_t month = 0;

    if(NULL == cardData) {
        return -1;
    }

    if( !isValidExpirationFormat(cardData) ) {
        return -1;
    }

    /* Getting month */
    month = (cardData->cardExpirationDate[0] - '0') * 10 + 
            (cardData->cardExpirationDate[1] - '0'); 

    if( (month < 1) || (month > 12) ) {
        return -1;
    }

    return month;
}

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                             PRIVATE FUNCTION DEFINITIONS                    */
/*                                                                             */
/*-----------------------------------------------------------------------------*/


/********************************************************************************
 * @brief       Check if the expiration date is valid or not
 * 
 * @details     The expiration date is valid if it is:
 *              * In the format MM/YY 
 *              * The month is between 1 and 12
 *              * The year is between 0 and 99
 * @param[in]   cardData: Pointer to the card data structure
 * @return      BOOL_t: TRUE if the date is valid, FALSE otherwise
 *******************************************************************************/
static BOOL_t isValidExpirationDate(const ST_cardData_t * const cardData) {
    int8_t month = 0, year = 0;

    if(NULL == cardData) {
        return FALSE;
    }

    if( !isValidExpirationFormat(cardData) ) {
        return FALSE;
    }

    /* Getting Month and Year*/
    month = getCardExpiryMonth(cardData);
    year = getCardExpiryYear(cardData);
    
    /* Validating the date */
    if( (year == -1) || (month == -1) ) {
        return FALSE;
    }

    return TRUE;
}

/********************************************************************************
 * @brief       Check if the expiration date has a valid format or not (MM/YY)
 * 
 * @details     The expiration date is valid if it is:
 *              * In the format MM/YY
 *              * The month and year are digits, and separated by a '/'
 * @param[in]   cardData: Pointer to the card data structure
 * @return      BOOL_t: TRUE if the format is valid, FALSE otherwise
 *******************************************************************************/
static BOOL_t isValidExpirationFormat(const ST_cardData_t * const cardData) {
    BOOL_t isValidFormat = FALSE;
    
    if(NULL == cardData) {
        return FALSE;
    }

    isValidFormat = isdigit(cardData->cardExpirationDate[0]) && 
                    isdigit(cardData->cardExpirationDate[1]) &&
                    (cardData->cardExpirationDate[2] == '/') &&
                    isdigit(cardData->cardExpirationDate[3]) &&
                    isdigit(cardData->cardExpirationDate[4]) ;
    
    if(!isValidFormat) {
        return FALSE;
    }

    return TRUE;
}

/********************************************************************************
 * @brief       Check if the PAN is valid or not
 * 
 * @details     The PAN is valid if it is:
 *              * Between 16 and 19 digits
 *              * All digits
 * @param[in]   cardData: Pointer to the card data structure
 * @return      BOOL_t: TRUE if the PAN is valid, FALSE otherwise
 *******************************************************************************/
static BOOL_t isPanValid(const ST_cardData_t * const cardData) {
    BOOL_t isValidLength = FALSE;
    uint8_t length = 0;

    if(NULL == cardData) {
        return FALSE;
    }

    length = strlen((char *) (cardData->primaryAccountNumber) );
    isValidLength = (length >= 16) && (length <= 19);
    if(!isValidLength) {
        return FALSE;
    }

    /* Checking if the PAN is valid (numeric) */
    while(length) {
        if(!isdigit(cardData->primaryAccountNumber[length - 1])) {
            return FALSE;
        }

        length--;
    }

    return TRUE;
}

