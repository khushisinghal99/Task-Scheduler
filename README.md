# Task-Scheduler

# Task Scheduling & Optimization System

A **C++ console-based Task Scheduler simulator** that demonstrates classic **CPU scheduling algorithms** with rich visualization, task dependencies, and detailed performance metrics. This project is designed for for showcasing how different scheduling strategies affect system performance.



---

## ✨ Features

* 📋 **Task Management**

  * Add, view, edit, and delete tasks
  * Each task has:

    * Task ID & Name
    * Burst Time
    * Arrival Time
    * Priority (1 = High, 5 = Low)

* 🔗 **Task Dependencies**

  * Define dependencies between tasks
  * Tasks execute only after dependencies are resolved
  * Circular dependency prevention

* ⚙️ **Scheduling Algorithms Implemented**

  1. **Shortest Job First (SJF)** – Non-preemptive
  2. **Shortest Job Remaining First (SJRF)** – Preemptive
  3. **Priority Scheduling** – Non-preemptive

* 📊 **Gantt Chart Visualization**

  * Colored execution blocks
  * Idle time clearly highlighted
  * Animated output for better understanding

* 📈 **Performance Metrics**

  * Average Waiting Time
  * Average Turnaround Time
  * Average Response Time
  * CPU Utilization
  * Context Switch Count
  * Total Execution Time
  * Detailed Execution Timeline

* 🎨 **Enhanced Console UI**

  * Windows console colors
  * Beep notifications
  * Animated menus and output

---

## 🛠️ Technologies Used

* **Language:** C++
* **Libraries:**
  * `<bits/stdc++.h>`
  * `<chrono>`
  * `<windows.h>` (for colors & sound)
* **Platform:** Windows (Console Application)


## 🚀 How to Run

### 1️⃣ Compile

```bash
g++ task_scheduler.cpp -o task_scheduler
```

### 2️⃣ Run

```bash
./task_scheduler
```
(or double-click the executable on Windows)

---

## 🧪 Example Workflow

1. Add multiple tasks with different arrival times and priorities
2. Set dependencies between tasks
3. Choose a scheduling algorithm
4. Run the scheduler
5. View:

   * Gantt Chart
   * Execution Timeline
   * Performance Metrics

---


## 📚 Concepts Demonstrated

* CPU Scheduling Algorithms
* Preemptive vs Non-Preemptive Scheduling
* Task Dependency Handling
* Gantt Chart Construction
* Performance Evaluation Metrics
* Context Switching
* CPU Utilization Analysis


## 🎯 Educational Use

This project is ideal for:

* Operating Systems Lab
* CPU Scheduling Demonstrations
* College Mini Projects
* Understanding real-time scheduling behavior

---

## 🔮 Future Enhancements

* ⏱️ Round Robin Scheduling
* 🧵 Multithreaded Simulation
* 💾 File-based task input
* 📊 Graphical UI

## 📬 **Contact**  

If you have any questions or suggestions, feel free to connect with us:  
- **GitHub Repo**:   https://github.com/Dhruvi-Bansal/Task-Scheduler
- **Contributors**:
  - [khushisinghal99](https://github.com/khushisinghal99)   
  - [Dhruvi-Bansal](https://github.com/Dhruvi-Bansal)  
  - [AnshikaRajput296](https://github.com/AnshikaRajput296)  

 
