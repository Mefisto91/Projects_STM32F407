
#include <stdint.h>

int main(void)
{
	uint32_t *Rcc 		= (uint32_t*) 0x40023830;
	uint32_t *GpioDMode = (uint32_t*) 0x40020C00;
	uint32_t *GpioDOut 	= (uint32_t*) 0x40020C14;

	*Rcc 		|= (1 << 3);
	*GpioDMode 	&= ~(3 << 30);
	*GpioDMode 	|= (1 << 30);

	while(1)
	{
		for(uint32_t i=1; i<100000; i++)
			*GpioDOut |= (1 << 15);

		for(uint32_t i=1; i<1000000; i++)
			*GpioDOut &= ~(1 << 15);
	}
}
