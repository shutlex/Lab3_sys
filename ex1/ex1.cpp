#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int startTime;
    int completionTime;
    int waitingTime;
    int turnaroundTime;

    Process(int id, int arrivalTime, int burstTime)
        : id(id), arrivalTime(arrivalTime), burstTime(burstTime),
        startTime(-1), completionTime(0), waitingTime(0), turnaroundTime(0) {
    }
};

void shortestJobFirst(std::vector<Process>& processes) {
    int currentTime = 0;
    int completed = 0;
    int n = processes.size();

    while (completed < n) {
        
        auto it = std::min_element(processes.begin(), processes.end(), [&](const Process& a, const Process& b) {
            if (a.startTime != -1) return false; 
            if (b.startTime != -1) return true;  
            if (a.arrivalTime > currentTime && b.arrivalTime > currentTime) return a.arrivalTime < b.arrivalTime;
            if (a.arrivalTime > currentTime) return false;
            if (b.arrivalTime > currentTime) return true;
            return a.burstTime < b.burstTime;
            });

        if (it == processes.end() || it->startTime != -1 || it->arrivalTime > currentTime) {
            currentTime++;
            continue;
        }

        
        it->startTime = currentTime;
        it->completionTime = currentTime + it->burstTime;
        it->turnaroundTime = it->completionTime - it->arrivalTime;
        it->waitingTime = it->turnaroundTime - it->burstTime;

        currentTime += it->burstTime;
        completed++;
    }

    std::cout << "ID\tArrival\tBurst\tStart\tCompletion\tWaiting\tTurnaround\n";
    for (const auto& p : processes) {
        std::cout << p.id << '\t' << p.arrivalTime << '\t' << p.burstTime << '\t'
            << p.startTime << '\t' << p.completionTime << '\t'
            << p.waitingTime << '\t' << p.turnaroundTime << '\n';
    }

    double avgWaitingTime = 0, avgTurnaroundTime = 0;
    for (const auto& p : processes) {
        avgWaitingTime += p.waitingTime;
        avgTurnaroundTime += p.turnaroundTime;
    }
    avgWaitingTime /= n;
    avgTurnaroundTime /= n;

    std::cout << "Average Waiting Time: " << std::fixed << std::setprecision(2) << avgWaitingTime << " ms\n";
    std::cout << "Average Turnaround Time: " << std::fixed << std::setprecision(2) << avgTurnaroundTime << " ms\n";
}

int main() {
    std::vector<Process> processes = {
        {1, 0, 6},
        {2, 1, 8},
        {3, 2, 7},
        {4, 3, 3}
    };

    std::cout << "Shortest Job First (Non-preemptive):\n";
    shortestJobFirst(processes);

    return 0;
}
