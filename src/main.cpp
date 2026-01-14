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

int main() {
	return 0;
}
