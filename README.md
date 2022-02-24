# Algorithm-Visualiser

# Presentation
#### An animation mechanism has been implemented in the project, which relies on a smoother transition of the board colors. The gifs below are recorded in 15 FPS.

## A* algorithm
#### This pathfinding algorithm is very similar to Dijkstra Shortest Path Algorithm. Major difference is that it uses *heuristic* value in order to determine wheter the path is profitable to choose. In this implementation the heuristics are expressed by segment length in the coordinate system. The main action is based on *priority_queue* from C++ standard library. Cell with the smallest cost to reach is chosen each time.
![aStarGifShort](https://user-images.githubusercontent.com/81765291/155477595-a689355a-b9a8-45f7-a044-6f2a2d7a1036.gif)
## Sudoku Solver
#### Algorithm to solve sudoku is obtained by using the recursive, backtrack algorithm strategy. The operation is based on inserting consecutive numbers until the correct result is obtained. In case of wrong selection of numbers, the algorithm will go back to the previous function call and try to use a different value.
#### Generating the sudoku board consists in initially filling it in the correct way, and then deleting some number of filled fields. 
![sudokuShort](https://user-images.githubusercontent.com/81765291/155480214-d4bd7186-6110-477d-bced-cce3752b9163.gif)
## Dijkstra Shortest Path algorithm
#### Contrary to the algorithm A*, each square on the map has a different cost and there is no heuristic value. It's also implemented using *priority_queue*. Cell with the smallest cost to reach is chosen each time.
![dijkstraShort](https://user-images.githubusercontent.com/81765291/155478263-5431fb35-af86-4460-8b91-d970cccf5240.gif)
## Depth-First Search algorithm
#### One of the graph searching algorithms. An in-depth search examines all edges that originate from the given vertex. After examining all edges coming from a given vertex, the algorithm returns to the vertex from which the given vertex was visited. 
![dfsShort](https://user-images.githubusercontent.com/81765291/155478766-0a76330e-8263-4d79-96d5-c350967143bb.gif)
## Breadth-First Search algorithm
#### Also a graph searching algorithm. The graph traverses from a given vertex s and consists in visiting all the vertices reachable from it. The result of the algorithm is a search tree rooted in s, containing all the vertices reachable from s. Each of these vertices leads exactly one path from s, which is also the shortest path in the input graph. 
![bfsShort](https://user-images.githubusercontent.com/81765291/155479307-61da25df-8f0d-42c8-a387-b18d81118497.gif)
## Rat in a maze problem
#### Pathfinding problem where it is only possible to move one cell down (row + 1, col) or one cell right (row, col + 1), source is the top left corner of the map and destination is the bottom right corner of the map. The intuitive solution is to use backtracking strategy and check wheter the given path provides correct solution. In case of the wrong path, algorithm will go back to the previous cells and try a different way.
![rimShort](https://user-images.githubusercontent.com/81765291/155480557-67accaab-2cd0-45b7-90cd-5c190f52a433.gif)
## Knight's tour problem
#### The problem is to find a sequence of the knight's moves on the chessboard such that the knight visits every square exactly once. The implemented solution checks whether the chosen move of the knight will lead to the correct solution. In case of wrong sequence, algorithm will mark current square as unvisited and then come back to previous squares.
![ktpFirst](https://user-images.githubusercontent.com/81765291/155481632-e9135639-d9d1-44dd-92fd-92796d683b17.gif)
#### It takes time for this algorithm to finish, so I decided to speed up the program in order to show that it actually finds a correct solution (although it did cause a lag :D).
![ktpSec](https://user-images.githubusercontent.com/81765291/155482101-2f98a47c-2513-4b33-8431-f055fc03f758.gif)
