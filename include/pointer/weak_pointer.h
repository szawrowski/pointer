// Copyright (c) 2024, Alexander Szawrowski
//
// This file is distributed under the MIT License.
// See LICENSE file for details.

#ifndef CAITLYN_MEMORY_WEAK_POINTER_H
#define CAITLYN_MEMORY_WEAK_POINTER_H

#include "shared_pointer.h"

namespace cait {

template <typename T>
class wptr {
public:
  using value_type = T;
  using pointer = T*;
  using reference = T&;
  using resource_type = sptr<T>;
  using null_type = std::nullptr_t;

public:
  wptr() : data_(nullptr) {}
  explicit wptr(const resource_type& resource) : data_{resource} {}
  wptr(const wptr& other) : data_{other.data_} {}
  wptr(wptr&& other) noexcept : data_{std::move(other.data_)} {}

public:
  wptr& operator=(const wptr& other) {
    data_ = other.data_;
    return *this;
  }

  wptr& operator=(const resource_type& sp) {
    this->data_ = sp;
    return *this;
  }

  wptr& operator=(wptr&& other) noexcept {
    data_ = std::move(other.data_);
    return *this;
  }

public:
  reference operator*() const {
    if (data_ == nullptr || !data_.get()) {
      throw std::runtime_error("Dereferencing null or expired weak pointer.");
    }
    return *data_;
  }

  pointer operator->() const {
    if (data_ == nullptr || !data_.get()) {
      throw std::runtime_error("Dereferencing null or expired weak pointer.");
    }
    return data_.get();
  }

public:
  bool operator==(const wptr& other) const { return data_ == other.data_; }
  bool operator!=(const wptr& other) const { return data_ != other.data_; }

  bool operator==(std::nullptr_t) const { return data_ == nullptr; }
  bool operator!=(std::nullptr_t) const { return data_ != nullptr; }

public:
  resource_type lock() const {
    if (data_ == nullptr || !data_.get()) {
      return resource_type();
    }
    return data_;
  }

public:
  [[nodiscard]] bool expired() const {
    return data_ == nullptr || !data_.get();
  }

private:
  resource_type data_;
};

}  // namespace cait

#endif  // CAITLYN_MEMORY_WEAK_POINTER_H
