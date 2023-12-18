#include "manila.h"
#include "clocks_and_modes.h"
#include "device_registers.h"
#include "lcd1602A.h"
#include <stdio.h>

/* Function Implementation Section (start)*/
void toggleLEDsInCycle()
{
    // ON -> Wait -> OFF
    PTC->PCOR |= (1 << PTC10);
    delay_ms(200);
    PTC->PSOR |= (1 << PTC10);
    PTC->PCOR |= (1 << PTC9);
    delay_ms(200);
    PTC->PSOR |= (1 << PTC9);
    PTC->PCOR |= (1 << PTC8);
    delay_ms(200);
    PTC->PSOR |= (1 << PTC8);
    PTC->PCOR |= (1 << PTC7);
    delay_ms(200);
    PTC->PSOR |= (1 << PTC7);
    PTC->PCOR |= (1 << PTC5);
    delay_ms(200);
    PTC->PSOR |= (1 << PTC5);
    PTC->PCOR |= (1 << PTC3);
    delay_ms(200);
    PTC->PSOR |= (1 << PTC3);
    PTC->PCOR |= (1 << PTC2);
    delay_ms(200);
    PTC->PSOR |= (1 << PTC2);
    PTC->PCOR |= (1 << PTC1);
    delay_ms(200);
    PTC->PSOR |= (1 << PTC1);
}

void reToggleLEDsInCycle()
{
    // ON -> Wait -> OFF
    PTC->PCOR |= (1 << PTC1);
    delay_ms(200);
    PTC->PSOR |= (1 << PTC1);
    PTC->PCOR |= (1 << PTC2);
    delay_ms(200);
    PTC->PSOR |= (1 << PTC2);
    PTC->PCOR |= (1 << PTC3);
    delay_ms(200);
    PTC->PSOR |= (1 << PTC3);
    PTC->PCOR |= (1 << PTC5);
    delay_ms(200);
    PTC->PSOR |= (1 << PTC5);
    PTC->PCOR |= (1 << PTC7);
    delay_ms(200);
    PTC->PSOR |= (1 << PTC7);
    PTC->PCOR |= (1 << PTC8);
    delay_ms(200);
    PTC->PSOR |= (1 << PTC8);
    PTC->PCOR |= (1 << PTC9);
    delay_ms(200);
    PTC->PSOR |= (1 << PTC9);
    PTC->PCOR |= (1 << PTC10);
    delay_ms(200);
    PTC->PSOR |= (1 << PTC10);
}

void compareFloors(int *c_floor, int *d_floor)
{

    if (*c_floor < *d_floor) // -> GOIND UP
    {
        // lcdGoingUp();
        for (int i = *c_floor + 1; i <= *d_floor; i++)
        {
            toggleLEDsInCycle();
            Seg_out(i);
            delay_ms(1000);
        }
        // lcdDoorOpenAndClose();
    }
    else if (*c_floor > *d_floor) // -> GOING DOWN
    {
        // lcdGoingDown();
        for (int i = *c_floor - 1; i >= *d_floor; i--)
        {
            reToggleLEDsInCycle();
            Seg_out(i);
            delay_ms(1000);
        }
        // lcdDoorOpenAndClose();
    }
    *c_floor = *d_floor;
}
void lcdInitialize()
{
    // text-char output
    while (msg_array1[i] != '\0')
    {
        lcdcharinput(msg_array1[i]); // 1(first) row text-char send to LCD module
        delay_ms(300);
        i++;
    }

    lcdinput(0x80 + 0x40); // second row
    delay_ms(200);
    i = 0;
    while (msg_array2[i] != '\0')
    {
        lcdcharinput(msg_array2[i]); // 2(second) row text-char send to LCD module
        delay_ms(300);
        i++;
    }

    // Lcd off, LCD display clear
    delay_ms(500);
    lcdinput(0x08); // lcd display off
    delay_ms(400);
    lcdinput(0x01); // Clear display
    delay_ms(500);
}

/* Function Implementation Section (end)*/