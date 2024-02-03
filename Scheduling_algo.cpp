#include <iostream>
#include <vector>
#include <string>
using namespace std;


//Abdulrahman Abudabaseh 20200514
//Osama Sarayreh 20200336

class Process {

public:
    static int count;

    int BurstTime;
    int ArrivalTime;
    int WaitingTime;
    int TuraroundTime;
    int ResponseTime;
    int finishTime;
    int ExecutedTime;
    int id;
    int Paused;
    int Resumed;

    Process() {
        BurstTime = 0;
        ArrivalTime = 0;
        WaitingTime = 0;
        TuraroundTime = 0;
        ResponseTime = 0;
        ExecutedTime = 0;
        Paused = 0;
        Resumed = 0;
        finishTime = 0;
        id = count++;
    }

    int timeLeft() {
        return BurstTime - ExecutedTime;
    }

    bool done() {
        return BurstTime == ExecutedTime;
    }

    void updateWaiting() {
        WaitingTime += (Resumed - Paused);
    }

    void updateTurnaround() {
        TuraroundTime = finishTime - ArrivalTime;
    }
};

int Process::count = 0;

int main() {

    int TotalTime = 0;
    cout << "Enter the number of processes: " << endl;

    int size = 0;
    cin >> size;

    Process* arr = new Process[size];

    //taking input
    for (int i = 0; i < size; i++) {

        cout << "Enter Burst Time for process: " << i << endl;
        cin >> arr[i].BurstTime;
        TotalTime += arr[i].BurstTime;

        cout << "Enter Arrival Time for process: " << i << endl;
        cin >> arr[i].ArrivalTime;
        arr[i].Paused = arr[i].ArrivalTime;
    }


    //sort
    for (int i = 0; i < size - 1; i++) {
        bool swapped = false;
        for (int j = size - 1; j > i; j--) {
            if (arr[j].ArrivalTime < arr[j - 1].ArrivalTime) {
                int at = arr[j].ArrivalTime;
                int bt = arr[j].BurstTime;
                int pt = arr[j].Paused;
                int id = arr[j].id;

                arr[j].ArrivalTime = arr[j - 1].ArrivalTime;
                arr[j].BurstTime = arr[j - 1].BurstTime;
                arr[j].Paused = arr[j - 1].Paused;
                arr[j].id = arr[j - 1].id;

                arr[j - 1].ArrivalTime = at;
                arr[j - 1].BurstTime = bt;
                arr[j - 1].Paused = pt;
                arr[j - 1].id = id;
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }


    int index = 0;
    int timePassed = 0;


    //strings for each level for the gant chart
    string level1[3];
    string level2[3];
    string level3[3];

    for (int i = 0; i < 3; i++) {
        level1[i] = "";
        level2[i] = "";
        level3[i] = "";
    }

    while (true) {

        //to select the next process and check if all finished
        bool finished = true;
        int t = 100;
        while (t--) {
            if (arr[index].ArrivalTime <= timePassed && !arr[index].done()) {
                finished = false;
                break;
            }
            index = (index + 1) % size;
        }

        if (finished)
            break;

        //to update the waiting time
        arr[index].Resumed = timePassed;
        arr[index].updateWaiting();

        //to update the response the if it's the first time for the process to execute
        //cout << arr[index].Paused;
        if (arr[index].ArrivalTime == arr[index].Paused) {
            arr[index].ResponseTime = timePassed - arr[index].ArrivalTime;
        }

        //level 1

        if ((float)timePassed / (float)TotalTime < 0.6) {

            level1[0] += to_string(arr[index].id);
            level1[2] += to_string(timePassed);
            

            if (arr[index].timeLeft() < 8) {
                timePassed += arr[index].timeLeft();

                for (int k = 0; k < arr[index].timeLeft() - 1; k++) {
                    level1[0] += " ";
                    level1[1] += static_cast<char>(178);
                    level1[2] += " ";
                }

                arr[index].ExecutedTime = arr[index].BurstTime;

                
            }
            else {
                for (int k = 0; k < 8 - 1; k++) {
                    level1[0] += " ";
                    level1[1] += static_cast<char>(178);
                    level1[2] += " ";
                }
                timePassed += 8;
                arr[index].ExecutedTime += 8;
            }
        }

        //level 2
        else if ((float)timePassed / (float)TotalTime < 0.85) {

            level2[0] += to_string(arr[index].id);
            level2[2] += to_string(timePassed);

            if (arr[index].timeLeft() < 16) {
                timePassed += arr[index].timeLeft();

                for (int k = 0; k < arr[index].timeLeft() - 1; k++) {
                    level2[0] += " ";
                    level2[1] += static_cast<char>(178);
                    level2[2] += " ";
                }

                arr[index].ExecutedTime = arr[index].BurstTime;

                
            }
            else {
                timePassed += 16;
                arr[index].ExecutedTime += 16;

                for (int k = 0; k < 16 - 1; k++) {
                    level2[0] += " ";
                    level2[1] += static_cast<char>(178);
                    level2[2] += " ";
                }
            }
        }

        //level 3
        else {
            level3[0] += to_string(arr[index].id);
            level3[2] += to_string(timePassed);

            for (int k = 0; k < arr[index].timeLeft() - 1; k++) {
                level3[0] += " ";
                level3[1] += static_cast<char>(178);
                level3[2] += " ";
            }
            timePassed += arr[index].timeLeft();
            arr[index].ExecutedTime = arr[index].BurstTime;
        }

        //to update when the process pasued
        arr[index].Paused = timePassed;

        //to update turnaround time
        if (arr[index].done()) {
            arr[index].finishTime = timePassed;
            arr[index].updateTurnaround();
        }

        index = (index + 1) % size;
    }


    cout << endl;
    cout << endl;

    for (int i = 0; i < size; i++) {
        cout << "waiting time for process " << arr[i].id << " is " << arr[i].WaitingTime << endl;
        cout << "Response time for process " << arr[i].id << " is " << arr[i].ResponseTime << endl;
        cout << "Turnaround time for process " << arr[i].id << " is " << arr[i].TuraroundTime << endl;
        //cout << "Arrival time for process " << arr[i].id << " is " << arr[i].ArrivalTime << endl;
        //cout << "Finish time for process " << arr[i].id << " is " << arr[i].finishTime << endl;
    }

    int waiting = 0;
    int response = 0;
    int turnaround = 0;

    for (int i = 0; i < size; i++) {
        waiting += arr[i].WaitingTime;
        response += arr[i].ResponseTime;
        turnaround += arr[i].TuraroundTime;
    }
    cout << endl;
    cout << endl;

    cout << "The average Waiting time is: " << (float) waiting / (float) size << endl;
    cout << "The average Response time is: " << (float) response / (float) size << endl;
    cout << "The average Turnaround time is: " << (float) turnaround / (float) size << endl;


    cout << endl;
    cout << endl;
    cout << "Gant chart" << endl;

    for (int i = 0; i < 3; i++) {
        cout << level1[i];
        cout << endl;
    }

    cout << endl;
    cout << endl;

    for (int i = 0; i < 3; i++) {
        cout << level2[i];
        cout << endl;
    }

    cout << endl;
    cout << endl;

    for (int i = 0; i < 3; i++) {
        cout << level3[i];
        cout << endl;
    }




}