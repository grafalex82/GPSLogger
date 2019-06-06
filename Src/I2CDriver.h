#ifndef I2CDRIVER_H
#define I2CDRIVER_H

void initI2C();
uint8_t readByteI2C(uint8_t devaddr, uint8_t memaddr);

#endif // I2CDRIVER_H
