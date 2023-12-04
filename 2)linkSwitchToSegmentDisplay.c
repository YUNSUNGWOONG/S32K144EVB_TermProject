#include "clocks_and_modes.h"
#include "device_registers.h"

int lpit0_ch0_flag_counter = 0; /*< LPIT0 timeout counter */

void PORT_init(void)
{
    
    PCC->PCCn[PCC_PORTE_INDEX] = PCC_PCCn_CGC_MASK; 

    PTE->PDDR |=
        1 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6 | 1 << 7 | 1 << 11; 

    PORTE->PCR[1] = PORT_PCR_MUX(1);                                  
    PORTE->PCR[2] = PORT_PCR_MUX(1);                                  
    PORTE->PCR[3] = PORT_PCR_MUX(1);                                  
    PORTE->PCR[4] = PORT_PCR_MUX(1);                                  
    PORTE->PCR[5] = PORT_PCR_MUX(1);                                  
    PORTE->PCR[6] = PORT_PCR_MUX(1);                                  
    PORTE->PCR[7] = PORT_PCR_MUX(1);                                 
    PORTE->PCR[11] = PORT_PCR_MUX(1); /* Port D11: MUX = GPIO */
}

void LPIT0_init(uint32_t delay)
{
    uint32_t timeout;
    PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);    
    PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK; 
    LPIT0->MCR |= LPIT_MCR_M_CEN_MASK; 
    timeout = delay * 40000;
    LPIT0->TMR[0].TVAL = timeout; 
    LPIT0->TMR[0].TCTRL |= 0x11;  
}

void delay_ms(volatile int ms)
{
    LPIT0_init(ms); 
    while (0 == (LPIT0->MSR & 0x01 ))
    {
    }                         
    lpit0_ch0_flag_counter++; 
    LPIT0->MSR |= 0x00;       
}


/* FND_DATA(number) */
void num(int nom);
/* DIGIT_SELECT */
void Seg_out(int number);

int main(void)
{
    int count = 94;

    PORT_init();           /* Configure ports */
    SOSC_init_8MHz();      /* Initialize system oscilator for 8 MHz xtal */
    SPLL_init_160MHz();    /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
    NormalRUNmode_80MHz(); /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */

    while (1)
    {
        Seg_out(count);
    }
}

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
    int j;
    int d10, d1;

    d10 = number % 100 / 10;
    d1 = number % 10;

    for (j = 0; j < 10; j++)
    {

        // 10
        num(d10);
        PTE->PSOR |= 1 << 10;
        PTE->PCOR |= 1 << 11;
        delay_ms(2);

        // 1
        num(d1);
        PTE->PCOR |= 1 << 10;
        PTE->PSOR |= 1 << 11;
        delay_ms(2);
    }
}
