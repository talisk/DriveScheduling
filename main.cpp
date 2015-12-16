#include <iostream>
#include <deque>

using namespace std;

int currentPhysicalAddress; // 当前物理地址
bool forwardAccess;             // 访问方向, true正, false反

typedef struct Process {
    string processName;
    int cylinder; // 柱面号 0-199
    int track;    // 磁道号 20片盘 0-19
    int physicalRecord; // 物理记录号 8个 0-7
    int physicalAddress;
}Process;

bool lessmark(const Process& s1,const Process& s2) {
    return s1.physicalAddress < s2.physicalAddress;
}
bool greatermark(const Process& s1,const Process& s2) {
    return s1.physicalAddress > s2.physicalAddress;
}

void receiveRequest(deque<Process> &waitingQueue, deque<Process> &schedulingQueue) {
    cout<<"Need a new process? Y/N --> ";
    string selection;
    cin>>selection;

    if (selection=="Y" || selection=="y") {

        Process process;
        cout<<"Process Name: "; cin>>process.processName;
        cout<<"Cylinder: "; cin>>process.cylinder;
        cout<<"Track: "; cin>>process.track;
        cout<<"Physical record: "; cin>>process.physicalRecord;

        process.physicalAddress = 0
                                  + (process.physicalRecord) * 1
                                  + (process.track) * 8
                                  + (process.cylinder) * 20 * 8;

        if (forwardAccess) {      // 正向访问

            if (process.physicalAddress >= currentPhysicalAddress) {

                schedulingQueue.push_back(process);
                sort(schedulingQueue.begin(), schedulingQueue.end(), lessmark);

            } else {

                waitingQueue.push_back(process);
                sort(waitingQueue.begin(), waitingQueue.end(), greatermark);

            }

        } else {

            if (process.physicalAddress <= currentPhysicalAddress) {

                schedulingQueue.push_back(process);
                sort(schedulingQueue.begin(), schedulingQueue.end(), greatermark);

            } else {

                waitingQueue.push_back(process);
                sort(waitingQueue.begin(), waitingQueue.end(), lessmark);

            }
        }
    }
    return;
}

void printRequestQueue(deque<Process> &waitingQueue, deque<Process> &schedulingQueue) {
    deque<Process>::iterator it;

    cout<<"scheduling:"<<endl;

    for (it = schedulingQueue.begin(); it != schedulingQueue.end(); it++) {
        cout<<"--------------------------"<<endl;
        cout<<(*it).processName<<endl<<(*it).cylinder<<endl<<(*it).track<<endl<<(*it).physicalRecord<<endl<<(*it).physicalAddress<<endl<<endl<<endl;
    }

    cout<<"waiting:"<<endl;

    for (it = waitingQueue.begin(); it != waitingQueue.end(); it++) {
        cout<<"--------------------------"<<endl;
        cout<<(*it).processName<<endl<<(*it).cylinder<<endl<<(*it).track<<endl<<(*it).physicalRecord<<endl<<(*it).physicalAddress<<endl<<endl<<endl;
    }
}

void driveSchedule(deque<Process> &waitingQueue, deque<Process> &schedulingQueue) {

    if (forwardAccess) {
        if (schedulingQueue.empty()) {
            forwardAccess = !forwardAccess;

            sort(schedulingQueue.begin(), schedulingQueue.end(), greatermark);
            schedulingQueue = waitingQueue;
            waitingQueue.clear();
        }
        currentPhysicalAddress = schedulingQueue.front().physicalAddress;
        schedulingQueue.pop_front();
    } else {
        if (schedulingQueue.empty()) {
            forwardAccess = !forwardAccess;

            sort(schedulingQueue.begin(), schedulingQueue.end(), lessmark);
            schedulingQueue = waitingQueue;
            waitingQueue.clear();
        }
        currentPhysicalAddress = schedulingQueue.back().physicalAddress;
        schedulingQueue.pop_back();
    }
}

int main() {

    forwardAccess = true;
    currentPhysicalAddress = 600;

    deque<Process> waitingQueue, schedulingQueue;

    for (int i = 0; i < 5; ++i) {
        receiveRequest(waitingQueue, schedulingQueue);
        printRequestQueue(waitingQueue, schedulingQueue);
    }

    while (1) {
        driveSchedule(waitingQueue, schedulingQueue);
        receiveRequest(waitingQueue, schedulingQueue);
        printRequestQueue(waitingQueue, schedulingQueue);
    }

}