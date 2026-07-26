# SAVITAR Operator Interface & Manual

This manual details the command-line primitives configured to manage the high-frequency battery fault isolation kernel.

---

## Cell Telemetry Configuration

### 1. Manual Cell Registry Ingestion
To package and cache a battery cell's electrical and thermal parameters inside active RAM tables, run the `cell` primitive:
```text
savitar-cli> cell <cell_id> <voltage> <temperature> <current>
```

### 2. Initiating Real-Time Parallel Monitoring
To spawn the background multi-threaded worker loops and arm the threshold interrupt dispatcher gates, execute the `monitor` command:
```text
savitar-cli> monitor
```

### 3. Archive Log Serialization
To serialize the active in-memory cell state matrix history back into a dense binary container file under our sovereign extension format, run the `save` primitive:
```text
savitar-cli> save battery_profile.sav
```
This isolates the output file directly within the `tests/` directory channel.
