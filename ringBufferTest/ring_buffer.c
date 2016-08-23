#include <stdint.h>
#include <stdio.h>
#include "ring_buffer.h"


// Summary - 
// param (ring_buffer_t *) rb_ptr - 
// param (uint8_t *) buffer_array_ptr - 
// param (uint8_t) buffer_len - 
void ring_buff_init(ring_buffer_t *rb_ptr, uint8_t *buffer_array_ptr, uint8_t buffer_len)
{
    rb_ptr->head = buffer_array_ptr;
    rb_ptr->tail = buffer_array_ptr;
    rb_ptr->start = buffer_array_ptr;
    rb_ptr->end = buffer_array_ptr + buffer_len;
    rb_ptr->unread_items = 0;
    rb_ptr->error = 0;
    rb_ptr->buff_size = buffer_len;
}


// Summary - 
// param (ring_buffer_t *) rb_ptr - 
// param (uint8_t *) return_item_ptr - 
// retval (uint8_t) - 
uint8_t ring_buff_pop(ring_buffer_t *rb_ptr, uint8_t *return_item_ptr)
{
    if (rb_ptr->unread_items == 0) {
        return BUFF_FAIL_UNDERFLOW;
    // } else if(rb_ptr->unread_items == rb_ptr->buff_size){  // not possible
    //     return BUFF_FAIL_OVERFLOW;
    } else {
        // no errors, pop item at the tail of the buffer
        *return_item_ptr = *(rb_ptr->tail);
        rb_ptr->unread_items -= 1;
        // increment the tail pointer
        if(rb_ptr->tail == rb_ptr->end) {
            rb_ptr->tail = rb_ptr->start;
            printf("(pop) reseting tail to start\n");
        } else {
            printf("(pop) incrementing tail\n");
            rb_ptr->tail += 1;
        }
    }
    printf("(pop) unread items: %d\n", rb_ptr->unread_items);
    return PASS;
}


// Summary - 
// param (ring_buffer_t *) rb_ptr - 
// param (uint8_t) new_val - 
// retval (uint8_t) - 
uint8_t ring_buff_push(ring_buffer_t *rb_ptr, uint8_t new_val)
{
    if(rb_ptr->unread_items == rb_ptr->buff_size) {
        return BUFF_FAIL_FULL;
    } else {
        // no errors, push item to the head of the buffer
        *(rb_ptr->head) = new_val;
        rb_ptr->unread_items += 1;
        // increment the head pointer
        if(rb_ptr->head == rb_ptr->end) {
            rb_ptr->head = rb_ptr->start;
            printf("(push) reseting head to start\n");
        } else {
            printf("(push) incrementing head\n");
            rb_ptr->head += 1;
        }
    }
    printf("(push) unread items: %d\n", rb_ptr->unread_items);
    return PASS;
}


// Summary - 
// param (ring_buffer_t *) rb_ptr - 
// retval (uint8_t) - 
uint8_t ring_buff_has_data(ring_buffer_t *rb_ptr)
{
    return rb_ptr->unread_items;
}


// Summary - 
// param (ring_buffer_t *) rb_ptr - 
void ring_buff_reset(ring_buffer_t *rb_ptr)
{
    rb_ptr->head = rb_ptr->start;
    rb_ptr->tail = rb_ptr->start;
    rb_ptr->unread_items = 0;
}