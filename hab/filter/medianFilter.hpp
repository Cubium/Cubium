#include <algorithm>
#include <deque>
#include <vector>

template <typename T>
class median_filter
{
private:
  std::deque<T> data;
  std::vector<T> sorted;
  size_t reserved;

public:
  median_filter(size_t reserved = 5) :
      reserved(reserved)
  {
    data = std::deque<T>();
    sorted = std::vector<T>();
    data.resize(reserved);
    sorted.reserve(reserved);
  }

  void push(T data_point)
  {
    // remove oldest entry if full
    if (data.size() == reserved)
      data.pop_front();

    data.push_back(data_point);
  }

  T median(void)
  {
    sorted = std::vector<T>(data.begin(), data.end());
    sort(sorted.begin(), sorted.end());
    size_t med = sorted.size() / 2;

    return sorted[med];
  }
};
