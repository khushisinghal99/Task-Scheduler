#include <bits/stdc++.h>
#include <chrono>
#include <windows.h>
using namespace std;

void setColor(int textColor, int bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void resetColor() {
    setColor(7, 0); // Default color (Light gray on black)
}

//printing of gantt chart
void printGanttChart(const vector<string>& gantt_chart, const vector<int>& timeline, int end_time) {
    cout << "\nGantt Chart:\n";
    // Top border
    cout << " ";
    for (const auto& task : gantt_chart) {
        cout << "-------";
        this_thread::sleep_for(chrono::milliseconds(30));
    }
    cout << "\n|";

    // Task blocks with color
    for (const auto& task : gantt_chart) {
        if (task == "IDLE") setColor(14);        // Yellow
        else if (task == "math") setColor(11);   // Light Aqua
        else setColor(10);                       // Light Green or custom for others
        cout << " " << setw(5) << left << task.substr(0, 5) << "|";
        setColor(7);  // Reset to default
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    // Bottom border
    cout << "\n ";
    for (const auto& task : gantt_chart) {
        cout << "-------";
        this_thread::sleep_for(chrono::milliseconds(30));
    }
    cout << "\n";

    // Timeline
    cout << setw(7);
    for (size_t i = 0; i < timeline.size(); ++i) {
        cout << timeline[i];
        cout << setw(7);
    }
    cout << end_time << "\n";

    string summary = "\nAll tasks completed at time " + to_string(end_time) + "\n";
    for (char c : summary) {
        cout << c;
        this_thread::sleep_for(chrono::milliseconds(40));
    }
}

class Task {
public:
    int task_id;
    string task_name;
    int burst_time;
    int arrival_time;
    int execution_time; // managed internally
    int priority;
    bool is_completed = false;
    int start_time = -1;        // When task began execution
    int completion_time = -1;   // When task completed
    int waiting_time = 0;       // Time spent waiting
    vector<Task*> dependencies;

    Task() {}

    Task(int t_id, string t_name, int b_time, int arr_time, int prior) {
        task_id = t_id;
        task_name = t_name;
        burst_time = b_time;
        arrival_time = arr_time;
        execution_time = 0; // start at 0
        priority = prior;
    }

    void addDependency(Task* task) {
        dependencies.push_back(task);
    }

    int remainingTime() {
        return burst_time - execution_time;
    }

    bool dependenciesResolved() {
        for (auto dep : dependencies) {
            if (!dep->is_completed) return false;
        }
        return true;
    }

    // New methods for performance metrics
    int getTurnaroundTime() const {
        if (completion_time == -1) return -1;
        return completion_time - arrival_time;
    }

    int getResponseTime() const {
        if (start_time == -1) return -1;
        return start_time - arrival_time;
    }

    string getStatus() {
        if (is_completed) return "Completed";
        if (execution_time > 0) return "In Progress";
        return "Waiting";
    }
};

// New class for tracking system performance
class PerformanceMetrics {
private:
    int total_execution_time = 0;
    vector<int> waiting_times;
    vector<int> turnaround_times;
    vector<int> response_times;
    int context_switches = 0;
    int idle_time = 0;
    double cpu_utilization = 0.0;
    vector<pair<int, string>> timeline; // Time and task name pairs

public:
    void recordTaskCompletion(const Task& task) {
        waiting_times.push_back(task.waiting_time);
        turnaround_times.push_back(task.getTurnaroundTime());
        response_times.push_back(task.getResponseTime());
    }

    void addToTimeline(int time, const string& task_name) {
        timeline.push_back({time, task_name});
    }

    void incrementContextSwitches() {
        context_switches++;
    }

    void addIdleTime(int time) {
        idle_time += time;
    }

    void setTotalExecutionTime(int time) {
        total_execution_time = time;
        if (total_execution_time > 0) {
            cpu_utilization = ((total_execution_time - idle_time) * 100.0) / total_execution_time;
        }
    }

    // Display performance metrics
    void displayMetrics() {
        cout << "\n==== PERFORMANCE METRICS ====\n";

        // Average waiting time
        double avg_waiting = 0;
        if (!waiting_times.empty()) {
            avg_waiting = accumulate(waiting_times.begin(), waiting_times.end(), 0.0) / waiting_times.size();
        }
        cout << "Average Waiting Time: " << fixed << setprecision(2) << avg_waiting << " ms\n";

        // Average turnaround time
        double avg_turnaround = 0;
        if (!turnaround_times.empty()) {
            avg_turnaround = accumulate(turnaround_times.begin(), turnaround_times.end(), 0.0) / turnaround_times.size();
        }
        cout << "Average Turnaround Time: " << fixed << setprecision(2) << avg_turnaround << " ms\n";

        // Average response time
        double avg_response = 0;
        if (!response_times.empty()) {
            avg_response = accumulate(response_times.begin(), response_times.end(), 0.0) / response_times.size();
        }
        cout << "Average Response Time: " << fixed << setprecision(2) << avg_response << " ms\n";

        cout << "Context Switches: " << context_switches << endl;
        cout << "CPU Utilization: " << fixed << setprecision(2) << cpu_utilization << "%\n";
        cout << "Total Execution Time: " << total_execution_time << " ms\n";
        displayTimeline();
    }

    // Visual representation of task execution
    void displayTimeline() {
        if (timeline.empty()) return;

        cout << "\n==== EXECUTION TIMELINE ====\n";
        // Sort by time
        sort(timeline.begin(), timeline.end());

        string last_task = "";
        int last_time = -1;
        for (auto& entry : timeline) {
            int time = entry.first;
            string task = entry.second;
            if (task != last_task || (time - last_time) > 1) {
                if (task == "IDLE") {
                    cout << "Time " << time << ": CPU Idle\n";
                } else {
                    cout << "Time " << time << ": Task " << task << "\n";
                }
                last_task = task;
                last_time = time;
            }
        }
    }

    void reset() {
        waiting_times.clear();
        turnaround_times.clear();
        response_times.clear();
        context_switches = 0;
        idle_time = 0;
        cpu_utilization = 0.0;
        total_execution_time = 0;
        timeline.clear();
    }
};

class TaskScheduler {
private:
    vector<Task> tasks;
    int algorithmChoice;
    PerformanceMetrics metrics; // Performance metrics instance

    Task* getTaskByID(int id) {
        for (auto& task : tasks) {
            if (task.task_id == id) return &task;
        }
        return nullptr;
    }

    // to update waiting times
    void updateWaitingTimes(int current_time) {
        for (auto& task : tasks) {
            if (!task.is_completed && task.arrival_time <= current_time && task.dependenciesResolved()) {
                if (task.start_time == -1) {
                    task.waiting_time = current_time - task.arrival_time;
                }
            }
        }
    }

public:
    TaskScheduler() : algorithmChoice(1) {} // Initialize with default algorithm

    void addTask();
    void viewAllTasks();
    void setDependencies();
    void chooseSchedulingAlgorithm();
    void SJF();
    void SJRF(); // Shortest Job Remaining First
    void PriorityScheduling();
    void runScheduling(); // runs based on selected algorithm
    void viewPerformanceMetrics(); // Method to display metrics
    void deleteTask(); // method to delete task
    void editTask(); // method to edit task
};

void TaskScheduler::addTask() {
    int id, burst, arrival, prio;
    string name;

    while (true) {
        cout << "Enter Task ID: ";
        cin >> id;

        bool exists = false;
        for (const auto& task : tasks) {
            if (task.task_id == id) {
                exists = true;
                break;
            }
        }

        if (exists) {
            cout << "Task ID already exists! Please enter a unique Task ID.\n";
        } else {
            break;
        }
    }

    cout << "Enter Task Name: ";
    cin >> name;
    cout << "Enter Burst Time (in ms): ";
    cin >> burst;
    cout << "Enter Arrival Time (in ms): ";
    cin >> arrival;
    cout << "Enter Priority (1 - High, 5 - Low): ";
    cin >> prio;

    Task newTask(id, name, burst, arrival, prio);
    tasks.push_back(newTask);

    setColor(2);
    cout << "\n\n\t\t\t\t\tTask added successfully!\n";
    resetColor();
    Beep(750, 300);
}

void TaskScheduler::viewAllTasks() {
    if (tasks.empty()) {
        setColor(4);
        cout << "No tasks available. Please add tasks first.\n";
        resetColor();
        Beep(750, 300);
        return;
    }

    cout << "============================================================================\n";
    setColor(15);
    cout << "TASK LIST:\n";
    resetColor();
    cout << "ID | Task Name | Burst Time | Arrival Time | Priority | Status\n";
    cout << "============================================================================\n";

    setColor(14);
    for (auto& task : tasks) {
        cout << task.task_id << " \t "
             << task.task_name << " \t "
             << task.burst_time << "ms \t\t "
             << task.arrival_time << "ms \t\t "
             << task.priority << " \t\t "
             << task.getStatus() << endl;
    }
    resetColor();
    cout << "============================================================================\n";
}

void TaskScheduler::setDependencies() {
    if (tasks.size() < 2) {
        cout << "Need at least 2 tasks to set dependencies. Please add more tasks.\n";
        return;
    }

    int parentID, dependentID;
    cout << "Enter Task ID that depends on another: ";
    cin >> parentID;
    cout << "Enter Task ID that it depends on: ";
    cin >> dependentID;

    Task* parent = getTaskByID(parentID);
    Task* dependent = getTaskByID(dependentID);

    if (parent && dependent) {
        if (parent->task_id == dependent->task_id) {
            cout << "Error: A task cannot depend on itself!\n";
            return;
        }

        // Check for circular dependencies (simple case)
        for (auto& dep : dependent->dependencies) {
            if (dep->task_id == parent->task_id) {
                cout << "Error: This would create a circular dependency!\n";
                return;
            }
        }

        parent->addDependency(dependent);
        cout << "Dependency Added: Task " << parentID << " depends on Task " << dependentID << endl;
    } else {
        cout << "Invalid Task IDs!" << endl;
    }
}

void TaskScheduler::chooseSchedulingAlgorithm() {
    cout << "Choose Scheduling Algorithm:\n";
    setColor(5);
    cout << "1. Shortest Job First (SJF)\n";
    cout << "2. Shortest Remaining Job First (SRJF)\n";
    cout << "3. Priority Scheduling\n";
    cout << "Enter your choice: ";
    resetColor();

    cin >> algorithmChoice;

    // Validate input
    if (algorithmChoice < 1 || algorithmChoice > 3) {
        cout << "Invalid choice. Defaulting to SJF (1).\n";
        algorithmChoice = 1;
    } else {
        cout << "Algorithm chosen successfully.\n";
    }
}

// SHORTEST JOB FIRST
void TaskScheduler::SJF() {
    if (tasks.empty()) {
        cout << "No tasks available. Please add tasks first.\n";
        return;
    }

    int time = 0;
    int completed = 0;
    int total = tasks.size();
    string last_task = "";

    // Reset metrics
    metrics.reset();
    cout << "Executing: Shortest Job First Scheduling\n";

    // Reset tasks for new simulation
    for (auto& task : tasks) {
        task.execution_time = 0;
        task.is_completed = false;
        task.start_time = -1;
        task.completion_time = -1;
        task.waiting_time = 0;
    }

    vector<string> gantt_chart;
    vector<int> timeline;

    while (completed < total) {
        Task* shortest = nullptr;
        for (auto& task : tasks) {
            if (!task.is_completed && task.arrival_time <= time && task.dependenciesResolved()) {
                if (!shortest || task.burst_time < shortest->burst_time) {
                    shortest = &task;
                }
            }
        }

        if (shortest) {
            if (shortest->start_time == -1) {
                shortest->start_time = time;
                shortest->waiting_time = time - shortest->arrival_time;
                if (last_task != shortest->task_name && last_task != "") {
                    metrics.incrementContextSwitches();
                }
                last_task = shortest->task_name;
            }

            for (int i = 0; i < shortest->burst_time; ++i) {
                shortest->execution_time++;
                gantt_chart.push_back(shortest->task_name);
                timeline.push_back(time);
                metrics.addToTimeline(time, shortest->task_name);
                time++;
            }

            shortest->is_completed = true;
            shortest->completion_time = time;
            completed++;
            metrics.recordTaskCompletion(*shortest);
            updateWaitingTimes(time);
        } else {
            gantt_chart.push_back("IDLE");
            timeline.push_back(time);
            metrics.addToTimeline(time, "IDLE");
            metrics.addIdleTime(1);
            time++;
            last_task = "";
        }
    }

    metrics.setTotalExecutionTime(time);
    printGanttChart(gantt_chart, timeline, time);
}

void TaskScheduler::SJRF() {
    if (tasks.empty()) {
        cout << "No tasks available. Please add tasks first.\n";
        return;
    }

    int time = 0;
    int completed = 0;
    int total = tasks.size();
    string last_task = "";
    vector<string> gantt_chart;
    vector<int> timeline;

    // Reset metrics
    metrics.reset();
    cout << "Executing: Shortest Job Remaining First Scheduling\n";

    // Reset tasks for new simulation
    for (auto& task : tasks) {
        task.execution_time = 0;
        task.is_completed = false;
        task.start_time = -1;
        task.completion_time = -1;
        task.waiting_time = 0;
    }

    while (completed < total) {
        Task* shortest = nullptr;
        for (auto& task : tasks) {
            if (!task.is_completed && task.arrival_time <= time && task.dependenciesResolved()) {
                if (!shortest || task.remainingTime() < shortest->remainingTime()) {
                    shortest = &task;
                }
            }
        }

        if (shortest) {
            // Context switch check
            if (last_task != shortest->task_name && last_task != "") {
                metrics.incrementContextSwitches();
            }
            last_task = shortest->task_name;

            // Record task starting if first time
            if (shortest->start_time == -1) {
                shortest->start_time = time;
                shortest->waiting_time = time - shortest->arrival_time;
                cout << "Time " << time << ": Starting Task " << shortest->task_name
                     << " (Remaining: " << shortest->remainingTime() << ")\n";
            } else {
                cout << "Time " << time << ": Executing Task " << shortest->task_name
                     << " (Remaining: " << shortest->remainingTime() << ")\n";
            }

            metrics.addToTimeline(time, shortest->task_name);
            gantt_chart.push_back(shortest->task_name);
            timeline.push_back(time);
            shortest->execution_time++;

            if (shortest->execution_time == shortest->burst_time) {
                shortest->is_completed = true;
                shortest->completion_time = time + 1;
                completed++;
                cout << "Task " << shortest->task_name << " completed at time " << (time + 1) << "\n";
                cout << "Turnaround Time: " << shortest->getTurnaroundTime() << "ms, ";
                cout << "Waiting Time: " << shortest->waiting_time << "ms\n";
                metrics.recordTaskCompletion(*shortest);
            }

            // Update waiting times for other tasks
            updateWaitingTimes(time);
        } else {
            cout << "Time " << time << ": CPU Idle (waiting for tasks/dependencies)\n";
            metrics.addToTimeline(time, "IDLE");
            gantt_chart.push_back("IDLE");
            timeline.push_back(time);
            metrics.addIdleTime(1);
            last_task = "";
        }
        time++;
    }

    metrics.setTotalExecutionTime(time);
    printGanttChart(gantt_chart, timeline, time);
}

// PRIORITY SCHEDULING with performance metrics
void TaskScheduler::PriorityScheduling() {
    if (tasks.empty()) {
        cout << "No tasks available. Please add tasks first.\n";
        return;
    }

    int time = 0;
    int completed = 0;
    int total = tasks.size();
    string last_task = "";
    metrics.reset();

    setColor(3);
    cout << "Executing: Priority Scheduling \n";
    Beep(700, 500);
    resetColor();

    for (auto& task : tasks) {
        task.execution_time = 0;
        task.is_completed = false;
        task.start_time = -1;
        task.completion_time = -1;
        task.waiting_time = 0;
    }

    // Store task execution order for Gantt chart
    vector<pair<int, string>> gantt_chart;

    while (completed < total) {
        Task* highest_priority_task = nullptr;
        for (auto& task : tasks) {
            if (!task.is_completed && task.arrival_time <= time && task.dependenciesResolved()) {
                if (!highest_priority_task || task.priority < highest_priority_task->priority) {
                    highest_priority_task = &task;
                }
            }
        }

        if (highest_priority_task) {
            if (highest_priority_task->start_time == -1) {
                highest_priority_task->start_time = time;
                highest_priority_task->waiting_time = time - highest_priority_task->arrival_time;
                if (last_task != highest_priority_task->task_name && last_task != "") {
                    metrics.incrementContextSwitches();
                }
                last_task = highest_priority_task->task_name;
                cout << "Time " << time << ": Starting Task " << highest_priority_task->task_name
                     << " (Priority: " << highest_priority_task->priority << ")\n";
            }

            // Execute task and add to Gantt chart
            for (int i = 0; i < highest_priority_task->burst_time; ++i) {
                highest_priority_task->execution_time++;
                gantt_chart.push_back({time, highest_priority_task->task_name});
                metrics.addToTimeline(time, highest_priority_task->task_name);
                time++;
            }

            highest_priority_task->is_completed = true;
            highest_priority_task->completion_time = time;
            completed++;
            cout << "Task " << highest_priority_task->task_name << " completed at time " << time << "\n";
            cout << "Turnaround Time: " << highest_priority_task->getTurnaroundTime() << "ms, ";
            cout << "Waiting Time: " << highest_priority_task->waiting_time << "ms\n";
            metrics.recordTaskCompletion(*highest_priority_task);
            updateWaitingTimes(time);
        } else {
            // Track CPU idle period
            gantt_chart.push_back({time, "IDLE"});
            metrics.addToTimeline(time, "IDLE");
            metrics.addIdleTime(1);
            time++;
            last_task = "";
        }
    }

    metrics.setTotalExecutionTime(time);
    vector<string> gantt_tasks;
    vector<int> timeline;
    for (size_t i = 0; i < gantt_chart.size(); ++i) {
        gantt_tasks.push_back(gantt_chart[i].second);
        timeline.push_back(gantt_chart[i].first);
    }
    printGanttChart(gantt_tasks, timeline, time);
}

void TaskScheduler::viewPerformanceMetrics() {
    metrics.displayMetrics();
}

void TaskScheduler::deleteTask() {
    if (tasks.empty()) {
        cout << "No tasks available to delete.\n";
        return;
    }

    int id;
    cout << "Enter Task ID to delete: ";
    cin >> id;

    // First find if the task exists
    bool found = false;
    for (const auto& task : tasks) {
        if (task.task_id == id) {
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Task ID not found.\n";
        return;
    }

    // Remove dependencies
    for (auto& task : tasks) {
        auto it = remove_if(task.dependencies.begin(), task.dependencies.end(),
                          [id](Task* t) { return t->task_id == id; });
        if (it != task.dependencies.end()) {
            task.dependencies.erase(it, task.dependencies.end());
        }
    }

    // Now remove the task
    auto it = remove_if(tasks.begin(), tasks.end(),
                      [id](const Task& task) { return task.task_id == id; });
    if (it != tasks.end()) {
        tasks.erase(it, tasks.end());
        cout << "Task deleted successfully.\n";
    }
}

void TaskScheduler::editTask() {
    if (tasks.empty()) {
        cout << "No tasks available to edit.\n";
        return;
    }

    int id;
    cout << "Enter Task ID to edit: ";
    cin >> id;

    for (auto& task : tasks) {
        if (task.task_id == id) {
            cout << "Editing Task: " << task.task_name << "\n";
            cout << "Enter new Task Name (current: " << task.task_name << "): ";
            cin >> task.task_name;
            cout << "Enter new Burst Time (current: " << task.burst_time << "): ";
            cin >> task.burst_time;
            cout << "Enter new Arrival Time (current: " << task.arrival_time << "): ";
            cin >> task.arrival_time;
            cout << "Enter new Priority (current: " << task.priority << "): ";
            cin >> task.priority;

            // Reset execution and completion
            task.execution_time = 0;
            task.is_completed = false;
            task.start_time = -1;
            task.completion_time = -1;
            cout << "Task updated successfully.\n";
            return;
        }
    }
    cout << "Task ID not found.\n";
}

void TaskScheduler::runScheduling() {
    if (tasks.empty()) {
        cout << "No tasks available. Please add tasks first.\n";
        return;
    }

    cout << "Running scheduling with algorithm choice: " << algorithmChoice << endl;
    switch (algorithmChoice) {
    case 1:
        SJF();
        break;
    case 2:
        SJRF();
        break;
    case 3:
        PriorityScheduling();
        break;
    default:
        cout << "Invalid algorithm choice. Please select a valid algorithm.\n";
        break;
    }
}

void displayMenu() {
    cout << R"(
______ ______ ______ ______ ______ ______ ______ ______ ______ ______ ______ ______ ______ ______ ______ ______
|______|______|______|______|______|______|______|______|______|______|______|______|______|______|______|______|
)";
    this_thread::sleep_for(chrono::seconds(1)); // 1-second delay
    setColor(11);
    cout << "\t\t\t\t\tTASK SCHEDULING & OPTIMIZATION SYSTEM\n";
    resetColor();
    cout << R"(
______ ______ ______ ______ ______ ______ ______ ______ ______ ______ ______ ______ ______ ______ ______ ______
|______|______|______|______|______|______|______|______|______|______|______|______|______|______|______|______|
)";
    setColor(11);
    cout << "\t\t\t\t\t1. Add Tasks\n";
    this_thread::sleep_for(chrono::milliseconds(250));
    setColor(7);
    cout << "\t\t\t\t\t2. View All Tasks\n";
    this_thread::sleep_for(chrono::milliseconds(250));
    setColor(11);
    cout << "\t\t\t\t\t3. Set Task Dependencies\n";
    this_thread::sleep_for(chrono::milliseconds(250));
    setColor(7);
    cout << "\t\t\t\t\t4. Choose Scheduling Algorithm\n";
    this_thread::sleep_for(chrono::milliseconds(250));
    setColor(11);
    cout << "\t\t\t\t\t5. Run Scheduling\n";
    this_thread::sleep_for(chrono::milliseconds(250));
    setColor(7);
    cout << "\t\t\t\t\t6. View Performance Metrics\n";
    this_thread::sleep_for(chrono::milliseconds(250));
    setColor(11);
    cout << "\t\t\t\t\t7. Exit\n";
    this_thread::sleep_for(chrono::milliseconds(250));
    setColor(7);
    cout << "\t\t\t\t\t8. Edit a Task\n";
    this_thread::sleep_for(chrono::milliseconds(250));
    setColor(11);
    cout << "\t\t\t\t\t9. Delete a Task\n";
    this_thread::sleep_for(chrono::milliseconds(250));
    resetColor();
    cout << R"(
______ ______ ______ ______ ______ ______ ______ ______ ______ ______ ______ ______ ______ ______ ______ ______
|______|______|______|______|______|______|______|______|______|______|______|______|______|______|______|______|
)";
    cout << "Enter your choice: ";
}

int main() {
    TaskScheduler scheduler;
    int choice;

    setColor(14, 0);
    cout << R"(
 _______              _      _____      _              _       _ _               _____            _
|__   __|            | |    / ____|    | |            | |     | (_)             / ____|          | |
   | | __ _ ___  ___  | |   | (___   ___| |__   ___  __| |_   _| |_ _ __   __ _  | (___  _   _ ___| |_ ___ _ __ ___
   | |/ _` / __|/ _ \ | |    \___ \ / __| '_ \ / _ \/ _` | | | | | | '_ \ / _` |  \___ \| | | / __| __/ _ \ '_ ` _ \
   | | (_| \__ \  __/ | |    ____) | (__| | | |  __/ (_| | |_| | | | | | | (_| |  ____) | |_| \__ \ ||  __/ | | | | |
   |_|\__,_|___/\___| |_|   |_____/ \___|_| |_|\___|\__,_|\__,_|_|_|_| |_|\__, | |_____/ \__, |___/\__\___|_| |_| |_|
                                                                           __/ |          __/ |
                                                                          |___/          |___/
)";
    resetColor();

    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1: scheduler.addTask(); break;
        case 2: scheduler.viewAllTasks(); break;
        case 3: scheduler.setDependencies(); break;
        case 4: scheduler.chooseSchedulingAlgorithm(); break;
        case 5: scheduler.runScheduling(); break;
        case 6: scheduler.viewPerformanceMetrics(); break;
        case 7:
            cout << "Exiting program. Goodbye!\n";
            return 0;
case 8: scheduler.editTask(); break;
case 9: scheduler.deleteTask(); break;
default: cout << "Invalid choice! Please enter a number between 1 and 9.\n";
}
}
return 0;



}




