#include "savitar.hpp"
#include "shell.hpp"
#include <iostream>

int main() {
    std::cout << "🧬 ======================================================== 🧬" << std::endl;
    std::cout << "🔥 SAVITAR: High-Frequency Fault Isolation Prompt Core      🔥" << std::endl;
    std::cout << "🧬 ======================================================== 🧬" << std::endl;

    Savitar::FaultIsolationKernel kernel;

    // Launch the interactive interpretation command loop on bare metal
    Savitar::launch_interactive_shell(kernel);

    return 0;
}
