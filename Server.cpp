// Server.cpp
//#include <duels/bulls_cows/msg.h>
#include </home/ecn/Desktop/bulls_cows/include/duels/bulls_cows/msg.h>

#include </home/ecn/Desktop/bulls_cows/include/duels/server.h>
#include <duels/bulls_cows/bulls_cows_ai.h>
#include <duels/bulls_cows/mechanics.h>

using namespace duels::bulls_cows;
using duels::Result;
using duels::Timeout;
using duels::Refresh;
using GameIO = duels::Server<InitDisplay, Input, Feedback, Display>;


int main(int argc, char** argv){
  GameIO game_io("bulls_cows", Timeout(1000), Refresh(1000));
  
  // simulation time
  [[maybe_unused]] const double dt(game_io.samplingTime());
  
  // TODO prepare game state / init message (for display)
  Mechanics mechanics;
  InitDisplay init = mechanics.initGame();

  // inform displays and get players (multithread by default for simultaneous games)
  const auto [player1, player2] = game_io.initPlayers<BullsCowsAI>(argc, argv, init, 2, 2); {}

  while(true)
  {
    // extract feedbacks

    const auto result = mechanics.buildPlayerFeedbacks(player1->feedback, player2->feedback);

    std::cout << "after buildPlayerFeedbacks" << std::endl;
    std::cout << "Guess1 = " << player1->input.guess << " bulls, cows: " << player1->feedback.bulls << player1->feedback.cows <<std::endl;
    std::cout << "Guess2 = " << player2->input.guess << " bulls, cows: " << player2->feedback.bulls << player2->feedback.cows <<std::endl;

    // stop if game over
    if(result != Result::NONE)
    {
      game_io.endsWith(result);
      break;
    }
    
    // TODO build display information

    game_io.sendDisplay(mechanics.display());
     
    // request player actions, exits if any disconnect / crash
    if(!game_io.syncBothPlayers())
      break;

    // TODO update game state from player1->input and player2->input
    mechanics.update(player1->input, player2->input);

  }

  // final results
  game_io.sendResult(mechanics.display());
}
