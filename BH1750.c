/**
 * @file BH1750.c
 * @author M.A.G (m.gala@mgala.eu)
 * @brief STM32 HAL-based procedures for BH1750 light sensor with I2C interface
 * @version 1.01
 * @date 2024-02-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "main.h"
#include "i2c.h"
#include "BH1750.h"

#define I2C_READ_BIT				0x01
#define I2C_WRITE_BIT				0x00

#define I2C_BH1750_ADDRESS 			0x23

#define	BH1750_POWER_DOWN			0x00
#define	BH1750_POWER_ON				0x01
#define	BH1750_RESET				0x07
#define BH1750_CONVERSION_FACTOR	1.2


/**
 * @brief Reset sensor (output registers) 
 * 
 * @param hi2c - I2C instance 
 * @return execution status 
 */

HAL_StatusTypeDef BHI1750Reset(I2C_HandleTypeDef *hi2c)
{
    uint8_t send_buffer = BH1750_RESET;
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(hi2c, ((I2C_BH1750_ADDRESS<<1)|I2C_WRITE_BIT), &send_buffer, sizeof(send_buffer), 20);
    return status;
}


/**
 * @brief Send power on command to chip
 * 
 *
 * @param hi2c - I2C instance 
 * @return execution status
 */
HAL_StatusTypeDef BHI1750PowerOn(I2C_HandleTypeDef *hi2c)
{
    uint8_t send_buffer = BH1750_POWER_ON;
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(hi2c, ((I2C_BH1750_ADDRESS<<1)|I2C_WRITE_BIT), &send_buffer, sizeof(send_buffer), 20);
    return status;
}


/**
 * @brief Send command to set resolution of measurement 
 * Allowed values are beetween 31 and 254, default value is 69
 *
 * @param hi2c - I2C instance
 * @param resolution as described above
 * @return execution status
 */
HAL_StatusTypeDef BHI1750SetResolution(I2C_HandleTypeDef *hi2c, uint8_t resolution)
{
    if ((resolution<31)||(resolution>254)) return HAL_ERROR;
    uint8_t send_buffer[2] = {0b01000000,0b01100000};
    //two lines below: preparing values to send to set MTreg, according to page 5 of BHI1750 datasheet
    send_buffer[0] = send_buffer[0]|(resolution>>5);
    send_buffer[1] = send_buffer[1]|(resolution&0b00011111);
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(hi2c, ((I2C_BH1750_ADDRESS<<1)|I2C_WRITE_BIT), send_buffer, sizeof(send_buffer), 20);
    return status;
}

/**
 * @brief Start measuring in continuous mode (hires, hires2 or lowres) as set in function call 
 * 
 * @param hi2c - I2C instance
 * @param mode BH1750_CONT_HIRES_MODE, BH1750_CONT_HIRES2_MODE or BH1750_CONT_LOWRES_MODE
 * @return execution status
 */

HAL_StatusTypeDef BHI1750StartContinuous(I2C_HandleTypeDef *hi2c, uint8_t mode)
{
    if ((!(mode == BH1750_CONT_HIRES_MODE)||(mode == BH1750_CONT_HIRES2_MODE)||(mode == BH1750_CONT_LOWRES_MODE))) return HAL_ERROR;
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(hi2c, ((I2C_BH1750_ADDRESS<<1)|I2C_WRITE_BIT), &mode, sizeof(mode), 20);
    return status; 
}

/**
 * @brief Start single measure (hires, hires2 or lowres) as set in function call 
 * 
 * @param hi2c - I2C instance
 * @param mode BH1750_OT_HIRES_MODE, BH1750_OT_HIRES2_MODE or BH1750_OT_LOWRES_MODE
 * @return execution status
 */

HAL_StatusTypeDef BHI1750StartSingle(I2C_HandleTypeDef *hi2c, uint8_t mode)
{
    if ((!(mode == BH1750_OT_HIRES_MODE)||(mode == BH1750_OT_HIRES2_MODE)||(mode == BH1750_OT_LOWRES_MODE))) return HAL_ERROR;
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(hi2c, ((I2C_BH1750_ADDRESS<<1)|I2C_WRITE_BIT), &mode, sizeof(mode), 20);
    return status; 
}


/**
 * @brief Read light intensity value from sensor (in lx) 
 * Remember to allow sensor to get (typically, at hires mode) 120ms of delay, 
 * after you call start measuring functions, to take measurement
 * If you change sensor sensitivity you must set different delay according to datasheet
 * 
 * @param hi2c - I2C instance
 * @return float light intensity in lx or -1.0 if error
 */

float BHI1750GetLightValueLx(I2C_HandleTypeDef *hi2c)
{
    uint8_t receive_buffer[2];
    if (HAL_I2C_Master_Receive(hi2c, (I2C_BH1750_ADDRESS<<1)|I2C_READ_BIT, receive_buffer, sizeof(receive_buffer), 20) == HAL_OK)
    {
	    uint16_t light_received = (receive_buffer[0]<<8)|receive_buffer[1];
	    float lightLx = light_received*BH1750_CONVERSION_FACTOR;
	    return lightLx;
    }
    else
    {
        return (-1.0);
    }
}

