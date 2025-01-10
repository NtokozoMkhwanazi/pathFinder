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

    Snake(vector<pair<int, int>> initialSnake, int rows, int cols, pair<int, int> goalPos)
        : head(nullptr), board(rows, vector<char>(cols, ' ')), goal(goalPos) {
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
    
        for (int j =0; j < board.size(); j++) {
        	for(int i =0;i < board[j].size();i++){
           		if(board[j][i] != '#'){
            			board[j][i] = ' ';
        		}
        	}
        }

        Node* current = head;
        while (current) {
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
                board[newRow][newCol] != '#' && board[newRow][newCol] != 'X') {
                moves.push_back({newRow, newCol});
            }
        }

        return moves;
    }

    vector<pair<int, int>> bfs() {
        queue<pair<int, int>> q;
        map<pair<int, int>, pair<int, int>> parent;
        set<pair<int, int>> visited;

        q.push(head->pos);
        visited.insert(head->pos);

        while (!q.empty()) {
            pair<int, int> curr = q.front();
            q.pop();

            if (curr == goal) {
                vector<pair<int, int>> path;
                while (curr != head->pos) {
                    path.push_back(curr);
                    curr = parent[curr];
                }
                reverse(path.begin(), path.end());
                return path;
            }

            for (auto& move : generateMoves(curr)) {
                if (visited.find(move) == visited.end()) {
                    visited.insert(move);
                    q.push(move);
                    parent[move] = curr;
                }
            }
        }

        return {}; // No path found
    }
};

vector<vector<char>> generateRandomMaze(int rows, int cols, double obstacleProbability) {
    
    vector<vector<char>> maze(rows, vector<char>(cols, ' '));
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (dis(gen) < obstacleProbability) {
                maze[i][j] = '#'; // Create obstacle
            }
        }
    }

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
    const int rows = 7, cols = 7;
    vector<pair<int, int>> initialSnake = {{2, 0}, {1, 0}, {1, 1}, {1, 2}};

    double obstacleProbability = 0.04;
    vector<vector<char>> randomBoard = generateRandomMaze(rows, cols, obstacleProbability);

    pair<int, int> goalPos = generateRandomGoal(rows, cols, randomBoard);

    Snake snake(initialSnake, rows, cols, goalPos);
    snake.board = randomBoard;

    sf::RenderWindow window(sf::VideoMode(450, 450), "Snake Pathfinding with BFS");
    window.setFramerateLimit(10);

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
            vector<pair<int, int>> path = snake.bfs();

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
                        } else if (snake.board[i][j] == 'O') {
                            cell.setFillColor(sf::Color::Red); // Goal color
                        } else if (snake.board[i][j] == '#') {
                            cell.setFillColor(sf::Color::Black); // Obstacle color
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
                    randomBoard = generateRandomMaze(rows, cols, obstacleProbability);
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
