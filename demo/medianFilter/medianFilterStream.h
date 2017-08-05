#ifndef MEDIAN_FILTER_STREAM_H
#define MEDIAN_FILTER_STREAM_H

#include <deque>
#include <algorithm>
#include <string>
#include <sstream>

class MedianFilterStream
{
public:
  MedianFilterStream(int wS) : windowSize(wS)
  { }

  void in(float data)
  {
    if (window.size() == windowSize)
    {
      window.pop_front();
    }

    window.push_back(data);
  }

  float out()
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

  float getMedian(std::deque<float> w)
  {
    std::sort(w.begin(), w.end());
    return w[w.size() / 2];
  }

  std::deque<float> window; 
  int windowSize;
};

#endif
