# Algorithm Visualiser in C++
### Algorithm visualiser created using C++14 and wxWidgets 3.1.5.

![image](https://user-images.githubusercontent.com/81765291/155283868-c2dc4094-0b2c-4fa6-a880-a13f7fb93b60.png)

# Contents
### The goal of this project was to visualize how pathfinding and backtracking algorithms work. 
Algorithms implemented at the moment are:
- **Dijkstra Shortest Path algorithm**
- **Depth-First Search algorithm**
- **Breadth-First Search algorithm**
- **A-Star algorithm**
- **Sudoku Solver**
- **Rat in a maze problem**
- **Knight's tour problem**

# How it works
###  To completely separate the direct operations of algorithms on GUI elements, the operation is based on the communication of the executing thread with the thread responsible for the GUI. Such communication is of course provided by the wxWidgets library, which was used for the visual part of the project.
### More information on inter-thread communication in wxWidgets can be found there: 
### [Inter-Thread and Inter-Process communication](https://wiki.wxwidgets.org/Inter-Thread_and_Inter-Process_communication)

# Time complexity
- **Dijkstra Shortest Path algorithm:** ![lagrida_latex_editor](https://user-images.githubusercontent.com/81765291/155402900-88ccfb7d-a4be-470a-97cc-d41b30c98648.png)
- **Depth-First Search algorithm:** ![lagrida_latex_editor (1)](https://user-images.githubusercontent.com/81765291/155403391-be2e0f48-19c8-40fc-8b3a-1c9ad7a2c196.png)
- **Breadth-First Search algorithm:** ![lagrida_latex_editor (1)](https://user-images.githubusercontent.com/81765291/155403396-c463b6a5-39c7-4d83-b7c5-3559e5abdf21.png)
- **A-Star algorithm:** ![lagrida_latex_editor (2)](https://user-images.githubusercontent.com/81765291/155403619-823355ee-056c-40ff-84b4-9c3953bbfe55.png)
- **Sudoku Solver:** ![lagrida_latex_editor (3)](https://user-images.githubusercontent.com/81765291/155403772-cbd4c29d-5004-4036-a88f-cd3022e486e5.png)
- **Rat in a maze problem:** ![lagrida_latex_editor (4)](https://user-images.githubusercontent.com/81765291/155403873-d6c758ff-81d2-497d-aed6-faa6de4aeba7.png)
- **Knight's tour problem:** ![lagrida_latex_editor (5)](https://user-images.githubusercontent.com/81765291/155404008-1683cd4c-937e-4986-b15a-90bfefe37911.png)


# Presentation

## A* algorithm
#### This pathfinding algorithm is very similar to Dijkstra Shortest Path Algorithm. Major difference is that it uses *heuristic* value in order to determine wheter the path is profitable to choose. In this implementation the heuristics are expressed by segment length in the coordinate system. The main action is based on *priority_queue* from C++ standard library. Cell with the smallest cost to reach is chosen each time.

## Sudoku Solver
#### Algorithm to solve sudoku is obtained by using the recursive, backtrack algorithm strategy. The operation is based on inserting consecutive numbers until the correct result is obtained. In case of wrong selection of numbers, the algorithm will go back to the previous function call and try to use a different value.
#### Generating the sudoku board consists in initially filling it in the correct way, and then deleting some number of filled fields. 

## Dijkstra Shortest Path algorithm
#### Contrary to the algorithm A*, each square on the map has a different cost and there is no heuristic value. It's also implemented using *priority_queue*. Cell with the smallest cost to reach is chosen each time.

## Depth-First Search algorithm
#### One of the graph searching algorithms. An in-depth search examines all edges that originate from the given vertex. After examining all edges coming from a given vertex, the algorithm returns to the vertex from which the given vertex was visited. 

## Breadth-First Search algorithm
#### Also a graph searching algorithm. The graph traverses from a given vertex s and consists in visiting all the vertices reachable from it. The result of the algorithm is a search tree rooted in s, containing all the vertices reachable from s. Each of these vertices leads exactly one path from s, which is also the shortest path in the input graph. 

## Rat in a maze problem
#### Pathfinding problem where it is only possible to move one cell down (row + 1, col) or one cell right (row, col + 1), source is the top left corner of the map and destination is the bottom right corner of the map. The intuitive solution is to use backtracking strategy and check wheter the given path provides correct solution. In case of the wrong path, algorithm will go back to the previous cells and try a different way.


## Knight's tour problem
#### The problem is to find a sequence of the knight's moves on the chessboard such that the knight visits every square exactly once. The implemented solution checks whether the chosen move of the knight will lead to the correct solution. In case of wrong sequence, algorithm will mark current square as unvisited and then come back to previous squares.
