// view provides the I/O for a game of Hydra
//   * the "view" and "controller" under MVC architecture
//   * dictates game flow, with optional testing mode

#ifndef _VIEW_H_
#define _VIEW_H_
#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <exception>
#include "hydra.h"

class View {
    std::istream & in;
    std::ostream & out;
    bool testing; // true iff testing mode is active
    bool cheats; // true iff cheats are active
    std::unique_ptr<Hydra> game; // the Hydra game
    //chooseNext(pNum) chooses player pNum's next card
    //  * used for testing mode
    void chooseNext(int pNum);
    // outputHeads() prints a message to out, containing info on active heads
    void outputHeads();
    // outputPlayers() prints a message to out, containing info on
    //   current players. If turn is non-negative, it also
    //   displays information about player turn - 1
    void outputPlayers(int turn = -1, int remaining = 0);
    // playerTurn() executes one "play" of player pNum's turn. 
    //   * if n heads are active, players have n "plays" per turn
    //   * Returns false iff turn is now over
    bool playerTurn(int pNum);
    // drawCard() returns a vector of strings representing a card in ASCII art
    std::vector<std::string> drawCard(const Card& card, bool faceDown = false);
    // drawCardRow() prints multiple cards side by side
    void drawCardRow(const std::vector<std::vector<std::string>>& cards);
    // getCardDisplay() returns formatted display for a card value/suit
    std::string getCardDisplay(const Card& card);
    // drawHeadsGraphical() prints heads with ASCII card graphics
    void drawHeadsGraphical();
    // drawPlayerHandGraphical() prints player's hand and reserve with graphics
    void drawPlayerHandGraphical(int pNum, const Card* heldCard = nullptr, int remaining = 0);
    public:
    View(std::istream & in, std::ostream & out);
    // toggleTesting() toggles testing mode
    //   * allows choice for all random card draws
    void toggleTesting();
    // toggleCheats() toggles cheats
    //   requires: testing = true
    //   * allows choice of initial deck size
    //   * allows heads to be set at game start
    void toggleCheats();
    // begins a game of Hydra
    //   * reads input from in
    //   * prints output to out
    //   * gamestate maintained in game object
    void playGame();
};

#endif
