#pragma once

#include <stdbool.h>
extern bool global_vibro_on;

/** Initialize vibro
 */
// void furi_hal_vibro_init(); // Not to be used in programs

/** Turn on/off vibro
 *
 * @param[in]  value  new state
 */
void furi_hal_vibro_on(bool value);