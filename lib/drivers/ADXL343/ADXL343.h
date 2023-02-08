#include <Arduino.h>
#if !defined(_ADXL343_H_)
#define _ADXL343_H_

class ADXL343 {
   private:
    uint8_t CS;
    uint32_t CLK;
    
    typedef struct {
        double x;
        double y;
        double z;
    } accel_t;

    void writeRegister();
    void readRegister();

   public:
    ADXL343(uint8_t cs, uint32_t clk);
    double getX();
    double getY();
    double getZ();
    accel_t getAccel();
};

#endif  // _ADXL343_H_
