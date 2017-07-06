#include "../../lib/components/drivers/Adafruit_BNO055/Adafruit_BNO055.h"

int main()
{
  Adafruit_BNO055 sensor_gyro = Adafruit_BNO055();
  while (!sensor_gyro.begin())
  {
    std::cerr << "Couldn't detect BNO055 gyroscope!" << std::endl;
  }
  sensor_gyro.setExtCrystalUse(true);  

  imu::Vector<3> acceleration = sensor_gyro.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  imu::Vector<3> euler = sensor_gyro.getVector(Adafruit_BNO055::VECTOR_EULER);

  std::stringstream ss;

  ss << acceleration.x() << ","
     << acceleration.y() << ","
     << acceleration.z() << ";"
     << euler.x() << ","
     << euler.y() << ","
     << euler.z();

  std::cout << ss.str() << std::endl;
}
}
