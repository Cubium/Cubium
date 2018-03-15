#include <algorithm>
#include <vector>

template <typename T> class median_filter {
private:
  std::vector<T> data;
  std::vector<T> sorted;

public:
  median_filter(size_t reserved = 20) {
    this->data = std::vector<T>();
    this->sorted = std::vector<T>();
    this->data.reserve(reserved);
    this->sorted.reserve(reserved);
  }

  void push(T data_point) {
    // remove oldest entry if full
    if (data.size() == data.capacity())
      data.erase(data.begin());

    data.push_back(data_point);
  }

  T median(void) {
    sorted = data;
    sort(sorted.begin(), sorted.end());
    size_t med = sorted.size() / 2;

    return sorted[med];
  }
};