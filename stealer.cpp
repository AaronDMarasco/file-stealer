// g++ stealer.cpp -o stealer.so -fPIC -dl -shared

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <dlfcn.h>
#include <iostream>

using namespace std;

#define COUT cout << "Server:" << __LINE__ << ": "

extern "C" {

static FILE *stolen_F = 0;

void cleanup(void) {
  COUT << "cleanup()" << endl;
  if (!stolen_F) return;
  int (*original_fclose)(FILE *);
  original_fclose = (int(*)(FILE *)) dlsym(RTLD_NEXT, "fclose"); // Memory leak?
  (*original_fclose)(stolen_F);
}

FILE *fopen(const char *path, const char *mode) {
  if (0 == strcmp(path, "/dev/device")) {
    COUT << "See my magic open!" << endl;
    if (stolen_F)
      fclose(stolen_F);
    else
      atexit(cleanup);
    stolen_F = fopen("/dev/null", "wb"); // Get next valid FD for this process
    COUT << "Intercepted, returning FD " << fileno(stolen_F) << endl;
    return stolen_F;
  } else {
    // From http://www.catonmat.net/blog/simple-ld-preload-tutorial-part-2/
    FILE *(*original_fopen)(const char*, const char*);
    original_fopen = (FILE *(*)(const char *, const char *)) dlsym(RTLD_NEXT, "fopen"); // Memory leak?
    COUT << "Not my magic: " << path << endl;
    return (*original_fopen)(path, mode);
  }
  return NULL; // ?
} // fopen

int fclose(FILE *fp) {
  if (fp == stolen_F) {
    COUT << "See my magic close!" << endl;
    // ignore - will catch atexit()
    return 0;
  } else {
    int (*original_fclose)(FILE *);
    original_fclose = (int(*)(FILE *)) dlsym(RTLD_NEXT, "fclose"); // Memory leak?
    COUT << "Not my magic: " << fileno(fp) << endl;
    return (*original_fclose)(fp);
  }
  return 0; // ?
} // fclose

int fseek(FILE *stream, long offset, int whence) {
  if (stream == stolen_F) {
    COUT << "fseek(" << offset << ") on stolen" << endl;

    /// DO SOMETHING.

    return 0; // Success
  } else {
    int (*original_fseek)(FILE *, long, int);
    original_fseek = (int(*)(FILE *, long, int)) dlsym(RTLD_NEXT, "fseek"); // Memory leak?
    COUT << "fseek(" << offset << ") on non-stolen" << endl;
    return (*original_fseek)(stream, offset, whence);
  }
  return 0; // ?

} // fseek

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
  // COUT calls fwrite()! Don't call it in here! ;)
  if (stream == stolen_F) {
    // COUT << "fwrite() on stolen" << endl;

    /// DO SOMETHING.

    return nmemb;
  } else {
    size_t (*original_fwrite)(const void *, size_t, size_t, FILE *);
    original_fwrite = (size_t(*)(const void *, size_t, size_t, FILE *)) dlsym(RTLD_NEXT, "fwrite"); // Memory leak?
    // COUT << "fwrite() on non-stolen" << endl;
    return (*original_fwrite)(ptr, size, nmemb, stream);
  }
} // fwrite

// fread() left as an exercise for the reader

} // extern

