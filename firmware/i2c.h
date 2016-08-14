#ifndef I2C_H__
#define I2C_H__

#include "hardware.h"
#include <stdint.h>

// I2C constant definitions
#define I2C_MODE_READ  1
#define I2C_MODE_WRITE 0
#define I2C_RETRY_COUNT 3

// I2C return codes
#define I2C_NO_ERRORS 0
#define I2C_ERROR 1

#define I2C_ACK  0
#define I2C_NACK 1


typedef struct {
    uint8_t address;
    uint8_t mode;
} i2c_admin_t;


// function definitions
uint8_t i2c_start(uint8_t address, uint8_t mode);
uint8_t i2c_write_byte(uint8_t reg_addr, uint8_t data);
uint8_t i2c_write(uint8_t start_adr, uint8_t *data_to_write_ptr, uint8_t data_len);
uint8_t i2c_read(uint8_t start_adr, uint8_t *data_out_ptr, uint8_t data_len);

#endif
