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
  COUT << "Opening /tmp/testfile to write..." << endl;
  FILE *test = fopen ("/tmp/testfile", "wb");
  COUT << "FD: " << fileno(dev) << endl;
  COUT << "Seek 333 dev" << endl;
  fseek(dev, 333, SEEK_SET);
  COUT << "Seek 333 null" << endl;
  fseek(null, 333, SEEK_SET);
  COUT << "Seek 333 test" << endl;
  fseek(test, 333, SEEK_SET);
  COUT << "Write 1 to dev" << endl;
  const char one = 1;
  fwrite(&one, 1, 1, dev);
  COUT << "Write 1 to null" << endl;
  fwrite(&one, 1, 1, null);
  COUT << "Write 1 to test" << endl;
  fwrite(&one, 1, 1, test);
  COUT << "Close dev" << endl;
  fclose(dev);
  COUT << "Close null" << endl;
  fclose(null);
  COUT << "Close test" << endl;
  fclose(test);

  return 0;

}

