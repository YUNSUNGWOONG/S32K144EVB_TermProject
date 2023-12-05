#include "clocks_and_modes.h"
#include "device_registers.h"
#include "ADC.h"

int lpit0_ch0_flag_counter = 0; /*< LPIT0 timeout counter */

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
    PORTE->PCR[8] = PORT_PCR_MUX(1); /* FND_SELECT */
	
		//VARIABLE_RESISTENCE
		PCC->PCCn[PCC_PORTD_INDEX ]|=PCC_PCCn_CGC_MASK;   
		PORTD->PCR[0]|=PORT_PCR_MUX(2);           		
			PORTD->PCR[1]|=PORT_PCR_MUX(2);  
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
    while (0 == (LPIT0->MSR & 0x01 ))
    {
    }                         
    lpit0_ch0_flag_counter++; 
    LPIT0->MSR |= 0x00 | LPIT_MSR_TIF0_MASK; 
}

void FTM_init (void){
	PCC->PCCn[PCC_FTM0_INDEX] &= ~PCC_PCCn_CGC_MASK;		
	PCC->PCCn[PCC_FTM0_INDEX] |= PCC_PCCn_PCS(0b010)		
								| PCC_PCCn_CGC_MASK;	
	FTM0->SC = FTM_SC_PWMEN2_MASK							
			  |FTM_SC_PWMEN3_MASK
				|FTM_SC_PS(0.5);								
	FTM0->MOD = 8000-1;								
	FTM0->CNTIN = FTM_CNTIN_INIT(0);
	FTM0->CONTROLS[2].CnSC |=FTM_CnSC_MSB_MASK;
	FTM0->CONTROLS[2].CnSC |=FTM_CnSC_ELSA_MASK;			
	FTM0->CONTROLS[3].CnSC |=FTM_CnSC_MSB_MASK;
	FTM0->CONTROLS[3].CnSC |=FTM_CnSC_ELSA_MASK;
	FTM0->SC|=FTM_SC_CLKS(3);

}

/* FND_DATA(number) */
void num(int nom);
/* DIGIT_SELECT */
void Seg_out(int number);

int main(void)
{
		uint32_t adcResultInMv=0;
		int D=0;

    PORT_init();           /* Configure ports */
    SOSC_init_8MHz();      /* Initialize system oscilator for 8 MHz xtal */
    SPLL_init_160MHz();    /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
    NormalRUNmode_80MHz(); /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */

		FTM_init();
		ADC_init(); 
		
		delay_ms(500);
    //Ready 
		for(int i=5; i>0; i--){
			Seg_out(i);
			delay_ms(1000);
		}
		//& Start
    while (1)
    {
			
      convertAdcChan(13);                   
			while(adc_complete()==0){}            
			adcResultInMv = read_adc_chx();      

			D=adcResultInMv*3.0;
			D = D / 1000;
			Seg_out(D);
			
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
    for (int j = 0; j < 10; j++)
    {
        // 1
        num(number);
        PTE->PSOR |= 1 << 8;
        delay_ms(2);
    }
}
