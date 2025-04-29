# 🚗 Car Manufacturing Simulator

A modular C++ application that simulates the vehicle manufacturing and assembly process from component build to final assembly. Designed to emulate real-world factory logic, the system demonstrates object-oriented design, logging, and build automation via terminal-driven workflows.

---

## ✅ Key Features

- **🧱 Component-Based Assembly Flow**  
  Simulates the sequential stages of car production including chassis setup, engine installation, and electronics integration.

- **📋 Logging System**  
  Tracks all manufacturing steps using a built-in logger, enabling process traceability and debugging during execution.

- **🛠️ Makefile-Driven Build**  
  Uses a `Makefile` to streamline compilation and linking, reflecting professional development practices.

- **🗃️ Modular Class Design**  
  Structured using header/source separation (`car_assemble.h`, `log.h`, `log.cpp`, `main.cpp`) to reflect scalable, maintainable architecture.

- **🖥️ Command-Line Execution**  
  Fully interactive through the terminal, simulating real-world manufacturing pipeline behavior.

---

## 🧠 Skills Demonstrated

- **C++ OOP Principles** – Class separation, encapsulation, header-driven architecture
- **Process Simulation** – Emulated manufacturing workflow using real-world logic
- **Terminal App Development** – CLI-based inputs/outputs and runtime controls
- **Build Automation** – Makefile for compiling multiple modules
- **System Logging** – Runtime activity tracking via custom log components

---

## 🧰 Technologies Used

- **Language**: C++
- **Standard Libraries**: `iostream`, `fstream`, `ctime`, `string`
- **Build Tool**: `make`
- **Core Files**: `main.cpp`, `log.cpp`, `log.h`, `car_assemble.h`

---

## 🔍 Sample Use Case

1. **User** initiates the simulation through the terminal.
2. **System** assembles a car by simulating various component installations.
3. **Logger** tracks each step, writing event logs for debugging or progress review.
4. **Binary Output** reflects a completed manufacturing sequence.

---

## 📈 Future Enhancements

- Add user-defined car configurations (e.g. engine type, model)
- Export logs to CSV or display in a GUI dashboard
- Add real-time status bar or progress meter
- Integrate error simulation (e.g., failed part, delay)

---

## 👤 Developed By

**Hooman Manesh**  
Software Developer | Software Engineering Student  
© 2025
