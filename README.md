# pathFinder

**Pathfinding Algorithms & Maze Solvers in C++ with SFML**

`pathFinder` is a visual, interactive C++ project that demonstrates multiple **pathfinding algorithms** solving **mazes** using the **SFML** (Simple and Fast Multimedia Library).  
It includes agent-based exploration, maze generation, goal placement (random and fixed), and algorithm comparisons.

---

## 🧠 Project Summary

This repository contains:

- **Maze generation** with both random and preset goal positions
- **Agents** that navigate the maze
- Multiple **pathfinding algorithms**, including:
  - Breadth-First Search (BFS)
  - Depth-First Search (DFS)
  - A* Search
- **Benchmarking** between algorithms to compare performance visually
- A basic **Snake agent** demonstrating Algorithm-based movement logic

The project is intended for learning algorithm behavior, grid navigation and optimization, real-time visualization techniques in C/C++.


<img width="1366" height="768" alt="SNAKE" src="https://github.com/user-attachments/assets/24e0bff8-7646-4146-bafe-5dd6870c77e3" />
<img width="1366" height="768" alt="snake" src="https://github.com/user-attachments/assets/539034f4-7809-4d11-a125-3dbc49a825ee" />
<img width="1366" height="768" alt="agent" src="https://github.com/user-attachments/assets/cdd65b6b-1ae1-4d82-8f24-074a3baa70f7" />

---

## 🧩 Features

### 🌀 Maze & Goal
- Random maze environments with obstacles at random and fixed positions 
- Goal randomized — other times fixed according to variation
- Several maze configurations for testing different scenarios

### 🤖 Agents & Algorithms
- Player or AI agents that explore and solve the maze
- Visual representation of search progress
- Multiple algorithms:
  - **BFS** — explores breadth first
  - **DFS** — explores depth first
  - **A\*** — shortest path using heuristics

### 🏁 Algorithm Races
- Watch two or more algorithms run side-by-side
- Compare speed and efficiency in solving the same maze

---

## 🛠️ Technologies

- **C++17**
- **SFML** for graphics and input
- Standard STL and custom utilities

---

## 🚀 Getting Started

### Prerequisites

Before building:

- A **C++17 compatible compiler**
- **SFML** installed (Graphics, Window, System modules)

Download SFML:  
https://www.sfml-dev.org/

---

### Build Instructions

1. Clone the repository:

   ```bash
   git clone https://github.com/NtokozoMkhwanazi/pathFinder.git
   cd pathFinder
g++ -std=c++17 *.cpp -lsfml-graphics -lsfml-window -lsfml-system -o run

run: ./run

