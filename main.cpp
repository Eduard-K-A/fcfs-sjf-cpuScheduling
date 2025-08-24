#include <iostream>
#include <iomanip>

using namespace std;

class Process
{
private:
    int processID;
    int arrivalTime;
    int burstTime;
    int waitingTime;
    int processCount;

public:
    int processCtr()
    { // function to set number of processes
        cout << "Enter number of processes: ";
        cin >> processCount;
    }

    int getProcessCount() const
    { // getter process count
        return processCount;
    }

    void setProcessCount(int processCount)
    { // setter process count
        Process::processCount = processCount;
    }
    int getProcessID() const
    { //    getter process ID
        return processID;
    }
    void setProcessID(int processID)
    { // setter process ID
        Process::processID = processID;
    }

    int getArrivalTime() const
    { // getter arrival time
        return arrivalTime;
    }

    void setArrivalTime(int arrivalTime)
    { // setter arrival time
        Process::arrivalTime = arrivalTime;
    }
};

Process createTable(Process u.processCount)
{
    cout << "--------------------------------------------------------" << endl;
    cout << "| Process ID|" << setw(10) << " Arrival Time |" << setw(10) << " Burst Time |" << setw(10) << " Waiting Time |" << setw(10) << "" << endl;
    // Function to create a table for visual aid in CPU scheduling
    for (int i = 0; i < u.process; i++)
    {
        cout << "|" << setw(6) << i + 1 << setw(6)             // Process ID
             << "|" << setw(7) << 0 << setw(8)                 // Arrival Time
             << "|" << setw(6) << 0 << setw(7)                 // burst Time
             << "|" << setw(8) << 0 << setw(7) << "|" << endl; // waiting Time

        // Logic to create a row for each process
    }
    cout << "--------------------------------------------------------" << endl;
}

int main()
{

    int processCount = 5;
    createTable(processCount);
    return 0;
}