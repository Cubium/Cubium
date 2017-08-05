#ifndef MEDIAN_FILTER_STREAM_H
#define MEDIAN_FILTER_STREAM_H
// #include <iostream>

namespace { const float PRECISION_VALUE = 0.001; }

template <typename T>
class MedianFilterStream {
public:
  MedianFilterStream() {}
  MedianFilterStream(T* timeBuff, T* valBuff, int buffLen)
    :timeBuff(timeBuff), valBuff(valBuff), buffLen(buffLen){
      timeCounter = 0;
      tempBuffLen = 0;
    }

  int getCurrentLength(){
    return tempBuffLen > buffLen ? buffLen : tempBuffLen;
  }

  void addDataPoint(T val){

    std::cout << "1f" << std::endl;

    tempBuffLen++;
    int currentLength = getCurrentLength();

    std::cout << "2f" << std::endl;

    if (tempBuffLen < buffLen)
    {
      // Handle when buffer is not yet full

      timeBuff[currentLength-1] = val;
      valBuff[currentLength-1] = val;

    } 
    else 
    {
      // If our buffer is full we need to replace values
      std::cout << "4f" << std::endl;

      T toReplace = timeBuff[timeCounter];

      int valPosition = findInBuff(valBuff, currentLength, toReplace);

      timeBuff[timeCounter] = val;

      if (valPosition >= 0 &&  valPosition < currentLength)
      {
        std::cout << "5f" << std::endl;
        valBuff[valPosition] = val;
      } 
      else 
      {
         std::cout << "6f" << std::endl;
        //TODO: Add some sort of error handling
      }
    }

    sortBuffer(valBuff, currentLength);
    std::cout << "Nope" << std::endl;
    timeCounter = incrementCount(timeCounter, buffLen);
    std::cout << "yup" << std::endl;
  }

  T getFilteredDataPoint()
  {
    int currentLength = getCurrentLength();
    sortBuffer(valBuff, currentLength);
    int midPoint = currentLength / 2;
    return valBuff[midPoint];
  }

  void sortBuffer(T* buff, int buffLen){
    for(int i = 1; i < buffLen; i++){
      for(int j = i - 1; j >= 0; j--){
        if(buff[j+1] < buff[j]){
          buffSwap(buff, j+1, j);
        }
      }
    }
  }

  void buffSwap(T* buff, int i, int j){
    T temp = buff[i];
    buff[i] = buff[j];
    buff[j] = temp;
  }

  int inline incrementCount(int timeCounter, int buffLen){
    return ++timeCounter %= buffLen;
  }

  int findInBuff(T* buff,int buffLen, T find){
    for(int i = 0; i < buffLen; i++){
      float diff = buff[i] - find;
      if(diff < 0) diff *= -1;
      if(diff <= PRECISION_VALUE) return i;
    }
    return -1;
  }

  protected:
  int timeCounter;
  int tempBuffLen;
  int buffLen;
  T* timeBuff;
  T* valBuff;
};
#endif
