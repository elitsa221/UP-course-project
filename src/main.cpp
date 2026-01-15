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
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
using namespace std;
const int DECK_SIZE = 108;
const int MAX_HAND = 30;
const int MAX_DISCARD = DECK_SIZE;
const char* SAVE_FILE = "uno_game.txt";
mt19937 randomGenerator(std :: random_device{}());
int drawPileCount = 0;
int discardCount = 0;
int playerCount = 0;
int currentColor = 4;
bool clockwise = true;
int drawPenalty = 0;
bool saidUno = false;
int drawPile[DECK_SIZE];
int discardPile[MAX_DISCARD];
int playerHand[MAX_HAND];
int getColor(int card){
	if(card >=104) return 4;
	return card /26;
}
int getNumber(int card){
	if(card >=104)return -1;
	int pos = card % 26;
	if(pos ==0)return 0;
	if(pos <=18)return ((pos-1)/2)+1;
	return -1;
}
int getType(int card){
	if(card>=104){
		return (card % 2 ==0 )?4:5;
	}
	int pos = card % 26;
    if (pos <= 18) return 0;              
    if (pos == 19 || pos == 20) return 1; 
    if (pos == 21 || pos == 22) return 2;
    if (pos == 23 || pos == 24) return 3; 
	return 0;
}
bool isWildCard(int card) {
    return card >= 104;
}
bool isWildDrawFour(int card) {
    return 
	card >= 104 && (card % 2 == 1);
}
string getShort(int card){
	string s;
	const char* colors = "RGBYW";
	int col = getColor(card);
	int typ = getType(card);
	int num = getNumber(card);
	s += colors[col];
	if(typ == 0){s += to_string(num);}
	else if(typ == 1){
		s += "S";
	}
	else if(typ == 2){
		s += "R";
	}
	else if(typ == 3){
		s += "+2";
	}
	else if(typ == 4){
		s += "W";
	}
	else if(typ == 5){
		s += "+4";
	}
	else{
		s += "?";
	}
	return s;
}
const char* getColorName(int col){
	const  char* names[] = {"Red", "Green", "Blue", "Yellow", "Wild"};
	return names[col];
}
void Deck(){
	drawPileCount = 0;
    for(int i = 0; i < DECK_SIZE; ++i){
		drawPile[drawPileCount++] = i;
	}
	shuffle(drawPile, drawPile + DECK_SIZE, randomGenerator);
}
int drawOne(){
	if(drawPileCount == 0){/////////////////
		if(discardCount <=1){
			cout << "No cards left to draw!\n";
			return -1;
		}
	
	int top = discardPile[discardCount-1];
	drawPileCount = discardCount -1;
	for(int i =0; i < drawPileCount;++i){
		drawPile[i] = discardPile[i];
	}
	discardCount =1 ;
	discardPile[0] = top;
	shuffle(drawPile, drawPile + drawPileCount, rng);
	cout <<"Deck reshuffled from discard pile.\n";}
	return drawPile[--drawPileCount];
}
bool canPlay(int card, int top){
	if(isWildCard(card))return true;
	if(getColor(card) == currentColor)return true;
	if(getType(card) == 0 && getType(top) == 0 && getNumber(card) == getNumber(top))return true;
	if(getType(card) == getType(top) && getType(top) != 0)return true;
}
void printHand(){
	cout <<"Your cards:\n";
	for(int i = 0;i< playerCount;++i){
		cout << "[" << i << "]"<< getShort(playerHand[i])<< " ";
		if((i+1))% 6 == 0) cout << "\n";
	}
	cout << "\n";
}
void printTop(){
	if(discardCount == 0){
		cout << "No top card yet\n";
		return;
	}
	cout << "Current card: "<< getShort(discardPile[discardCount-1])<<"\n";
	cout << "Color to match: "<< getColorName(currentColor) << "\n\n";
}
void playerTurn(){
	if(drawPenalty > 0){
		cout <<"You must draw "<< drawPenalty<< " cards (penalty).\n";
		for(int i = 0; i <drawPenalty && playerCount < MAX_HAND;++i)
		{
			playerHAND[PlayerCount++] = drawOne();

		}
		drawPenalty = 0;
		return;
	}
	printTop();
	printHand();
	if(playerCount == 1  && !saidUno){
		cout <<"You have ONE card left! Type 'uno' before playing!\n";
	}
	cout <<"Enter index to play, 'draw' or 'uno'";
	string input;
	getline(cin >> ws, input);
	if(input == "uno"){
		if(playerCount == 1){
			saidUno = true;
			cout << "UNO called!\n";
		}
		else {
			cout << "You don't have 1 card - ignored.\n";
		}
		playerTurn();
		return;
	}
	if(input = "draw"){
		int draw = drawOne();
		if(drawn >=0 ){
			cout <<"You drew: "<<getShort(drawn)<< "\n";
			if(playerCount <MAX_HAND)playerHand[playerCount++] = drawn;
		}
		return;
	}
	int idx = -1;
	if(idx < 0 || idx >=playerCount){
		cout <<"Invalid choice - draw 1 card penalty\n";
		if(plyerCount<MAX_HAND)playerHand[playerCount++] = drawOne();
		return;
	}
	int chosen = playerand[idx];
	if(!canPlay(chosen, discardPile[discardCount-1])){
		cout << "Illegal card- draw 1 card penalty\n";
		if(playerCount<MAX_HAND)playerHand[playerCount++] = drawOne();
		return;
	}
}
discardPile[discardCount++] = chosen;
for(int j = idx; i < playerCount -1 ;++j){
	playerHand[j] = playerHand[j+1];

}playerCount--;
cout << "You played: " << getShort(chosen)<< "\n";
if(playerCount == 0 && !saidUno){
	cout << "You forgot to say UNO! Draw 2 penalty cards.\n";
	if(playerCount +2 <= MAX_HAND){
		playerHand[playerCount++] = drawOne();
		playerHand[playerCount++] = drawOne();

	}
}
saidUno =false;
bool skipNext = false;
    if (isWildCard(chosen)) {
        cout << "Choose new color (r/g/b/y): ";
        char ch; cin >> ch; cin.ignore();
        ch = tolower(ch);
        if (ch == 'r') currentColor = 0;
        else if (ch == 'g') currentColor = 1;
        else if (ch == 'b') currentColor = 2;
        else if (ch == 'y') currentColor = 3;
        else currentColor = rng() % 4;
        cout << "Color is now " << getColorName(currentColor) << "\n";
    }
    int typ = getType(chosen);
    if (typ == 1) { 
        skipNext = true;
        cout << "Skip! (your turn again)\n";
    } else if (typ == 2) { 
        clockwise = !clockwise;
        cout << "Reverse!\n";      
    } else if (typ == 3) { 
        drawPenalty = 2;
        skipNext = true;
        cout << "+2 penalty on yourself!\n";
    } else if (typ == 5) { 
        drawPenalty = 4;
        skipNext = true;
        cout << "Wild +4 penalty on yourself!\n";
	}
bool gameOver(){
	if(playerCount == 0){
		cout << "\n*** YOU WIN! ***\n";
		cout << "All cards played!\n";
		return true;
	}
	return false;
}
bool saveGame(){
	ofstream f(SAVE_FILE);
	if(!f)return false;
	f << drawPenalty << " "<< currentColor << " "<< clockwise << " "<< saidUno;
	f << playerCount << "\n";
	for(int i  = 0; i < playerCount;++i)f << playerHand[i]<< "\n";
	f << discardCount << "\n";
	for(int i = 0;i <discardCount;++i)f << discardPile[i]<< "\n";
	f << drawPileCount << "\n";
	for(int i =0;i< drawPileCount;++i)f << drawPile[i]<< "\n";
	cout << "Game saved.\n";
	return true;
}
bool loadGame(){
	ifstream f(SAVE_FILE);
    if (!f) return false;
    f >> drawPenalty >> currentColor >> clockwise >> saidUno;
    f >> playerCount;
    for (int i = 0; i < playerCount; ++i) f >> playerHand[i];
    f >> discardCount;
    for (int i  = 0; i  < discardCount; ++i) f >> discardPile[i];
    f >> drawPileCount;
    for (int i= 0; i < drawPileCount; ++i) f >> drawPile[i];
    cout << "Game loaded.\n";
    return true;
}
void newGame(){
	Deck();
	playerCount = discardCount = 0;
	drawPenalty =0;
	clockwise =true;
	saidUno = false;
	currentColor = 4;
	for(int i = 0;i <7;++i){
		playerHand[playerCount++] = drawOne();		
	}
	discardPile[discardCount++] = drawOne();
	currentColor = getColor(discardPile[0]);
	cout <<"New game started!\n";
}
int main() {
	return 0;
}
