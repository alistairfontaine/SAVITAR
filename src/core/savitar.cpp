#include "savitar.hpp"
#include <fstream>
#include <iostream>
#include <chrono>
#include <cstring>

namespace Savitar {

FaultIsolationKernel::FaultIsolationKernel() {}

FaultIsolationKernel::~FaultIsolationKernel() {}

/**
 * MICROSECOND MONOTONIC CLOCK HOOK
 * Grabs high-precision steady ticks directly off your local CPU registers.
 */
uint64_t FaultIsolationKernel::get_system_microseconds() const {
    auto now = std::chrono::steady_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::microseconds>(now).count();
}

/**
 * CELL REGISTRY MATRIX INGEST PRIMITIVE
 * Bundles hardware metrics into an un-padded, fixed-size cell state structure in RAM.
 */
bool FaultIsolationKernel::register_cell_to_cache(uint32_t id, float voltage, float temp, float current) {
    // Overwrite existing cell metrics if the unique index key collision occurs
    for (auto& cell : cell_registry_cache_) {
        if (cell.cell_id == id) {
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
    state.is_isolated = 0; // Initialize cell as fully engaged and operational

    cell_registry_cache_.push_back(state);
    return true;
}

/**
 * 📦 .SAV CONTAINER SERIALIZATION PIPELINE 📦
 * Flushes volatile matrix caches directly to an independent binary file target on disk.
 */
bool FaultIsolationKernel::serialize_matrix_to_disk(const std::string& host_path) {
    if (cell_registry_cache_.empty()) return false;

    std::ofstream out_file(host_path, std::ios::binary | std::ios::out | std::ios::trunc);
    if (!out_file.is_open()) {
        std::cerr << "❌ Data Write Exception: Failed to secure hardware file locks on: " << host_path << std::endl;
        return false;
    }

    // Dump continuous vector bytes straight onto drive partition sectors to eliminate serialization lag
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
 * 🖨️ IMMUTABLE SYSTEM PARAMETER REPORT MATRIX PRINTER 🖨️
 */
void FaultIsolationKernel::display_active_cells() const {
    std::cout << "\n⚡ [SAVITAR Kernel Registry] Active High-Voltage Core Monitor:" << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << "CELL_ID\tVOLTAGE(V)\tTEMP(C)\tCURRENT(A)\tTIMESTAMP_US\tSTATUS" << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;

    if (cell_registry_cache_.empty()) {
        std::cout << "[Empty cell tracking state cache]" << std::endl;
        return;
    }

    for (const auto& cell : cell_registry_cache_) {
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

} // namespace Savitar
