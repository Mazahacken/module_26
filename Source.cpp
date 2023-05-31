#include <iostream>
#include <thread>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>

struct sums_storage {
private:
	int sum_1_ = 0;
	int sum_2_ = 0;
	int sum_3_ = 0;
	int sum_4_ = 0;
	int call_count_ = 1;
	// when each thread will call set sum method
	// call_counter will auto increment
	// so each sum will be writen safely whit out conflicting whit the threads
	// in other words there is no difference wich thread will call set sum first
public:
	sums_storage() = default;
	void set_sum(const int& input_sum) {
		switch (call_count_) {
		case 1:
			sum_1_ = input_sum;
			break;
		case 2:
			sum_2_ = input_sum;
			break;
		case 3:
			sum_3_ = input_sum;
			break;
		case 4:
			sum_4_ = input_sum;
			break;
		}
		call_count_++;
	}

	int get_sum(int sum_number) {

		switch (sum_number) {
		case 1:
			return sum_1_;
			break;
		case 2:
			return sum_2_;
			break;
		case 3:
			return sum_3_;
			break;
		case 4:
			return sum_4_;
			break;
		}
	}
};

sums_storage global_sums_storage;

class ThreadCounter {
	int sum_ = 0;
public:
	ThreadCounter() = default;
	~ThreadCounter() {
	}
	void operator() (int* n, int& m) {
		for (int i = 0; i < m; i++) {
			sum_ += n[i];
		}
		global_sums_storage.set_sum(sum_);
	}
};

int main() {
	std::cout << "thread counter 1.0" << std::endl;
	auto main_thread_start = std::chrono::high_resolution_clock::now();
	// time check for main thread
	ThreadCounter thread_counter;
	// functional object whit overloaded operator()
	int total_sum = 0;
	// this variable will be used as total sum when all threads will finish work

	const int size = 20;
	int* arr = new int[size];
	// main array
	int l_border = 1;
	int range_len = 1000000;

	srand(time(nullptr));
	for (int i = 0; i < size; i++) {
		arr[i] = l_border + std::rand() % range_len;
	} // willing up array whit random numbers

	int parts = size / 4;
	// dividing array into 4 parts

	int* a1 = new int[parts];
	int* a2 = new int[parts];
	int* a3 = new int[parts];
	int* a4 = new int[parts];


	int sub_counter = 0;
	// sub_counter will be reset for each sub array copy

	int i = 0;
	for (; i < parts; i++) {
		a1[i] = arr[i];
	}

	std::cout << "sub array 1 : ";
	for (auto j = 0; j < parts; j++) {
		std::cout << a1[j] << " ";
	}
	std::cout << std::endl;

	for (; i < parts * 2; i++) {
		a2[sub_counter] = arr[i];
		sub_counter++;
	}

	std::cout << "sub array 2 : ";
	for (auto j = 0; j < parts; j++) {
		std::cout << a2[j] << " ";
	}
	std::cout << std::endl;

	sub_counter = 0;
	for (; i < parts * 3; i++) {
		a3[sub_counter] = arr[i];
		sub_counter++;
	}

	std::cout << "sub array 3 : ";
	for (auto j = 0; j < parts; j++) {
		std::cout << a3[j] << " ";
	}
	std::cout << std::endl;

	sub_counter = 0;
	for (; i < parts * 4; i++) {
		a4[sub_counter] = arr[i];
		sub_counter++;
	}
	std::cout << "sub array 4 : ";
	for (auto j = 0; j < parts; j++) {
		std::cout << a4[j] << " ";
	}
	std::cout << std::endl;

	auto start = std::chrono::high_resolution_clock::now();
	std::thread t1(thread_counter, a1, std::ref(parts));

	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "thread 1 time elapsed : " << elapsed.count() << " sec" << std::endl;

	start = std::chrono::high_resolution_clock::now();
	std::thread t2(thread_counter, a2, std::ref(parts));

	finish = std::chrono::high_resolution_clock::now();
	elapsed = finish - start;
	std::cout << "thread 2 time elapsed : " << elapsed.count() << " sec" << std::endl;

	start = std::chrono::high_resolution_clock::now();
	std::thread t3(thread_counter, a3, std::ref(parts));

	finish = std::chrono::high_resolution_clock::now();
	elapsed = finish - start;
	std::cout << "thread 3 time elapsed : " << elapsed.count() << " sec" << std::endl;

	start = std::chrono::high_resolution_clock::now();
	std::thread t4(thread_counter, a4, std::ref(parts));

	finish = std::chrono::high_resolution_clock::now();
	elapsed = finish - start;
	std::cout << "thread 4 time elapsed : " << elapsed.count() << " sec" << std::endl;
	// checking time for how long each thread worked

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	// waiting for threads to finish work

	std::cout << "sum 1 = " << global_sums_storage.get_sum(1) << std::endl;
	std::cout << "sum 2 = " << global_sums_storage.get_sum(2) << std::endl;
	std::cout << "sum 3 = " << global_sums_storage.get_sum(3) << std::endl;
	std::cout << "sum 4 = " << global_sums_storage.get_sum(4) << std::endl;

	total_sum = global_sums_storage.get_sum(1) + global_sums_storage.get_sum(2)
		+ global_sums_storage.get_sum(3) + global_sums_storage.get_sum(4);
	// claculating total sum

	std::cout << "total sum = " << total_sum << std::endl;

	auto main_thread_finish = std::chrono::high_resolution_clock::now();
	elapsed = main_thread_start - main_thread_finish;
	std::cout << "main thread time elapsed : " << elapsed.count() << " sec" << std::endl;

	std::cout << "program end!" << std::endl;

	delete[] arr;
	delete[] a1;
	delete[] a2;
	delete[] a3;
	delete[] a4;
	// cleaning up
	return 0;
}