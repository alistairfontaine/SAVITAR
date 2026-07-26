#ifndef SAVITAR_HPP
#define SAVITAR_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <mutex>
#include <atomic>

namespace Savitar {

#pragma pack(push, 1) // Enforce zero-padding array constraints for raw contiguous byte access
/**
 * Packed Battery Cell State Record
 * Size: Exactly 25 bytes per cell, eliminating internal cache fragmentation blocks.
 */
struct CellState {
    uint32_t cell_id;         // 32-bit unique hardware cell identification index key
    float    voltage_v;       // 32-bit floating point cell electrical voltage parameter
    float    temperature_c;   // 32-bit floating point cell thermal metric parameter
    float    current_a;       // 32-bit floating point raw amperage current load metric
    uint64_t timestamp_us;    // 64-bit microsecond clock log timestamp
    uint8_t  is_isolated;     // 8-bit boolean isolation gate marker (0 = Active, 1 = SHUTDOWN)
};
#pragma pack(pop)

class FaultIsolationKernel {
public:
    FaultIsolationKernel();
    ~FaultIsolationKernel();

    // Milestone 1 Core Primitives
    bool register_cell_to_cache(uint32_t id, float voltage, float temp, float current);
    bool serialize_matrix_to_disk(const std::string& host_path);
    bool deserialize_matrix_from_disk(const std::string& host_path);
    void display_active_cells() const;

    // Milestone 2 Core Primitives (Multi-Threaded Observer Loops & Interrupt Gates)
    void spawn_monitor_thread();
    void terminate_monitor_thread();
    void execute_hardware_interrupt_isolation(uint32_t target_cell_id);

    // State Tracking Getters
    size_t get_cached_cell_count();
    std::vector<CellState> get_cell_registry_copy();

private:
    std::vector<CellState> cell_registry_cache_; // Contiguous in-memory telemetry table array cache
    uint64_t get_system_microseconds() const;

    // Concurrency synchronization modules
    std::mutex              kernel_mutex_;
    std::atomic<bool>       monitor_active_;
    uint64_t                critical_fault_counter_;

    void background_observer_worker_loop();
};

} // namespace Savitar

#endif // SAVITAR_HPP
