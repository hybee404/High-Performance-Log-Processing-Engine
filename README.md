# High-Performance Log Processing Engine

A C++ log processing engine that detects error spikes using a sliding window approach, parse and interprets the data.

## Features

- Processes logs in real-time (a logfile is used for this example)
- Maintains a fixed-size sliding window
- Detects anomaly spikes based on error thresholds
- Thread-safe design using mutex

## How It Works

- Keeps the last **N log events** (window)
- Tracks:
  - Error count
  - Notice count
- Triggers alert when: errorCount > threshold