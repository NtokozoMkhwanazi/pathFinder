#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

using namespace std;

class node {
public:
    vector<vector<char>> board;
    node *parent;
    string move;
    bool visited;
    int depth;
   
    int costMove;
    int g;
    int h;
    int f;

    node() : parent(nullptr), visited(false) {} //constructor
};

// Function to create a new node
node* createNode(const vector<vector<char>>& board) {
    node* newNode = new node;
    newNode->board = board;
    return newNode;
}

// Function to check if the current board is the goal state
bool isGoalState(const vector<vector<char>>& board, const vector<vector<char>>& goalBoard) {
    return board == goalBoard;
}

// Function to find the position of the agent ('X')
pair<int, int> findAgent(const vector<vector<char>>& board) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (board[i][j] == 'X') {
                return {i, j};
            }
        }
    }
    return {-1, -1}; // Agent not found (should not happen)
}

// Function to generate all valid moves from the current state
vector<node*> generateMoves(const vector<vector<char>>& board, node* currentNode) {
    pair<int, int> agentPos = findAgent(board);
    int XRow = agentPos.first;
    int XCol = agentPos.second;

    vector<node*> moves;
    vector<vector<int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    vector<string> moveNames = {"UP", "DOWN", "LEFT", "RIGHT"};

    for (int i = 0; i < 4; i++) {
        int newRow = XRow + directions[i][0];
        int newCol = XCol + directions[i][1];

        // Check bounds and obstacles
        if (newRow >= 0 && newRow < board.size() && newCol >= 0 && newCol < board[0].size() && board[newRow][newCol] != '#') {
            vector<vector<char>> newBoard = board;
            swap(newBoard[XRow][XCol], newBoard[newRow][newCol]);

            node* newNode = createNode(newBoard);
            newNode->move = moveNames[i];
            newNode->parent = currentNode;
            moves.push_back(newNode);
        }
    }

    return moves;
}

node *dfs(vector<vector<char>> &startBoard, vector<vector<char>> &goalBoard)
{
    node* start = createNode(startBoard);

    stack<node *> stack;
    stack.push(start);

    while (!stack.empty())
    {
        node *curr = stack.top();
        stack.pop();

        if (isGoalState(curr->board, goalBoard))
        {
            return curr;
        }

        curr->visited = true;

        vector<node *> moves = generateMoves(curr->board, curr);
        for (node *next : moves)
        {
            if (next != nullptr && !next->visited)
            {
                next->parent = curr;
                next->g = curr->g + next->costMove; // Update g value
                next->depth = curr->depth + 1;
                stack.push(next);
            }
            else
            {
                delete next;
            }
        }
    }

    return nullptr;
}
// Function to generate a random maze
vector<vector<char>> generateRandomMaze(int rows, int cols, double obstacleProbability) {
    vector<vector<char>> maze(rows, vector<char>(cols, ' '));

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (dis(gen) < obstacleProbability) {
                maze[i][j] = '#';
            }
        }
    }

    // Ensure start and goal positions are not blocked
    maze[0][0] = 'X';
    maze[rows - 1][cols - 1] = ' ';

    return maze;
}

// Function to render the board using SFML
void renderBoard(sf::RenderWindow& window, const vector<vector<char>>& board, int cellSize, const vector<sf::Vertex>& path) {
    window.clear(sf::Color::White);

    sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));

    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (board[i][j] == '#') {
                cell.setFillColor(sf::Color::Black);
            } else if (board[i][j] == 'X') {
                cell.setFillColor(sf::Color::Red);
            } else {
                cell.setFillColor(sf::Color::White);
            }

            if (i == 0 && j == 0) {
                cell.setFillColor(sf::Color::Green); // Start position
            } else if (i == board.size() - 1 && j == board[i].size() - 1) {
                cell.setFillColor(sf::Color::Blue); // Goal position
            }

            cell.setPosition(j * cellSize, i * cellSize);
            cell.setOutlineColor(sf::Color::Blue);
            cell.setOutlineThickness(-1);
            window.draw(cell);
        }
    }

    // Draw the path
    if (!path.empty()) {
        window.draw(&path[0], path.size(), sf::LinesStrip);
    }

    window.display();
}

int main() {
    // Define the board size and initialize the start and goal boards
    int rows = 5, cols = 5;
    int cellSize = 20; // Adjust cell size for better fit

    // Create an SFML window
    sf::RenderWindow window(sf::VideoMode(cols * cellSize, rows * cellSize), "Maze Solver");

    // Generate random maze
    vector<vector<char>> board = generateRandomMaze(rows, cols, 0.3);
    vector<vector<char>> goalBoard = board;
    goalBoard[rows - 1][cols - 1] = 'X';
    goalBoard[0][0] = ' ';

    // Print the maze for debugging
    cout << "Generated Maze:" << endl;
    for (const auto& row : board) {
        for (char cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }

	
    // Run BFS to find the solution
    node* result = dfs(board, goalBoard);
    if (result) {
        cout << "Goal state reached!" << endl;

        // Trace the path to the goal
        vector<node*> pathNodes;
        while (result != nullptr) {
            pathNodes.push_back(result);
            result = result->parent;
        }

        reverse(pathNodes.begin(), pathNodes.end());

        // Create the path line
        vector<sf::Vertex> pathLine;
        for (node* step : pathNodes) {
            pair<int, int> agentPos = findAgent(step->board);
            pathLine.emplace_back(sf::Vector2f(agentPos.second * cellSize + cellSize / 2.0f, 
                                               agentPos.first * cellSize + cellSize / 2.0f), sf::Color::Cyan);
        }

        // Display the solving process
        for (node* step : pathNodes) {
            renderBoard(window, step->board, cellSize, pathLine);
            sf::sleep(sf::seconds(0.5)); // Adjust time for slower walking pace

            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    break;
                }
            }
        }

        // Keep the final state visible until the window is closed
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }
        }
    } else {
        cout << "No solution found." << endl;
    }

    return 0;
}
