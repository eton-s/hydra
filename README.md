# hydra
A CLI implementation of the card game Hydra, developed for an object-oriented programming course I took in 2021.

### Rules of Hydra

To start a game of Hydra, one needs as many standard, 54-card decks (i.e., with jokers) as there are players. These decks are shuffled together, then distributed equally to each player, face down in a pile. This pile is each player’s draw pile, and each player additionally has a discard pile, which starts empty, and during a turn, a reserve. To start the game, the first player places a single card from the top of their draw pile face up in the center of the table, creating the first “head”. If the card happens to be a joker, then it is taken to have the value 2 (see the discussion of jokers below). From there, play proceeds clockwise. In a turn of Hydra, one must draw and play as many cards as there are currently heads, unless an action ends the player’s turn early. For instance, if there are three heads, the player must play three cards. To play a card, the player draws it from the top of the player’s draw pile, and performs an action with it. If the player’s draw pile is empty, the player must first shuffle their discard pile to form a new draw pile, then draw from that. If the player’s draw and discard piles are both empty, and the player has no reserve, the player wins. There are several possible actions, and conditions under which the actions are allowed:

1. If the card’s face value is strictly less than the value of the top card of any head (an ace is considered to have the value 1, and so has the lowest value), the card may be placed, face up, on that head.
2. If the card’s face value is equal to the face value of the top card of any head, the card may be placed, face up, on that head, but this ends the player’s turn immediately, and they draw no further cards.
3. If the top card of any head is an ace, and the played card is not, then it may be placed, face up, on that head.
4. If the player has no reserve, and there is more than one head, they may place this card into reserve. To do so, the player simply places the card, face down, in front of them.
5. If the player has a reserve, then the player may swap the played card for the reserve card.

In the first three actions, if the played card is a joker, then the player determines and announces a value for the joker while playing it, and the value announced must satisfy the requirements for the action. For instance, the player may place a joker on a 3, by announcing that the joker is now an ace, 2, or 3. In the latter case, as this would fit action 2, this also ends the player’s turn. The joker retains its announced value as long as it remains on top of the head it was placed on, so in our above example, the next player must treat the joker as an ace, 2, or 3 (whichever it was announced to be), not a joker. If the player had more cards to draw in the current turn, then after the first, third, or fourth action, they simply continue. If they are required to draw more cards, and they have more cards in their draw and/or discard pile, they draw another card; if they are required to draw more cards, but their only remaining card is the reserve, then their reserve card is returned to the top of their draw pile, and their turn ends; if they are out of cards entirely, they win; if they are not required to draw another card and still have cards, then their reserve, if any, is returned to the top of their draw pile, and their turn ends. After the second action, the player’s reserve, if any, is returned, face down, to the top of their draw pile, and play continues with the next player, unless the current player is out of cards, in which case they win. After the fifth action, the player must play the card retrieved from the reserve, and this does not count as drawing another card: That is, if the player must play five cards (because there are five heads), swapping four times does not free them of this requirement.
A few subtleties to note:

• Aces are “universal”: They may be played on any head, and if they are the top card on any head, then any card may be played on them. The only caveat is that if a player plays an ace on an ace, that ends their turn, leaving the universal card free for the next player.
• Jokers are wildcards, but are not universal in the sense that aces are, since their value is restricted by announcing it. A joker can be played as an ace, of course, in which case it is universal.
• A player who’s drawn a card of equal value to an existing head may end their turn, removing any further risk that turn, but also keeping all of the cards they would otherwise have been able to play that turn. Deciding when it’s best to end one’s turn involves strategy.
• A player cannot win in a turn in which they’ve used the reserve, because the reserve card itself is not played.
• Otherwise, a player can win at any point during their turn.
If none of the first four actions apply, and the player does not wish to swap with their reserve, the player must perform the following sequence of actions, called “cutting off” a head:
• Place the played card into their discard pile, face down.
• Place their reserve card, if any, on their draw pile, face down.
• Move every card in the oldest head, face down, into their discard pile.
• With the top two cards from the player’s draw pile (which may include what was formerly their reserve, and may require reshuffling the discard pile if they have insufficient cards in the draw pile), create two new heads by placing the two cards, face up, as two new piles. Any jokers placed in this manner are given the value 2.

Generally speaking, this action will give them more cards, rendering them further away from victory. After that, play continues with the next player. Note however that it is possible to “win by losing”: If the player’s last card does not match any action, they will be forced to cut off a head. But, if the oldest head consists of only a single card, and the player had no reserve at the time, then the two cards they play to create two new heads are their last two cards. They will thus be rendered without any cards, and win!
Play continues until a player wins.

### Setup

Prerequisites (linux):
- `make`, `gcc`

1. Use the `make` command in the hydra folder to build the program.
2. Start the game by running the game program with `./hydra`.
3. Running the program with `-testing` or `-cheats` activates extra features. See demo.pdf for details.

### How to Play

Once the program is run, you play the game through a series of text prompts. After indicating the number of players, you can press **Enter** to move to the next turn. On a player's turn, they can choose an action based on the following inputs:
- 0, referring to either placing the current card in reserve or swapping the current card with the reserve should it be non-empty.
- A number corresponding to an active head they can make a valid move on.
    - If the current card is a Joker, the player must specify its value (A, J, Q, K, or a number from 2-10).
If a valid action is taken, the game progresses to the next turn, repeating until a winner is determined.

Have fun!
