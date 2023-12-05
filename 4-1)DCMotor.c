#include "device_registers.h"
#include "clocks_and_modes.h"
#include "ADC.h"
//#include "math.h"
int lpit0_ch0_flag_counter = 0; /*< LPIT0 timeout counter */




void LPIT0_init (uint32_t delay)
{
   uint32_t timeout;

	  PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);    
	  PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK; 

	  LPIT0->MCR |= LPIT_MCR_M_CEN_MASK;  

  timeout=delay* 40;
  LPIT0->TMR[0].TVAL = timeout;     
  LPIT0->TMR[0].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK;
                                    
}

void delay_us (volatile int us){
   LPIT0_init(us);           
   while (0 == (LPIT0->MSR & LPIT_MSR_TIF0_MASK)) {} 
               lpit0_ch0_flag_counter++;         
               LPIT0->MSR |= LPIT_MSR_TIF0_MASK; 
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


 void PORT_init (void)
{

  PCC->PCCn[PCC_PORTD_INDEX ]|=PCC_PCCn_CGC_MASK;   
  PORTD->PCR[0]|=PORT_PCR_MUX(2);           		
  PORTD->PCR[1]|=PORT_PCR_MUX(2);           		
}




int main(void)
{
  uint32_t adcResultInMv=0;
  int D=0;
  SOSC_init_8MHz();      
  SPLL_init_160MHz();   
  NormalRUNmode_80MHz(); 
  FTM_init();
  ADC_init();            
  PORT_init();           
	  for(;;)
	  {
    	  convertAdcChan(13);                   
    	  while(adc_complete()==0){}            
    	  adcResultInMv = read_adc_chx();      

    	  D=adcResultInMv*3.0;

    	  FTM0->COMBINE |= FTM_COMBINE_SYNCEN1_MASK | FTM_COMBINE_COMP1_MASK | FTM_COMBINE_DTEN1_MASK;

    	  FTM0->CONTROLS[2].CnV=FTM_CnV_VAL(D); //8000~0 duty; ex(7200=> Duty 0.1 / 800=>Duty 0.9)
    	  FTM0->CONTROLS[3].CnV=FTM_CnV_VAL(D); //8000~0 duty; ex(7200=> Duty 0.1 / 800=>Duty 0.9)

			}



	  }
