#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority;
    int originalPriority; 
    int startTime;
    int completionTime;
    int waitingTime;
    int turnaroundTime;

    Process(int id, int arrivalTime, int burstTime, int priority)
        : id(id), arrivalTime(arrivalTime), burstTime(burstTime), remainingTime(burstTime),
        priority(priority), originalPriority(priority), startTime(-1),
        completionTime(0), waitingTime(0), turnaroundTime(0) {
    }
};

void prioritySchedulingWithAging(std::vector<Process>& processes, int agingFactor) {
    int currentTime = 0;
    int completed = 0;
    int n = processes.size();
    std::vector<Process*> readyQueue;

    while (completed < n) {
        
        for (auto& p : processes) {
            if (p.arrivalTime <= currentTime && p.remainingTime > 0 &&
                std::find(readyQueue.begin(), readyQueue.end(), &p) == readyQueue.end()) {
                readyQueue.push_back(&p);
            }
        }

        
        for (auto& p : readyQueue) {
            if (p->remainingTime > 0 && currentTime > p->arrivalTime) {
                p->priority = std::max(0, p->priority - agingFactor);
            }
        }

        
        auto it = std::min_element(readyQueue.begin(), readyQueue.end(),
            [](const Process* a, const Process* b) {
                return a->priority < b->priority ||
                    (a->priority == b->priority && a->arrivalTime < b->arrivalTime);
            });

        if (it == readyQueue.end() || (*it)->remainingTime == 0) {
            currentTime++;
            continue;
        }

        Process* currentProcess = *it;
        readyQueue.erase(it);

        
        if (currentProcess->startTime == -1) {
            currentProcess->startTime = currentTime;
        }

        
        currentProcess->remainingTime--;
        currentTime++;

        
        if (currentProcess->remainingTime == 0) {
            completed++;
            currentProcess->completionTime = currentTime;
            currentProcess->turnaroundTime = currentProcess->completionTime - currentProcess->arrivalTime;
            currentProcess->waitingTime = currentProcess->turnaroundTime - currentProcess->burstTime;
        }
    }

    
    std::cout << "ID\tArrival\tBurst\tStart\tCompletion\tWaiting\tTurnaround\tOriginal Priority\tFinal Priority\n";
    for (const auto& p : processes) {
        std::cout << p.id << '\t' << p.arrivalTime << '\t' << p.burstTime << '\t'
            << p.startTime << '\t' << p.completionTime << '\t'
            << p.waitingTime << '\t' << p.turnaroundTime << '\t'
            << p.originalPriority << "\t\t\t" << p.priority << '\n';
    }

  
    double avgWaitingTime = 0, avgTurnaroundTime = 0;
    for (const auto& p : processes) {
        avgWaitingTime += p.waitingTime;
        avgTurnaroundTime += p.turnaroundTime;
    }
    avgWaitingTime /= n;
    avgTurnaroundTime /= n;

    std::cout << "Average Waiting Time: " << avgWaitingTime << " ms\n";
    std::cout << "Average Turnaround Time: " << avgTurnaroundTime << " ms\n";
}

int main() {
    std::vector<Process> processes = {
        {1, 0, 6, 5},  
        {2, 1, 8, 3},
        {3, 2, 7, 4},
        {4, 3, 3, 2}
    };

    int agingFactor = 1; 
    std::cout << "Priority Scheduling with Aging:\n";
    prioritySchedulingWithAging(processes, agingFactor);

    return 0;
}
