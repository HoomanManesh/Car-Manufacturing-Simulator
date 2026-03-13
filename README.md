# 🚗 Car Manufacturing Simulator

A multithreaded C++ application that simulates a real-world vehicle manufacturing pipeline using POSIX threads, mutexes, and semaphores. Two engine producers feed a shared queue, two chassis-assembly robots consume from it concurrently, and a final-assembly robot completes each car — all running in parallel. An interactive browser-based animation ships alongside the simulator to visualise the full pipeline in real time.

---

## ✅ Key Features

- **🔀 Multithreaded Pipeline**
  Five concurrent threads (GAS producer, HYBRID producer, Titano, MegaForce, RoboMount) communicate through two bounded queues using POSIX semaphores and mutexes.

- **🧱 Component-Based Assembly Flow**
  Powertrains are produced → queued → married to chassis → queued again → final-assembled into cars, mirroring a real factory line.

- **📋 Per-Event Logging**
  Every produce/consume event is timestamped and printed, showing queue state and running totals for full traceability.

- **🛠️ Makefile-Driven Build**
  Single `make` command compiles and links all modules, reflecting professional build practices.

- **🗃️ Modular Class Design**
  Clean header/source separation across `shared_datastruc`, `powertrain_producer`, `powertrain_consumer`, `powered_chassis`, and `log`.

- **🎬 Browser Animation**
  `simulation.html` — a self-contained HTML/CSS/JS file that visualises all 5 pipeline stages with live robot states, animated queues, and a running car counter. No install required.

---

## 🚀 How to Run

### 1 — Compile (requires WSL or Linux with g++ and make)

```bash
make
```

### 2 — Run the simulator

```bash
./carassemble -n 100
```

| Flag | Description | Default |
|------|-------------|---------|
| `-n` | Total powertrains to produce | `100` |
| `-g` | GAS engine production delay (ms) | `0` |
| `-h` | HYBRID engine production delay (ms) | `0` |
| `-t` | Titano assembly delay (ms) | `0` |
| `-m` | MegaForce assembly delay (ms) | `0` |
| `-r` | RoboMount installation delay (ms) | `0` |

**Example with delays:**
```bash
./carassemble -n 50 -g 10 -h 15 -t 8 -m 8 -r 5
```

**Expected output (tail):**
```
REQUEST REPORT
----------------------------------------
GAS powertrain producer generated 51 requests
HYBRID powertrain producer generated 49 requests
Titano consumed 27 GAS + 29 HYBRID = 56 total
MegaForce consumed 24 GAS + 20 HYBRID = 44 total
```

### 3 — Open the pipeline animation

Open `simulation.html` in any browser (double-click the file — no server needed).

The animation visualises all 5 stages in real time:

| Stage | Actor | Role |
|-------|-------|------|
| 1 | ⚙️ GAS Engine + ⚡ Hybrid Engine | Produce powertrains into the queue |
| 2 | Powertrain Queue | Bounded buffer (max 8, max 4 hybrid) |
| 3 | 🤖 Titano + 🤖 MegaForce | Consume powertrains, assemble powered chassis |
| 4 | Chassis Queue | Bounded buffer (max 6) |
| 5 | 🔧 RoboMount | Final assembly → completed car |

---

## 🧠 Skills Demonstrated

- **Multithreading (POSIX pthreads)** – Producer/consumer pattern with concurrent threads
- **Synchronisation** – Semaphores (`sem_t`) and mutexes (`pthread_mutex_t`) for race-free queue access
- **C++ OOP Principles** – Class separation, encapsulation, header-driven architecture
- **Process Simulation** – Emulated manufacturing workflow matching real factory logic
- **Build Automation** – Makefile for compiling multiple interdependent modules
- **System Logging** – Per-event runtime logging with elapsed-time tracking
- **Frontend Visualisation** – Vanilla HTML/CSS/JS animation with no dependencies

---

## 🧰 Technologies Used

- **Language**: C++11
- **Concurrency**: POSIX Threads (`pthread`), POSIX Semaphores (`semaphore.h`)
- **Build Tool**: `make` / `g++`
- **Animation**: HTML5 + CSS3 + Vanilla JavaScript (self-contained, no frameworks)
- **Platform**: Linux / WSL (Windows Subsystem for Linux)

---

## 📁 Project Structure

```
├── main.cpp                  Entry point, thread creation, poison-pill shutdown
├── shared_datastruc.h/.cpp   Shared queues, counters, and synchronisation primitives
├── powertrain_producer.h/.cpp GAS & HYBRID engine producer threads
├── powertrain_consumer.h/.cpp Titano & MegaForce chassis-assembly threads
├── powered_chassis.h/.cpp    RoboMount final-assembly thread
├── car_assemble.h            Enums and struct definitions
├── log.h/.cpp                Thread-safe per-event logging
├── Makefile                  Build configuration
└── simulation.html           Interactive browser animation of the full pipeline
```

---

## 👤 Developed By

**Hooman Manesh**
Software Developer | Software Engineering Student
© 2025
