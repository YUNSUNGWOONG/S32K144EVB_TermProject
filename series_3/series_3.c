#include "clocks_and_modes.h"
#include "device_registers.h"
#include "lcd1602A.h"
#include <stdio.h>

// LED
#define PTC1 1
#define PTC2 2
#define PTC3 3
#define PTC5 5
#define PTC7 7
#define PTC8 8
#define PTC9 9
#define PTC10 10

// SWITCH
#define PTD1 1
#define PTD2 2
#define PTD3 3
#define PTD4 4
#define PTD5 5

// SEGMENT
#define PTE1 1
#define PTE2 2
#define PTE3 3
#define PTE4 4
#define PTE5 5
#define PTE6 6
#define PTE7 7
#define PTE8 8

// LCD_DISPLAY
#define PTD9 9
#define PTD10 10
#define PTD11 11
#define PTD12 12
#define PTD13 13
#define PTD14 14
#define PTD15 15

int lpit0_ch0_flag_counter = 0;
unsigned int i = 0;
int error = 0;
/* TEXT_Store: "---Welcome---" */
char msg_array1[16] = {0x20, 0x2d, 0x2d, 0x2d, 0x57, 0x65, 0x6c,
                       0x63, 0x6f, 0x6d, 0x65, 0x2d, 0x2d, 0x2d}; // 1-row text-char

/* TEXT_Store: "SELECT FLOOR   " */
char msg_array2[16] = {0x20, 0x20, 0x53, 0x45, 0x4c, 0x45, 0x43,
                       0x54, 0x20, 0x46, 0x4c, 0x4f, 0x4f, 0x52}; // 2-row text-char

/* TEXT_Store: " --1st FLOOR--" */
char msg_array_1[16] = {0x20, 0x2d, 0x2d, 0x31, 0x73, 0x74, 0x20,
                        0x46, 0x4c, 0x4f, 0x4f, 0x52, 0x2d, 0x2d}; // 2-row text-char
/* TEXT_Store: " --2nd FLOOR--"*/
char msg_array_2[16] = {0x20, 0x2d, 0x2d, 0x32, 0x6E, 0x64, 0x20,
                        0x46, 0x4C, 0x4f, 0x4f, 0x52, 0x2d, 0x2d}; // 1-row text-char

/* TEXT_Store: " --3rd FLOOR--"*/
char msg_array_3[16] = {0x20, 0x2d, 0x2d, 0x33, 0x72, 0x64, 0x20,
                        0x46, 0x4C, 0x4f, 0x4f, 0x52, 0x2d, 0x2d}; // 1-row text-char

/* TEXT_Store: " --4th FLOOR--" */
char msg_array_4[16] = {0x20, 0x2d, 0x2d, 0x34, 0x74, 0x68, 0x20,
                        0x46, 0x4C, 0x4f, 0x4f, 0x52, 0x2d, 0x2d}; // 1-row text-char

/* TEXT_Store: " --5th FLOOR--" */
char msg_array_5[16] = {0x20, 0x2d, 0x2d, 0x35, 0x74, 0x68, 0x20,
                        0x46, 0x4C, 0x4f, 0x4f, 0x52, 0x2d, 0x2d}; // 1-row text-char

/* TEXT_Store: " --GOING DOWN--" */
char msg_array_down[16] = {0x2d, 0x2d, 0x47, 0x4f, 0x49, 0x4e, 0x47,
                           0x20, 0x44, 0x4f, 0x57, 0x4e, 0x2d, 0x2d}; // 2-row text-char

/* TEXT_Store: " --GOING UP--" */
char msg_array_up[16] = {0x2d, 0x2d, 0x2d, 0x47, 0x4f, 0x49, 0x4e,
                         0x47, 0x20, 0x55, 0x50, 0x2d, 0x2d, 0x2d}; // 2-row text-char
unsigned int num = 0;
unsigned int num0, num1, num2, num3 = 0;

unsigned int j = 0; /*FND select pin index */

unsigned int FND_DATA[10] = {0x7E, 0x0C, 0xB6, 0x9E, 0xCC, 0xDA, 0xFA, 0x4E, 0xFE, 0xCE}; // 0~9 number
unsigned int Delaytime = 0; /* Delay Time Setting Variable*/

unsigned int FND_SEL[4] = {0x0100, 0x0200, 0x0400, 0x0800};

void PORT_init(void)
{
    // LED
    PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;
    PTC->PDDR |= 1 << PTC1 | 1 << PTC2 | 1 << PTC3 | 1 << PTC5 | 1 << PTC7 | 1 << PTC8 | 1 << PTC9 | 1 << PTC10;
    PORTC->PCR[1] = PORT_PCR_MUX(1);
    PORTC->PCR[2] = PORT_PCR_MUX(1);
    PORTC->PCR[3] = PORT_PCR_MUX(1);
    PORTC->PCR[5] = PORT_PCR_MUX(1);
    PORTC->PCR[7] = PORT_PCR_MUX(1);
    PORTC->PCR[8] = PORT_PCR_MUX(1);
    PORTC->PCR[9] = PORT_PCR_MUX(1);
    PORTC->PCR[10] = PORT_PCR_MUX(1);

    // SWITCH
    PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
    PTD->PDDR &= ~((unsigned int)1 << PTD1) & ~((unsigned int)1 << PTD2) & ~((unsigned int)1 << PTD3) &
                 ~((unsigned int)1 << PTD4) & ~((unsigned int)1 << PTD5);
    PORTD->PCR[1] = PORT_PCR_MUX(1);
    PORTD->PCR[2] = PORT_PCR_MUX(1);
    PORTD->PCR[3] = PORT_PCR_MUX(1);
    PORTD->PCR[4] = PORT_PCR_MUX(1);
    PORTD->PCR[5] = PORT_PCR_MUX(1);

    // LCD_DISPLAY
    PCC->PCCn[PCC_PORTD_INDEX] &= ~PCC_PCCn_CGC_MASK;
    PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_PCS(0x001);
    PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
    PCC->PCCn[PCC_FTM2_INDEX] &= ~PCC_PCCn_CGC_MASK;
    PCC->PCCn[PCC_FTM2_INDEX] |= (PCC_PCCn_PCS(1) | PCC_PCCn_CGC_MASK);
    PTD->PDDR |= 1 << PTD9 | 1 << PTD10 | 1 << PTD11 | 1 << PTD12 | 1 << PTD13 | 1 << PTD14 | 1 << PTD15;
    PORTD->PCR[9] = PORT_PCR_MUX(1);
    PORTD->PCR[10] = PORT_PCR_MUX(1);
    PORTD->PCR[11] = PORT_PCR_MUX(1);
    PORTD->PCR[12] = PORT_PCR_MUX(1);
    PORTD->PCR[13] = PORT_PCR_MUX(1);
    PORTD->PCR[14] = PORT_PCR_MUX(1);
    PORTD->PCR[15] = PORT_PCR_MUX(1);

    // SEGMENT
    PCC->PCCn[PCC_PORTE_INDEX] = PCC_PCCn_CGC_MASK;
    PTE->PDDR |= 1 << PTE1 | 1 << PTE1 | 1 << PTE3 | 1 << PTE4 | 1 << PTE5 | 1 << PTE6 | 1 << PTE7 | 1 << PTE8;
    PORTE->PCR[1] = PORT_PCR_MUX(1);
    PORTE->PCR[2] = PORT_PCR_MUX(1);
    PORTE->PCR[3] = PORT_PCR_MUX(1);
    PORTE->PCR[4] = PORT_PCR_MUX(1);
    PORTE->PCR[5] = PORT_PCR_MUX(1);
    PORTE->PCR[6] = PORT_PCR_MUX(1);
    PORTE->PCR[7] = PORT_PCR_MUX(1);
    PORTE->PCR[8] = PORT_PCR_MUX(1);
}
void WDOG_disable(void)
{
    WDOG->CNT = 0xD928C520;
    WDOG->TOVAL = 0x0000FFFF;
    WDOG->CS = 0x00002100;
}
void LPIT0_init(uint32_t delay)
{
    uint32_t timeout;
    PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);
    PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK;
    LPIT0->MCR |= LPIT_MCR_M_CEN_MASK;
    timeout = delay * 40000;
    LPIT0->TMR[0].TVAL = timeout;
    LPIT0->TMR[0].TCTRL |= 0x11 | LPIT_TMR_TCTRL_T_EN_MASK;
}
void delay_ms(volatile int ms)
{
    LPIT0_init(ms);
    while ((LPIT0->MSR & LPIT_MSR_TIF0_MASK) == 0)
    {
    }
    lpit0_ch0_flag_counter++;
    LPIT0->MSR |= 0x00 | LPIT_MSR_TIF0_MASK;
}
void Seg_out(int number)
{

    Delaytime = 1;

    num3 = (number / 1000) % 10;
    num2 = (number / 100) % 10;
    num1 = (number / 10) % 10;
    num0 = number % 10;

    // 1000의 자리
    PTE->PSOR = FND_SEL[j];
    PTE->PCOR = 0x7f;
    PTE->PSOR = FND_DATA[num3];
    delay_ms(Delaytime);
    PTE->PCOR = 0xfff;
    j++;

    // 100의 자리
    PTE->PSOR = FND_SEL[j];
    PTE->PCOR = 0x7f;
    PTE->PSOR = FND_DATA[num2];
    delay_ms(Delaytime);
    PTE->PCOR = 0xfff;
    j++;

    // 10의 자리
    PTE->PSOR = FND_SEL[j];
    PTE->PCOR = 0x7f;
    PTE->PSOR = FND_DATA[num1];
    delay_ms(Delaytime);
    PTE->PCOR = 0xfff;
    j++;

    // 1의 자리
    PTE->PSOR = FND_SEL[j];
    PTE->PCOR = 0x7f;
    PTE->PSOR = FND_DATA[num0];
    delay_ms(Delaytime);
    PTE->PCOR = 0xfff;
    j = 0;
}
/* Function Definition Section (start)*/
void toggleLEDsInCycle();
void reToggleLEDsInCycle();
void compareFloors(int *c_floor, int *d_floor);
void lcdInitialize();
// void lcdGoingDown();
// void lcdGoingUp();
// void lcdDoorOpenAndClose();
/* Function Definition Section (end)*/
int main(void)
{
    PORT_init();
    WDOG_disable();
    SOSC_init_8MHz();
    SPLL_init_160MHz();
    NormalRUNmode_80MHz();
    SystemCoreClockUpdate();
    delay_ms(20);

    /* LED_Init */
    PTC->PSOR |=
        (1 << PTC1) | (1 << PTC2) | (1 << PTC3) | (1 << PTC5) | (1 << PTC7) | (1 << PTC8) | (1 << PTC9) | (1 << PTC10);

    lcdinit();
    delay_ms(200);
    int c_floor = 1;
    int d_floor = 1;

    // Ready
    for (int i = 5; i > 0; i--)
    {
        Seg_out(i);
        delay_ms(1000);
    }
    //& Start
    Seg_out(c_floor); // Initially located on the first floor
    lcdInitialize();  //-> lcd? ?? ??? ????? ????? ????!
    while (1)
    {
        // button 1 press
        if (!(PTD->PDIR & (1 << PTD1)))
        {
            d_floor = 1;
            compareFloors(&c_floor, &d_floor);
        }
        // button 2 press
        if (!(PTD->PDIR & (1 << PTD2)))
        {
            d_floor = 2;
            compareFloors(&c_floor, &d_floor);
        }
        // button 3 press
        if (!(PTD->PDIR & (1 << PTD3)))
        {
            d_floor = 3;
            compareFloors(&c_floor, &d_floor);
        }
        // button 4 press
        if (!(PTD->PDIR & (1 << PTD4)))
        {
            d_floor = 4;
            compareFloors(&c_floor, &d_floor);
        }
        // button 5 press
        if (!(PTD->PDIR & (1 << PTD5)))
        {
            d_floor = 5;
            compareFloors(&c_floor, &d_floor);
        }
    }
}

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