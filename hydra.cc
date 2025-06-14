#include "hydra.h"

// Hydra implementation

std::unique_ptr<Pile> Hydra::createDeck() {
    std::unique_ptr<Pile> temp = std::make_unique<Pile>();
    // add non-joker cards
    for (std::string v : VALUES) {
        for (char s : SUITS) {
            temp->push(std::make_unique<Card>(v,s));
        }
    }
    // add jokers
    for (int i = 0; i < 2; i ++) {
        temp->push(std::make_unique<Card>("2",'J'));
    }
    return temp;
}
void Hydra::deal() {
    Pile tempPile;
    // makes one deck per player
    for (int i = 0; i < int(players.size()); i++) {
        createDeck()->movePile(tempPile);
    }
    tempPile.shufflePile(); // shuffles decks together
    //deals cards
    for (int i = 0; i < int(players.size()); i++) {
        for (int j = 0; j < DECK_SIZE; j++) {
            players[i]->pushDraw(tempPile.pop());
        }
    }
}
Hydra::Hydra(int numPlayers) {
    for (int i = 0; i < numPlayers; i++) {
        players.push_back(std::make_unique<Player>(i));
    }
}
void Hydra::makeHead(int pNum) {
    heads.push_back(std::make_unique<Pile>());
    heads.back()->push(players[pNum]->popDraw());
    if (heads.back()->top().isJoker()) {
        heads.back()->top().setValue("2"); // jokers become 2
    }
}
int Hydra::numHeads() {
    int ret = 0;
    for (int i = 0; i < int(heads.size()); i++) {
        if (!heads[i]->isEmpty()) ret++;
    }
    return ret;
}
bool Hydra::cutHead(const Card held, int hNum) {
    int i;
    if (held.getSuit() == 'J') return false; // can always play joker
    for (i = 0; i < int(heads.size()) && heads[i]->isEmpty(); i++) {}
    // i now is the index of the oldest valid head
    if (i != hNum) return false; // can only cut oldest head
    for (int j = 0; j < int(heads.size()); j++) {
        if (!heads[j]->isEmpty()){
            int headVal = heads[j]->top().getNumValue();
            // implies play can still be made
            if (headVal >= held.getNumValue() || headVal == 1) return false;
        }
    }
    return true;
}

// Player implementation

Player::Player(int pn): pNum{pn}, draw{std::make_unique<Pile>()},
    discard{std::make_unique<Pile>()}, reserve{nullptr} {}
void Player::pushDraw(std::unique_ptr<Card> card) {
    draw->push(std::move(card));
}
std::unique_ptr<Card> Player::popDraw() {
    fillDraw(); // fills draw pile if needed
    return draw->pop();
}
bool Player::playerWon() {
    return (draw->isEmpty() && discard->isEmpty() && reserve == nullptr);
}
void Player::fillDraw() {
    if (draw->isEmpty()) {
        discard->shufflePile();
        discard->movePile(*draw);
    }
}
