
# NTHU I2P3   MiniChess AI (~~Artificial Idiot~~)

the submission one is the alpha-beta prunning player


## Introduction
This is an AI program about Mini Chess. It designs a state value function to evaluate each game position and utilizes minimax and alpha-beta pruning techniques to calculate the optimal solution.


## State Value Function
Firstly, we need to determine whether the "**player**" passed in is our own side or the opponent's. If it is our own side, we start calculating from our own chessboard; otherwise, we start calculating from the opponent's chessboard. Next, we calculate the remaining number of chess pieces on the board. We iterate through each point on the chessboard and assign weights to each remaining chess piece based on the "**chess_value**" table, then sum them up to estimate the value "**h**".

Firstly, we need to determine whether the "**player**" passed in is our own side or the opponent's. If it is our own side, we start calculating from our own chessboard; otherwise, we start calculating from the opponent's chessboard. Next, we calculate the remaining number of chess pieces on the board. We iterate through each point on the chessboard and assign weights to each remaining chess piece based on the "**chess_value**" table, then sum them up to estimate the value "**h**".

```
int chess_value[6] = {10, 50, 30, 35, 90, 10000};
...
int chess_value[6] = {10, 50, 30, 35, 90, 10000};
...
int me;

  if (max_player)
  {
    me = this->player;
  }
  else
    me = !this->player;

  auto self_board = this->board.board[me];
  auto oppo_board = this->board.board[!me];

  int now_piece, dist, oppn_piece;
  int now_piece, dist, oppn_piece;

  for (int i = 0; i < BOARD_H; i++)
  {
    for (int j = 0; j < BOARD_W; j++)
    {
      now_piece = self_board[i][j];
      oppn_piece = oppo_board[i][j];
      if (now_piece)
      {
        h += chess_value[now_piece - 1];
```

Next, we calculate the number of chess pieces that can be captured in the next move. If more chess pieces can be captured, it means that this "**movement**" offers more choices, and the higher the number of choices, the higher our score. Furthermore, we consider the types of opponent's chess pieces that can be captured. If we can capture higher-value opponent's chess pieces, the score for this "**movement**" will be higher.

```
switch (now_piece)
        {
        case 1:
          if (this->player)
          {
            if (oppo_board[i + 1][j + 1] && i < BOARD_H - 1)
              h += chess_value[oppo_board[i + 1][j + 1] - 1];
            if (oppo_board[i + 1][j - 1] && i < BOARD_H - 1)
              h += chess_value[oppo_board[i + 1][j - 1] - 1];
            if (i == 4)
              h += chess_value[4];
          }
          else
          {
            if (oppo_board[i - 1][j + 1] && i > 0)
              h += chess_value[oppo_board[i - 1][j + 1] - 1];
            if (oppo_board[i - 1][j - 1] && i > 0)
              h += chess_value[oppo_board[i - 1][j - 1] - 1];
            if (i == 1)
              h += chess_value[4];
          }
          break;
        case 2:
        case 4:
        case 5:
          int st, end;
          switch (now_piece)
          {
          case 2:
            st = 0;
            end = 4;
            break; // rook
          case 4:
            st = 4;
            end = 8;
            break; // bishop
          case 5:
            st = 0;
            end = 8;
            break; // queen
          default:
            st = 0;
            end = -1;
          }
          for (int part = st; part < end; part += 1)
          {
            auto move_list = move_table_rook_bishop[part]; // move_table_rook_bishop[0-3]:rook , move_table_rook_bishop[4-7]:bishop
            for (int k = 0; k < std::max(BOARD_H, BOARD_W); k += 1)
            {
              int p[2] = {move_list[k][0] + i, move_list[k][1] + j};

              if (p[0] >= BOARD_H || p[0] < 0 || p[1] >= BOARD_W || p[1] < 0)
                break;
              now_piece = self_board[p[0]][p[1]];
              if (now_piece)
                break; // check whether there is any chess of mine , if yes then break

              oppn_piece = oppo_board[p[0]][p[1]];
              if (oppn_piece)
              {
                if (oppn_piece == 6)
                {
                  this->game_state = WIN;
                  return 1e6;
                }
                else
                {
                  h += chess_value[oppn_piece - 1];
                  break;
                }
              };
            }
          }
          break;

        case 3:
          for (auto move : move_table_knight)
          {
            int x = move[0] + i;
            int y = move[1] + j;

            if (x >= BOARD_H || x < 0 || y >= BOARD_W || y < 0)
              continue;
            now_piece = self_board[x][y];
            if (now_piece)
              continue;

            oppn_piece = oppo_board[x][y];
            if (oppn_piece == 6)
            {
              this->game_state = WIN;
              return 1e6;
            }
            else
            {
              h += chess_value[oppn_piece - 1];
              break;
            }
          }
          break;

        default:
          break;
        }
      }
```
The same calculations are performed on the opponent's game position, but with a difference. We subtract the value obtained from calculating the "**h**" from their side. The final result is the estimated value, known as the "**State Value**," that we have assigned to this particular state.
```
else if (oppn_piece)
      {
        h -= chess_value[oppn_piece - 1] * 3;
        switch (now_piece)
        {
        case 1:
          if (this->player)
          {
            if (self_board[i + 1][j + 1] && i < BOARD_H - 1)
              h -= chess_value[self_board[i + 1][j + 1] - 1];
            if (self_board[i + 1][j - 1] && i < BOARD_H - 1)
              h -= chess_value[self_board[i + 1][j - 1] - 1];
          }
          else
          {
            if (self_board[i - 1][j + 1] && i > 0)
              h -= chess_value[self_board[i - 1][j + 1] - 1];
            if (self_board[i - 1][j - 1] && i > 0)
              h -= chess_value[self_board[i - 1][j - 1] - 1];
          }
          break;
        case 2:
        case 4:
        case 5:
          int st, end;
          switch (now_piece)
          {
          case 2:
            st = 0;
            end = 4;
            break; // rook
          case 4:
            st = 4;
            end = 8;
            break; // bishop
          case 5:
            st = 0;
            end = 8;
            break; // queen
          default:
            st = 0;
            end = -1;
          }
          for (int part = st; part < end; part += 1)
          {
            auto move_list = move_table_rook_bishop[part]; // move_table_rook_bishop[0-3]:rook , move_table_rook_bishop[4-7]:bishop
            for (int k = 0; k < std::max(BOARD_H, BOARD_W); k += 1)
            {
              int p[2] = {move_list[k][0] + i, move_list[k][1] + j};

              if (p[0] >= BOARD_H || p[0] < 0 || p[1] >= BOARD_W || p[1] < 0)
                break;
              now_piece = oppo_board[p[0]][p[1]];
              if (now_piece)
                break; // check whether there is any chess of mine , if yes then break

              oppn_piece = self_board[p[0]][p[1]];
              if (oppn_piece)
              {
                if (oppn_piece == 6)
                {
                  this->game_state = WIN;
                  return -1e6;
                }
                else
                {
                  h -= chess_value[oppn_piece - 1];
                  break;
                }
              };
            }
          }
          break;

        case 3:
          for (auto move : move_table_knight)
          {
            int x = move[0] + i;
            int y = move[1] + j;

            if (x >= BOARD_H || x < 0 || y >= BOARD_W || y < 0)
              continue;
            oppn_piece = oppo_board[x][y];
            if (oppn_piece)
              continue;

            now_piece = self_board[x][y];
            if (now_piece == 6)
            {
              this->game_state = WIN;
              return 1e6;
            }
            else
            {
              h -= chess_value[now_piece - 1];
              break;
            }
          }
          break;
        default:
          break;
        }
      }
    }
  }
  return h;
}
```
## MiniMax
"**Minimax**" is a decision-making algorithm used in games and artificial intelligence. It aims to find the best move by considering all possible future game states. The algorithm alternates between **maximizing its own gain** and **minimizing the opponent's gain**. It efficiently evaluates game states and prunes unnecessary branches using other techniques like alpha-beta pruning we'll implement next. Minimax efficiently evaluates game states and optimizes its search process by employing techniques such as alpha-beta pruning, which we will implement next. 

![](https://hackmd.io/_uploads/Bk6pwf1d3.png)

In each layer of our turn, we need to choose the "**Movement**" that can achieve the maximum "**State Value**." In the opponent's turn, we choose the minimum value. This algorithm is known as "**Minimax**."


Here, we need to start by constructing a tree structure to represent all possible "**Movements**". Then, in the recursive function, we stop building the tree at a specified depth and begin returning the "**State Value**" for that particular state.

![](https://hackmd.io/_uploads/BkJHyzydn.png)

In non-terminal nodes of the tree, we execute a function that selects the maximum or minimum value. During our own turn, we choose the maximum value, whereas during the opponent's turn, we choose the minimum value.

Finally, in the main function "**get_move**," we iterate through each "**legal_action**" and execute them one by one. Among them, we select the one with the highest estimated value ("**h**") as the most advantageous and promising "**Movement**" for us.
```
Move MiniMax::get_move(State *state, int depth)
{

  if (!state->legal_actions.size())
    state->get_legal_actions();

  int counter = 0, locat = 0;
  int Max = -1e6;
  int heur = 0;

  for (auto it : state->legal_actions)
  {
    State *next_state = state->next_state(it);
    heur = minimax(next_state, depth - 1, false);
    if (heur > Max)
    {
      Max = heur;
      locat = counter;
    }
    counter++;
  }

  auto actions = state->legal_actions;
  return actions[locat];
}
...
int MiniMax::minimax(State *state, int depth, bool max_player)
{
  int val = 0;
  if (state->legal_actions.empty())
    state->get_legal_actions();
  if (state->game_state == WIN)
  {
    if (max_player)
      return 2e6;
    else
      return -2e6;
  }
  else if (!depth)
  {
    return state->evaluate(max_player);
  }
  else if (max_player)
  {
    val = -1e6;
    for (auto &it : state->legal_actions)
    {
      State *next = state->next_state(it);
      val = std::max(val, minimax(next, depth - 1, !max_player));
    }
  }
  else
  {
    val = 1e6;
    for (auto &it : state->legal_actions)
    {
      State *next = state->next_state(it);
      val = std::min(val, minimax(next, depth - 1, !max_player));
    }
  }
  return val;
}
```


## Alpha-Beta Prunning
**Alpha-beta pruning** is a technique used in the Minimax algorithm to efficiently reduce the number of evaluated nodes in a game tree. It cuts off branches that are unlikely to lead to a better outcome, saving computational resources by focusing on the most promising paths.

![](https://hackmd.io/_uploads/HkAsPzy_3.png)

During the search, the algorithm maintains two values: **alpha**, representing the **maximum lower bou**nd of the best move for the maximizing player, and **beta**, representing the **minimum upper bound** of the best move for the minimizing player. As the algorithm explores different branches of the game tree, it updates these values and compares them with alpha and beta to determine if certain branches can be pruned.

![](https://hackmd.io/_uploads/SkGdcMyun.png)

When alpha becomes greater than or equal to beta, it indicates that the current branch is not worth further exploration because the opponent already has a better option. This pruning avoids unnecessary evaluations and allows the algorithm to focus on more promising branches, significantly improving the search efficiency.


```
int State::minimax(State *state, int depth, bool max_player, int alpha, int beta)
{
  int val = 0;
  if (state->legal_actions.empty())
    state->get_legal_actions();
  if (state->game_state == WIN)
  {
    if (max_player)
      return 2e6;
    else
      return -2e6;
  }
  else if (!depth)
  {
    return state->evaluate(max_player);
  }
  else if (max_player)
  {
    val = -1e6;
    for (auto &it : state->legal_actions)
    {
      State *next = state->next_state(it);
      val = std::max(val, minimax(next, depth - 1, !max_player, alpha, beta));
      alpha = std::max(alpha, val);
      if (alpha >= beta)
        break;
    }
  }
  else
  {
    val = 1e6;
    for (auto &it : state->legal_actions)
    {
      State *next = state->next_state(it);
      val = std::min(val, minimax(next, depth - 1, !max_player, alpha, beta));
      beta = std::min(val, beta);
      if (alpha >= beta)
        break;
    }
  }
  return val;
}
  return val;
}
```


### reference
TA's video,
TA's PPT,
Wiki pedia,