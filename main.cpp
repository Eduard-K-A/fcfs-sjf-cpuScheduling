#include <iostream>
#include <iomanip>
#include <vector>
#include <set> // for storing used process IDs
using namespace std;

class Process
{
private:
    int processID;
    int arrivalTime;
    int burstTime;
    int waitingTime;
    int turnaroundTime;

public:
    Process(int id = 0, int at = 0, int bt = 0)
    {
        processID = id;
        arrivalTime = at;
        burstTime = bt;
        waitingTime = 0;
        turnaroundTime = 0;
    }

    // Getters
    int getProcessID() const { return processID; }
    int getArrivalTime() const { return arrivalTime; }
    int getBurstTime() const { return burstTime; }
    int getWaitingTime() const { return waitingTime; }
    int getTurnaroundTime() const { return turnaroundTime; }

    // Setters
    void setWaitingTime(int wt) { waitingTime = wt; }
    void setTurnaroundTime(int tat) { turnaroundTime = tat; }
};

// Function to compute waiting time and turnaround time (FCFS)
void computeTimes(vector<Process> &processes)
{
    int n = processes.size();
    int currentTime = 0;

    for (int i = 0; i < n; i++)
    {
        if (currentTime < processes[i].getArrivalTime())
        {
            currentTime = processes[i].getArrivalTime();
        }

        int wt = currentTime - processes[i].getArrivalTime();
        if (wt < 0)
            wt = 0;
        processes[i].setWaitingTime(wt);

        currentTime += processes[i].getBurstTime();
        int tat = processes[i].getBurstTime() + processes[i].getWaitingTime();
        processes[i].setTurnaroundTime(tat);
    }
}

// Function to print results in a table
void createTable(const vector<Process> &processes)
{
    cout << "---------------------------------------------------------------------------\n";
    cout << "| Process ID | Arrival Time | Burst Time | Waiting Time | Turnaround Time |\n";
    cout << "---------------------------------------------------------------------------\n";

    double totalWT = 0, totalTAT = 0;
    for (const auto &p : processes)
    {
        cout << setw(7) << p.getProcessID()
             << setw(14) << p.getArrivalTime()
             << setw(14) << p.getBurstTime()
             << setw(14) << p.getWaitingTime()
             << setw(16) << p.getTurnaroundTime() << "\n";

        totalWT += p.getWaitingTime();
        totalTAT += p.getTurnaroundTime();
    }

    cout << "---------------------------------------------------------------------------\n";
    cout << "Average Waiting Time     : " << fixed << setprecision(2) << (totalWT / processes.size()) << "\n";
    cout << "Average Turnaround Time  : " << fixed << setprecision(2) << (totalTAT / processes.size()) << "\n";
}

int main()
{
    int processCount;

    // Validate number of processes
    do
    {
        cout << "Enter number of processes (3-10): ";
        cin >> processCount;
        if (cin.fail() || processCount < 3 || processCount > 10)
        {
            cout << "Invalid input! Please enter a number between 3 and 10.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    } while (processCount < 3 || processCount > 10);

    vector<Process> processes;
    set<int> usedIDs; // to store already entered process IDs

    for (int i = 0; i < processCount; i++)
    {
        int id, at, bt;

        cout << "\nEnter details for Process " << (i + 1) << ":\n";

        // ✅ Process ID validation (must be unique)
        while (true)
        {
            cout << "Process ID: ";
            if (!(cin >> id))
            {
                cout << "Invalid input! Enter a valid integer for Process ID.\n";
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }
            if (usedIDs.count(id))
            {
                cout << "Process ID already exists! Enter a unique Process ID.\n";
                continue;
            }
            usedIDs.insert(id);
            break;
        }

        // Arrival Time validation
        cout << "Arrival Time: ";
        while (!(cin >> at) || at < 0)
        {
            cout << "Invalid input! Arrival Time must be >= 0: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        // Burst Time validation
        cout << "Burst Time: ";
        while (!(cin >> bt) || bt <= 0)
        {
            cout << "Invalid input! Burst Time must be > 0: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        processes.emplace_back(id, at, bt);
    }

    // Compute times using FCFS
    computeTimes(processes);

    // Print results
    createTable(processes);

    return 0;
}
