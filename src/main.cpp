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
int drawPilrCount = 0;
int discardCount = 0;
int playerCount = 0;
int aiCount = 0;
int currentColor = 4;
bool clockwise = true;
int currentPlayer = 0;
int drawPenalty = 0;
bool saidUno = false;
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
    return card >= 104 && (card % 2 == 1);
}
int main() {
	return 0;
}
