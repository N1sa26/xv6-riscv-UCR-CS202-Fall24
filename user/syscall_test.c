#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
  info(1);
  info(2);
  info(2);
  info(3);
  info(2);
  info(1);
  exit(0);
}
