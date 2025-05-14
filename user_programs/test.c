#include "syscall.h"

void _start() {
    const char* msg = "hello from userland\n";
    syscall_write(msg);

    // Exit to kernel (if you implement an exit syscall, do it here)
    syscall_exit();
}
