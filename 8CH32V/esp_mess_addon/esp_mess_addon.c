#include "../ch32v003fun/ch32v003fun.h"
#include "ch32v_hal.inl"
#include <stdio.h>
// #include "TestClass.h"
// #include "i2c_slave.h"

/* some bit definitions for systick regs */
#define SYSTICK_SR_CNTIF (1<<0)
#define SYSTICK_CTLR_STE (1<<0)
#define SYSTICK_CTLR_STIE (1<<1)
#define SYSTICK_CTLR_STCLK (1<<2)
#define SYSTICK_CTLR_STRE (1<<3)
#define SYSTICK_CTLR_SWIE (1<<31)

volatile uint32_t systick_cnt;

// Start up the SysTick IRQ
void systick_init(void) {
	SysTick->CTLR = 0;                  // disable default SysTick behavior   
	NVIC_EnableIRQ(SysTicK_IRQn);       // enable the SysTick IRQ
	
	/* Set the tick interval to 1ms for normal op */
	SysTick->CMP = (FUNCONF_SYSTEM_CORE_CLOCK/1000)-1;
	
	/* Start at zero */
	SysTick->CNT = 0;
	systick_cnt = 0;
	
	/* Enable SysTick counter, IRQ, HCLK/1 */
	SysTick->CTLR = SYSTICK_CTLR_STE | SYSTICK_CTLR_STIE | SYSTICK_CTLR_STCLK;
}

// SysTick ISR just counts ticks
// note - the __attribute__((interrupt)) syntax is crucial!
void SysTick_Handler(void) __attribute__((interrupt));
void SysTick_Handler(void) {
	// move the compare further ahead in time. as a warning, if more than this length of time
	// passes before triggering, you may miss your interrupt.
	SysTick->CMP += (FUNCONF_SYSTEM_CORE_CLOCK/1000);
	SysTick->SR = 0;     // clear IRQ
	systick_cnt++;       // update counter
}

uint32_t delayTime = 1000;

uint8_t bh17Out[2];
uint8_t sht31ReadCmd[2];

volatile uint8_t i2c_registers[32] = {0x00};
volatile uint32_t systick_cnt;

void toggleLed() {
   digitalWrite(0xC0, 1);
   digitalWrite(0xA1, 1);
   Delay_Ms(delayTime);
   digitalWrite(0xC0, 0);
   digitalWrite(0xA1, 0);
   Delay_Ms(delayTime);
}

int main() {
	uint32_t count = 0;
	
	SystemInit();
	Delay_Ms(100);

   pinMode(0xC0, OUTPUT);
   pinMode(0xA1, OUTPUT);
   pinMode(0xD0, INPUT_PULLUP);
   pinMode(0xD2, INPUT_PULLUP);
   pinMode(0xD3, INPUT_PULLUP);
   digitalWrite(0xA1, 1);

   // SetupI2CSlave(0xfa, i2c_registers, sizeof(i2c_registers));

   // I2CInit(0xC1, 0xC2, 100000);
   // uint8_t bh17Mode[1] = { 0x13 };
   // I2CWrite(0x23, bh17Mode, sizeof(bh17Mode));

	// init systick @ 1ms rate
	printf("initializing systick...");
	systick_init();

	while(1) {
      // int check = I2CTest(0x23);
      // I2CRead(0x23, bh17Out, 2);
      // printf("\nCheck = %d, Value = %02X %02X", check, bh17Out[0], bh17Out[1]);
      
      // uint8_t read1 = digitalRead(0xD0);
      // uint8_t read2 = digitalRead(0xD2);
      // uint8_t read3 = digitalRead(0xD3);
      // printf("\nRead1 = %u, Read2 = %u, Read3 = %u", read1, read2, read3);

      // if (i2c_registers[0] & 1) { // Turn on LED (PD0) if bit 1 of register 0 is set
      //    digitalWrite(0xC0, 1);
      //    printf("\n\n**********IM HEREEEEEEEEEEEE");
      //    Delay_Ms(1000);
      // } else {
      //    digitalWrite(0xC0, 0);
      // }

      toggleLed();
		// Delay_Ms(1000);
		printf( "Print #: %lu / Milliseconds: %lu / CNT: %lu\n", count++, systick_cnt, SysTick->CNT );

      // int read = UART_Read(100);
      // if (read != -1) {
      //    toggleLed();
      //    // printf("\nch32v: %d", read);
      // }
	}
}
