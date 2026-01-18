/**
*  
* Solution to course project # 04
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2025/2026
*
* @author Elitsa Angelska
* @idnumber 1MI0600556
* @compiler GCC
*
* main file
*
*/
#define _CRT_SECURE_NO_WARNINGS
#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <cstring>
#include <cstdlib>

using namespace std;
const int DECK_SIZE = 108;
const int MAX_HAND = 30;
const int MAX_DISCARD = DECK_SIZE;
const char* SAVE_FILE = "uno_game.txt";
mt19937 randomGenerator(std::random_device{}());
int drawPileCount = 0;
int discardCount = 0;
int playerCount = 0;
int currentColor = 4;
bool clockwise = true;
int drawPenalty = 0;
//
int currentPlayer = 0;
int player2Count = 0;
bool saidUno2 = false;
bool saidUno = false;
struct Card {
	int color;
	int type;
	int value;
};
Card drawPile[DECK_SIZE];
Card discardPile[MAX_DISCARD];
Card playerHand[MAX_HAND];
//
Card player2Hand[MAX_HAND];
int getColor(const Card& c) {
	return c.color;
}
int getNumber(const Card& c) {
	return c.value;
}
int getType(const Card& c) {
	return c.type;
}
bool isWildCard(const Card& c) {
	return c.color == 4;
}
bool isWildDrawFour(const Card& c) {
	return c.color == 4 && c.type == 5;
}
const char* getShort(const Card& c) {
	static char s[32]; 
	const char* colors = "RGBYW";
	const char* reset = "\033[0m";
	const char* code = "";


	if (c.color == 0) code = "\033[31m";      
	else if (c.color == 1) code = "\033[32m"; 
	else if (c.color == 2) code = "\033[34m"; 
	else if (c.color == 3) code = "\033[33m";
	else code = "\033[35m";                  


	snprintf(s, sizeof(s), "%s%c", code, colors[c.color]);

	if (c.type == 0) {
		if (c.value >= 0 && c.value <= 9) {
			char tmp[4];
			snprintf(tmp, sizeof(tmp), "%d", c.value);
			strncat(s, tmp, sizeof(s) - strlen(s) - 1);
		}
	}
	else if (c.type == 1) {
		strncat(s, "S", sizeof(s) - strlen(s) - 1);
	}
	else if (c.type == 2) {
		strncat(s, "R", sizeof(s) - strlen(s) - 1);
	}
	else if (c.type == 3) {
		strncat(s, "+2", sizeof(s) - strlen(s) - 1);
	}
	else if (c.type == 4) {
		snprintf(s, sizeof(s), "%sWW", code);
	}
	else if (c.type == 5) {
		snprintf(s, sizeof(s), "%sW+4", code);
	}
	else {
		strcpy(s, "??");
	}


	strncat(s, reset, sizeof(s) - strlen(s) - 1);

	return s;

}
const char* getColorName(int col) {
	const  char* names[] = { "Red", "Green", "Blue", "Yellow", "Wild" };
	return names[col];
}

	void Deck() {
		drawPileCount = 0;
		for (int i = 0; i < DECK_SIZE; ++i) {
			Card c;
			c.color = (i >= 104) ? 4 : i / 26;
			if (i >= 104) {
				c.type = (i % 2 == 0) ? 4 : 5;
				c.value = (c.type == 5) ? 4 : 0;
			}
			else {
				int localIdx = i % 26;
				c.type = (localIdx <= 18) ? 0 : (localIdx <= 20 ? 1 : (localIdx <= 22 ? 2 : 3));
				c.value = (c.type == 0) ? (localIdx == 0 ? 0 : (localIdx + 1) / 2) : 20;
			}
			drawPile[drawPileCount++] = c;
		}
		shuffle(drawPile, drawPile + DECK_SIZE, randomGenerator);
	}

Card drawOne() {
	if (drawPileCount == 0) {
		if (discardCount <= 1) {
			cout << "No cards left to draw!\n";
			Card empty = { 4,0,-1 };
			return empty;
		}

		Card top = discardPile[discardCount - 1];
		drawPileCount = discardCount - 1;
		for (int i = 0; i < drawPileCount;++i) {
			drawPile[i] = discardPile[i];
		}
		discardCount = 1;
		discardPile[0] = top;
		shuffle(drawPile, drawPile + drawPileCount, randomGenerator);
		cout << "Deck reshuffled from discard pile.\n";
	}
	return drawPile[--drawPileCount];
}
bool canPlay(const Card& card, const Card& top) {
	if (isWildCard(card))return true;
	if (getColor(card) == currentColor)return true;
	if (getType(card) == 0 && getType(top) == 0 && getNumber(card) == getNumber(top))return true;
	if (getType(card) == getType(top) && getType(top) != 0)return true;
	return false;
}
void printHand() {
	const char* title = (currentPlayer == 0) ? "Player 1 cards:" : "Player 2 cards:";
    Card* hand = (currentPlayer == 0) ? playerHand : player2Hand;
    int count = (currentPlayer == 0) ? playerCount : player2Count;
    cout << title << "\n";
	//cout << "Your cards:\n";
	for (int i = 0;i < count;++i) {
		cout << "[" << i << "]" << getShort(hand[i]) << " ";
		if ((i + 1) % 6 == 0) cout << "\n";
	}
	cout << "\n";
}
void printTop() {
	if (discardCount == 0) {
		cout << "No top card yet\n";
		return;
	}
	cout << "Current card: " << getShort(discardPile[discardCount - 1]) << "\n";
	cout << "Color to match: " << getColorName(currentColor) << "\n\n";
}
void playerTurn()
{	
	Card* hand = (currentPlayer == 0) ? playerHand : player2Hand;
	int& count = (currentPlayer == 0) ? playerCount : player2Count;
	bool& unoFlag = (currentPlayer == 0) ? saidUno : saidUno2;
	cout << "\n--- Player " << (currentPlayer + 1) << "'s turn ---\n";
	if (drawPenalty > 0) {
		cout << "You must draw " << drawPenalty << " cards (penalty).\n";
		for (int i = 0; i < drawPenalty && count < MAX_HAND;++i)
		{
			hand[count++] = drawOne();

		}
		drawPenalty = 0;
		currentPlayer = 1 - currentPlayer;
		return;
	}
	printTop();
	printHand();
	if (count == 1 && !unoFlag) {
		cout << "You have ONE card left! Type 'uno' before playing!\n";
	}
	cout << "Enter index to play:'draw', 'uno' or 'save'";
	char input[32];
	cin.getline(input, sizeof(input));
	if (strcmp(input, "save") == 0) {
    saveGame();
    cout << "Game saved. Continuing...\n";
    return;
	}
	if (strcmp(input, "uno") == 0) {
		if (count == 1) {
			unoFlag = true;
			cout << "UNO called!\n";
		}
		else {
			cout << "You don't have 1 card - ignored.\n";
		}
		playerTurn();
		return;
	}
	if (strcmp(input, "draw") == 0)
	{
		Card drawn = drawOne();
		cout << "You drew: " << getShort(drawn) << "\n";
		if (count < MAX_HAND)hand[count++] = drawn;
		return;
	}
	int idx = -1;
	try { idx = atoi(input); }
	catch (...) { idx = -1; }
	if (idx < 0 || idx >= count) {
		cout << "Invalid choice - draw 1 card penalty\n";
		if (count < MAX_HAND)hand[count++] = drawOne();
		return;
	}

	Card chosen = hand[idx];
	if (!canPlay(chosen, discardPile[discardCount - 1])) {
		cout << "Illegal card- draw 1 card penalty\n";
		if (count < MAX_HAND)playerHand[count++] = drawOne();
		return;

	}
	currentColor = chosen.color;
	discardPile[discardCount++] = chosen;
	for (int j = idx; j < count - 1;++j)
	{
		hand[j] = hand[j + 1];
	}
	count--;
	cout << "You played: " << getShort(chosen) << "\n";
	if (count == 0 && !unoFlag) {
		cout << "You forgot to say UNO! Draw 2 penalty cards.\n";
		if (count + 2 <= MAX_HAND) {
			hand[count++] = drawOne();
			hand[count++] = drawOne();
		}
	}
	unoFlag = false;
	bool skipNext = false;
	if (isWildCard(chosen))
	{
		cout << "Choose new color (r/g/b/y): ";
		char ch; cin >> ch; cin.ignore();
		ch = tolower(ch);
		if (ch == 'r') currentColor = 0;
		else if (ch == 'g') currentColor = 1;
		else if (ch == 'b') currentColor = 2;
		else if (ch == 'y') currentColor = 3;
		else currentColor = randomGenerator() % 4;
		cout << "Color is now " << getColorName(currentColor) << "\n";
	}
	int typ = getType(chosen);
	if (typ == 1) {
		skipNext = true;
		cout << "Skip! (your turn again)\n";
	}
	else if (typ == 2) {
		clockwise = !clockwise;
		cout << "Reverse!\n";
	}
	else if (typ == 3)
	{
		drawPenalty = 2;
		skipNext = true;
		cout << "+2 penalty on yourself!\n";
	}
	else if (typ == 5)
	{
		drawPenalty = 4;
		skipNext = true;
		cout << "Wild +4 penalty on yourself!\n";
	}	
	currentPlayer = 1 - currentPlayer;

}
bool gameOver() {
	 if (playerCount == 0) {
        cout << "\n*** PLAYER 1 WINS! ***\n";
        return true;
    }
    if (player2Count == 0) {
        cout << "\n*** PLAYER 2 WINS! ***\n";
        return true;
    }
    return false;
}
bool saveGame() {
	ofstream f(SAVE_FILE);
	if (!f)return false;
	f << drawPenalty << " " << currentColor << " " << clockwise << " " << saidUno << "\n";
	f << playerCount << "\n";
	for (int i = 0; i < playerCount;++i)f << playerHand[i].color << " " << playerHand[i].type << " " << playerHand[i].value << "\n";
	f << discardCount << "\n";
	for (int i = 0;i < discardCount;++i)f << discardPile[i].color << " " << discardPile[i].type << " " << discardPile[i].value << "\n";
	f << drawPileCount << "\n";
	for (int i = 0;i < drawPileCount;++i)f << drawPile[i].color << " " << drawPile[i].type << " " << drawPile[i].value << "\n";
	cout << "Game saved.\n";
	return true;
}
bool loadGame() {
	ifstream f(SAVE_FILE);
	if (!f) return false;
	f >> drawPenalty >> currentColor >> clockwise >> saidUno;
	f >> playerCount;
	for (int i = 0; i < playerCount; ++i) {
		int c, t, v;
		f >> c >> t >> v;
		playerHand[i].color = c;
		playerHand[i].type = t;
		playerHand[i].value = v;
	} f >> discardCount;
	for (int i = 0; i < discardCount; ++i) {
		int c, t, v;
		f >> c >> t >> v;
		discardPile[i].color = c;
		discardPile[i].type = t;
		discardPile[i].value = v;
	}
	f >> drawPileCount;
	for (int i = 0; i < drawPileCount; ++i) {
		int c, t, v;
		f >> c >> t >> v;
		drawPile[i].color = c;
		drawPile[i].type = t;
		drawPile[i].value = v;
	}
	cout << "Game loaded.\n";
	return true;
}
void newGame() {
	Deck();
	playerCount = discardCount = 0;
	drawPenalty = 0;
	clockwise = true;
	saidUno = false;
	currentColor = 4;
	playerCount = player2Count = 0;
	for (int i = 0; i < 7; ++i) {
		playerHand[playerCount++] = drawOne();
		player2Hand[player2Count++] = drawOne();
	}
	discardPile[discardCount++] = drawOne();
	currentColor = getColor(discardPile[0]);
	cout << "New game started!\n";
}
void gameLoop() {
	while (!gameOver()) {
		playerTurn();
	}
}
int main() {
#ifdef _WIN32
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
#endif
	cout << "--- UNO Game ---\n";
	while (true) {
		cout << "1.New game\n2.Continue Saved Game\n3.Save & Exit\n4.Exit\n";
		int choice;
		cin >> choice;
		cin.ignore();
		if (choice == 1) {
			newGame();
			gameLoop();
		}
		else if (choice == 2) {
			if (loadGame())gameLoop();
			else cout << "No saved game found.";

		}
		else if (choice == 3) {
			saveGame();
			break;
		}
		else if (choice == 4) {
			break;
		}
	}
	cout << "Goodbye!\n";
	return 0;
}
