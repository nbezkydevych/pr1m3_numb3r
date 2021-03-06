// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H

#include <algorithm>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cstddef> 
#include <chrono>

using std::cout;
using std::cin;
using std::max_element;
using std::endl;
using std::ref;
using std::swap;
using std::vector;
using std::thread;
using std::mutex;
using std::size_t;
using std::chrono::duration_cast;
using std::chrono::hours;
using std::chrono::minutes;
using std::chrono::seconds;
using std::chrono::microseconds;
using std::chrono::high_resolution_clock;

#endif //PCH_H
