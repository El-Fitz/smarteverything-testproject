#include "BlueTest.h"

uint8_t ft_movingAverage(uint8_t counter, uint8_t currentAverage, uint8_t newVal) {
  unsigned int  temp;

  temp = currentAverage * (counter - 1);
  temp += newVal;
  temp /= counter;
  return ((uint8_t) counter);
}

