/*!
    \file    main.c
    \brief   GD32A503CCT3 project template entry
*/

#include "app.h"
#include "board.h"

int main(void)
{
    board_init();
    app_init();

    while(1) {
        app_loop();
    }
}
