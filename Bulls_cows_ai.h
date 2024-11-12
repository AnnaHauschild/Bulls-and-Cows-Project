//Bulls_cows_ai.h
#ifndef BULLS_COWS_AI_H
#define BULLS_COWS_AI_H

#include <duels/player.h>
//#include <duels/mechanics.h>
#include <duels/bulls_cows/msg.h>
#include <iostream>
#include <random>
#include <algorithm>
#include <string>
#include <vector>
#include <list>

namespace duels {
namespace bulls_cows {

// built-in AI class, should be heavily adapted to your needs
class BullsCowsAI : public duels::Player<Input, Feedback>
{
public:
  BullsCowsAI(int difficulty = 1) : difficulty(difficulty) {}

  void updateInput()
  {
      std::cout << "inside updateInput" << std::endl;
      std::cout << "Guess = " << input.guess << " bulls, cows: " << feedback.bulls << feedback.cows <<std::endl;

    turn++;
    std::cout << "Turn = " << turn << std::endl;
    //input.guess;
    //feedback.bulls;
    //feedback.cows;

    // in this function the `feedback` member variable was updated from the game
    // TODO update the `input` member variable
    // the `difficulty` member variable may be used to tune your AI (0 = most stupidest)
    // do not hesitate to create a .cpp file if this function is long
    // Check the difficulty level

    if (difficulty == 0) {
        if(turn <= 2){
            input.guess = generateSecretNumber();
            guess0 = input.guess;
        }
        // Always use the same guess
        else{
            input.guess = guess0;
        }
        //std::cout << "Guess = " << input.guess << " guess0 = " << guess0 << " bulls, cows: " << feedback.bulls << feedback.cows << std::endl;

    } else if (difficulty == 1) {   // Make a random guess
        input.guess = generateSecretNumber();
        //std::cout << "Guess = " << input.guess << " bulls, cows: " << feedback.bulls << feedback.cows <<std::endl;

    }  else if(difficulty == 2) {
        if(turn == 1){
            input.guess = generateSecretNumber();
            guess0 = input.guess;
            for (int i = 123456; i <= 987654; i++) {
                if (isValidGuess(i)) {           //no dublicates
                    possibleGuesses.push_back(i);
                }
            }
        }
        if(turn == 2){
            input.guess = guess0;   //1. guess = secret number
        }
        if(turn >=3){


           /* std::cout << "lenght before erase = " << possibleGuesses.size() << std::endl;
            for (uint i=0; i<possibleGuesses.size(); i++){
                Feedback guessFeedback = computeBullsAndCows(input.guess, possibleGuesses[i]);
                if (feedback.bulls != guessFeedback.bulls || feedback.cows != guessFeedback.cows){
                    possibleGuesses[i] = 0;
                }
            }*/

            //possibleGuesses.erase(std::remove(possibleGuesses.begin(), possibleGuesses.end(), 0), possibleGuesses.end());

            /*for (int guess : possibleGuesses) {
                Feedback guessFeedback = computeBullsAndCows(guess, input.guess);
                std::cout << "guessFeedback bulls,cows: " << guessFeedback.bulls << guessFeedback.cows << " actual Feedback bulls,cows: " << feedback.bulls << feedback.cows << std::endl;
                possibleGuesses.erase(std::remove_if(possibleGuesses.begin(), possibleGuesses.end(), [&](int) {
                    return ((guessFeedback.bulls != feedback.bulls) || (guessFeedback.cows != feedback.cows));
                }), possibleGuesses.end());
            }*/

            possibleGuesses.erase(std::remove_if(possibleGuesses.begin(), possibleGuesses.end(), [&](int guess) {
                Feedback guessFeedback = computeBullsAndCows(guess, input.guess);
                return ((guessFeedback.bulls != feedback.bulls) || (guessFeedback.cows != feedback.cows));
            }), possibleGuesses.end());

            int randomIndex = 0;
            randomIndex = rand() % possibleGuesses.size();
            input.guess = possibleGuesses[randomIndex];
            std::cout <<"randomIndex = " << randomIndex << " New guess = " <<input.guess << std::endl;
        }

    } else {
        // Handle other difficulty levels
        // Add your custom logic here
    }
  }
  
  

  int generateSecretNumber(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(123456,987654);

        int secretNumber;
        bool isValid = false;

        while(isValid==false){
            secretNumber = dist(gen);
            isValid = isValidGuess(secretNumber);
        }
        return secretNumber;
    }
  
  
		
private:
  int difficulty = 1;
  int guess0 = 0;
  int turn = 0;
  std::vector<int> possibleGuesses;  // List of possible guesses
  

  bool isValidGuess(int guess){
      // Check if guess is a 6-digit number with unique digits
      std::string guessStr = std::to_string(guess); //change from integer to string to check the length
      if (guessStr.length() != 6) {
          return false;
      }

      for (int i = 0; i < 6; i++) {
          for (int j = i + 1; j < 6; j++) {
              if (guessStr[i] == guessStr[j]) {
                  return false;
              }
          }
      }
      return true;
  }

  Feedback computeBullsAndCows(int guess, int secret) {
      std::string guessStr = std::to_string(guess);
      std::string secretStr = std::to_string(secret);
      Feedback feedback1;
      feedback1.bulls = 0;
      feedback1.cows = 0;

      for (int i = 0; i < 6; i++) {
          if (guessStr[i] == secretStr[i]) {
              // Digit at the same position matches (bull)
              feedback1.bulls++;
          } else if (secretStr.find(guessStr[i]) != std::string::npos) {		//finds pos of guessStr[i] in secretStr and returns pos. If not found, returns value called std::string::npos ("no position").
              // Digit exists in the secret number but at a different position (cow)
              feedback1.cows++;
          }
      }
      return feedback1;
  }

};
}
}
#endif
