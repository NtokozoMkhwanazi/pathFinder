#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

using namespace std;

class Node {
	public:
    		pair<int, int> pos;
    		Node* next;

    		Node(pair<int, int> position) : pos(position), next(nullptr) {}
};

class Snake {
	public:
    		Node* head;
    		vector<vector<char>> board;
    		pair<int, int> goal;

    		Snake(vector<pair<int, int>> initialSnake, int rows, int cols, pair<int, int> goalPos) : head(nullptr), board(rows, vector<char>(cols, ' ')), goal(goalPos) {
        		for (auto& segment : initialSnake) {
            			addSegment(segment);
        	}
        	placeSnakeOnBoard();
    }

	void addSegment(pair<int, int> pos) {
        	Node* newNode = new Node(pos);
        	if (!head) {
            		head = newNode;
        	} else {
            		Node* current = head;
            	while (current->next) {
                	current = current->next;
            	}
            		current->next = newNode;
        	}
    	}

    	void placeSnakeOnBoard() {
        	for (int j = 0; j < board.size(); j++) {
        	    for (int i = 0; i < board[j].size(); i++) {
        	        if (board[j][i] != '#' && board[j][i] != '*') {
        	            board[j][i] = ' ';
        	        }
        	    }
        }

        Node* current = head;
        while (current) {
            if (board[current->pos.first][current->pos.second] != '#' && board[current->pos.first][current->pos.second] != '*')
                board[current->pos.first][current->pos.second] = 'X';
            current = current->next;
        }
        board[goal.first][goal.second] = 'O';
    }

    	void moveSnake(pair<int, int> newHeadPos) {
        	Node* current = head;
        	pair<int, int> prevPos = current->pos;

        	current->pos = newHeadPos;
        	current = current->next;

        	while (current) {
        	    pair<int, int> temp = current->pos;
        	    current->pos = prevPos;
        	    prevPos = temp;
        	    current = current->next;
        	}

        	placeSnakeOnBoard();
    	}

    	bool isGoalReached() {
        	return head->pos == goal;
    	}

    	vector<pair<int, int>> generateMoves(pair<int, int> headPos) const {
        
        	vector<pair<int, int>> moves;
        	vector<vector<int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        	for (auto& dir : directions) {
        	    int newRow = headPos.first + dir[0];
        	    int newCol = headPos.second + dir[1];

        	    if (newRow >= 0 && newRow < board.size() &&
        	        newCol >= 0 && newCol < board[0].size() &&
        	        board[newRow][newCol] != '#' && board[newRow][newCol] != 'X' && board[newRow][newCol] != '*') {
        	        moves.push_back({newRow, newCol});
        	    }
        	}

        	return moves;
    	}

	void updateDynamicObstacles() {
	
	        vector<pair<int, int>> newPositions;
	        for (int i = 0; i < board.size(); i++) {
	            for (int j = 0; j < board[0].size(); j++) {
	                if (board[i][j] == '*') {
	                    board[i][j] = ' ';
	                    vector<pair<int, int>> moves = generateMoves({i, j});
	                    if (!moves.empty()) {
	                        newPositions.push_back(moves[rand() % moves.size()]);
	                    }
	                }
	            }
	        }
	        for (auto& pos : newPositions) {
	            board[pos.first][pos.second] = '*';  // Place new dynamic obstacles
	        }
    	
    	}

    int heuristic(pair<int, int> a, pair<int, int> b) const { //Manhattan distance
        return abs(a.first - b.first) + abs(a.second - b.second);
    }

    vector<pair<int, int>> astar() {
        priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<>> pq;
        map<pair<int, int>, pair<int, int>> parent;
        map<pair<int, int>, int> gCost;
        set<pair<int, int>> visited;

        pq.push({0, head->pos});
        gCost[head->pos] = 0;
        parent[head->pos] = head->pos;

        while (!pq.empty()) {
            auto [currentCost, currentPos] = pq.top();
            pq.pop();

            if (currentPos == goal) {
                vector<pair<int, int>> path;
                while (currentPos != head->pos) {
                    path.push_back(currentPos);
                    currentPos = parent[currentPos];
                }
                reverse(path.begin(), path.end());
                return path;
            }

            if (visited.find(currentPos) != visited.end()) continue;
            visited.insert(currentPos);

            for (auto& move : generateMoves(currentPos)) {
                int tentative_gCost = gCost[currentPos] + 1;
                if (gCost.find(move) == gCost.end() || tentative_gCost < gCost[move]) {
                    gCost[move] = tentative_gCost;
                    int fCost = tentative_gCost + heuristic(move, goal);
                    pq.push({fCost, move});
                    parent[move] = currentPos;
                }
            }
        }

        return {}; // No path found
    }
};

vector<vector<char>> generateRandomMaze(int rows, int cols, float obstacleProbability, float dynamicObstacleProbability) {
    vector<vector<char>> maze(rows, vector<char>(cols, ' '));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (rand() % 100 < obstacleProbability * 100) {
                maze[i][j] = '#';  // Wall
            }
            if (rand() % 100 < dynamicObstacleProbability * 100) {
                maze[i][j] = '*';  // Dynamic obstacle
            }
        }
    }
    maze[0][0] = ' ';  // Ensure the start position is clear
    maze[rows - 1][cols - 1] = ' ';  // Ensure the goal position is clear
    return maze;
}

pair<int, int> generateRandomGoal(int rows, int cols, const vector<vector<char>>& maze) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> disRow(0, rows - 1);
    uniform_int_distribution<> disCol(0, cols - 1);

    pair<int, int> goal;
    do {
        goal = {disRow(gen), disCol(gen)};
    } while (maze[goal.first][goal.second] == '#');

    return goal;
}

int main() {
    const int rows = 20, cols = 20;
    vector<pair<int, int>> initialSnake = {{2, 0}, {1, 0}, {1, 1}, {1, 2}};

    float obstacleProbability = 0.2;
    float dynamicObstacleProbability = 0.2;

    vector<vector<char>> randomBoard = generateRandomMaze(rows, cols, obstacleProbability, dynamicObstacleProbability);

    pair<int, int> goalPos = generateRandomGoal(rows, cols, randomBoard);

    Snake snake(initialSnake, rows, cols, goalPos);
    snake.board = randomBoard;

    sf::RenderWindow window(sf::VideoMode(450, 450), "Snake Pathfinding with A* Search");
    window.setFramerateLimit(60);

    sf::RectangleShape cell(sf::Vector2f(window.getSize().x / cols, window.getSize().y / rows));
    cell.setOutlineColor(sf::Color::Black);
    cell.setOutlineThickness(1);

    bool goalReached = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (!goalReached) {
            snake.updateDynamicObstacles();
            vector<pair<int, int>> path = snake.astar();

            if (!path.empty()) {
                snake.moveSnake(path[0]);

                window.clear(sf::Color::White);

                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        cell.setPosition(j * cell.getSize().x, i * cell.getSize().y);

                        if (snake.board[i][j] == 'X') {
                            if (snake.head->pos == make_pair(i, j)) {
                                cell.setFillColor(sf::Color::Blue); // Head color
                            } else {
                                cell.setFillColor(sf::Color::Green); // Body color
                            }
                        } else if (snake.board[i][j] == '*') {
                            cell.setFillColor(sf::Color::Yellow);  // Dynamic obstacles
                        } else if (snake.board[i][j] == 'O') {
                            cell.setFillColor(sf::Color::Red); // Goal color
                        } else if (snake.board[i][j] == '#') {
                            cell.setFillColor(sf::Color::Black); // Wall Obstacle color
                        } else {
                            cell.setFillColor(sf::Color::White); // Empty space
                        }

                        window.draw(cell);
                    }
                }

                window.display();

                if (snake.isGoalReached()) {
                    cout << "Goal Reached!" << endl;
                    goalReached = true;

                    // Reset the game with a new random board and goal
                    randomBoard = generateRandomMaze(rows, cols, obstacleProbability, dynamicObstacleProbability);
                    goalPos = generateRandomGoal(rows, cols, randomBoard);
                    snake = Snake(initialSnake, rows, cols, goalPos);
                    snake.board = randomBoard;
                    goalReached = false;
                }
            }
        }
    }

    return 0;
}
