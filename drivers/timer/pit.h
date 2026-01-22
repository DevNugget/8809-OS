#ifndef PIT_H
#define PIT_H

#include <stdint.h>

#include "../../util.h"
#include "../../idt.h"
#include "../../kernel/console.h"
#include "../../task.h"

void pit_init(uint32_t frequency);

#endif

