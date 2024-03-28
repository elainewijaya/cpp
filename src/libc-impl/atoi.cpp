#include <iostream>

using namespace std;

int myAtoi(const char* str) {
  if (str == nullptr) return 0;

  int sign = 1;
  int result = 0;
  for (int i = 0; str[i] != '\0'; ++i) {
    if (str[i] == ' ') continue;
    else if ((result == 0) && (sign == 1) && (str[i] == '-')) {
      sign = -1;
      continue;
    } else if (str[i] >= '0' && str[i] <= '9') {
      result = result * 10 + (str[i] - '0');
    }
  }
  return sign * result;
}

int main() {
  cout << "myAtoi tests:" << endl;
  cout << "myAtoi 12345: " << myAtoi("12345") << endl;
  cout << "myAtoi -54321: " << myAtoi("-54321") << endl;
  cout << "myAtoi -5  4 3 2     1: " << myAtoi("-5  4 3 2     1") << endl;
  cout << "myAtoi ` -  22334 23 33 4 143`: " << myAtoi(" -  22334 23 33 4 143") << endl;
  return 0;
}
