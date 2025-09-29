#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <climits>

using namespace std;

class Process {
private:
    string processID;
    int arrivalTime;
    int burstTime;
    int waitingTime;
    int turnaroundTime;

public:
    Process(string id = "", int at = 0, int bt = 0)
        : processID(id), arrivalTime(at), burstTime(bt), waitingTime(0), turnaroundTime(0) {}

    string getProcessID() const { return processID; }
    int getArrivalTime() const { return arrivalTime; }
    int getBurstTime() const { return burstTime; }
    int getWaitingTime() const { return waitingTime; }
    int getTurnaroundTime() const { return turnaroundTime; }

    void setWaitingTime(int wt) { waitingTime = wt; }
    void setTurnaroundTime(int tat) { turnaroundTime = tat; }
};

// Compute times for FCFS
void computeFCFSTimes(vector<Process>& processes) {
    int currentTime = 0;
    for (auto& p : processes) {
        if (currentTime < p.getArrivalTime()) currentTime = p.getArrivalTime();
        int wt = currentTime - p.getArrivalTime();
        p.setWaitingTime(max(wt, 0));
        currentTime += p.getBurstTime();
        p.setTurnaroundTime(p.getWaitingTime() + p.getBurstTime());
    }
}

// Compute times for SJF Non-Preemptive (refactored from your TypeScript reference)
void computeSJFTimes(vector<Process>& processes) {
    int n = processes.size();
    int currentTime = 0;
    int completedCount = 0;
    vector<bool> completed(n, false);

    while (completedCount < n) {
        // find ready processes
        int idx = -1;
        int minBT = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!completed[i] && processes[i].getArrivalTime() <= currentTime) {
                if (processes[i].getBurstTime() < minBT) {
                    minBT = processes[i].getBurstTime();
                    idx = i;
                } else if (processes[i].getBurstTime() == minBT) {
                    // tie-breaker: earlier arrival
                    if (processes[i].getArrivalTime() < processes[idx].getArrivalTime()) {
                        idx = i;
                    }
                }
            }
        }

        // if no job is ready, jump to next available
        if (idx == -1) {
            int nextArrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (!completed[i]) {
                    nextArrival = min(nextArrival, processes[i].getArrivalTime());
                }
            }
            currentTime = nextArrival;
            continue;
        }

        // execute chosen job
        int startTime = max(currentTime, processes[idx].getArrivalTime());
        int finishTime = startTime + processes[idx].getBurstTime();
        int waitingTime = startTime - processes[idx].getArrivalTime();

        processes[idx].setWaitingTime(waitingTime);
        processes[idx].setTurnaroundTime(waitingTime + processes[idx].getBurstTime());

        currentTime = finishTime;
        completed[idx] = true;
        completedCount++;
    }

    // stable sort by arrival time, then ID
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        if (a.getArrivalTime() == b.getArrivalTime())
            return a.getProcessID() < b.getProcessID();
        return a.getArrivalTime() < b.getArrivalTime();
    });
}

// Print results in a table
void createTable(const vector<Process>& processes) {
    cout << "-------------------------------------------------------------------------------\n";
    cout << "| Process ID | Arrival Time | Burst Time | Waiting Time | Turnaround Time |\n";
    cout << "-------------------------------------------------------------------------------\n";

    double totalWT = 0, totalTAT = 0;
    for (const auto& p : processes) {
        cout << setw(10) << p.getProcessID()
             << setw(14) << p.getArrivalTime()
             << setw(12) << p.getBurstTime()
             << setw(14) << p.getWaitingTime()
             << setw(16) << p.getTurnaroundTime() << "\n";

        totalWT += p.getWaitingTime();
        totalTAT += p.getTurnaroundTime();
    }

    cout << "-------------------------------------------------------------------------------\n";
    cout << "Average Waiting Time    : " << fixed << setprecision(2) << (totalWT / processes.size()) << "\n";
    cout << "Average Turnaround Time : " << fixed << setprecision(2) << (totalTAT / processes.size()) << "\n";
}

// Input process details with unique arrival time validation
void inputProcesses(vector<Process>& processes, int processCount) {
    set<string> usedIDs;
    set<int> usedArrivalTimes;

    for (int i = 0; i < processCount; i++) {
        string id;
        int at, bt;

        cout << "\nEnter details for Process " << (i + 1) << ":\n";

        // Process ID validation
        while (true) {
            cout << "Process ID: ";
            cin >> id;
            transform(id.begin(), id.end(), id.begin(), ::toupper);
            if (usedIDs.count(id)) {
                cout << "Process ID already exists! Enter a unique Process ID.\n";
                continue;
            }
            usedIDs.insert(id);
            break;
        }

        // Arrival Time validation (unique)
        while (true) {
            cout << "Arrival Time: ";
            if (!(cin >> at) || at < 0) {
                cout << "Invalid input! Arrival Time must be >= 0: ";
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }
            if (usedArrivalTimes.count(at)) {
                cout << "Arrival Time already used! Enter a unique Arrival Time.\n";
                continue;
            }
            usedArrivalTimes.insert(at);
            break;
        }

        // Burst Time validation
        cout << "Burst Time: ";
        while (!(cin >> bt) || bt <= 0) {
            cout << "Invalid input! Burst Time must be > 0: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        processes.emplace_back(id, at, bt);
    }
}

int main() {
    char retry;
    do {
        int processCount, choice;
        vector<Process> processes;

        cout << "Choose a CPU Scheduling Algorithm:\n";
        cout << "1. First-Come, First-Served (FCFS) Non-Preemptive\n";
        cout << "2. Shortest Job First (SJF) Non-Preemptive\n";
        cout << "Enter your choice (1 or 2): ";
        cin >> choice;

        do {
            cout << "Enter number of processes (3-10): ";
            cin >> processCount;
            if (cin.fail() || processCount < 3 || processCount > 10) {
                cout << "Invalid input! Please enter a number between 3 and 10.\n";
                cin.clear();
                cin.ignore(1000, '\n');
            }
        } while (processCount < 3 || processCount > 10);

        inputProcesses(processes, processCount);

        switch (choice) {
            case 1:
                cout << "\nFirst-Come, First-Served (FCFS) Non-Preemptive CPU Scheduling\n";
                sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
                    return a.getArrivalTime() < b.getArrivalTime();
                });
                computeFCFSTimes(processes);
                break;
            case 2:
                cout << "\nShortest Job First (SJF) Non-Preemptive CPU Scheduling\n";
                computeSJFTimes(processes);
                break;
            default:
                cout << "Invalid choice! Exiting program.\n";
                return 0;
        }

        createTable(processes);

        cout << "\nDo you want to try again? (Y/N): ";
        cin >> retry;
        retry = toupper(retry);

    } while (retry == 'Y');

    cout << "Program terminated.\n";
    return 0;
}
