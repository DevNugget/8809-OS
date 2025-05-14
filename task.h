#ifndef TASK_H
#define TASK_H

void switch_to_user_mode();
void switch_to_user_mode_elf(uint32_t entry_point);
void return_to_usermode(uint32_t eip);

#endif
