#include "deck.h"

// Pile implementation

void Pile::push(std::unique_ptr<Card> c) {
    cards.push_back(std::move(c));
}
std::unique_ptr<Card> Pile::pop() {
    if (cards.size() == 0) return nullptr;
    std::unique_ptr<Card> temp = std::move(cards.back());
    cards.pop_back();
    return temp;
}
void Pile::shufflePile(unsigned int seed) {
    std::srand(seed);
    random_shuffle(cards.begin(),cards.end());
}
void Pile::movePile(Pile & p) {
    while(cards.size() != 0)  {
        p.push(this->pop());
    }
}
bool Pile::isEmpty() {
    if (int(cards.size()) == 0) {
        return true;
    } else {
        return false;
    }
}
Card Pile::top() {
    return *(cards.back());
}
int Pile::pileSize() {return int(cards.size());}

// Card implementation

Card::Card(std::string value, char suit)
    : value{value}, suit{suit} {}
int Card::getNumValue() const {
    if (value == "A") {
        return 1;
    } else if (value == "J") {
        return 11;
    } else if (value == "Q") {
        return 12;
    } else if (value == "K") {
        return 13;
    }
    return std::stoi(value);
}
std::string Card::cardString() const {
    return (value + suit);
}
void Card::setValue(std::string v) {
    if (std::find(VALUES.begin(), VALUES.end(), v) != VALUES.end()) {
        value = v;
    }
}
bool Card::isJoker() const {return suit == 'J';}
char Card::getSuit() const {return suit;}
