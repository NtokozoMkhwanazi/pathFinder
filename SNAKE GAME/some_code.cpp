int minimax(State *state, char AgentSymbol, int depth, int alpha, int beta, bool isMaximizing) const
{

	char userSymbol = (AgentSymbol == 'X') ? 'O' : 'X';
	int score = evaluateBoard(state->Board, AgentSymbol);

	if (score == 10 || score == -10 || depth == 0 || checkDraw(state->Board))
	{
		return score;
	}

	if (isMaximizing)
	{
		int bestScore = INT_MIN;
		for (int i = 0; i < state->Board.size(); i++)
		{
			for (int j = 0; j < state->Board[i].size(); j++)
			{
				if (state->isSafe(i, j) && state->isEmpty(i, j))
				{
					vector<vector<char>> newBoard = state->Board;
					newBoard[i][j] = AgentSymbol;
					unique_ptr<State> newState = make_unique<State>(newBoard, state);
					int score = minimax(newState.get(), AgentSymbol, depth - 1, alpha, beta, false);
					bestScore = max(bestScore, score);
					alpha = max(alpha, bestScore);
					if (beta <= alpha)
					{
						break;
					}
				}
			}
		}
		return bestScore;
	}
	else
	{
		int bestScore = INT_MAX;
		for (int i = 0; i < state->Board.size(); i++)
		{
			for (int j = 0; j < state->Board[i].size(); j++)
			{
				if (state->isSafe(i, j) && state->isEmpty(i, j))
				{
					vector<vector<char>> newBoard = state->Board;
					newBoard[i][j] = userSymbol;
					unique_ptr<State> newState = make_unique<State>(newBoard, state);
					int score = minimax(newState.get(), AgentSymbol, depth - 1, alpha, beta, true);
					bestScore = min(bestScore, score);
					beta = min(beta, bestScore);
					if (beta <= alpha)
					{
						break;
					}
				}
			}
		}
		return bestScore;
	}
}



    	cout << "Generated Maze and snake :" << endl;
    	for (const auto& row : startBoard->board) {
        	for (char cell : row) {
        	    cout << cell << " ";
       	 }
        	cout << endl;
    	}
    		cout << "Generated Maze and snake :" << endl;
    	for (const auto& row : goalBoard->board) {
        	for (char cell : row) {
        	    cout << cell << " ";
       	 }
        	cout << endl;
    	}
	
	node* result = bfs(startBoard->board,goalBoard->board);
	
	if (result) {
        	cout << "Goal state reached!" << endl;

    	} else {
        	cout << "No solution found." << endl;
    	}
