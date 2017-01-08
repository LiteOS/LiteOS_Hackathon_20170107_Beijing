#pragma once

// Exported types
typedef struct
{
  int HDStatus;
  uint16_t LightOut;
  uint8_t LightIn;
} SensorData_t;

// Sensor data opaque pointer
extern SensorData_t* pSensorData;

extern void DataEngineInit(void);