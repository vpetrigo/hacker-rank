/**
 * Author: vpetrigo
 * Task:
 * Julius Caesar protected his confidential information by 
 * encrypting it in a cipher. Caesar's cipher rotated every 
 * letter in a string by a fixed number, K, making it 
 * unreadable by his enemies. Given a string, S, and a 
 * number, K, encrypt S and print the resulting string.
 *
 * Note: The cipher only encrypts letters; symbols, 
 * such as -, remain unencrypted.
 *
 * Input Format
 *
 * The first line contains an integer, N, which is 
 * the length of the unencrypted string. 
 * The second line contains the unencrypted string, S. 
 * The third line contains the integer encryption 
 * key, K, which is the number of letters to rotate.
 *
 * Constraints 
 * 1 ≤ N ≤100 
 * 0 ≤ K ≤ 100 
 * S is a valid ASCII string and doesn't contain any spaces.
 *
 * Output Format
 *
 * For each test case, print the encoded string.
 *
 * Sample Input
 *
 * 11
 * middle-Outz
 * 2
 * Sample Output
 *
 * okffng-Qwvb
 */
#include <iostream>
#include <cstdlib>
#include <cstddef>

using namespace std;

int main() {
  unsigned n;
  string input;
  unsigned offset;
  constexpr unsigned letters_q = 26;
  constexpr char upper_start = 'A';
  constexpr char lower_start = 'a';

  cin >> n >> input >> offset;

  for (auto& ch : input) {
    if (isalpha(ch)) {
      auto new_ch = isupper(ch) ? upper_start + (ch - upper_start + offset) % letters_q : 
         lower_start + (ch - lower_start + offset) % letters_q;
      ch = new_ch;
    }
  }

  cout << input << endl;

  return 0;
}
