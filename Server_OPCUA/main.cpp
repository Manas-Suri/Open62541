#include "pumpVariables.h"

int main(int argc, char *argv[]) {
  OPCUAServer server_;
  int result = server_.run(argc, argv);
  if (result == EXIT_FAILURE) {
    std::cerr << "Server encountered an error." << std::endl;
  }
  return result;
}
