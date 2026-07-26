#include "shell.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

namespace Savitar {

void launch_interactive_shell(FaultIsolationKernel& kernel) {
    std::string line;
    std::cout << "\n🛡️ [SAVITAR High-Frequency Fault Isolation Prompt Engaged] Type 'help' to review command matrices." << std::endl;

    while (true) {
        std::cout << "savitar-cli> ";
        if (!std::getline(std::cin, line)) break;

        if (line.empty()) continue;

        // Tokenize input string fragments cleanly using standard string stream arrays
        std::stringstream ss(line);
        std::string command;
        ss >> command;

        std::vector<std::string> args;
        std::string arg;
        while (ss >> arg) {
            args.push_back(arg);
        }

        // --- COMMAND ROUTING INDEX MATRIX ---
        if (command == "exit" || command == "quit") {
            std::cout << "👋 Deactivating kernel monitor interfaces. System dropping offline." << std::endl;
            break;
        }
        else if (command == "help") {
            std::cout << "\n📋 Operational SAVITAR Core Primitives:" << std::endl;
            std::cout << "  cell <id> <v> <temp> <curr> - Packages and registers a battery cell entry into memory" << std::endl;
            std::cout << "  monitor                    - Spawns the background high-frequency parallel observer thread" << std::endl;
            std::cout << "  view                       - Prints out a detailed grid list view of all cached core cells" << std::endl;
            std::cout << "  save <file.sav>            - Serializes the active real-time cell state matrix directly to disk" << std::endl;
            std::cout << "  load <file.sav>            - Ingests a raw .sav container file and rebuilds table caches" << std::endl;
            std::cout << "  exit / quit                - Safely releases descriptors and terminates the shell environment\n" << std::endl;
        }
        else if (command == "cell") {
            if (args.size() < 4) {
                std::cerr << "⚠️ Usage error: cell <cell_id> <voltage> <temperature> <current>" << std::endl;
                continue;
            }
            uint32_t id = std::stoul(args[0]);
            float voltage = std::stof(args[1]);
            float temp = std::stof(args[2]);
            float current = std::stof(args[3]);

            if (kernel.register_cell_to_cache(id, voltage, temp, current)) {
                std::cout << "✓ Cell telemetry successfully registered in RAM cache matrix." << std::endl;
            }
        }
        else if (command == "monitor") {
            kernel.spawn_monitor_thread();
        }
        else if (command == "view") {
            kernel.display_active_cells();
        }
        else if (command == "save") {
            if (args.empty()) {
                std::cerr << "⚠️ Usage error: save <filename.sav>" << std::endl;
                continue;
            }
            std::string dest_path = "tests/" + args[0]; // Isolate output files into tests/ directory channel
            kernel.serialize_matrix_to_disk(dest_path);
        }
        else if (command == "load") {
            if (args.empty()) {
                std::cerr << "⚠️ Usage error: load <filename.sav>" << std::endl;
                continue;
            }
            std::string source_path = "tests/" + args[0];
            kernel.deserialize_matrix_from_disk(source_path);
        }
        else {
            std::cerr << "❌ Unknown primitive: Type 'help' to review structural command arrays." << std::endl;
        }
    }
}

} // namespace Savitar
