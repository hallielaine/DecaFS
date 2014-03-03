#include "espresso_core.h"

int main(int argc, char** argv) {

  printf("hello from espresso %s!\n", argv[1]);

  svc_main_loop(0, NULL); // svc_main_loop(int argc, char** argv);

  return 0;
}
