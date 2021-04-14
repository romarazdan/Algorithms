/*******************************************************************************
 * Name        : sieve.cpp
 * Author      : Roma Razdan
 * Date        : Septemeber 18th, 2020
 * Description : Sieve of Eratosthenes
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

class PrimesSieve {
public:
    PrimesSieve(int limit);

    ~PrimesSieve() {
        delete [] is_prime_;
    }

    int num_primes() const {
        return num_primes_;
    }

    void display_primes() const;

private:
    // Instance variables
    bool * const is_prime_;
    const int limit_;
    int num_primes_, max_prime_;

    // Method declarations
    int count_num_primes() const;
    void sieve();
    static int num_digits(int num);
};

PrimesSieve::PrimesSieve(int limit) :
        is_prime_{new bool[limit + 1]}, limit_{limit} {
    sieve();
}

void PrimesSieve::display_primes() const {
    //displays the primes in the format specified in the requirements document.
    cout << endl;

    cout << "Number of primes found: " << num_primes() << endl; //displays number found
    cout << "Primes up to " << limit_ << ":" << endl; //displays the limit

    //width and row
    const int max_prime_width =num_digits(max_prime_);
    const int primes_per_row = 80/(max_prime_width + 1);

    //checks if prime or not, if not max then prints val
    if(num_primes_<= primes_per_row){
        for(int i = 2; i <= limit_; i++){
        	if(is_prime_[i]){
        		if(i != max_prime_){
        			cout << i << " ";
        		}
        		else{
        			cout << i;
        		}
        	}
        }
    }
    //check if prime or not, and calculates width and row
    else {
        int count = 0;
        for(int i = 0; i <= limit_; i++){
            if(is_prime_[i] == true){
                int j =(count == 0) ? 0 : 1;
                cout << setw(max_prime_width + j) << i;
                if (count == primes_per_row - 1){
                    cout << endl;
                }
                count = (count + 1) % primes_per_row;
            }
        }
    }

}

int PrimesSieve::count_num_primes() const {
    //counts the number of primes found
    int number_of_primes_ = 0;
    for(int i = 2; i <= limit_; i++){
        if(is_prime_[i]){
        	number_of_primes_++;
        }
    }
    return number_of_primes_;
}

void PrimesSieve::sieve() {
    // TODO: write sieve algorithm
	//sets prime to true
    for(int i = 2; i <= limit_; i++){
    	is_prime_[i] = true;
    }
    //if prime is true, run for loop (limit sqrt)
    for(int j = 2; j <= sqrt(limit_); j++){
        if(is_prime_[j] == true){
        	for(int k = pow(j,2); k <= limit_; k = k + j){
            is_prime_[k] = false;
        	}
        }
    }
    //gets total primes
        num_primes_=count_num_primes();
        for(int i = limit_; i >= 2; i--){
        	if(is_prime_[i]){
        		max_prime_ = i;
        		break;
        	}
        }
}

int PrimesSieve::num_digits(int num) {
    //determines how many digits are in an integer
	int digits_in_int = 0;
    while(num!=0){
    	num = num / 10;
    	digits_in_int++;
    }
        return digits_in_int;
}

int main() {
    cout << "**************************** " <<  "Sieve of Eratosthenes" <<
            " ****************************" << endl;
    cout << "Search for primes up to: ";
    string limit_str;
    cin >> limit_str;
    int limit;

    // Use stringstream for conversion. Don't forget to #include <sstream>
    istringstream iss(limit_str);

    // Check for error.
    if ( !(iss >> limit) ) {
        cerr << "Error: Input is not an integer." << endl;
        return 1;
    }

    if (limit < 2) {
        cerr << "Error: Input must be an integer >= 2." << endl;
        return 1;
    }

    //uses my class to produce the desired output.
    PrimesSieve sieve(limit);
    sieve.display_primes();

    return 0;
}
