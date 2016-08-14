#include "LED_drvr_STP16CPC26.h"
#include "LED.h"
#include "ports.h"
#include <stdint.h>

#define COLOR_DEPTH 4

bit_target = 0;

// Notes:
//   red is bit 1
//   green is bit 2
//   blue is bit 0

// Summary - 
void LED_drvr_init(LED_drvr_t * LED_ptr)
{
    LED_ptr->LED_errors.missed_multiplex_timer_interrupts = 0;
    LED_ptr->active_column = 0;
    LED_ptr->active_color_bit = 0;
}


// Summary - 
// param (LED_drvr_t *) LED_ptr - 
// param (uint8_t) LED_multiplex_timer_count - 
void LED_drvr_run(LED_drvr_t *LED_ptr, uint8_t LED_multiplex_timer_count)
{
    uint16_t color_mask = (1 << LED_ptr->active_color_bit);
    uint16_t bitstream = 0;
    uint16_t bit = 0;
    uint8_t row;
    uint8_t i;

    // if we missed some multiplex timer counts, increment the errors
    if (LED_multiplex_timer_count != 1) {
        LED_ptr->LED_errors.missed_multiplex_timer_interrupts += (LED_multiplex_timer_count - 1);
    }
    // build up the bitstream to be clocked out
    for (row = 0; row < 4; row++) { // rows
        /*  --  RED  --   */
        bit = (LED_ptr->LED_array[row][LED_ptr->active_column][RED_IND] & color_mask); // mask for color
        // shift for bitstream placement and compensate for selected color bit
        bitstream |= (bit << ((3 - row) * 3 + RED_IND - LED_ptr->active_color_bit));
        /*  --  GREEN  --   */
        bit = (LED_ptr->LED_array[row][LED_ptr->active_column][GREEN_IND] & color_mask);
        bitstream |= (bit << ((3 - row) * 3 + GREEN_IND - LED_ptr->active_color_bit));
        /*  --  BLUE  --   */
        bit = (LED_ptr->LED_array[row][LED_ptr->active_column][BLUE_IND] & color_mask);
        bitstream |= (bit << ((3 - row) * 3 + BLUE_IND - LED_ptr->active_color_bit));
    }
    // clock out the LED bits
    for(i = 0; i < 12; i++) {
        LEDDRV_CLK_SET(0);
        LEDDRV_SDI_SET((bitstream >> i) & 0x0001);
        LEDDRV_CLK_SET(1);
    }
    // Turn off LED until everything is set (OE is active low)
    LEDDRV_OE_SET(1);
    // Latch the LED driver data
    LEDDRV_LE_SET(1);
    LEDDRV_LE_SET(0);
    // Enable the anode high side
    // the 6th column isn't sequential like the other 5
    if (LED_ptr->active_column != 5) {
        ANO_PORT = ~(1 << LED_ptr->active_column);
    } else {
        ANO_PORT = ~(1 << (LED_ptr->active_column + 1));
    }
    // output the LED driver data
    LEDDRV_OE_SET(0);
    // increment the variables in the LED_ptr struct
    if (LED_ptr->active_column < 5) {
        LED_ptr->active_column += 1;
    } else {
        LED_ptr->active_column = 0;
        // update the color bit too
        if (LED_ptr->active_color_bit < COLOR_DEPTH) {
            LED_ptr->active_color_bit += 1;
        } else {
            LED_ptr->active_color_bit = 0;
        }
    }
}