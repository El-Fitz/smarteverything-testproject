#include "Data.h"

void  ft_getData(void) {
  payload.humidity = ft_movingAverage(timer.data, payload.humidity, smeHumidity.readHumidity());
  payload.temp = ft_movingAverage(timer.data, payload.temp, smeHumidity.readTemperature());
  payload.pressure = ft_movingAverage(timer.data, payload.pressure, smePressure.readPressure() - 1000);
}

void  ft_printData(uint8_t param) {
  if (!SerialUSB)
    return ;
  if (param == 0x74) {
    SerialUSB.print("Temp : ");
    SerialUSB.print(payload.temp, DEC);
    SerialUSB.print("ºCelsius");
    printed = 1;
  }
  if (param == 0x68) {
    SerialUSB.print("\tHumidity : ");
    SerialUSB.print(payload.humidity);
    SerialUSB.print("%");
    printed = 1;
  }
  if (param == 0x70) {
    SerialUSB.print("\tPressure : ");
    SerialUSB.print(payload.pressure + 1000);
    SerialUSB.print("mbar");
    printed = 1;
  }
}
