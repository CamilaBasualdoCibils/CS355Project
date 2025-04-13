#include <pch.hpp>
#include "Driver/Driver.hpp"
#include <fstream>
int main()
{
    Driver driver;
    driver.RunAllTests();


std::stringstream ss;


const uint64_t max_thread_test = std::thread::hardware_concurrency();
std::cerr << "Running " << max_thread_test << " Threads\n";
const uint64_t Test_count_each = 10;
const uint64_t N = 10000000;
for (int thr_cnt = 1; thr_cnt <= max_thread_test;thr_cnt++) {

    const uint64_t progress_bar_char_count = max_thread_test;
    const float progress =  (float(thr_cnt)/max_thread_test);
    const int char_filled = progress*progress_bar_char_count;
    std::cout << "\r[";
    for (int i = 0; i < char_filled;i++) {
        std::cout << 'X';
    }
    for (int i = 0; i < progress_bar_char_count-char_filled;i++) {
        std::cout << '-';
    }
    std::cout << "] " <<(int)(progress*100)<<"%    " << std::flush;

    for (int test_count = 0; test_count< Test_count_each;test_count++ ) {
        RecordTimeType time = driver.IotaIntTestTimed(N,true,thr_cnt);
        ss << thr_cnt << "," 
        << std::fixed << std::setprecision(6) << std::chrono::duration<double>(time).count()
        <<std::endl;
    }
}
std::cout << "\n";


    
std::ofstream csvFile("mergesort.csv");
if (!csvFile.is_open()) {
    std::cerr << "Error: Could not open file!" << std::endl;
    return 1;
}

    // Append the data to the file
csvFile << ss.str();


// close it somewhere
csvFile.close();
}