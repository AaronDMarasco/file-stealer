// g++ stealee.cpp -o stealee
#include <cstdio>
#include <iostream>

using namespace std;
#define COUT cout << "Client:" << __LINE__ << ": "

int main () {
  COUT << "Opening /dev/null to write..." << endl;
  FILE *null = fopen ("/dev/null", "wb");
  COUT << "FD: " << fileno(null) << endl;
  COUT << "Opening /dev/device to read/write..." << endl;
  FILE *dev = fopen ("/dev/device", "rwb");
  COUT << "FD: " << fileno(dev) << endl;
  COUT << "Seek 333" << endl;
  fseek(dev, 333, SEEK_SET);
  COUT << "Write 1" << endl;
  const char one = 1;
  fwrite(&one, 1, 1, dev);
  COUT << "Close" << endl;
  fclose(dev);
  fclose(null);

  return 0;

}

