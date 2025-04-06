#include <iostream> //cout, endl
#include <random>  
#include <algorithm>//copy, random_shuffle
#include <iterator> //ostream_iterator
#include <ctime>    //std::time (NULL) to seed srand
#include "mergesort.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <fstream>



int main() {
    // Open the CSV file in append mode
    std::ofstream csvFile("mergesort.csv", std::ios::app);
    if (!csvFile.is_open()) {
        std::cerr << "Error: Could not open file!" << std::endl;
        return 1;
    }
	const int n = 10000;
	int a[n];
	for ( int i=0; i<n; ++i ) { a[i] = i; }
    //std::copy(a, a + n, std::ostream_iterator<int>(std::cout, " ")); 
    //std::cout << std::endl;
    bool OK = true;
    for ( int i=0; i<5000 && OK; ++i ) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle( a, a+n, g );
        //std::copy(a, a + n, std::ostream_iterator<int>(std::cout, " ")); 
        //std::cout << std::endl;
        int number_of_threads = 1;
        auto tp1 = std::chrono::high_resolution_clock::now();
        mergesort(a,0,n);
        auto tp2 = std::chrono::high_resolution_clock::now();
        auto time = tp2 - tp1;
        // Append the data to the file
        csvFile
            << number_of_threads << "," 
            << std::fixed << std::setprecision(6) << std::chrono::duration<double>(time).count()
            << std::endl;
        //check result
        for ( int i=0; i<n; ++i ) {
            if ( a[i] != i ) {
                //std::cout << "Error at index " << i << std::endl;
                OK = false;
            }
        }
    }

    if ( OK ) {
        //std::cout << "OK" << std::endl;
    }
    csvFile.close();
    return !OK;
}
