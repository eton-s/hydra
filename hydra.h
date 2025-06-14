// hydra contains the required classes to model a game of Hydra
//   * The "model" under MVC architecture
//   * Player class objects represent a player (draw/discard piles, reserve)
//   * Hydra class objects represent a Hydra game (players + heads)

#ifndef _HYDRA_H_
#define _HYDRA_H_
#include <vector>
#include <memory>
#include "deck.h"

const int DECK_SIZE = 54; // standard deck size
const int SEED = 42; // constand seed, if needed

class Player;
//class View;

class Hydra {
    // players in the game. players[0] corresponds to player 1 (p1)
    std::vector<std::unique_ptr<Player>> players; 
    // heads in the game. heads[0] corresponds to head 1
    std::vector<std::unique_ptr<Pile>> heads;
    // createDeck() creates a unique pointer to a standard deck of cards
    std::unique_ptr<Pile> createDeck();
    public:
    Hydra(int numPlayers);
    // deal() distributes DECK_SIZE cards to each player after
    //   shuffling that many cards together.
    void deal(); 
    void makeHead(int pNum); // make a head from player pNum's draw pile
    int numHeads(); // returns number of active heads in the game
    // cutHead(held,hNum) returns true iff 
    //   head hNum can be cut given the held card
    bool cutHead(const Card held, int hNum); 
    friend class View;
};

class Player {
    int pNum; // player number in this player's game of Hydra
    std::unique_ptr<Pile> draw, discard;
    std::unique_ptr<Card> reserve;
    public:
    Player(int pn);
    void pushDraw(std::unique_ptr<Card>); // pushes card onto top of draw
    std::unique_ptr<Card> popDraw(); // pops card from top of draw
    bool playerWon(); // true iff player has won
    // fillDraw() fills the draw pile if necessary, by shuffling
    //   the discard pile, then moving it to the top of draw
    void fillDraw(); 
    friend class View;
};

#endif
