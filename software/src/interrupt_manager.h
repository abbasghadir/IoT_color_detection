#ifndef INTERRUPT_MANAGER_H
#define INTERRUPT_MANAGER_H
#include "Storage_manager.h"

void interrupt_init();
bool pedal_interrupt_result();
bool reset_interrupt_result();
#endif