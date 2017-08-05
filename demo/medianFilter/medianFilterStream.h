#ifndef MEDIAN_FILTER_STREAM_H
#define MEDIAN_FILTER_STREAM_H

#include <deque>
#include <algorithm>
#include <stdint.h>
#include <string>
#include <sstream>

template <typename T>
class MedianFilterStream
{
public:
  MedianFilterStream(uint32_t wS) : windowSize(wS)
  { }

  void in(T data)
  {
    if (window.size() == windowSize)
    {
      window.pop_front();
    }

    window.push_back(data);
  }

  T out()
  {
    return getMedian(window);
  }

  std::string print()
  {
    std::stringstream ss;
    ss << "[";
    for (auto&& e : window)
    {
      ss << e << ", ";
    }
    ss << "]";
    return ss.str();
  }

private:

  T getMedian(std::deque<T> w)
  {
    std::sort(w.begin(), w.end());
    return w[windowSize / 2];
  }

  std::deque<T> window; 
  uint32_t windowSize;
};

#endif
