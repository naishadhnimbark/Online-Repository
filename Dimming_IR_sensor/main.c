#include <msp430g2553.h>

#define     LED0                  BIT0
#define     LED1                  BIT6

unsigned int value=0;
void ConfigureAdc(void)
{
  /* Configure ADC  Channel */
  ADC10CTL1 = INCH_5 + ADC10DIV_3 ;         // Channel 5, ADC10CLK/4
  ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;  //Vcc & Vss as reference
  ADC10AE0 |= BIT5;                         //P1.5 ADC option
}
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  BCSCTL1 = CALBC1_1MHZ;                    // Set range
  DCOCTL = CALDCO_1MHZ;
  BCSCTL2 &= ~(DIVS_3);                  // SMCLK = DCO = 1MHz
  P1DIR |= LED0 + LED1;
  P1SEL |= BIT5;                             //ADC Input pin P1.5
  P1OUT &= ~(LED0 + LED1);

  ConfigureAdc();
  __enable_interrupt();                     // Enable interrupts.
  while(1)
    {
      __delay_cycles(1000);                   // Wait for ADC Ref to settle
      ADC10CTL0 |= ENC + ADC10SC;             // <span class="fub881vt5l" id="fub881vt5l_18">Sampling</span> and conversion start
      __bis_SR_register(CPUOFF + GIE);        // LPM0 with interrupts enabled
      value = ADC10MEM;
      if (value>511)
      {
         P1OUT &= ~(LED0 + LED1);
         P1OUT |= LED0;
      }
      else
      {
         P1OUT &= ~(LED0 + LED1);
         P1OUT |= LED1;}
      }
  }
// ADC10 interrupt service routine
#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Return to active mode
}
