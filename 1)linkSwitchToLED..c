#include "clocks_and_modes.h"
#include "device_registers.h"

//LED
#define PTD1 1
#define PTD2 2
#define PTD3 3
#define PTD4 4
#define PTD5 5
#define PTD6 6
#define PTD7 7
#define PTD8 8

//SWITCH
#define PTD10 10
#define PTD11 11
#define PTD12 12
#define PTD13 13
#define PTD14 14

int lpit0_ch0_flag_counter = 0;

void PORT_init(void)
{
    //LED
    PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
    PTD->PDDR |= 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6 | 1 << 7 | 1 << 8;
    PORTD->PCR[1] = PORT_PCR_MUX(1);
    PORTD->PCR[2] = PORT_PCR_MUX(1);
    PORTD->PCR[3] = PORT_PCR_MUX(1);
    PORTD->PCR[4] = PORT_PCR_MUX(1);
    PORTD->PCR[5] = PORT_PCR_MUX(1);
    PORTD->PCR[6] = PORT_PCR_MUX(1);
    PORTD->PCR[7] = PORT_PCR_MUX(1);
    PORTD->PCR[8] = PORT_PCR_MUX(1);

    //SWITCH
    PTD->PDDR &= ~((unsigned int)1 << PTD10);
    PTD->PDDR &= ~((unsigned int)1 << PTD11);
    PTD->PDDR &= ~((unsigned int)1 << PTD12);
    PTD->PDDR &= ~((unsigned int)1 << PTD13);
    PTD->PDDR &= ~((unsigned int)1 << PTD14);
    PORTD->PCR[10] = PORT_PCR_MUX(1);
    PORTD->PCR[11] = PORT_PCR_MUX(1);
    PORTD->PCR[12] = PORT_PCR_MUX(1);
    PORTD->PCR[13] = PORT_PCR_MUX(1);
    PORTD->PCR[14] = PORT_PCR_MUX(1);


}

void WDOG_disable(void)
{
    WDOG->CNT = 0xD928C520;WDOG->TOVAL = 0x0000FFFF;WDOG->CS = 0x00002100;
}
void LPIT0_init(uint32_t delay)
{
    uint32_t timeout;
    PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);    
    PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK; 
    LPIT0->MCR |= LPIT_MCR_M_CEN_MASK; 
    timeout = delay * 40000;
    LPIT0->TMR[0].TVAL = timeout; 
    LPIT0->TMR[0].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK;
}

void delay_ms(volatile int ms)
{
    LPIT0_init(ms); 
    while ((LPIT0->MSR & LPIT_MSR_TIF0_MASK) == 0){ lpit0_ch0_flag_counter++; }
    LPIT0->MSR = LPIT_MSR_TIF0_MASK; 
}

/* Function Definition Section (start)*/
void toggleLEDsInCycle();
void compareFloors(int c_floor, int d_floor);
/* Function Definition Section (end)*/
int main()
{
    WDOG_disable();        
    PORT_init();          
    SOSC_init_8MHz();      
    SPLL_init_160MHz();    
    NormalRUNmode_80MHz(); 

    /* LED_Init */
    PTD->PSOR |= (1 << PTD1) | (1 << PTD2) | (1 << PTD3) | (1 << PTD4) | (1 << PTD5) | (1 << PTD6) |(1 << PTD7) | (1 << PTD8); 

    int c_floor = 1;
    int d_floor;
    int error = 0;

    while (1)
    {
        // button 1 press
        if (!(PTD->PDIR & (1 << PTD10)))
        {
            d_floor = 1 ;
            compareFloors(c_floor, d_floor);
            
        }
        // button 2 press
        if (!(PTD->PDIR & (1 << PTD11)))
        {
            d_floor = 2 ;
            compareFloors(c_floor, d_floor);

        }
        // button 3 press
        if (!(PTD->PDIR & (1 << PTD12)))
        {
            d_floor = 3 ;
            compareFloors(c_floor, d_floor);

        }
        // button 4 press
        if (!(PTD->PDIR & (1 << PTD13)))
        {
            d_floor = 4 ;
            compareFloors(c_floor, d_floor);

        }
        // button 5 press
        if (!(PTD->PDIR & (1 << PTD14)))
        {
            d_floor = 5 ;
            compareFloors(c_floor, d_floor);

        }
    }
}


/* Function Implementation Section (start)*/
void toggleLEDsInCycle()
{
    // ON -> Wait -> OFF
    PTD->PCOR |= (1 << PTD8);delay_ms(500);PTD->PSOR |= (1 << PTD8);
    PTD->PCOR |= (1 << PTD7);delay_ms(500);PTD->PSOR |= (1 << PTD7);
    PTD->PCOR |= (1 << PTD6);delay_ms(500);PTD->PSOR |= (1 << PTD6);
    PTD->PCOR |= (1 << PTD5);delay_ms(500);PTD->PSOR |= (1 << PTD5);
    PTD->PCOR |= (1 << PTD4);delay_ms(500);PTD->PSOR |= (1 << PTD4);
    PTD->PCOR |= (1 << PTD3);delay_ms(500);PTD->PSOR |= (1 << PTD3);
    PTD->PCOR |= (1 << PTD2);delay_ms(500);PTD->PSOR |= (1 << PTD2);
    PTD->PCOR |= (1 << PTD1);delay_ms(500);PTD->PSOR |= (1 << PTD1);
}
void compareFloors(int c_floor, int d_floor)
{
    if(c_floor < d_floor)
    {
        for(int i = c_floor; i<= d_floor; i++)
        {
            toggleLEDsInCycle();
        }
    } else if(c_floor > d_floor){
        for(int i = c_floor; i>=d_floor; i--)
        {
            toggleLEDsInCycle();
        }
    }
    c_floor = d_floor;
}
/* Function Implementation Section (end)*/


