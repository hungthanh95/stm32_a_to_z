#include "ex7_fault_handlers.h"

static void EnableHandlers(void);

void RunApp(void) {
    EnableHandlers();

    // Trigger Usage Fault
    /* Fill a meaningless value */
    *(uint32_t*)0x20010000 = 0XFFFFFFFF;

    // set pc with lsb being 1 to indicate Thumb state */
    void (*pFunc)(void) = (void*)0x20010001;

    pFunc();
    while (1) {};
}

static void EnableHandlers(void) {
    uint32_t* pSHCSR = (uint32_t *) 0xE000ED24;
    *pSHCSR |= (1 << 16); // Memory Fault
    *pSHCSR |= (1 << 17); // Bus Fault
    *pSHCSR |= (1 << 18); // Usage Fault

    uint32_t* pCCR = (uint32_t *)0xE000ED14;
    *pCCR |= (1 << 4); // Div by Zero
}

void MemManage_Handler(void) {
    printf("Exception: Usage Fault\n");
    while (1);
}

void BusFault_Handler(void) {
    printf("Exception: Usage Fault\n");
    while (1);
}

void UsageFault_Handler(void) {
    printf("Exception: Usage Fault\n");
    while (1);
}
