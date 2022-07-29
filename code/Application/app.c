/********************************************************************************
 * @file    app.c
 * @author  Mahmoud Karam Emara (ma.karam272@gmail.com)
 * @brief   This file contains the application main function
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
#include "../Server/server.h"
#include "state.h"
#include "app.h"


int main(void) {
    char tryAgain = 0;

    do {
        appStart();

        printf("\n\n\nStart new entry? (y/n): ");
        scanf(" %c%*c", &tryAgain);
        tryAgain = toupper(tryAgain);
    } while('Y' == tryAgain);

    return 0;
}

void appStart(void) {
    uint8_t i = 0;
    BOOL_t stateSuccess = TRUE;
    ST_transaction_t transData = {0};
    
    for (i = 0; i < countStates; ++i) {
        if(NULL == stateMachine[i].func) {
            continue;
        }

        stateSuccess = stateMachine[i].func(&transData);
        if(FALSE == stateSuccess) {
            printf("Failed to execute state %s\n", stateMachine[i].name);
            break;
        }
    }
}
