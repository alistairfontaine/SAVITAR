#include "savitar.hpp"
#include <iostream>
#include <unistd.h>
#include <vector>

int main() {
    std::cout << "🧬 ======================================================== 🧬" << std::endl;
    std::cout << "🔥 SAVITAR: High-Frequency Fault Isolation Kernel Subsystem 🔥" << std::endl;
    std::cout << "🧬 ======================================================== 🧬" << std::endl;

    Savitar::FaultIsolationKernel kernel;
    std::string test_storage_vault = "tests/cascade_stress_log.sav";

    // 1. Initialize our high-voltage battery cell matrix table cache
    std::cout << "📝 [Telemetry Initialization] Pre-populating active cell matrix..." << std::endl;
    kernel.register_cell_to_cache(201, 4.15f, 26.2f, 10.0f); // Cell 201: Balanced
    kernel.register_cell_to_cache(202, 4.14f, 27.1f, 10.2f); // Cell 202: Balanced
    kernel.register_cell_to_cache(203, 4.12f, 28.5f, 10.1f); // Cell 203: Balanced

    kernel.display_active_cells();

    // 2. Boot up the high-frequency parallel thread background observer worker loops
    std::cout << "\n📡 [Asynchronous Boot] Engaging multi-threaded parallel observer loops..." << std::endl;
    kernel.spawn_monitor_thread();

    // Allow the background thread to settle and synchronize smoothly
    usleep(100000);

    // 3. Simulate an active, exponential multi-cell thermal runaway cascade event
    std::cout << "\n🔥 [Simulation Event] Triggering high-current load and exponential thermal runaway..." << std::endl;
    std::cout << "   Simulating Cell 202 separator breakdown and puncture cascade..." << std::endl;

    // Step A: Feed extreme current load metrics into Cell 202
    kernel.register_cell_to_cache(202, 3.95f, 42.0f, 95.0f);
    usleep(50000); // Latency tick progression

    // Step B: Cell 202 completely breaches the thermodynamic threshold safety envelope
    kernel.register_cell_to_cache(202, 2.10f, 85.5f, 120.0f); // Temperature hits 85.5C (Envelope: >65C)

    // Hold main thread execution briefly to witness the microsecond background interrupt execution loop
    usleep(100000);

    // 4. Terminate background processes and display the final system registry state
    std::cout << "\n📡 [Asynchronous Shutdown] Terminating core parallel monitor loops..." << std::endl;
    kernel.terminate_monitor_thread();

    std::cout << "\n📊 [Post-Incident Review] Examining isolated battery cell state records:" << std::endl;
    kernel.display_active_cells();

    // 5. Serialize the final post-incident profile report directly into your custom format container
    kernel.serialize_matrix_to_disk(test_storage_vault);

    std::cout << "\n🚀 [Status Update] Milestone 2 asynchronous tracking loops functional!" << std::endl;
    std::cout << "🧬 ======================================================== 🧬" << std::endl;
    return 0;
}
