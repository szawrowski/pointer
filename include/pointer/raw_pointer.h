// Copyright (c) 2024, Alexander Szawrowski
//
// This file is distributed under the MIT License.
// See LICENSE file for details.

#ifndef CAITLYN_MEMORY_RAW_POINTER_H
#define CAITLYN_MEMORY_RAW_POINTER_H

#include <memory>
#include <stdexcept>

namespace cait {

template <typename T>
class ptr final {
public:
  using value_type = T;
  using pointer = T*;
  using reference = T&;
  using null_type = std::nullptr_t;

public:
  ptr() = default;
  explicit ptr(pointer data) : data_{data} {}
  ptr(const ptr& other) : data_{other.data_} {}

public:
  ptr& operator=(pointer data) {
    this->data_ = data;
    return *this;
  }

  ptr& operator=(const ptr& other) {
    this->data_ = other.data_;
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
  bool operator==(const ptr& other) const { return data_ == other.data_; }
  bool operator!=(const ptr& other) const { return data_ != other.data_; }

  bool operator==(null_type) const { return data_ == nullptr; }
  bool operator!=(null_type) const { return data_ != nullptr; }

public:
  void reset(pointer new_data = nullptr) {
    delete data_;
    data_ = new_data;
  }

  void destroy() {
    delete data_;
    data_ = nullptr;
  }

private:
  pointer data_{};
};

}  // namespace cait

#endif  // CAITLYN_MEMORY_RAW_POINTER_H
