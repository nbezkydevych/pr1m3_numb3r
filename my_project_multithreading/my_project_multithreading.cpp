// my_project_multithreading.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

typedef unsigned long long int ULL;

struct pair {
	ULL res, a, b;
	pair() : res(0), a(0), b(0) {} //default constructor
};

void calc_max_palindrome(const vector<ULL> &vec_prime_numb, vector <pair> &vec_res_pair, size_t current_part, size_t number_all_part);
void calc_max_palindrome_one_thread(const vector<ULL> &vec_prime_numb, vector <pair> &vec_res_pair);
void calc_max_palindrome_v2(const vector<ULL> &vec_prime_numb, pair &vec_res_pair, size_t current_part, size_t number_all_part);
bool compare_pair(pair a, pair b);
void time_exec(microseconds ms);

mutex mtx;

int main() {
	ULL A, B;
	cout << "Enter A and B number.\n";
	cin >> A >> B;
	auto start = high_resolution_clock::now();
	if (A > B) std::swap(A, B);
	cout << "A: " << A << " B: " << B << endl;

	const ULL from_range = A;
	const ULL to_range = B;

	vector <bool> prime(to_range + 1, 1);
	prime[0] = prime[1] = 0;
	vector <ULL> colletion;
	colletion.reserve(to_range / 6);

	//find prime numbers by eratosthenes sieve
	for (size_t i = 0; i < to_range; ++i) {
		if (1 == prime[i]) {
			if (from_range < i) {
				colletion.push_back(i);
			}
			if (i * 1Ui64 * i <= to_range) {
				for (size_t j = i * i; j < to_range; j += i) {
					prime[j] = 0;
				}
			}
		}
	}

	cout << "capacity \t= " << colletion.capacity() << endl
		<< "size \t\t= " << colletion.size() << endl
		<< "max prime \t= " << colletion[colletion.size() - 1] << endl;


	//find max palindrome wich get by multiplication prime a and b numbers 
	vector <pair> vec_pair;
	pair max;

	//calc_max_palindrome_one_thread(colletion, vec_pair);
	
	const int length = 6;
	thread thread_array[length];
	for (int i = 0; i < length; i++) {
		thread_array[i] = thread(calc_max_palindrome, ref(colletion), ref(vec_pair), i, length);
		//thread_array[i] = thread(calc_max_palindrome_v2, ref(colletion), ref(max), i, length);
	}

	for (int i = 0; i < length; i++) {
		if (thread_array[i].joinable()) {
			thread_array[i].join();
		}
	}

	cout << "vec_pair size = " << vec_pair.size() << endl;

	if (!vec_pair.empty()) {
		max = *max_element(vec_pair.begin(), vec_pair.end(), compare_pair);		
	} else {
		cout << "vector is empty\n";
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);

	cout << max.a << " * " << max.b << " = " << max.res << endl;

	time_exec(duration);	

	return 0;
}

void time_exec(microseconds ms) {
	auto secs = duration_cast<seconds>(ms);
	ms -= duration_cast<microseconds>(secs);
	auto mins = duration_cast<minutes>(secs);
	secs -= duration_cast<seconds>(mins);
	auto hour = duration_cast<hours>(mins);
	mins -= duration_cast<minutes>(hour);

	cout << "Time taken by program is" << endl
		<< "Hours\t\t:" << hour.count() << endl
		<< "Minutes\t\t:" << mins.count() << endl
		<< "Seconds\t\t:" << secs.count() << endl
		<< "Milliseconds\t:" << ms.count() << endl;
}
		

void calc_max_palindrome(const vector<ULL> &vec_prime_numb, vector <pair> &vec_res_pair, size_t current_part, size_t number_all_part) {
	pair result;
	size_t size_current_part = vec_prime_numb.size() / number_all_part;
	size_t size_mod = 0;

	if (current_part == number_all_part - 1) {
		size_mod = vec_prime_numb.size() % number_all_part;
	}

	size_t begin = current_part * size_current_part;
	size_t end = ((current_part + 1) * size_current_part + size_mod) - 1;
	
/*	mtx.lock();
	cout << "begin current_part = " << current_part << " from = " << begin << " to = " << end << endl;
	mtx.unlock();*/

	ULL a = 0, b = 0, m = 0, n = 0, t = 0;

	for (size_t n1 = end; n1 > begin; --n1) {
		a = vec_prime_numb[n1];
		for (size_t n2 = vec_prime_numb.size() - 1; n2 > 0; --n2) {
			b = vec_prime_numb[n2];
			m = a * b;
			n = m;
			t = 0;

			//cout << "m = " << m << endl;
			while (0 != m) {
				t = t * 10 + m % 10;
				m /= 10;
			}
			//cout << "t = " << t << endl << endl;

			if (n == t && result.res < t) {
				result.a = a;
				result.b = b;
				result.res = t;
				//mtx.lock();
				//cout << "current_part = " << current_part << " t_res = " << t << endl;
				//mtx.unlock();
			}
		}
	}
	mtx.lock();
	//cout << "end current_part = " << current_part << endl;	
	vec_res_pair.push_back(result);
	mtx.unlock();
}

void calc_max_palindrome_one_thread(const vector<ULL> &vec_prime_numb, vector <pair> &vec_res_pair) {
	pair result;
	size_t begin = 0;
	size_t end = vec_prime_numb.size()-1;

	/*	mtx.lock();
		cout << "begin current_part = " << current_part << " from = " << begin << " to = " << end << endl;
		mtx.unlock();*/

	ULL a = 0, b = 0, m = 0, n = 0, t = 0;

	for (size_t n1 = end; n1 > begin; --n1) {
		a = vec_prime_numb[n1];
		for (size_t n2 = vec_prime_numb.size() - 1; n2 > 0; --n2) {
			b = vec_prime_numb[n2];
			m = a * b;
			n = m;
			t = 0;

			//cout << "m = " << m << endl;
			while (0 != m) {
				t = t * 10 + m % 10;
				m /= 10;
			}
			//cout << "t = " << t << endl << endl;

			if (n == t && result.res < t) {
				result.a = a;
				result.b = b;
				result.res = t;
				//mtx.lock();
				//cout << "current_part = " << current_part << " t_res = " << t << endl;
				//mtx.unlock();
			}
		}
	}
	mtx.lock();
	//cout << "end current_part = " << current_part << endl;	
	vec_res_pair.push_back(result);
	mtx.unlock();
}


void calc_max_palindrome_v2(const vector<ULL> &vec_prime_numb, pair &max_pair, size_t current_part, size_t number_all_part) {

	size_t size_current_part = vec_prime_numb.size() / number_all_part;
	size_t size_mod = 0;

	if (current_part == number_all_part - 1) {
		size_mod = vec_prime_numb.size() % number_all_part;
	}

	size_t begin = current_part * size_current_part;
	size_t end = ((current_part + 1) * size_current_part + size_mod) - 1;
	
/*	mtx.lock();
	cout << "begin current_part = " << current_part << " from = " << begin << " to = " << end << endl;
	mtx.unlock();*/

	ULL a = 0, b = 0, m = 0, n = 0, t = 0;

	for (size_t n1 = end; n1 > begin; --n1) {
		a = vec_prime_numb[n1];
		for (size_t n2 = vec_prime_numb.size() - 1; n2 > 0; --n2) {
			b = vec_prime_numb[n2];
			m = a * b;
			n = m;
			t = 0;

			//cout << "m = " << m << endl;
			while (0 != m) {
				t = t * 10 + m % 10;
				m /= 10;
			}
			//cout << "t = " << t << endl << endl;

			if (n == t && max_pair.res < t) {
				mtx.lock();
				max_pair.a = a;
				max_pair.b = b;
				max_pair.res = t;
				//cout << "current_part = " << current_part << " t_res = " << t << endl;
				mtx.unlock();
			}
		}
	}
/*	mtx.lock();
	cout << "end current_part = " << current_part << endl;
	mtx.unlock();*/
}


bool compare_pair(pair a, pair b) {
	return (a.res < b.res);
}