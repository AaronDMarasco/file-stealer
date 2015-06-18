# file-stealer
Proof of concept to intercept calls to a certain file.

Usage:
 1. Build with commands listed in first line of each
 2. Run ./stealee and it will crash after trying to write to (bad) open of /dev/device
 3. Run "LD_PRELOAD=./stealer.so ./stealee"
 4. Run "LD_PRELOAD=./stealer.so valgrind --leak-check=full --show-leak-kinds=all ./stealee" to observe no memory leaks beyond dlsym() there's nothing I can do about
 5. "od -tu1 /tmp/testfile" to see the single 1 successfully written

