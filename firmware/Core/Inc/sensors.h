/*
 * sensors.h
 *
 *  Created on: Oct 20, 2025
 *      Author: Admin
 */

#ifndef INC_SENSORS_H_
#define INC_SENSORS_H_

#include "main.h"

void FLOW_SENSOR_Init();
void FLOW_SENSOR_GetValue(uint16_t* flowValue);
void FLOW_SENSOR_ResetValue();


#endif /* INC_SENSORS_H_ */
