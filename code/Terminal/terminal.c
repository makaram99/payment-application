/********************************************************************************
 * @file    terminal.c
 * @author  Mahmoud Karam Emara (ma.karam272@gmail.com)
 * @brief   This file contains the terminal module implementation.
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
#include "terminal.h"

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                     PRIVATE FUNCTIONS PROTOTYPES                     */
/*                                                                      */
/*----------------------------------------------------------------------*/
static BOOL_t isLeapYear(const uint16_t year);
static BOOL_t isValidCurrentDate(const ST_terminalData_t * const termData);
static BOOL_t isValidDateFormat(const ST_terminalData_t * const termData);
static int8_t getCurrentYear(const ST_terminalData_t * const termData);
static int8_t getCurrentMonth(const ST_terminalData_t * const termData);
static int8_t getCurrentDay(const ST_terminalData_t * const termData);


/*----------------------------------------------------------------------*/
/*                                                                      */
/*                     PUBLIC FUNCTIONS DEFINITIONS                     */
/*                                                                      */
/*----------------------------------------------------------------------*/

EN_terminalError_t getTransactionDate(ST_terminalData_t * const termData) {
    BOOL_t  isValidLength = TRUE;
    uint8_t length = 0;

    if(NULL == termData) {
        return WRONG_DATE;
    }

    /* Getting current date  */
    printf("\nEnter Current Date (10 characters): ");
    fgets((char *)termData->transactionDate, sizeof(termData->transactionDate), stdin);    
    length = strlen((char *)termData->transactionDate);
    isValidLength = ( (sizeof(termData->transactionDate) - 1) == length);

    /* Validating the length of data    */
    if(length && ('\n' == termData->transactionDate[length - 1]) ) {
        isValidLength = FALSE;
    } else {
        if(isValidLength && ('\n' == getchar()) ) {
            isValidLength = TRUE;
        } else {
            isValidLength = FALSE;

            /* Flush stdin */
            while('\n' != getchar());

            /* Setting the first character to '\0' */
            termData->transactionDate[0] = '\0';
        }
    }

    if( !isValidLength ) {
        return WRONG_DATE;
    }
    
    /* Validating the date  */
    if( !isValidCurrentDate(termData) ) {
        return WRONG_DATE;
    }

    return TERMINAL_OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData) {
    int8_t cardYear = 0, cardMonth = 0, currentYear = 0, currentMonth = 0;

    if( !isValidCurrentDate(&termData)) {
        return EXPIRED_CARD;
    }
    
    /* Getting Current Months, Year   */
    currentMonth = getCurrentMonth(&termData);
    currentYear = getCurrentYear(&termData);

    /* Getting Card Months, Year   */
    cardMonth = getCardExpiryMonth(&cardData);
    cardYear = getCardExpiryYear(&cardData);

    /*! No need to validate months and years because they are validated in 
        the isValidCurrentDate() check above */

    /* Validating Expiration    */
    if(currentYear >= cardYear) {
        if( (currentYear > cardYear) || (currentMonth > cardMonth) ) {
            return EXPIRED_CARD;
        }
    }

    return TERMINAL_OK;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData) {



    return TERMINAL_OK;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t * const termData) {

    if(NULL == termData) {
        return INVALID_AMOUNT;
    }

    termData->transAmount = 0.0f;

    printf("\nEnter the required amount: ");
    scanf("%f", &(termData->transAmount));  
    termData->transAmount = (int64_t)termData->transAmount;  

    /* Validating the amount    */
    if(0 >= termData->transAmount) {
        return INVALID_AMOUNT;
    }

    return TERMINAL_OK;
}

EN_terminalError_t isBelowMaxAmount(const ST_terminalData_t * const termData) {

    if(NULL == termData) {
        return EXCEED_MAX_AMOUNT;
    }

    if(termData->transAmount > termData->maxTransAmount) {
        return EXCEED_MAX_AMOUNT;
    }

    return TERMINAL_OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t * const termData) {

    if(NULL == termData) {
        return INVALID_MAX_AMOUNT;
    }

    termData->maxTransAmount = 0.0f;
    
    printf("\nEnter the maximum amount: ");
    scanf("%f", &(termData->maxTransAmount));  
    termData->maxTransAmount = (int64_t)termData->maxTransAmount;  
    
    /* Validating the amount    */
    if(0 >= termData->maxTransAmount) {
        return INVALID_MAX_AMOUNT;
    }

    return TERMINAL_OK;
}

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                     PRIVATE FUNCTIONS DEFINITIONS                    */
/*                                                                      */
/*----------------------------------------------------------------------*/

static BOOL_t isLeapYear(const uint16_t year) {
    BOOL_t isDivisibleBy4 = FALSE, isDivisibleBy400 = FALSE, isDivisibleBy100 = FALSE;
    BOOL_t isLeap = FALSE;

    isDivisibleBy4      = (BOOL_t) ((year % 4)   == 0);
    isDivisibleBy400    = (BOOL_t) ((year % 400) == 0);
    isDivisibleBy100    = (BOOL_t) ((year % 100) == 0);

    if(isDivisibleBy4) {
        if(! isDivisibleBy100) {
            isLeap = TRUE;
        } else {
            if(isDivisibleBy400) {
                isLeap = TRUE;
            } else {
                isLeap = FALSE;
            }
        }
    } else {
        isLeap = FALSE;
    }

    return (BOOL_t)isLeap;
}

static BOOL_t isValidCurrentDate(const ST_terminalData_t * const termData) {
    uint8_t day = 0, month = 0, year = 0;

    if(NULL == termData) {
        return FALSE;
    }

    if( !isValidDateFormat(termData) ) {
        return FALSE;
    }

    /* Getting Month and Year*/
    day = getCurrentDay(termData);
    month = getCurrentMonth(termData);
    year = getCurrentYear(termData);

    /* Validating the date */
    if( (year == -1) || (month == -1) || (-1 == day)) {
        return FALSE;
    }

    switch(month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            if( (day > 31) || (day < 1) ) {
                return FALSE;
            }
            break;
        case 4: case 6: case 9: case 11:
            if( (day > 30) || (day < 1) ) {
                return FALSE;
            }
            break;
        case 2:
            if(isLeapYear(year)) {
                if( (day > 29) || (day < 1) ) {
                    return FALSE;
                }
            } else {
                if( (day > 28) || (day < 1) ) {
                    return FALSE;
                }
            }
            break;
        
        default:
            return FALSE;
            break;
    }

    return TRUE;
}

static BOOL_t isValidDateFormat(const ST_terminalData_t * const termData) {
    BOOL_t isValidFormat = FALSE;
    
    if(NULL == termData) {
        return -1;
    }

    isValidFormat = isdigit(termData->transactionDate[0]) && 
                    isdigit(termData->transactionDate[1]) &&
                    (termData->transactionDate[2] == '/') &&
                    isdigit(termData->transactionDate[3]) &&
                    isdigit(termData->transactionDate[4]) &&
                    (termData->transactionDate[5] == '/') &&
                    isdigit(termData->transactionDate[6]) &&
                    isdigit(termData->transactionDate[7]) &&
                    isdigit(termData->transactionDate[8]) &&
                    isdigit(termData->transactionDate[9]) ;
    
    if(!isValidFormat) {
        return FALSE;
    }

    return TRUE;
}

static int8_t getCurrentYear(const ST_terminalData_t * const termData) {
    uint16_t year = 0;

    if(NULL == termData) {
        return -1;
    }

    if( !isValidDateFormat(termData) ) {
        return -1;
    }

    /* Getting Year */
    year    = (termData->transactionDate[6] - '0') * 1000   + 
              (termData->transactionDate[7] - '0') * 100    + 
              (termData->transactionDate[8] - '0') * 10     + 
              (termData->transactionDate[9] - '0');

    year -= 2000;

    if( (year >= 0) && (year <= 99) ) {
        return year;
    } else {
        return -1;
    }
}

static int8_t getCurrentMonth(const ST_terminalData_t * const termData) {
    uint8_t month = 0;

    if(NULL == termData) {
        return -1;
    }

    if( !isValidDateFormat(termData) ) {
        return -1;
    }

    /* Getting month */
    month = (termData->transactionDate[3] - '0') * 10     + 
            (termData->transactionDate[4] - '0');

    return month;
}

static int8_t getCurrentDay(const ST_terminalData_t * const termData) {
    uint8_t day = 0;

    if(NULL == termData) {
        return -1;
    }

    if( !isValidDateFormat(termData) ) {
        return -1;
    }

    /* Getting day */
    day = (termData->transactionDate[0] - '0') * 10     + 
          (termData->transactionDate[1] - '0'); 

    return day;
}


