/*
 * sensors.h
 *
 *  Created on: Oct 20, 2025
 *      Author: Admin
 */

#ifndef INC_SENSORS_H_
#define INC_SENSORS_H_

#include "main.h"

#define PULSE_PER_ML 5.2

void FLOW_SENSOR_Init();
uint32_t FLOW_SENSOR_GetValue(void);
void FLOW_SENSOR_ResetValue();
uint32_t FLOW_SENSOR_GetValue_ml(void);


#endif /* INC_SENSORS_H_ */
