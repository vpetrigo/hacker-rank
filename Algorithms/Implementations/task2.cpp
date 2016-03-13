/**
 * Author: vpetrio
 * Task:
 * Sherlock Holmes suspects his archenemy, Professor Moriarty, 
 * is once again plotting something diabolical. Sherlock's companion, 
 * Dr. Watson, suggests Moriarty may be responsible for MI6's 
 * recent issues with their supercomputer, The Beast.
 *
 * Shortly after resolving to investigate, Sherlock receives a note 
 * from Moriarty boasting about infecting The Beast with a virus; 
 * however, he also gives him a clue - a number, N. Sherlock determines 
 * the key to removing the virus is to find the largest Decent Number having N digits.
 *
 * A Decent Number has the following properties:
 *
 * Its digits can only be 3's and/or 5's.
 * The number of 3's it contains is divisible by 5.
 * The number of 5's it contains is divisible by 3.
 * If there are more than one such number, we pick the largest one.
 * Moriarty's virus shows a clock counting down to The Beast's destruction, 
 * and time is running out fast. Your task is to help Sherlock find the key before The Beast is destroyed!
 *
 * Constraints
 * 1 <= T <= 20
 * 1 <= N <= 100000
 *
 * Input Format
 *
 * The first line is an integer, T, denoting the number of test cases.
 *
 * The T subsequent lines each contain an integer, N, 
 * detailing the number of digits in the number.
 *
 * Output Format
 *
 * Print the largest Decent Number having N digits;
 * if no such number exists, tell Sherlock by printing -1.
 *
 * Sample Input
 *
 * 4
 * 1
 * 3
 * 5
 * 11
 * Sample Output
 *
 * -1
 *  555
 *  33333
 *  55555533333
 */
#include <iostream>
#include <vector>
#include <cstddef>

using namespace std;

string beast_number(const int q_digits) {
    string answer;
    int counter = q_digits;
    // if number of digits is divided by 3 without remainder
    // return a number with '5' in all digits
    if (q_digits % 3 == 0) {
        return string(q_digits, '5');
    }

    while (counter - 5 >= 0 && counter % 3 != 0) {
        answer.append(5, '3');
        counter -= 5;
    }

    if (counter % 3 != 0) {
        answer = "-1";
    }
    else {
        answer.insert(0, counter, '5');
    }

    return answer;
}

int main() {
    int n;

    cin >> n;

    for (std::size_t i = 0; i < n; ++i) {
        int q_digits;

        cin >> q_digits;
        cout << beast_number(q_digits) << endl; 
    }

    return 0;
}
