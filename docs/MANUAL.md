# SAVITAR Operator Interface & Manual

This manual details the command-line primitives configured to manage the high-frequency battery fault isolation kernel.

---

## Cell Telemetry & Configuration Operations

### 1. Registering Core Cell Parameters
To package, timestamp, and cache an individual cell's electrical voltage and thermal parameters inside active RAM tables, run the `cell` primitive:
```text
savitar-cli> cell <cell_id> <voltage> <temperature> <current>
```
*Example:* To map a balanced cell at hardware index 101, type:
```text
savitar-cli> cell 101 4.12 25.4 8.5
```

### 2. Launching Asynchronous Parallel Monitoring
To spawn the background high-frequency parallel observer thread and arm the physical thermodynamic envelope interrupt gates, execute the `monitor` command:
```text
savitar-cli> monitor
```
The background thread will instantly begin running 250-microsecond array validation loops.

### 3. Reviewing Active System State Records
To output an immutable report ledger list view of all cached core cells and their current isolation status, run the `view` primitive:
```text
savitar-cli> view
```

---

## 📦 Data Storage & Serialization Operations

### 1. Serializing Active Matrices to Disk
To commit the entire active in-memory cell state matrix history directly into a dense binary container format file, run the `save` primitive:
```text
savitar-cli> save battery_profile.sav
```
This isolates the output file directly within the `tests/` directory channel.

### 2. Ingesting Existing Memory Profiles
To stream a raw .sav database file off disk and completely reconstruct your active in-memory cache tables, run the `load` primitive:
```text
savitar-cli> load battery_profile.sav
```
