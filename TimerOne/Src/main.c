#include <stdint.h>

int main(void)
{
	/*Declaración de registros*/
    uint32_t *RCCAPB2      = (uint32_t*) 0x40023844;
    uint32_t *TimerOneCR1  = (uint32_t*) 0x40010000;
    uint32_t *TimerOneEGR  = (uint32_t*) 0x40010014;
    uint32_t *TimerOneCCMR = (uint32_t*) 0x40010018;
    uint32_t *TimerOneCCER = (uint32_t*) 0x40010020;
    uint32_t *TimerOneCNT  = (uint32_t*) 0x40010024;
    uint32_t *TimerOnePSC  = (uint32_t*) 0x40010028;
    uint32_t *TimerOneCCR1 = (uint32_t*) 0x40010034;
    uint32_t *TimerOneBDTR = (uint32_t*) 0x40010044;
    uint32_t *TimerOneARR  = (uint32_t*) 0x4001002C;

    uint32_t *RCCGPIOA     = (uint32_t*) 0x40023830;
    uint32_t *GPIOAMODER   = (uint32_t*) 0x40020000;
    uint32_t *GPIOAAFRH    = (uint32_t*) 0x40020024;

    /* Habilitar reloj */
    *RCCAPB2 |= (1 << 0);   // TIM1
    *RCCGPIOA |= (1 << 0);  // GPIOA

    /* Configuración del PWM */
    *TimerOnePSC  = 15;     // 16 MHz / 16 = 1 MHz
    *TimerOneARR  = 999;    // 1 MHz / 1000 = 1 kHz
    *TimerOneCCR1 = 100;    // duty inicial 10%
    *TimerOneCNT  = 0;

    /* Auto-reload preload */
    *TimerOneCR1 |= (1 << 7);

    /* PWM mode 1 + preload CCR1 */
    *TimerOneCCMR &= ~(7 << 4);
    *TimerOneCCMR |= (6 << 4) | (1 << 3);

    /* Habilitar salida CH1 */
    *TimerOneCCER &= ~(1 << 1);
    *TimerOneCCER |= (1 << 0);

    /* Main output enable */
    *TimerOneBDTR |= (1 << 15);

    /* Configurar PA8 como AF1 TIM1_CH1 */
    *GPIOAMODER &= ~(3 << 16);
    *GPIOAMODER |=  (2 << 16);

    *GPIOAAFRH &= ~(0xF << 0);
    *GPIOAAFRH |=  (1 << 0);

    /* Actualizar registros */
    *TimerOneEGR |= (1 << 0);

    /* Iniciar timer */
    *TimerOneCR1 |= (1 << 0);

    uint32_t duty = 100;
    int step = 20;

    while(1)
    {
        *TimerOneCCR1 = duty;

        duty += step;

        if(duty >= 900)
        {
            duty = 900;
            step = -20;
        }

        if(duty <= 100)
        {
            duty = 100;
            step = 20;
        }

        for(volatile int i = 0; i < 50000; i++);
    }
}
