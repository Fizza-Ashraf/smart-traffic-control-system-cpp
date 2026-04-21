#include<iostream>
#include<cstdlib>
#include<ctime>
#include<string>
using namespace std;

//================== DATA STRUCTURES ==================//

// QUEUE (for vehicles waiting)

class Queue{
    private:
    int * arr;
    int front, rear, capacity;

    public:
    Queue(int size){
        capacity=size;
        arr=new int[capacity];
        front=0;
        rear=-1;
    }
    void enqueue(int vehicle){
        if (rear>=capacity-1){
            cout<<"Queue Overflow!"<<endl;
            return;
        }
        rear++;
        arr[rear]=vehicle;
    }
    int dequeue(){
        if (front>rear){
            return -1;
        }
        int vehicle=arr[front];
        front++;
        return vehicle;
    }
    bool isEmpty(){
        return front>rear;
    }
    int getSize(){
        return rear-front+1;
    }
    
    void display(){
        cout<<"Vehicles in Queue:";
        for(int i=front;i<=rear;i++){
            cout<<arr[i]<<" ";
        }
        cout<<endl;

    }
    ~Queue(){
        delete[] arr;
    }
};

// PRIORITY QUEUE (Heap for busiest road)
class PriorityQueue{
    private:
    int * heap;
    int size;
    int * roadIndex;

    void swap(int i,int j){
        int temp=heap[i];
        heap[i]=heap[j];
        heap[j]=temp;
    }

public:
PriorityQueue(int maxSize){
    heap=new int[maxSize];
     roadIndex=new int[maxSize];
    size=0;
}

void push(int road,int density){
    heap[size]=density;
    roadIndex[size]=road;
    int current=size;
    size++;

    while(current>0){
        int parent= (current-1)/2;
        if (heap[current]>heap[parent]){

            int tempheap=heap[current];
            heap[current]=heap[parent];
            heap[parent]=tempheap;

            int tempRoad=roadIndex[current];
            roadIndex[current]=roadIndex[parent];
            roadIndex[parent]=tempRoad;

            current = parent;
        }else{
            break;
        }
    }
}

int pop(){
    if (size==0){
        return -1;
    }
    int maxRoad=roadIndex[0];
    heap[0]=heap[size-1];
    roadIndex[0]=roadIndex[size-1];
    size--;

    int current=0;
    while(true){
        int left=2*current+1;
        int right=2*current+2;
        int largest=current;

        if(left<size && heap[left]>heap[largest]){
            largest=left;
        }
        if(right<size && heap[right]>heap[largest]){
            largest=right;
        }
        
        if(largest !=current){

            int tempheap=heap[current];
            heap[current]=heap[largest];
            heap[largest]=tempheap;

            int tempRoad=roadIndex[current];
            roadIndex[current]=roadIndex[largest];
            roadIndex[largest]=tempRoad;

            current = largest;
        }
        else{
            break;
        }
    }
    return maxRoad;
}

bool isEmpty(){
    return size==0;
}
~PriorityQueue(){
    delete[] heap;
    delete[] roadIndex;
}
};

// LINKED LIST (for simulation logs)
class Linkedlist{
    private:
    struct LogNode{
        string log;
        LogNode* next;
        LogNode(string msg){
            log=msg;
            next=nullptr;
        }

    };
    LogNode * head;

public:
Linkedlist(){
    head=nullptr;
}
void addLog(string message){
    LogNode * newNode=new LogNode(message);
    if (head==nullptr){
        head=newNode;
    }
    else{
        LogNode * temp=head;
        while(temp->next !=nullptr){
            temp=temp->next;
        }
        temp->next=newNode;
     }
    }
void displayLogs(){
    cout<<"\n === SIMULATION LOGS ==="<<endl;
    LogNode * temp=head;
    while(temp!=nullptr){
        cout<<temp->log<<endl;
        temp=temp->next;

    }
}
};

// GRAPH (for intersection layout)
class Graph{
private:
int roads;
int ** matrix;

public:
Graph(int r){
    roads=r;
    matrix=new int *[roads];
    for(int i=0;i< roads;i++){
        matrix[i]=new int[roads];
        for(int j=0;j< roads;j++){
            matrix[i][j]=0; //0 means no connection
    }
}
}
void addConnection(int road1,int road2){
    matrix[road1][road2]=1;
    matrix[road2][road1]=1;  
}

void displayConnections(){
    cout<<"\n=== INTERSECTION GRAPH ==="<<endl;
    for(int i=0;i< roads;i++){
        cout<<"Road "<<i<<" connects to: ";
        for(int j=0;j< roads;j++){
            if (matrix[i][j]==1){
                cout<<j<<" ";
            }
}
        cout<<endl;
}
}
};

// ========MAIN SYSTEM ==============

class TrafficSystem{
    private:
    int roads;
    Queue **vehicleQueues;
    PriorityQueue * priorityQueue;
    Linkedlist * logs;
    Graph * intersection;
    int * vehicleCounts;
    string * densityLevels;

    public:
    TrafficSystem(int r){
        roads=r;
        vehicleQueues=new Queue* [roads];
        for(int i=0;i<roads;i++){
            vehicleQueues[i]=new Queue(100);
        }
       priorityQueue =new PriorityQueue(roads);
       logs=new Linkedlist();
       intersection=new Graph(roads);

       vehicleCounts=new int[roads];
       densityLevels=new string[roads];

       setupIntersection();
       intersection->displayConnections();

    }
    void setupIntersection(){
        //Road 0 connects to 1 and 2
        intersection->addConnection(0,1);
        intersection->addConnection(0,2);

        //Road 1 connects to road 3
        intersection->addConnection(1,3);

        //Road 2 connects to road 3
        intersection->addConnection(2,3);

    }
    void getTrafficInput(){
        int choice;
        cout<<" ===== TRAFFIC INPUT======"<<endl;
        cout<<"1. Manual Input\n";
        cout<<"2. Random Input\n";
        cout<<"Choice: ";
        cin>>choice;

        if (choice==1){
            for(int i=0;i<roads;i++){
                cout<<"Vehicles on Road "<<i<<": ";
                cin>>vehicleCounts[i];
                if(vehicleCounts[i]<0){
                    vehicleCounts[i]=0;
                }    
            }
        }
    else{
    srand(time(0));
    for(int i=0;i<roads;i++){
        vehicleCounts[i]=rand()%40;
        cout<<"Road "<<i<<": "<<vehicleCounts[i]<<" vehicles\n";
    }
}
    }

    void preprocessSensorData(){
         for(int i=0;i<roads;i++){
            // Filling queue with vehicles
            for(int j=0;j< vehicleCounts[i];j++){
                vehicleQueues[i]->enqueue(j+1);
            }
            // Calculate density
            if (vehicleCounts[i]==0){
                densityLevels[i]="None";
            }
            else if (vehicleCounts[i]<=10){
                densityLevels[i]="Low";
            }
            else if (vehicleCounts[i]<=25){
                densityLevels[i]="Medium";
            }
            else{
                densityLevels[i]="High";
            }
            // Add to priority queue (higher density = higher priority)
            priorityQueue->push(i, vehicleCounts[i]);

            //log the data
            logs->addLog("Road " + to_string(i)+ ": " 
            + to_string(vehicleCounts[i]) + " vehicles (" + densityLevels[i] + "density)");
         }

    }

    int getGreenTime(string density){
        if (density=="High"){
            return 40;
        }
        if (density=="Medium"){
            return 25;
        }
        if (density=="Low"){
            return 15;
        }
        return 5;
    }
    void realTimeDecision(){
        cout<<"\n === REAL-TIME DECISION MAKING ==="<<endl;

        // Get busiest road from priority queue
        int selectedRoad=priorityQueue->pop();

        if (selectedRoad==-1){
            cout<<"No traffic detected!"<<endl;
            return;
        }

        int greenTime=getGreenTime(densityLevels[selectedRoad]);

        cout<<"\n SELECTING ROAD "<<selectedRoad<<" FOR GREEN SIGNAL"<<endl;
        cout<<"Reason: Highest traffic density (" <<densityLevels[selectedRoad]<< ")"<<endl;
        cout<<"Green time: "<<greenTime<<" seconds"<<endl;

        logs->addLog("Decision: Road " + to_string(selectedRoad) +
                    " selected for green signal (" + to_string(greenTime) +" seconds)");

            runSimulation(selectedRoad,greenTime);
    }
    void runSimulation(int road,int time){
        cout<<"\n === SIMULATION STARTED ===="<<endl;

        //Display signal states

        for(int i=0;i<roads;i++){
            if (i==road){
                cout<<"Road "<<i<<": GREEN";
                cout<<" | Vehicles passing: ";

                int vehiclesToPass=vehicleQueues[i]->getSize()>5?5: vehicleQueues[i]->getSize();
                for(int j=0;j<vehiclesToPass;j++){
                    int vehicle=vehicleQueues[i]->dequeue();
                    if(vehicle!=-1){
                        cout<<" V"<<vehicle<<" ";
                    }
                }
                cout<<endl;
                logs->addLog("Road "+ to_string(i)+ ": Green signal, "+ 
                to_string(vehiclesToPass) + " vehicles passed");
            }
            else{
                cout<<"Road "<<i<<": RED";
                cout<<" | Waiting: "<<vehicleQueues[i]->getSize()<<" vehicles\n";

                logs->addLog("Road " + to_string(i) +
                        ": Red signal, " +
                        to_string(vehicleQueues[i]->getSize()) + " vehicles waiting");
            }
        }
        cout<<"\n Countdown: ";
        for(int t=time;t>0;t--){
            cout<<t<<" ";

            for(int d=0;d<100000000;d++);
        }
        cout<<"0"<<endl;

        cout<<"===  SIMULATION ENDED==="<<endl;
    }
    void displayCurrentStatus(){
        cout<<"\n === CURRENT TRAFFIC STATUS ===="<<endl;
        for (int i=0;i<roads;i++){
            cout<<"Roads "<<i<<": ";
            cout<<vehicleCounts[i]<<" vehicles";
            cout<<" | Density: "<<densityLevels[i];
            cout<<" | In queue: "<<vehicleQueues[i]->getSize()<<endl;
        }
    }
    void saveToFile(){
        const char * filename ="traffic_log.txt";
         FILE * file=fopen(filename,"w");
         if (!file){
            cout<<"Failed to create "<<filename<<". Check folder permission"<<endl;
            return;
         }
         
            fprintf(file,"=== TRAFFIC SIMULATION REPORT ===\n\n");
            fprintf(file, "Road, Vehicles, Density\n");
            for (int i=0;i<roads;i++)
                fprintf(file,"  %d,   %d,    %s\n",
                i, vehicleCounts[i], densityLevels[i].c_str());
         
         fclose(file);
         cout<<"\nReport saved to 'traffic_log.txt' \n";
    }

};
int main(){
    cout<<" ==== SIMPLE TRAFFIC MANAGEMENT SYSTEM ====\n\n";

    TrafficSystem system(4); // 4roads

    system.getTrafficInput();

    system.preprocessSensorData();

    system.displayCurrentStatus();

    system.realTimeDecision();

    system.saveToFile();

    cout<<"\n=== SYSTEM SHUTDOWN ===\n";
    return 0;
}