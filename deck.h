// deck contains classes required to structure a standard deck of cards
//   * Card class objects represent cards with a value and suit
//   * Pile class objects represent a pile of cards
//     - functionally a stack of Cards
//     - can be shuffled randomly or with a seed

#ifndef _DECK_H_
#define _DECK_H_
#include <vector>
#include <memory>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>

const std::vector<std::string> VALUES // non-joker values
    = {"A","2","3","4","5","6","7","8","9","10","J","Q","K"};
const std::vector <char> SUITS = {'S','H','C','D'}; // non-joker suits

class Card;

class Pile {
    std::vector<std::unique_ptr<Card>> cards; // cards in the pile
    public:
    void push(std::unique_ptr<Card> c); // pushes c to the top of the pile
    std::unique_ptr<Card> pop(); // pops top card off of pile, else nullptr
    // shufflePile(seed) shuffles the pile. 
    //   If seed is given, it is used to shuffle.
    void shufflePile(unsigned int seed = 
                        static_cast<unsigned int>(std::time(0)));
    void movePile(Pile & p); // moves all cards onto top of p
    bool isEmpty(); // true iff pile is empty
    Card top(); // returns top card by value
    int pileSize(); // returns the #cards in the pile
};

class Card {
    std::string value;
    char suit;
    public:
    Card(std::string value, char suit);
    // getNumValue() returns numerical value of the card. Note that
    //   A=1,J=11,Q=12,K=13
    int getNumValue() const;
    void setValue(std::string v); // sets value to v if v in VALUES
    // cardString() returns a string representing the card of form
    //   "[value][suit]" or "Joker" iff card is Joker
    std::string cardString() const; 
    bool isJoker() const; // true iff card is Joker
    char getSuit() const;
};

#endif
