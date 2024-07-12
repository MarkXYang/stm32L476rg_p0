#include <inttypes.h>
#include <stdbool.h>

#define BIT(x) (1UL << (x))
#define PIN(bank, num) ((((bank) -'A') << 8) | (num))
#define PINNO(pin) ((pin) & 0xFF)
#define PINBANK(pin) ((pin) >> 8)

struct gpio {
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
};
#define GPIO(bank) ((struct gpio *) (0x48000000 + 0x400 * (bank)))

enum {GPIO_INPUT, GPIO_OUTPUT, GPIO_AF, GPIO_ANALOG};

static inline void gpio_set_mode(uint16_t pin, uint8_t mode) {
    struct gpio *gpio = GPIO(PINBANK(pin)); // GPIO initilized with BANK
    int n = PINNO(pin);                     // GPIO Pin num
    gpio->MODER &= ~(3U << (n * 2));        // Clear GPIO pin setting
    gpio->MODER |= (mode & 3) << (n * 2);   // Set GPIO mode
}

struct rcc {
    volatile uint32_t CR, ICSCR, CFGR, PLLCFGR, PLLSAI1CFGR, PLLSAI2CFGR, CIER, CIFR, CICR, RESERVED0,\
             AHB1RSTR, AHB2RSTR, AHB3RSTR, RESERVED1,\
             APB1RSTR1, APB1RSTR2, APB2RSTR, RESERVED2,\
             AHB1ENR, AHB2ENR, AHB3ENR, RESERVED3,\
             APB1ENR1, APB1ENR2, APB2ENR, RESERVED4,\
             AHB1SMENR, AHB2SMENR, AHB3SMENR, RESERVED5,\
             APB1SMENR1, APB1SMENR2, APB2SMENR, RESERVED6,\
             CCIPR, RESERVED7,\
             BDCR, CSR, CRRCR, CCIPR2;
};

#define RCC ((struct rcc *) 0x40021000)

static inline void gpio_write(uint16_t pin, bool val) {
    struct gpio *gpio = GPIO(PINBANK(pin));
    gpio->BSRR = (1U << PINNO(pin)) << (val ? 0 : 16);
}

static inline void spin(volatile uint32_t count) {
    while(count--) {
        asm("nop");
    }
}

int main(void)
{
    uint16_t led = PIN('A', 5);
    RCC->AHB2ENR |= BIT(PINBANK(led));  // Enable GPIO clock for GPIO BANK A
    gpio_set_mode(led, GPIO_OUTPUT); // Set LED to output mode
    while(1) {
        gpio_write(led, true);
        spin(500000);
        gpio_write(led, false);
        spin(500000);
    }
    return 0;
}

// startup code
__attribute__((naked, noreturn)) void _reset(void) {
    /* memset .bss to zero, and copy .data section to RAM region */
    extern long _sbss, _ebss, _sdata, _edata, _sidata;
    for(long *dst = &_sbss; dst < &_ebss; dst++) *dst = 0;
    for(long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

    main(); /* call main() here */
    while(1) {
        (void) 0;
    }
}

extern void _estack(void); /* Defined in link.ld */

/* 16 standard and 91 STM32-specific handlers */
__attribute__((section(".vectors"))) void (*const tab[16 + 91])(void) = {
    _estack, _reset};
