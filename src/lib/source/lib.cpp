#include "../headers/lib.hpp"

namespace lib_test{

void counting_sort(std::string& str) noexcept{
	std::string new_str;
	int coincidence_arr[10] = {0};
	size_t size_new_str = 0;

	for(auto &x : str){
		int digit = x - '0';
		coincidence_arr[digit]++;
		if(digit % 2 == 0){
			size_new_str += 2UL;
		} else{
			size_new_str++;
		}
	}

	new_str.resize(size_new_str);
	size_t index_new_str = 0;
	for(int digit = 9; digit >= 0; --digit){
		while(coincidence_arr[digit]-- > 0){
			if(digit % 2 == 0){
				new_str[index_new_str++] = 'K';
				new_str[index_new_str++] = 'B';
			} else{
				new_str[index_new_str++] = digit + '0';
			}
		}
	}

	str.clear();
	str = std::move(new_str);
}

[[nodiscard]] unsigned int summ(std::string& str) noexcept{
	unsigned int res = 0;

	for(size_t i = 0; i < str.size(); ++i){
		if(str[i] < '0' || str[i] > '9'){
			continue;
		}

		res += str[i] - '0';
	}

	return res;
}

//Условия этой функции в задаче странные. Если сумма элементов кратна 32, то количество элементов всегда будет больше 2
[[nodiscard]] bool check_multiple_of_32(std::string& str) noexcept{
	if((str.size() > 2UL) && (summ(str) % 32 == 0)){
		return true;
	} else{
		return false;
	}
}

};
