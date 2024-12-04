#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>

// Структура процесу
struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority;
    int originalPriority; // Для демонстрації змін
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

// Допоміжна функція для обчислення середніх значень
void calculateAverageTimes(const std::vector<Process>& processes, const std::string& algorithmName) {
    double totalWaitingTime = 0, totalTurnaroundTime = 0;
    int makespan = 0;

    for (const auto& p : processes) {
        totalWaitingTime += p.waitingTime;
        totalTurnaroundTime += p.turnaroundTime;
        makespan = std::max(makespan, p.completionTime);
    }

    double avgWaitingTime = totalWaitingTime / processes.size();
    double avgTurnaroundTime = totalTurnaroundTime / processes.size();

    std::cout << "Algorithm: " << algorithmName << "\n";
    std::cout << "Average Waiting Time: " << avgWaitingTime << " ms\n";
    std::cout << "Average Turnaround Time: " << avgTurnaroundTime << " ms\n";
    std::cout << "Makespan: " << makespan << " ms\n\n";
}

// Реалізація SJF
void shortestJobFirst(std::vector<Process>& processes) {
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

        auto it = std::min_element(readyQueue.begin(), readyQueue.end(),
            [](const Process* a, const Process* b) {
                return a->burstTime < b->burstTime ||
                    (a->burstTime == b->burstTime && a->arrivalTime < b->arrivalTime);
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

        currentTime += currentProcess->burstTime;
        currentProcess->completionTime = currentTime;
        currentProcess->turnaroundTime = currentProcess->completionTime - currentProcess->arrivalTime;
        currentProcess->waitingTime = currentProcess->turnaroundTime - currentProcess->burstTime;

        completed++;
    }

    calculateAverageTimes(processes, "Shortest Job First (SJF)");
}

// Реалізація Priority Scheduling із старінням
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

    calculateAverageTimes(processes, "Priority Scheduling with Aging");
}

int main() {
    std::srand(std::time(0));
    std::vector<Process> processes;

    // Генеруємо випадкові процеси
    int numProcesses = 10;
    for (int i = 0; i < numProcesses; ++i) {
        int arrivalTime = std::rand() % 10;
        int burstTime = 1 + std::rand() % 10;
        int priority = 1 + std::rand() % 10;
        processes.emplace_back(i + 1, arrivalTime, burstTime, priority);
    }

    // Копії процесів для різних алгоритмів
    std::vector<Process> processesForSJF = processes;
    std::vector<Process> processesForPriority = processes;

    std::cout << "Simulating Shortest Job First (SJF):\n";
    shortestJobFirst(processesForSJF);

    std::cout << "Simulating Priority Scheduling with Aging:\n";
    prioritySchedulingWithAging(processesForPriority, 1);

    return 0;
}

