/* End of RAM/Start of stack */
/* (96K SRAM) */
_estack = 0x20018000;

MEMORY
{
    FLASH ( rx )      : ORIGIN = 0x08000000, LENGTH = 1024K
    RAM ( rxw )       : ORIGIN = 0x20000000, LENGTH = 96K
    RAM2 ( rxw )       : ORIGIN = 0x10000000, LENGTH = 32K
}