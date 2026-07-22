# High-Performance Log Processing Engine

A high-performance **C++ log processing engine** that parses log streams, maintains a fixed-size sliding window, and detects error spikes in real time. The engine is designed with efficiency and thread safety in mind, making it suitable for monitoring applications where rapid anomaly detection is essential.

---

## Features

-  Real-time log processing (using a log file as the event source)
-  Parses and interprets log entries
- Fixed-size sliding window for recent log events
-  Detects anomaly spikes based on configurable error thresholds
- Thread-safe implementation using `std::mutex`
- Tracks event statistics within the active window
- Efficient memory usage by retaining only the most recent events

---

## How It Works

The engine continuously processes incoming log entries and maintains only the latest **N** events in memory.

For every new log entry:

1. Parse the log line.
2. Classify the log level (e.g., `ERROR`, `NOTICE`).
3. Add the event to the sliding window.
4. Remove the oldest event if the window exceeds its maximum size.
5. Update event counters.
6. Trigger an alert if the number of errors exceeds the configured threshold.

### Statistics Tracked

- Error Count
- Notice Count

### Alert Condition

```text
errorCount > threshold
```

---



> *Directory names may vary depending on your implementation.*

---

## Example Log Entries

```text
INFO: Server started
NOTICE: New connection established
ERROR: Database connection failed
INFO: Request processed
ERROR: Timeout while reading socket
```

---

## Example Output

```text
Window Size: 100

Errors : 8
Notices: 15

ALERT!
Error threshold exceeded.
```

---

## Technologies Used

- C++17
- Standard Template Library (STL)
- `std::deque`
- `std::mutex`
- File Streams (`fstream`)
- CMake

---

## Building the Project

### Clone the Repository

```bash
git clone https://github.com/your-username/high-performance-log-engine.git
cd high-performance-log-engine
```

### Build with CMake

```bash
mkdir build
cd build

cmake ..
cmake --build .
```

---

## Run

```bash
./LogEngine
```

or on Windows

```bash
LogEngine.exe
```

---

## Configuration

The following parameters can be customized:

| Parameter | Description |
|-----------|-------------|
| Window Size | Number of recent log events retained |
| Error Threshold | Maximum allowed errors before an alert is raised |
| Log File | Input log source |

---

## Design Highlights

- Efficient **O(1)** insertion and removal using a sliding window.
- Constant memory usage regardless of log file size.
- Thread-safe updates with `std::mutex`.
- Modular architecture separating parsing, processing, and window management.
- Easily extensible for additional log levels or alerting strategies.

---

## Future Improvements

- Multi-threaded log ingestion
- Live log streaming from sockets
- Configurable alert policies
- Support for JSON log formats
- Regular expression filtering
- Metrics export (Prometheus)
- Dashboard integration (Grafana)
- Asynchronous processing
- File rotation support

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.
