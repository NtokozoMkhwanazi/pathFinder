#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

using namespace std;

class node {
public:
    vector<vector<char>> board;
    node *parent;
    string move;
    bool visited;
    int costMove;
    int depth;
    int g;
    int h;
    int f;

    node() : parent(nullptr), visited(false) {} //constructor
};
struct CompareNodes
{
    bool operator()(const node *node1, const node *node2) const
    {

        // Compare nodes based on their f values
        // Smaller f value has higher priority

        return node1->f > node2->f;
    }
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
int Manhattan_start_distance(node* currboard, node* startboard)
{
    
    pair<int, int> start_xy = findAgent(startboard->board);
    pair<int, int> current_xy = findAgent(currboard->board);

    int start_i = start_xy.first;
    int start_j = start_xy.second;

    int i = current_xy.first;
    int j = current_xy.second;

    return abs(i - start_i) + abs(j - start_j);
}

node* bfs(const vector<vector<char>>& startBoard, const vector<vector<char>>& goalBoard) {
    node* start = createNode(startBoard);
    queue<node*> queue;
    queue.push(start);

    while (!queue.empty()) {
        node* curr = queue.front();
        queue.pop();

        if (isGoalState(curr->board, goalBoard)) {
            return curr; // Goal found
        }

        curr->visited = true;
        vector<node*> moves = generateMoves(curr->board, curr);

        for (node* next : moves) {
            if (next && !next->visited) {
                queue.push(next);
            } else if (next) {
                delete next; // Free unused nodes
            }
        }
    }

    return nullptr; // No solution found
}

node *AStarSearch(vector<vector<char>> &startBoard, vector<vector<char>> &goalBoard)
{

    node* startNode = createNode(startBoard);
    node* goalNode = createNode(goalBoard);
    priority_queue<node *, vector<node *>, CompareNodes> openList;
    map<vector<vector<char>>, bool> visited;

   

    startNode->h = Manhattan_start_distance(startNode,goalNode);
    startNode->f = startNode->g + startNode->h;
    startNode->depth = 0;

    openList.push(startNode);
    node *solutionNode = nullptr;

    while (!openList.empty())
    {
        node *currentNode = openList.top();
        openList.pop();

        if (isGoalState(currentNode->board, goalBoard))
        {
            solutionNode = currentNode;
            break;
        }

        visited[currentNode->board] = true;

        for (node *nextMove : generateMoves(currentNode->board, currentNode))
        {
            nextMove->g = currentNode->g + 1;
            nextMove->h = Manhattan_start_distance(nextMove, goalNode);
            nextMove->f = nextMove->g + nextMove->h;

            if (visited.find(nextMove->board) == visited.end())
            {
                nextMove->parent = currentNode;
                openList.push(nextMove);
            }
            else
            {
                delete nextMove;
            }
        }
    }

    return solutionNode;
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
    maze[1][0] = 'X';
    maze[rows - 1][cols - 2] = ' ';

    return maze;
}

// Function to render the board using SFML
void renderBoard(sf::RenderWindow& window, const vector<vector<char>>& board, int cellSize, const vector<sf::Vertex>& path, const vector<sf::Vertex>& path2) {
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

            if (i == 1 && j == 0) {
                cell.setFillColor(sf::Color::Green); // Start position
            } else if (i == board.size() - 1 && j == board[i].size() - 2) {
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
        window.draw(&path2[0], path2.size(), sf::LinesStrip);
    }

    window.display();
}

int main() {
    
    // Define the board size and initialize the start and goal boards
    int rows = 15, cols =14;
    int cellSize = 40; // Adjust cell size for better fit

    // Create an SFML window
    sf::RenderWindow window(sf::VideoMode(cols * cellSize, rows * cellSize), "Maze Solver");

    // Generate random maze
    vector<vector<char>> board = generateRandomMaze(rows, cols, 0.3);
    vector<vector<char>> goalBoard = board;
    goalBoard[rows - 1][cols - 2] = 'X';
    goalBoard[1][0] = ' ';

    // Print the maze for debugging
    cout << "Generated Maze:" << endl;
    for (const auto& row : board) {
        for (char cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }

    // Run BFS to find the solution
    node* result = AStarSearch(board, goalBoard);
    node* result2 = bfs(board,goalBoard);
    if (result && result2) {
        cout << "Goal state reached!" << endl;

        // Trace the path to the goal
        vector<node*> pathNodes;
        vector<node*> pathNodes2;
        while (result != nullptr and result2 != nullptr) {
            pathNodes.push_back(result);
            pathNodes2.push_back(result2);
            result = result->parent;
            result2 = result2->parent;
        }

        reverse(pathNodes.begin(), pathNodes.end());
	reverse(pathNodes2.begin(),pathNodes2.end());
        // Create the path line
        vector<sf::Vertex> pathLine;
        for (node* step : pathNodes) {
            pair<int, int> agentPos = findAgent(step->board);
            pathLine.emplace_back(sf::Vector2f(agentPos.second * cellSize + cellSize / 2.0f, 
                                               agentPos.first * cellSize + cellSize / 2.0f), sf::Color::Black);
        }
        
         vector<sf::Vertex> pathLine2;
        for (node* step : pathNodes2) {
            pair<int, int> agentPos = findAgent(step->board);
            pathLine2.emplace_back(sf::Vector2f(agentPos.second * cellSize + cellSize / 2.0f, 
                                               agentPos.first * cellSize + cellSize / 2.0f), sf::Color::Cyan);
        }
        
        
        

        // Display the solving process
        vector<vector<node*>>pathL;
        pathL.push_back(pathNodes);
        pathL.push_back(pathNodes2);
        for(int j =0;j<pathL.size();j++){
        	for (int i =0;i<pathL[j].size();i++) {
        		node* step = pathL[j][i];
            		renderBoard(window, step->board, cellSize, pathLine, pathLine2);
            		sf::sleep(sf::seconds(0.1)); // Adjust time for slower walking pace

            		sf::Event event;
            		while (window.pollEvent(event)) {
                		if (event.type == sf::Event::Closed) {
                    		window.close();
                    		break;
                		}
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
