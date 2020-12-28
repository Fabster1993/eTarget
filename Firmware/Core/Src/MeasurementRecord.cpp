/*
 * MeasurementRecord.cpp
 *
 *  Created on: Sep 15, 2020
 *      Author: Fabian Hegg
 */

#include "MeasurementRecord.h"

MeasurementRecord::MeasurementRecord()
{
  reset();
}

void MeasurementRecord::reset()
{
  measurementOngoing = false;
  registeredSensorA = false;
  registeredSensorB = false;
  registeredSensorC = false;
  registeredSensorD = false;
  timeSensorA = 0;
  timeSensorB = 0;
  timeSensorC = 0;
  timeSensorD = 0;
}

bool MeasurementRecord::newRecordCapured() const
{
  return registeredSensorA && registeredSensorB && registeredSensorC && registeredSensorD;
}
