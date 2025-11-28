// "stdio.h" is part of the glibc which is the standard library for C in
// Linux.
//
// The preprocessor effectively copy and paste the stdio.h contents in place of
// the #include macro.
//
// Those "<>" tells to the preprocessor that this file comes from the system.
// Replacing them with "" means to search for it in the working directory.
//
// In case of a "<>" the linker will search for the header in the system's
// library directories.
#include <stdio.h>

// Every "#" macro will be replaced by the preprocessor.
#define UPPER 300
#define LOWER 0
#define STEP 20

// count digits, white space, others
void count_all() {
  int c, i, nwhite, nother;
  int ndigit[10];

  nwhite = nother = 0;

  for (i = 0; i < 10; ++i)
    ndigit[i] = 0;

  // EOF on linux can be manually typed by pressing ctrl + D on the terminal.
  while ((c = getchar()) != EOF)
    if (c >= '0' && c <= '9')
      ++ndigit[c - '0'];
    else if (c == ' ' || c == '\n' || c == '\t')
      ++nwhite;
    else
      ++nother;

  // the code for "printf" actually is hardcoded in the C compiler itself, just
  // the definition that lives in "stdio.h". The actual "printf" is included in
  // the linking phase which happens after compilation.
  printf("digits =");

  for (i = 0; i < 10; ++i)
    printf(" %d", ndigit[i]);

  printf(", white space = %d, other = %d\n", nwhite, nother);
}

// n C, all function arguments are passed ``by value.'' This means
// that the called function is given the values of its arguments in temporary
// variables rather than the originals. The exception to this are arrays, passed
// by the memory address.
int power(int base, int n) {
  int p = 1;
  // we can take advantage of this "by value" feature,
  // the original "n" is not modified because the "n" in this scope is a copy.
  for (; n > 0; n--) {
    p *= base;
  }
  return p;
}

int main() {
  // int c;
  // while ((c = getchar()) != EOF)
  //   putchar(c);
  // count_all();

  for (int i = 0; i < 10; ++i)
    printf("2 ** %d = %d\n", i, power(2, i));

  // in UNIX return 0 means the program exited with success. Codes > 0 are
  // considered error codes.
  return 0;
}
