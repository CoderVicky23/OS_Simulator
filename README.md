# 🧠 OS Job Scheduling and Process Management Simulator (C++)

This project simulates core Operating System functionalities such as job scheduling, process management, resource allocation, memory handling, and deadlock prevention — using standard C++ and STL.

---

## 🧩 Features Implemented

### ✅ Process Management
- Each process is modeled via a PCB (Process Control Block) containing:
  - PID, priority, memory requirement, required resource, current state
- States: `NEW`, `READY`, `RUNNING`, `BLOCKED`, `TERMINATED`

### ✅ Memory Management
- Simulates a physical memory (default: 1024 MB)
- Allocates/deallocates memory during process lifecycle
- Denies creation if memory is insufficient

### ✅ Job Scheduling
- **FCFS (First-Come, First-Served)** scheduling algorithm
- Ready queue and context switching simulation
- Logging of every scheduling and state transition

### ✅ Resource Allocation
- Resource Table mapping each resource to the PID using it
- Blocked queues per resource
- Supports requesting and releasing named resources

### ✅ CPU Simulation
- Runs for fixed cycles using `simulateCPU(int cycles)`
- Simulates:
  - Resource acquisition
  - Blocking/unblocking
  - Termination
  - Execution logging

---

## Build and Run

if on windows make sure to install make. Can follow the link for help: https://stackoverflow.com/a/32127632

### 🧱 Build the Project

Open a terminal in the os_simulator/ directory and run:

make

This will:

Compile all .cpp files in src/ and main.cpp
Link them into an executable named: os_simulator

### ▶️ Run the Simulation

After successful build, execute:

./os_simulator

You’ll see console logs showing:

Process creation and memory allocation
Scheduling and resource handling
Blocking/unblocking transitions
Process terminations and memory release

### 🧹 Clean Build Artifacts

If you want to remove compiled files and executable:

make clean


### If using VSCode without Make

g++ -std=c++17 -Iinclude src/*.cpp main.cpp -o os_simulator.exe
.\os_simulator.exe



## 🗂️ Project Structure

os_simulator/
├── include/
│ ├── memory_manager.h
│ ├── pcb.h
│ ├── process_queues.h
│ └── scheduler.h
│
├── src/
│ ├── memory_manager.cpp
│ ├── process_queues.cpp
│ └── scheduler.cpp
│
├── main.cpp # Simulation driver
├── Makefile # Build instructions
└── README.md



## 🧪 Future Extensions

Support for Round Robin and Priority Scheduling

Logging to file

Deadlock detection/prevention (e.g., Banker's Algorithm)

Real-time process simulation using system() calls

## 📚 Dependencies

C++17 or higher

Standard Template Library (STL)

## 👨‍💻 Author

Developed by: CoderVicky23

Inspired by core OS scheduling concepts taught in Operating Systems curriculum.