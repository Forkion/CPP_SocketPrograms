#include "../hdr/main_test.hpp"

using namespace lib;

// counting_sort
TEST(counting_sort, test_true) {
  std::string arr1 = {"1234665"};
  std::string arr2 = {"KBKB5KB3KB1"};
  counting_sort(arr1);
  EXPECT_EQ(arr1, arr2);
}

TEST(counting_sort, test_false) {
  std::string arr1 = {"1234665"};
  std::string arr2 = (arr1);
  counting_sort(arr1);
  EXPECT_NE(arr1, arr2);
}

// sum
TEST(sum, test_true) {
  std::string arr1 = {"1234665"};
  std::string arr2 = {"KBKB5KB3KB1"};
  counting_sort(arr1);
  EXPECT_EQ(sum(arr1), 9);
}

TEST(sum, test_false) {
  std::string arr1 = {"12346659"};
  std::string arr2 = {"KBKB5KB3KB1"};
  counting_sort(arr1);
  EXPECT_NE(sum(arr1), 9);
}

// check_multiple_of_32
TEST(check_multiple_of_32, test_true) {
  std::string arr1 = {"0994633365"};
  std::string arr2 = {"99KBKB5KB333KB"};
  counting_sort(arr1);
  unsigned new_sum = sum(arr1);
  bool res = check_multiple_of_32(arr1, new_sum);
  EXPECT_EQ(res, true);
}

TEST(check_multiple_of_32, test_false) {
  std::string arr1 = {"1234665"};
  std::string arr2 = {"KBKB5KB3KB1"};
  counting_sort(arr1);
  unsigned new_sum = sum(arr1);
  bool res = check_multiple_of_32(arr1, new_sum);
  EXPECT_NE(res, true);
}
