#include "espresso_core.h"

int main(int argc, char** argv) {
  if (argc < 2)
    return 1;

  printf("hello from espresso %s!\n", argv[1]);

  // svc_main_loop(int argc, char** argv);

  return 0;
}
