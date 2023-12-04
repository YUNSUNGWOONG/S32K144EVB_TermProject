#include "clocks_and_modes.h"
#include "device_registers.h"

//SWITCH
#define PTD10 10
#define PTD11 11
#define PTD12 12
#define PTD13 13
#define PTD14 14

int lpit0_ch0_flag_counter = 0; 

void PORT_init(void)
{
    //SEGMENT
    PCC->PCCn[PCC_PORTE_INDEX] = PCC_PCCn_CGC_MASK; 
    PTE->PDDR |=
        1 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6 | 1 << 7 | 1 << 8; 
    PORTE->PCR[1] = PORT_PCR_MUX(1);                                  
    PORTE->PCR[2] = PORT_PCR_MUX(1);                                  
    PORTE->PCR[3] = PORT_PCR_MUX(1);                                  
    PORTE->PCR[4] = PORT_PCR_MUX(1);                                  
    PORTE->PCR[5] = PORT_PCR_MUX(1);                                  
    PORTE->PCR[6] = PORT_PCR_MUX(1);                                  
    PORTE->PCR[7] = PORT_PCR_MUX(1);                                 
    PORTE->PCR[8] = PORT_PCR_MUX(1); 

    //SWITCH
    PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
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
    LPIT0->TMR[0].TCTRL |= 0x11 | LPIT_TMR_TCTRL_T_EN_MASK;  
}
void delay_ms(volatile int ms)
{
    LPIT0_init(ms); 
    while ((LPIT0->MSR & LPIT_MSR_TIF0_MASK) == 0){ lpit0_ch0_flag_counter++; }
    LPIT0->MSR = LPIT_MSR_TIF0_MASK; 
}
/* Function Definition Section (start)*/
void num(int nom); // FND_DATA(number)
void Seg_out(int number); // DIGIT_SELECT
void compareFloors(int c_floor, int d_floor);
/* Function Definition Section (end)*/
int main(void)
{
    PORT_init();          
    WDOG_disable();   
    SOSC_init_8MHz();      
    SPLL_init_160MHz();    
    NormalRUNmode_80MHz();

    int c_floor = 1;
    int d_floor;
    int error = 0;

    Seg_out(c_floor); // Initially located on the first floor

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
void num(int nom)
{
switch (nom)
    {
	/*PSOR: set 1 -> ON, PCOR: set 0 -> OFF */
    case 0:
        PTE->PSOR |= 1 << 1; // PTE1; // FND A ON
        PTE->PSOR |= 1 << 2; // PTE2; // FND B ON
        PTE->PSOR |= 1 << 3; // PTE3; // FND C ON
        PTE->PSOR |= 1 << 4; // PTE4; // FND D ON
        PTE->PSOR |= 1 << 5; // PTE5; // FND E ON
        PTE->PSOR |= 1 << 6; // PTE6; // FND F ON
        PTE->PCOR |= 1 << 7; // PTE7; //
        break;
    case 1:
        PTE->PCOR |= 1 << 1; // PTE1; //
        PTE->PSOR |= 1 << 2; // PTE2; // FND B ON
        PTE->PSOR |= 1 << 3; // PTE3; // FND C ON
        PTE->PCOR |= 1 << 4; // PTE4; //
        PTE->PCOR |= 1 << 5; // PTE5; //
        PTE->PCOR |= 1 << 6; // PTE6; //
        PTE->PCOR |= 1 << 7; // PTE7; //
        break;
    case 2:
        PTE->PSOR |= 1 << 1; // PTE1; // FND A ON
        PTE->PSOR |= 1 << 2; // PTE2; // FND B ON
        PTE->PCOR |= 1 << 3; // PTE3; //
        PTE->PSOR |= 1 << 4; // PTE4; // FND D ON
        PTE->PSOR |= 1 << 5; // PTE5; // FND E ON
        PTE->PCOR |= 1 << 6; // PTE6; //
        PTE->PSOR |= 1 << 7; // PTE7; // FND G ON
        break;
    case 3:
        PTE->PSOR |= 1 << 1; // PTE1; // FND A ON
        PTE->PSOR |= 1 << 2; // PTE2; // FND B ON
        PTE->PSOR |= 1 << 3; // PTE3; // FND C ON
        PTE->PSOR |= 1 << 4; // PTE4; // FND D ON
        PTE->PCOR |= 1 << 5; // PTE5; //
        PTE->PCOR |= 1 << 6; // PTE6; //
        PTE->PSOR |= 1 << 7; // PTE7; // FND G ON
        break;
    case 4:
        PTE->PCOR |= 1 << 1; // PTE1; //
        PTE->PSOR |= 1 << 2; // PTE2; // FND B ON
        PTE->PSOR |= 1 << 3; // PTE3; // FND C ON
        PTE->PCOR |= 1 << 4; // PTE4; //
        PTE->PCOR |= 1 << 5; // PTE5; //
        PTE->PSOR |= 1 << 6; // PTE6; // FND F ON
        PTE->PSOR |= 1 << 7; // PTE7; // FND G ON
        break;
    case 5:
        PTE->PSOR |= 1 << 1; // PTE1; // FND A ON
        PTE->PCOR |= 1 << 2; // PTE2; //
        PTE->PSOR |= 1 << 3; // PTE3; // FND C ON
        PTE->PSOR |= 1 << 4; // PTE4; // FND D ON
        PTE->PCOR |= 1 << 5; // PTE5; //
        PTE->PSOR |= 1 << 6; // PTE6; // FND F ON
        PTE->PSOR |= 1 << 7; // PTE7; // FND G ON
        break;
    case 6:
        PTE->PSOR |= 1 << 1; // PTE1; // FND A ON
        PTE->PCOR |= 1 << 2; // PTE2; //
        PTE->PSOR |= 1 << 3; // PTE3; // FND C ON
        PTE->PSOR |= 1 << 4; // PTE4; // FND D ON
        PTE->PSOR |= 1 << 5; // PTE5; // FND E ON
        PTE->PSOR |= 1 << 6; // PTE6; // FND F ON
        PTE->PSOR |= 1 << 7; // PTE7; // FND G ON
        break;
    case 7:
        PTE->PSOR |= 1 << 1; // PTE1; // FND A ON
        PTE->PSOR |= 1 << 2; // PTE2; // FND B ON
        PTE->PSOR |= 1 << 3; // PTE3; // FND C ON
        PTE->PCOR |= 1 << 4; // PTE4; //
        PTE->PCOR |= 1 << 5; // PTE5; //
        PTE->PSOR |= 1 << 6; // PTE6; // FND F ON
        PTE->PCOR |= 1 << 7; // PTE7; //
        break;
    case 8:
        PTE->PSOR |= 1 << 1; // PTE1; // FND A ON
        PTE->PSOR |= 1 << 2; // PTE2; // FND B ON
        PTE->PSOR |= 1 << 3; // PTE3; // FND C ON
        PTE->PSOR |= 1 << 4; // PTE4; // FND D ON
        PTE->PSOR |= 1 << 5; // PTE5; // FND E ON
        PTE->PSOR |= 1 << 6; // PTE6; // FND F ON
        PTE->PSOR |= 1 << 7; // PTE7; // FND G ON
        break;
    case 9:
        PTE->PSOR |= 1 << 1; // PTE1; // FND A ON
        PTE->PSOR |= 1 << 2; // PTE2; // FND B ON
        PTE->PSOR |= 1 << 3; // PTE3; // FND C ON
        PTE->PSOR |= 1 << 4; // PTE4; // FND D ON
        PTE->PCOR |= 1 << 5; // PTE5; //
        PTE->PSOR |= 1 << 6; // PTE6; // FND F ON
        PTE->PSOR |= 1 << 7; // PTE7; // FND G ON
        break;
    }
}
void Seg_out(int number)
{
    for (int j = 0; j < 10; j++)
    {
        // 1
        num(number);
        PTE->PSOR |= 1 << 8;
        delay_ms(2);
    }
}
void compareFloors(int c_floor, int d_floor)
{
    Seg_out(c_floor);
    if(c_floor < d_floor)
    {
        for(int i = c_floor+1; i<= d_floor; i++)
        {
            Seg_out(i);
            delay_ms(1000);
        }
    } else if(c_floor > d_floor){
        for(int i = c_floor-1; i>=d_floor; i--)
        {
            Seg_out(i);
            delay_ms(1000);
        }
    }
    c_floor = d_floor;
}
/* Function Implementation Section (end)*/