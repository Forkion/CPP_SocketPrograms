#include "../hdr/lib.hpp"

namespace lib {

void counting_sort(std::string& str) noexcept {
  std::string new_str;
  int coincidence_arr[10] = {0};
  short size_new_str = 0;

  for (auto& x : str) {
    int digit = x - '0';
    coincidence_arr[digit]++;
    if (digit % 2 == 0) {
      size_new_str += 2UL;
    } else {
      ++size_new_str;
    }
  }

  new_str.resize(size_new_str);
  short index_new_str = 0;
  for (int digit = 9; digit >= 0; --digit) {
    while (coincidence_arr[digit]-- > 0) {
      if (digit % 2 == 0) {
        new_str[index_new_str++] = 'K';
        new_str[index_new_str++] = 'B';
      } else {
        new_str[index_new_str++] = digit + '0';
      }
    }
  }

  str.clear();
  str = std::move(new_str);
}

[[nodiscard]] unsigned sum(std::string& str) noexcept {
  unsigned res = 0;

  for (size_t i = 0; i < str.size(); ++i) {
    if (str[i] < '0' || str[i] > '9') {
      continue;
    }

    res += str[i] - '0';
  }

  return res;
}

// Формулировка условия функции 3 в задании очень неоднозначная, прошу принять к
// сведению моё скромное мнение
[[nodiscard]] bool check_multiple_of_32(std::string& str,
                                        unsigned& new_sum) noexcept {
  // Если смысл функции в проверке на то, что количество символов строки(размер
  // строки) > 2 и сумма элементов(цифр) кратна 32
  //   if ((str.size() > 2UL) && ((new_sum % 32) == 0)) {

  // Если смысл функции в проверке строки на то, что сумма элементов(цифр) > 2 и
  // сумма элементов(цифр) кратна 32
  (void)str;
  if ((new_sum > 2U) && ((new_sum % 32) == 0)) {
    return true;
  } else {
    return false;
  }
}

};  // namespace lib
