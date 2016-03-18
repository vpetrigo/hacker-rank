/**
 * Author: vpetrigo
 * Task:
 *
 * Little Bob loves chocolate, and he goes to a store with $N in 
 * his pocket. The price of each chocolate is $C. The store offers 
 * a discount: for every M wrappers he gives to the store, he 
 * gets one chocolate for free. How many chocolates does Bob get to eat?
 *
 * Input Format: 
 * The first line contains the number of test cases, TT. 
 * TT lines follow, each of which contains three integers, NN, CC, and MM.
 *
 * Output Format: 
 * Print the total number of chocolates Bob eats.
 *
 * Constraints: 
 * 1≤T≤10001≤T≤1000 
 * 2≤N≤1052≤N≤105 
 * 1≤C≤N1≤C≤N 
 * 2≤M≤N2≤M≤N
 *
 * Sample input
 *
 * 3
 * 10 2 5
 * 12 4 4
 * 6 2 2
 * Sample Output
 *
 * 6
 * 3
 * 5
 */
#include <iostream>
#include <vector>
#include <cstddef>

using namespace std;

int main() {
  unsigned n;

  cin >> n;
  
  for (size_t i = 0; i < n; ++i) {
    unsigned money, cost, ch_for_discount;

    cin >> money >> cost >> ch_for_discount;
    
    auto choco = money / cost;
    auto wrappers = choco;

    while (wrappers >= ch_for_discount) {
      auto discount_choco = wrappers / ch_for_discount;
      
      wrappers %= ch_for_discount;
      wrappers += discount_choco;
      choco += discount_choco;
    }

    cout << choco << endl;
  }

  return 0;
}
