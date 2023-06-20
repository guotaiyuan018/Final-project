#include <cstdlib>

#include "../state/state.hpp"
#include "./submission.hpp"

/**
 * @brief Randomly get a legal action
 *
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move
 */
Move AlphaBeta::get_move(State *state, int depth)
{

  if (!state->legal_actions.size())
    state->get_legal_actions();

  int counter = 0, locat = 0;
  int Max = -1e6;
  int heur = 0;

  for (auto it : state->legal_actions)
  {
    State *next_state = state->next_state(it);
    heur = minimax(next_state, depth - 1, false, -1e6, 1e6);
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

int AlphaBeta::minimax(State *state, int depth, bool max_player, int alpha, int beta)
{
  int val = 0;
  if (state->legal_actions.empty())
    state->get_legal_actions();
  if (state->game_state == WIN)
  {
    if (max_player)
      return 1e6;
    else
      return -1e6;
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