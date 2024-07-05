// Copyright (c) 2024, Alexander Szawrowski
//
// This file is distributed under the MIT License.
// See LICENSE file for details.

#ifndef CAITLYN_MEMORY_UNIQUE_POINTER_H
#define CAITLYN_MEMORY_UNIQUE_POINTER_H

#include <memory>
#include <stdexcept>

namespace cait {

template <typename T>
class uptr {
public:
  using value_type = T;
  using pointer = T*;
  using reference = T&;
  using null_type = std::nullptr_t;

public:
  uptr() = default;
  uptr(const pointer data) : data_{data} {}
  uptr(const uptr& other) = delete;
  uptr(uptr&& other) noexcept : data_{other.data_} { other.data_ = nullptr; }
  ~uptr() { delete data_; }

public:
  uptr& operator=(pointer data) {
    reset(data);
    return *this;
  }

  uptr& operator=(uptr&& other) noexcept {
    if (this != &other) {
      delete data_;
      data_ = other.data_;
      other.data_ = nullptr;
    }
    return *this;
  }

public:
  reference operator*() const {
    if (data_ == nullptr) {
      throw std::runtime_error{"Dereferencing null pointer."};
    }
    return *data_;
  }

  pointer operator->() const {
    if (data_ == nullptr) {
      throw std::runtime_error{"Dereferencing null pointer."};
    }
    return data_;
  }

public:
  pointer get() const { return data_; }

public:
  bool operator==(const uptr& other) const { return data_ == other.data_; }
  bool operator!=(const uptr& other) const { return data_ != other.data_; }

  bool operator==(null_type) const { return data_ == nullptr; }
  bool operator!=(null_type) const { return data_ != nullptr; }

public:
  void reset(pointer new_data = nullptr) {
    if (data_ != new_data) {
      delete data_;
      data_ = new_data;
    }
  }

  pointer release() {
    pointer tmp = data_;
    data_ = nullptr;
    return tmp;
  }

private:
  pointer data_{};
};

}  // namespace cait

#endif  // CAITLYN_MEMORY_UNIQUE_POINTER_H
