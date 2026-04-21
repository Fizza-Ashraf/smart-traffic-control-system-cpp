# Smart Traffic Light Control System (C++)

## 📌 Project Overview
This project is a smart traffic light control system developed in C++ using data structures. It simulates real-time traffic management using a sensor-based approach to detect vehicle density on multiple lanes and dynamically adjust traffic signals to improve flow efficiency.

---

## 🚦 Features
- Simulates vehicle detection using sensor input
- Classifies traffic density into High, Medium, Low, and None
- Dynamically prioritizes lanes based on traffic load
- Allocates green signal time according to density
- Continuously updates signal cycles in real-time simulation
- Implements priority-based decision-making logic

---

## 🛠️ Technologies Used
- C++
- Data Structures (Arrays / Queues / Priority Logic)
- Algorithm Design

---

## ⚙️ How It Works
1. Sensors detect the number of vehicles on each lane  
2. The system calculates traffic density for each lane  
3. The lane with highest density is given priority  
4. Green signal duration is adjusted based on traffic load  
5. Remaining lanes are scheduled in descending priority  
6. The cycle repeats continuously for real-time adaptation  

---

## ▶️ How to Run
```bash
# Compile the program
g++ main.cpp -o traffic

# Run the program
./traffic
