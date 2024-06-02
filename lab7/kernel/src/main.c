#include "uart1.h"
#include "shell.h"
#include "dtb.h"
#include "vfs.h"
#include "memory.h"
#include "exception.h"
#include "irqtask.h"
#include "timer.h"
#include "sched.h"
#include "syscall.h"
extern thread_t *curr_thread;

void main(char* arg){
    // char input_buffer[CMD_MAX_LEN];

    dtb_ptr = PHYS_TO_VIRT(arg);
    traverse_device_tree(dtb_ptr, dtb_callback_initramfs); // get initramfs location from dtb

    init_allocator();

    uart_init();
    irqtask_init_list();
    timer_list_init();

    // init_thread_sched();
    init_rootfs();

    uart_interrupt_enable();
    uart_flush_FIFO();


    el1_interrupt_enable();  // enable interrupt in EL1 -> EL1
    core_timer_enable();

#if DEBUG
    cli_cmd_read(input_buffer); // Wait for input, Windows cannot attach to SERIAL from two processes.
#endif

    // cli_print_banner();
    init_syscall();
    init_thread_sched();
    cli_cmd_init();
    (  (void (*)()) curr_thread->context.lr)();
}
