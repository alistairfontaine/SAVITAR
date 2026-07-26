#include "savitar.hpp"
#include <iostream>

int main() {
    std::cout << "🧬 ======================================================== 🧬" << std::endl;
    std::cout << "🔥 SAVITAR: High-Frequency Fault Isolation Kernel Subsystem 🔥" << std::endl;
    std::cout << "🧬 ======================================================== 🧬" << std::endl;

    Savitar::FaultIsolationKernel kernel;
    std::string test_storage_vault = "tests/battery_pack_01.sav";

    // 1. Ingest test telemetry data fields representing distinct power cells into memory
    std::cout << "📝 [Data Ingress] Ingesting real-time cell parameters into RAM cache..." << std::endl;
    kernel.register_cell_to_cache(101, 4.12f, 28.5f, 15.4f); // Cell 101: Balanced State
    kernel.register_cell_to_cache(102, 4.09f, 32.1f, 15.2f); // Cell 102: Balanced State
    kernel.register_cell_to_cache(103, 3.82f, 54.8f, 45.0f); // Cell 103: Extreme Draw Thermal Spike

    // 2. Serialize the active matrix structures straight into our custom format file container
    if (kernel.serialize_matrix_to_disk(test_storage_vault)) {
        // Instantiate a separate independent object parser to cross-examine data locality bytes on disk
        Savitar::FaultIsolationKernel diagnostic_parser;
        if (diagnostic_parser.deserialize_matrix_from_disk(test_storage_vault)) {
            diagnostic_parser.display_active_cells();
        }
    }

    std::cout << "\n🚀 [Status Update] Milestone 1 core data serialization primitives functional!" << std::endl;
    std::cout << "🧬 ======================================================== 🧬" << std::endl;
    return 0;
}
