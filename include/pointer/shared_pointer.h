// Copyright (c) 2024, Alexander Szawrowski
//
// This file is distributed under the MIT License.
// See LICENSE file for details.

#ifndef CAITLYN_MEMORY_SHARED_POINTER_H
#define CAITLYN_MEMORY_SHARED_POINTER_H

#include <memory>
#include <stdexcept>
#include <atomic>

namespace cait {

template <typename T>
class sptr {
public:
  using value_type = T;
  using pointer = T*;
  using reference = T&;
  using counter = std::atomic_size_t;
  using null_type = std::nullptr_t;

public:
  sptr() = default;

  sptr(const pointer data) : data_{data}, ref_count_{data ? new counter(1) : nullptr} {}

  sptr(const sptr& other) : data_{other.data_}, ref_count_{other.ref_count_} {
    ref_count_->fetch_add(1, std::memory_order_relaxed);
  }

  sptr(sptr&& other) noexcept : data_{other.data_}, ref_count_{other.ref_count_} {
    other.data_ = nullptr;
    other.ref_count_ = nullptr;
  }

  ~sptr() {
    release();
  }

public:
  sptr& operator=(pointer data) {
    reset(data);
    return *this;
  }

  sptr& operator=(const sptr& other) {
    if (this != &other) {
      release();
      data_ = other.data_;
      ref_count_ = other.ref_count_;
      if (ref_count_) {
        ref_count_->fetch_add(1);
      }
    }
    return *this;
  }

  sptr& operator=(sptr&& other) noexcept {
    if (this != &other) {
      release();
      data_ = other.data_;
      ref_count_ = other.ref_count_;
      other.data_ = nullptr;
      other.ref_count_ = nullptr;
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
  bool operator==(const sptr& other) const { return data_ == other.data_; }
  bool operator!=(const sptr& other) const { return data_ != other.data_; }

  bool operator==(null_type) const { return data_ == nullptr; }
  bool operator!=(null_type) const { return data_ != nullptr; }

public:
  void reset(pointer data = nullptr) {
    if (data_ != data) {
      release();
      data_ = data;
      ref_count_ = data ? new counter(1) : nullptr;
    }
  }

private:
  void release() {
    if (ref_count_ && ref_count_->fetch_sub(1, std::memory_order_relaxed) == 0) {
      delete data_;
      delete ref_count_;
    }
  }

private:
  pointer data_{};
  counter* ref_count_{};
};

}  // namespace cait

#endif  // CAITLYN_MEMORY_SHARED_POINTER_H
