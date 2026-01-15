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
int main() {
	return 0;
}
