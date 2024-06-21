The first stm32L476 program

1. The program is to write a simple assembly program on stm32L476rg. Also config the environment/tools
2. The envrionment is Linux. It has been tested at Unbuntu 22.04
3. Install arm compile tools and so on 
    $ openocd -f board/stm32l4discovery.cfg stlink-tools

4. Install OpenOCD
5. Flash with OpenOCD
5.1. start OpenOCD server in one terminal
    $ openocd -f board/stm32l4discovery.cfg
5.2. In another terminal, cd to directory of the project direct. Then run 
    $ arm-none-eabi-gdb -q main.elf
    or $ arm-none-eabi-gdb main.elf
        (gdb) target extended-remote :3333
            Remote debugging using :3333
            0x08000012 in reset_handler ()

    Then in gdb shell, run
    (gdb) load
    Loading section .text, size 0x1c lma 0x8000000
    Start address 0x08000000, load size 28
    Transfer rate: 60 bytes/sec, 28 bytes/write.

5.3. When the program load successfully, type continue, then break it, and check register contents with info registers
    (gdb) c
    Continuing.
    ^C
    Program received signal SIGINT, Interrupt.
    0x08000010 in reset_handler ()
    (gdb) info registers
    r0             0x3d8aa85           64531077
    r1             0x0                 0
    r2             0x0                 0
    r3             0x0                 0
    r4             0x0                 0
    r5             0x0                 0
    r6             0x0                 0
    r7             0xdeadbeef          -559038737
    r8             0x0                 0
    r9             0x0                 0
    r10            0x0                 0
    r11            0x0                 0
    --Type <RET> for more, q to quit, c to continue without paging--
    r12            0x0                 0
    sp             0x20018000          0x20018000
    lr             0xffffffff          -1
    pc             0x8000010           0x8000010 <reset_handler+8>
    xPSR           0x1000000           16777216
    fpscr          0x0                 0
    msp            0x20018000          0x20018000
    psp            0x0                 0x0
    primask        0x0                 0
    basepri        0x0                 0
    faultmask      0x0                 0
    control        0x0                 0

    We can see the contents of register r7 is 0xdeadbeef


reference
1. https://vivonomicon.com/category/stm32_baremetal_examples/
2. https://github.com/cpq/bare-metal-programming-guide