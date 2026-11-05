#include <cstddef>
#include <new>
#include <utility>

template <typename T> class Vector {
public:
  Vector() : data_(nullptr), size_(0), capacity_(0) {}

  ~Vector() { destroyAndFree(data_, size_); }

  Vector(std::size_t count)
      : data_(static_cast<T *>(::operator new(sizeof(T) * count))), size_(0),
        capacity_(count) {
    for (; size_ < count; ++size_) {
      new (&data_[size_]) T();
    }
  }

  Vector(std::size_t count, const T &val)
      : data_(static_cast<T *>(::operator new(sizeof(T) * count))), size_(0),
        capacity_(count) {
    for (; size_ < count; ++size_) {
      new (&data_[size_]) T(val);
    }
  }

  Vector(const Vector &other)
      : data_(static_cast<T *>(::operator new(sizeof(T) * other.capacity_))),
        size_(0), capacity_(other.capacity_) {
    for (; size_ < other.size_; ++size_)
      new (&data_[size_]) T(other.data_[size_]);
  }

  Vector(Vector &&other) noexcept
      : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  Vector &operator=(Vector other) {
    swap(*this, other);
    return *this;
  }

  friend void swap(Vector &lhs, Vector &rhs) noexcept {
    std::swap(lhs.data_, rhs.data_);
    std::swap(lhs.size_, rhs.size_);
    std::swap(lhs.capacity_, rhs.capacity_);
  }

  void push_back(T val) {
    if (size_ == capacity_) {
      std::size_t new_capacity_ = capacity_ == 0 ? 1 : capacity_ * 2;
      std::size_t new_size_ = 0;
      T *new_data_ =
          static_cast<T *>(::operator new(sizeof(T) * new_capacity_));

      for (; new_size_ < size_; ++new_size_)
        new (&new_data_[new_size_]) T(std::move_if_noexcept(data_[new_size_]));

      destroyAndFree(data_, size_);

      data_ = new_data_;
      capacity_ = new_capacity_;
      size_ = new_size_;
    }

    new (&data_[size_]) T(val);
  }

  T &operator[](std::size_t idx) { return *(data_ + idx); }
  const T &operator[](std::size_t idx) const { return *(data_ + idx); }

private:
  T *data_;
  std::size_t size_;
  std::size_t capacity_;

  void destroyAndFree(T *data, std::size_t n) noexcept {
    for (std::size_t i = 0; i < n; ++i)
      data[i].~T();
    ::operator delete(data);
  }
};

int main() {
  // default init
  Vector<int> v;

  // Size init
  Vector<int> v1(5);

  // Size & fill
  Vector<int> v2(5, 10);

  // Copy Ctor
  Vector<int> v3(v2);

  // Copy assign
  Vector<int> v4 = v3;

  // Move ctor
  Vector<int> v5(std::move(v4));

  // Move assign
  Vector<int> v6 = std::move(v5);

  // push back
  Vector<int> v7;
  v7.push_back(1);
  v7.push_back(2);
  v7.push_back(3);

  // Operator []
}