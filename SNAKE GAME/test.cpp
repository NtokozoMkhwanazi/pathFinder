#include <cassert>
#include <iostream>

void testTicTacToe() {
    vector<vector<char>> board = getBoard();
    
    // Test 1: Check if the board initializes correctly
    assert(board[0][0] == '#');
    assert(board[1][1] == '#');
    assert(board[2][2] == '#');
    
    // Test 2: Test user move (X)
    board = userPlay(board, 0, 0, 'X');
    assert(board[0][0] == 'X');
    
    // Test 3: Test if the board is not full
    assert(!isBoardFull(board));
    
    // Test 4: Test AI move functionality
    unique_ptr<TicTacToeNode> TTT = make_unique<State>(board);
    auto aiMove = TTT->makeAIMove('O');
    assert(aiMove->Board[1][1] == 'O'); // Assuming AI picks center
    
    std::cout << "All tests passed!" << std::endl;
}

int main() {
    testTicTacToe();
    return 0;
}
