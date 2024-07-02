#include <iostream>

#include <gtest/gtest.h>

#include "pointer/pointer.h"

TEST(PointerTest, AllCases) {
  // ptr
  cait::ptr<int> ptr1(new int{10});
  std::cout << "ptr1: " << *ptr1 << std::endl;
  ptr1.destroy();
  ptr1.reset(new int{20});
  std::cout << "ptr1: " << *ptr1 << std::endl;

  // uptr
  cait::uptr<int> uptr1(new int(30));
  std::cout << "uptr1: " << *uptr1 << std::endl;
  cait::uptr<int> uptr2 = std::move(uptr1);
  std::cout << "uptr2: " << *uptr2 << std::endl;
  uptr1 = new int(40);
  std::cout << "uptr1: " << *uptr1 << std::endl;
  std::cout << "uptr2: " << *uptr2 << std::endl;
  uptr1.reset(new int(50));
  std::cout << "uptr1: " << *uptr1 << std::endl;
  uptr2 = uptr1.release();
  std::cout << "uptr2: " << *uptr2 << std::endl;

  // sptr
  cait::sptr<int> sptr1(new int(60));
  std::cout << "sptr1: " << *sptr1 << std::endl;

  cait::sptr<int> sptr2 = sptr1;
  std::cout << "sptr2: " << *sptr2 << std::endl;

  sptr1 = new int(70);
  std::cout << "sptr1: " << *sptr1 << std::endl;
  std::cout << "sptr2: " << *sptr2 << std::endl;

  sptr2.reset(new int(80));
  std::cout << "sptr2: " << *sptr2 << std::endl;

  // wptr
  cait::wptr<int> wptr1(sptr1);
  std::cout << "wptr1: " << *wptr1 << std::endl;
  sptr1.reset(new int(90));
  std::cout << "wptr1: " << *wptr1 << std::endl;

  if (wptr1.expired()) {
    std::cout << "wptr1 expired." << std::endl;
  }
  ASSERT_TRUE(true);
}
