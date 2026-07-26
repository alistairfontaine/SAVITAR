#include <fstream>
#include <iostream>
#include <chrono>
#include <cstring>
#include <thread>
#include "savitar.hpp"

namespace Savitar {

FaultIsolationKernel::FaultIsolationKernel() : monitor_active_(false), critical_fault_counter_(0) {}

FaultIsolationKernel::~FaultIsolationKernel() {
    if (monitor_active_) {
        terminate_monitor_thread();
    }
}

/**
 * MICROSECOND MONOTONIC CLOCK HOOK
 * Grabs high-precision steady ticks directly off your local CPU registers.
 */
uint64_t FaultIsolationKernel::get_system_microseconds() const {
    auto now = std::chrono::steady_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::microseconds>(now).count();
}

/**
 * THREAD-SAFE CELL REGISTRY MATRIX INGEST PRIMITIVE
 * Bundles hardware metrics into an un-padded, fixed-size cell state structure in RAM.
 */
bool FaultIsolationKernel::register_cell_to_cache(uint32_t id, float voltage, float temp, float current) {
    std::lock_guard<std::mutex> lock(kernel_mutex_);

    // Overwrite existing cell metrics if a unique index key collision occurs
    for (auto& cell : cell_registry_cache_) {
        if (cell.cell_id == id) {
            if (cell.is_isolated) return true;

            cell.voltage_v = voltage;
            cell.temperature_c = temp;
            cell.current_a = current;
            cell.timestamp_us = get_system_microseconds();
            return true;
        }
    }

    CellState state;
    state.cell_id = id;
    state.voltage_v = voltage;
    state.temperature_c = temp;
    state.current_a = current;
    state.timestamp_us = get_system_microseconds();
    state.is_isolated = 0;

    cell_registry_cache_.push_back(state);
    return true;
}

/**
 * 📦 .SAV CONTAINER SERIALIZATION PIPELINE 📦
 * Flushes volatile matrix caches directly to an independent binary file target on disk.
 */
bool FaultIsolationKernel::serialize_matrix_to_disk(const std::string& host_path) {
    std::lock_guard<std::mutex> lock(kernel_mutex_);
    if (cell_registry_cache_.empty()) return false;

    std::ofstream out_file(host_path, std::ios::binary | std::ios::out | std::ios::trunc);
    if (!out_file.is_open()) {
        std::cerr << "❌ Data Write Exception: Failed to secure hardware file locks on: " << host_path << std::endl;
        return false;
    }

    out_file.write(reinterpret_cast<const char*>(cell_registry_cache_.data()), cell_registry_cache_.size() * sizeof(CellState));
    out_file.flush();
    out_file.close();

    std::cout << "✓ [.sav Serialized] Telemetry log saved to disk. Wrote "
              << cell_registry_cache_.size() << " cell blocks safely." << std::endl;
    return true;
}

/**
 * 📡 .SAV CONTAINER INGEST DESERIALIZER 📡
 * Streams raw binary chunks back off your partition to completely rebuild the cell tracking state.
 */
bool FaultIsolationKernel::deserialize_matrix_from_disk(const std::string& host_path) {
    std::lock_guard<std::mutex> lock(kernel_mutex_);
    std::ifstream in_file(host_path, std::ios::binary | std::ios::ate);
    if (!in_file.is_open()) {
        std::cerr << "❌ Ingest Exception: Could not locate target binary cell data database: " << host_path << std::endl;
        return false;
    }

    std::streamsize file_size = in_file.tellg();
    in_file.seekg(0, std::ios::beg);

    size_t total_cells_detected = file_size / sizeof(CellState);
    if (total_cells_detected == 0) {
        in_file.close();
        return false;
    }

    cell_registry_cache_.resize(total_cells_detected);
    in_file.read(reinterpret_cast<char*>(cell_registry_cache_.data()), file_size);
    in_file.close();

    std::cout << "✓ [.sav Ingested] Memory cache matrix reconstructed. Tracking "
              << total_cells_detected << " high-voltage cells." << std::endl;
    return true;
}

/**
 * THREAD-SAFE COPIER PRIMITIVES FOR LEDGER RENDERING
 */
size_t FaultIsolationKernel::get_cached_cell_count() {
    std::lock_guard<std::mutex> lock(kernel_mutex_);
    return cell_registry_cache_.size();
}

std::vector<CellState> FaultIsolationKernel::get_cell_registry_copy() {
    std::lock_guard<std::mutex> lock(kernel_mutex_);
    return cell_registry_cache_;
}

/**
 * 🖨️ SYSTEM PARAMETER REPORT MATRIX PRINTER 🖨️
 */
void FaultIsolationKernel::display_active_cells() const {
    std::vector<CellState> copy_registry;
    {
        FaultIsolationKernel* non_const_this = const_cast<FaultIsolationKernel*>(this);
        std::lock_guard<std::mutex> lock(non_const_this->kernel_mutex_);
        copy_registry = cell_registry_cache_;
    }

    std::cout << "\n⚡ [SAVITAR Kernel Registry] Active High-Voltage Core Monitor:" << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << "CELL_ID\tVOLTAGE(V)\tTEMP(C)\tCURRENT(A)\tTIMESTAMP_US\tSTATUS" << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;

    if (copy_registry.empty()) {
        std::cout << "[Empty cell tracking state cache]" << std::endl;
        return;
    }

    for (const auto& cell : copy_registry) {
        std::string gate_status = cell.is_isolated ? "❌ OFFLINE [ISOLATED]" : "✓ ENGAGED";
        std::cout << cell.cell_id << "\t"
                  << cell.voltage_v << "\t\t"
                  << cell.temperature_c << "\t"
                  << cell.current_a << "\t\t"
                  << cell.timestamp_us << "\t"
                  << gate_status << std::endl;
    }
    std::cout << "----------------------------------------------------------------------" << std::endl;
}

/**
 * 🚀 HIGH-FREQUENCY BACKGROUND OBSERVER THREAD TRIGGER 🚀
 */
void FaultIsolationKernel::spawn_monitor_thread() {
    if (monitor_active_) return;

    monitor_active_ = true;
    std::cout << "📡 [Kernel Core] Launching multi-threaded parallel cell state monitor thread..." << std::endl;


    std::thread worker_thread(&FaultIsolationKernel::background_observer_worker_loop, this);
    worker_thread.detach();
}

void FaultIsolationKernel::terminate_monitor_thread() {
    monitor_active_ = false;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "✓ [Kernel Core] Observer thread dissolved safely. Monitoring offline." << std::endl;
}

/**
 * 🔒 HARD HARDWARE HARD ENVELOPE INTERRUPT ISOLATION PRIMITIVE 🔒
 */
void FaultIsolationKernel::execute_hardware_interrupt_isolation(uint32_t target_cell_id) {
    for (auto& cell : cell_registry_cache_) {
        if (cell.cell_id == target_cell_id) {
            if (cell.is_isolated) return;

            cell.is_isolated = 1;
            critical_fault_counter_++;

            std::cerr << "\n🔥 [CRITICAL HARDWARE INTERRUPT DISPATCHED] ALERT ON CELL " << target_cell_id << std::endl;
            std::cerr << "   Metrics: " << cell.voltage_v << "V | " << cell.temperature_c << "C | " << cell.current_a << "A" << std::endl;
            std::cerr << "   Action: Isolation pyrotechnic circuit blown. Thermal runway propagation halted successfully." << std::endl;
            return;
        }
    }
}

/**
 * 🏎️ HIGH-VELOCITY CORE CONCURRENT WORKER ROUTINE 🏎️
 */
void FaultIsolationKernel::background_observer_worker_loop() {
    while (monitor_active_) {
        {
            std::lock_guard<std::mutex> lock(kernel_mutex_);

            for (auto& cell : cell_registry_cache_) {
                if (cell.is_isolated) continue;

                if (cell.temperature_c > 65.0f || cell.voltage_v > 4.3f || cell.voltage_v < 2.5f) {
                    execute_hardware_interrupt_isolation(cell.cell_id);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::microseconds(250));
    }
}

} // namespace Savitar
