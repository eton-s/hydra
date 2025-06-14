#include "view.h"

// View implementation

View::View(std::istream & in, std::ostream & out)
    : in{in}, out{out}, testing{false}, 
      cheats{false}, game{nullptr} {}
void View::toggleTesting() {
    testing = !testing;
}
void View::toggleCheats() {
    cheats = !cheats;
}
void View::chooseNext(int pNum) {
    std::string value;
    std::string suit = "";
    if (game->players[pNum]->draw->isEmpty()) return;
    game->players[pNum]->popDraw();
    out << "\n";
    // choose next card's value
    do {
        out << "Card value?" << std::endl;
        if (!(in >> value)) throw std::logic_error("EOF");
        if (value == "Joker") { // joker unique case
            value = "2";
            suit = "J";
            out << "\n";
            break;
        }
    } while (std::find(std::begin(VALUES), std::end(VALUES), value) 
        == std::end(VALUES)); // loops unless value is valid
    if (suit != "J") {
        // choose next card's suit
        do {
            out << "Suit?" << std::endl;
            if(!(in >> suit)) throw std::logic_error("EOF");
        } while (std::find(std::begin(SUITS), std::end(SUITS), suit[0]) 
        == std::end(SUITS)); // loops unless suit is valid
    }
    // push card to top of draw
    game->players[pNum]->draw->push(std::make_unique<Card>(value, suit[0]));
}
void View::outputHeads() {
    out << "Heads:" << std::endl;
    for (int i = 0; i < int(game->heads.size()); i++) {
        if (!game->heads[i]->isEmpty()) {
            out << i + 1 << ": " << game->heads[i]->top().cardString();
            out << " (" << game->heads[i]->pileSize() << ")" << std::endl;
        }
    }
    out << std::endl;
}
void View::outputPlayers(int turn, int remaining) {
    out << "Players:" << std::endl;
    for (int i = 0; i < int(game->players.size()); i++) {
        int drawSize = game->players[i]->draw->pileSize();
        if (turn == i) drawSize--; // since card is "in hand", it's not in draw
        int discardSize = game->players[i]->discard->pileSize();
        out << "Player " << i + 1 << ": ";
        out << drawSize + discardSize << " (";
        out << drawSize << " draw, " << discardSize << " discard)";
        if (turn == i) { // if it is player i's turn, extra info displayed
            out << " + 1 in hand, " << remaining << " remaining, ";
            out << (game->players[i]->reserve != nullptr) << " in reserve";
        }
        out << std::endl;
    }
    out << std::endl;
}
bool View::playerTurn(int pNum) {
    game->players[pNum]->fillDraw();
    if (game->players[pNum]->draw->isEmpty()) return false;
    std::string str;
    int cmd;
    if (testing) chooseNext(pNum);
    std::unique_ptr<Card> held = game->players[pNum]->popDraw();
    do {
        // output turn message
        out << "Player " << pNum + 1 << ", you are holding a ";
        if (held->isJoker()) {
            out << "Joker";
        } else {
            out << held->cardString();
        }
        out << ". Your move?" << std::endl;
        // read in player's move
        if(!(in >> str)) throw std::logic_error("EOF");
        try {
            cmd = stoi(str) - 1;
            if (cmd < -1 || cmd >= int(game->heads.size())) continue;
        } catch (...) {
            continue;
        }
        if (cmd == -1) { // if input is "0"
            if (game->numHeads() <= 1) continue;
            // move held to reserve
            if (game->players[pNum]->reserve == nullptr) {
                game->players[pNum]->reserve =
                    std::move(held);
                if (game->players[pNum]->draw->isEmpty()) return false;
                break;
            } else { // swap held with reserve
                game->players[pNum]->pushDraw(
                    std::move(game->players[pNum]->reserve));
                game->players[pNum]->reserve = std::move(held);
                if (testing) chooseNext(pNum);
                held = game->players[pNum]->popDraw();
                continue;
            }
        } else if (!game->heads[cmd]->isEmpty()) { // input is a head #
            int headVal = game->heads[cmd]->top().getNumValue();
            if (held->isJoker()) { // try to play joker on a head
                out << "Joker value?" << std::endl;
                if(!(in >> str)) throw std::logic_error("EOF");
                if (std::find(std::begin(VALUES), std::end(VALUES), str) 
                    != std::end(VALUES)) {
                    held->setValue(str);
                } else {continue;}
            }
            if (game->cutHead(*held, cmd)) { // if head must be cut
                // begin cutting head cmd
                // discard held
                game->players[pNum]->discard->push(std::move(held));
                if (game->players[pNum]->reserve != nullptr) { // clear reserve
                    game->players[pNum]->
                        pushDraw(std::move(game->players[pNum]->reserve));
                    game->players[pNum]->reserve = nullptr;
                }
                // move head to discard
                game->heads[cmd]->movePile(*(game->players[pNum]->discard));
                game->players[pNum]->fillDraw();
                if (testing) chooseNext(pNum);
                // new head from top of draw
                game->makeHead(pNum);
                // check for win
                if (game->players[pNum]->playerWon()) return false;
                if (testing) chooseNext(pNum);
                // new head from top of draw
                game->makeHead(pNum);
                return false;
            }
            if (held->getNumValue() == headVal) { // held is same val as head
                game->heads[cmd]->push(std::move(held));
                return false;
            }
            // held is lower value as head, or head is ace
            if (held->getNumValue() < headVal  || headVal == 1) {
                game->heads[cmd]->push(std::move(held));
                break;
            }
        }
    } while (1);
    return true;
}
void View::playGame() {
    std::string input;
    // read in # of players
    out << "How many players?" << std::endl;
    if(!(in >> input)) throw std::logic_error("EOF");
    try {
        game = std::make_unique<Hydra>(stoi(input));
        if (stoi(input) <= 1) return;
    } catch (...) {
        return;
    }
    // if cheats and testing mode are on
    if (cheats && testing) {
        out << "\n";
        int dSize = 0;
        // read in deck size per player
        for (int i = 0; i < int(game->players.size()); i++) {
            do {
                out << "How many cards in player " << i + 1;
                out <<"'s deck?" << std::endl;
                if(!(in >> input)) throw std::logic_error("EOF");
                out << std::endl;
                try {
                    dSize = stoi(input);
                } catch (...) {
                    continue;
                }
            } while (dSize < 1);
            for (int j = 0; j < dSize; j++) {
                // pushes an arbitrary card to the top of player i's deck
                game->players[i]->pushDraw(std::make_unique<Card>("A",'S'));
            }
        }
    } else { // otherwise deal normally
        game->deal();
    }
    // if cheats and testing mode are on
    if (cheats && testing) {
        int nHeads = 0;
        // read in the number of starting heads
        do {
            out << "How many starting heads?" << std::endl;
            if(!(in >> input)) throw std::logic_error("EOF");
            out << std::endl;
            try {
                nHeads = stoi(input);
            } catch (...) {
                continue;
            }
        } while (nHeads < 1);
        // for each head:
        for (int i = 1; i <= nHeads; i++) {
            int hSize = 0;
            // read in the number of cards in the head
            do {
                out << "Choose the number of cards in head " << i << ":";
                out << std::endl;
                if(!(in >> input)) throw std::logic_error("EOF");
                out << std::endl;
                try {
                    hSize = stoi(input);
                } catch (...) {
                    continue;
                }
            } while (hSize < 1);
            // read in the top card
            out << "Choose the card on top of head " << i << ":" << std::endl;
            if (hSize == 1) {
                game->players[0]->pushDraw(std::make_unique<Card>("A",'S'));
                chooseNext(0);
                game->makeHead(0);
            } else {
                game->players[0]->pushDraw(std::make_unique<Card>("A",'S'));
                game->makeHead(0);
                for (int j = 0; j < hSize - 2; j++) {
                    game->heads[i - 1]->push(std::make_unique<Card>("A",'S'));
                }
                game->players[0]->pushDraw(std::make_unique<Card>("A",'S'));
                chooseNext(0);
                game->heads[i - 1]->push(std::move(game->players[0]->popDraw()));
            }
        }
    } else {
        if (testing) chooseNext(0);
        game->makeHead(0);
    }
    // loop i through all players, then repeat indefinitely
    for (int i = (game->players.size() == 1) ? 0 : 1; // p2 starts if possible
         ; // indefinite loop 
         i >= int(game->players.size()) - 1 ? i = 0 : i++) { // loop back to p1
        // output game info
        out << std::endl;
        outputHeads();
        outputPlayers();
        out << "Player " << i + 1 << ", it is your turn." << std::endl;
        in.ignore();
        if(!std::getline(in,input)) throw std::logic_error("EOF");
        // loop over j once per head
        for (int j = 0; j < game->numHeads(); j++) {
            game->players[i]->fillDraw();
            // output game info
            out << "\n";
            outputHeads();
            outputPlayers(i, game->numHeads() - 1 - j);
            // player draws and plays card
            if (!playerTurn(i)) {j = game->numHeads();}
            if (game->players[i]->playerWon()) { // check for win
                out << "Player " << i + 1 << " wins!" << std::endl;
                return;
            }
        }
        if (game->players[i]->reserve != nullptr) { // empty reserve
            game->players[i]->pushDraw(std::move(game->players[i]->reserve));
            game->players[i]->reserve = nullptr;
        }
    }
}
