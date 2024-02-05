/**
 * @file BH1750.h
 * @author M.A.G (m.gala@mgala.eu)
 * @brief STM32 HAL-based procedures for BH1750 light sensor with I2C interface
 * @version 1.01
 * @date 2024-02-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef INC_BH1750_H_
#define INC_BH1750_H_

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#define BH1750_CONT_HIRES_MODE		0x10 	//continuous hires mode
#define BH1750_CONT_HIRES2_MODE		0x11	//continuous hires mode 2
#define BH1750_CONT_LOWRES_MODE		0x12	//continuous low res mode
#define BH1750_OT_HIRES_MODE		0x20	//one time hires mode
#define BH1750_OT_HIRES2_MODE		0x21	//one time hires mode 2
#define BH1750_OT_LOWRES_MODE		0x22	//one time low res mode

HAL_StatusTypeDef BHI1750Reset(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef BHI1750PowerOn(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef BHI1750SetResolution(I2C_HandleTypeDef *hi2c, uint8_t resolution);
HAL_StatusTypeDef BHI1750StartContinuous(I2C_HandleTypeDef *hi2c, uint8_t mode)
HAL_StatusTypeDef BHI1750StartSingle(I2C_HandleTypeDef *hi2c, uint8_t mode);
float BHI1750GetLightValueLx(I2C_HandleTypeDef *hi2c);

#endif /* INC_BH1750_H_ */
