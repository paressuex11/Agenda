#include <signal.h>
#include "AgendaUI.hpp"

AgendaUI agenda;

void sig_int(int signal) {
  std::cout << std::endl
            << "SIGINT received." << std::endl
            << "All changes are saved." << std::endl;
  agenda.quitAgenda(1);
}

int main() {
  if (signal(SIGINT, sig_int) == SIG_ERR)
    std::cout << "Warning: signal handler for SIGINT isn't set properly"
              << std::endl;

  agenda.OperationLoop();

  return 0;
}