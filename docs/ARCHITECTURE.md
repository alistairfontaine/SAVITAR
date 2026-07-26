# SAVITAR Deep Technical Architecture & Concurrency Specification

This engineering manifest details the packed binary layout schemas, asynchronous thread parameters, and hardware threshold interrupt mechanisms governing the SAVITAR kernel layers.

---

## 1. Contiguous Binary Geometry Format (The .sav Standard)
Every `.sav` telemetry matrix container file completely avoids serialization and parsing overhead by writing memory structures directly to hardware storage sectors. Blocks are serialized at a static size profile using an un-padded compilation footprint to optimize CPU L1/L2 cache locality.

```cpp
#pragma pack(push, 1)
struct CellState {
    uint32_t cell_id;         // 32-bit unique hardware cell identification index key
    float    voltage_v;       // 32-bit floating point cell electrical voltage parameter
    float    temperature_c;   // 32-bit floating point cell thermal metric parameter
    float    current_a;       // 32-bit floating point raw amperage current load metric
    uint64_t timestamp_us;    // 64-bit microsecond clock log entry timestamp marker
    uint8_t  is_isolated;     // 8-bit boolean isolation gate state indicator flag (0=Active, 1=SHUTDOWN)
};
#pragma pack(pop)
```
*Total Footprint Size:* Exactly 25 bytes per cell telemetry block, guaranteeing zero internal memory padding leaks or block fragmentation.

---

## 2. Asynchronous Multi-Threaded Parallel Observer Layer
Traditional battery management software layers scan cells sequentially. This creates a dangerous latency blindspot when dealing with exponential thermal cascades. SAVITAR completely bypasses sequential iteration limits by instantiating a high-frequency background worker thread (`std::thread`) running concurrently to the main application context shell.

```text
  [Main Thread: Ingress / Shell] ──► Ingests Live Cell Telemetry Data
                                          │ (Thread-Safe Mutex Lock)
                                          ▼
  [Volatile Cache Registry Map] ◄── Contiguous 25-Byte Memory Arrays
                                          ▲
                                          │ (250-Microsecond Inspection Passes)
  [Background Observer Thread]  ──► Evaluates Physical Threshold Envelopes
```

### The Thermodynamic Safety Envelope Boundaries
The background worker thread loops at maximum processor velocity, executing inspection passes across the volatile contiguous memory tables every **250 microseconds**. It cross-examines active variable states against three rigid physical boundaries:
1.  **Over-Temperature Envelope Limit:** $T > 65.0^\circ\text{C}$
2.  **Over-Voltage Charging Envelope Limit:** $V > 4.3\text{V}$
3.  **Under-Voltage Discharge Envelope Limit:** $V < 2.5\text{V}$

---

## 3. High-Frequency Threshold Interrupt Isolation Logic
The moment a cell's internal metrics breach any of the three physical safety envelope boundaries, the background loop calls a real-time logical interrupt handler, bypassing standard iterations:

*   **Sub-Microsecond Isolation Lock:** The kernel locks the global `std::mutex` mutex channel, completely blacklists the target cell index ID, and flips its internal `is_isolated` parameter gate flag to `1` (SHUTDOWN) in under a microsecond.
*   **Cascade Halting:** Future incoming telemetry frames are blocked from overwriting an isolated slot back to active states, halting thermal runaway propagation across adjacent core packs before a physical cascade can initiate.
