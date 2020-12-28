/*
 * MeasurementRecord.h
 *
 *  Created on: Sep 15, 2020
 *      Author: Fabian Hegg
 */

#include <stdint.h>

#ifndef SRC_MEASUREMENTRECORD_H_
#define SRC_MEASUREMENTRECORD_H_

class MeasurementRecord {
public:
  MeasurementRecord();
  void reset();
  bool newRecordCapured() const;

  volatile bool measurementOngoing;
  volatile bool registeredSensorA;
  volatile bool registeredSensorB;
  volatile bool registeredSensorC;
  volatile bool registeredSensorD;
  uint32_t timeSensorA;
  uint32_t timeSensorB;
  uint32_t timeSensorC;
  uint32_t timeSensorD;
  const uint32_t maxMeasureTime = 50000;
};

#endif /* SRC_MEASUREMENTRECORD_H_ */
