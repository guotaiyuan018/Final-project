#include <cstdlib>

#include "../state/state.hpp"
#include "./minimax.hpp"

/**
 * @brief Randomly get a legal action
 *
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move
 */
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