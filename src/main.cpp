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
#include <cstdlib>
#include <cctype>
using namespace std;

const int DECK_SIZE = 108;
const int MAX_HAND = 30;
const int MAX_DISCARD = DECK_SIZE;

mt19937 randomGenerator(random_device{}());

struct Card {
    int color;
    int type;
    int value;
};


int myStrLen(const char* s) {
    int len = 0;
    while (s[len] != '\0') len++;
    return len;
}

void myStrCopy(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') { dest[i] = src[i]; i++; }
    dest[i] = '\0';
}

bool myStrEqual(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        i++;
    }
    return a[i] == b[i];
}

bool strIsDigit(const char* s) {
    for (int i = 0; s[i]; ++i)
        if (!isdigit(s[i])) return false;
    return true;
}


int getColor(const Card& c) { return c.color; }
int getNumber(const Card& c) { return c.value; }
int getType(const Card& c) { return c.type; }
bool isWildCard(const Card& c) { return c.color == 4; }
bool isWildDrawFour(const Card& c) { return c.color == 4 && c.type == 5; }

void getShort(const Card& c, char* out) {
    int pos = 0;

    char colors[] = { 'R','G','B','Y','W' };
    out[pos++] = colors[c.color];

    if (c.type == 0) out[pos++] = '0' + c.value;
    else if (c.type == 1) out[pos++] = 'S';
    else if (c.type == 2) out[pos++] = 'R';
    else if (c.type == 3) { out[pos++] = '+'; out[pos++] = '2'; }
    else if (c.type == 4) out[pos++] = 'W';
    else if (c.type == 5) { out[pos++] = 'W'; out[pos++] = '+'; out[pos++] = '4'; }


    out[pos] = '\0';
}

const char* getColorName(int col) {
    const char* names[] = { "Red","Green","Blue","Yellow","Wild" };
    return names[col];
}


void createDeck(Card drawPile[], int& drawPileCount, mt19937& rng) {
    drawPileCount = 0;
    for (int i = 0; i < DECK_SIZE; ++i) {
        Card c;
        c.color = (i >= 104) ? 4 : i / 26;
        if (i >= 104) { c.type = (i % 2 == 0) ? 4 : 5; c.value = (c.type == 5) ? 4 : 0; }
        else {
            int idx = i % 26;
            c.type = (idx <= 18) ? 0 : (idx <= 20 ? 1 : (idx <= 22 ? 2 : 3));
            c.value = (c.type == 0) ? (idx == 0 ? 0 : (idx + 1) / 2) : 20;
        }
        drawPile[drawPileCount++] = c;
    }
    shuffle(drawPile, drawPile + DECK_SIZE, rng);
}

Card drawOne(Card drawPile[], int& drawPileCount, Card discardPile[], int& discardCount, mt19937& rng) {
    if (drawPileCount == 0) {
        if (discardCount <= 1) return {4,0,-1};
        Card top = discardPile[discardCount-1];
        drawPileCount = discardCount-1;
        for (int i = 0; i < drawPileCount; ++i) drawPile[i] = discardPile[i];
        discardCount = 1; discardPile[0] = top;
        shuffle(drawPile, drawPile + drawPileCount, rng);
        cout << "Deck reshuffled from discard pile.\n";
    }
    return drawPile[--drawPileCount];
}

bool canPlay(const Card& card, const Card& top, int currentColor) {
    if (isWildCard(card)) return true;
    if (card.color == currentColor) return true;
    if (card.type == 0 && top.type == 0 && card.value == top.value) return true;
    if (card.type == top.type && top.type != 0) return true;
    return false;
}


void printHand(Card hand[], int count, int currentPlayer) {
    cout << (currentPlayer == 0 ? "Player 1 cards:" : "Player 2 cards:") << "\n";
    for (int i = 0; i < count; ++i) {
        char buf[16];
        getShort(hand[i], buf);
        cout << "[" << i << "]" << buf << " ";
        if ((i+1)%6==0) cout << "\n";
    }
    cout << "\n";
}

void printTop(Card discardPile[], int discardCount, int currentColor) {
    if (discardCount==0) { cout << "No top card yet\n"; return; }
    char buf[16]; 
    getShort(discardPile[discardCount-1], buf);
    cout << "Current card: " << buf << "\n";
    cout << "Color to match: " << getColorName(currentColor) << "\n\n";
}


bool saveGame(const char* filename,
              Card playerHand[], Card player2Hand[], Card drawPile[], Card discardPile[],
              int& playerCount, int& player2Count, int& drawPileCount, int& discardCount,
              int& currentColor, int& drawPenalty, int& currentPlayer,
              bool& saidUno, bool& saidUno2, bool& clockwise,
              mt19937& rng) {
    ofstream f(filename);
    if (!f) return false;
    f << drawPenalty << " " << currentColor << " " << clockwise << " "
      << currentPlayer << " " << saidUno << " " << saidUno2 << "\n";
    f << playerCount << "\n";
    for (int i=0;i<playerCount;++i) f << playerHand[i].color<<" "<<playerHand[i].type<<" "<<playerHand[i].value<<"\n";
    f << player2Count << "\n";
    for (int i=0;i<player2Count;++i) f << player2Hand[i].color<<" "<<player2Hand[i].type<<" "<<player2Hand[i].value<<"\n";
    f << discardCount << "\n";
    for (int i=0;i<discardCount;++i) f << discardPile[i].color<<" "<<discardPile[i].type<<" "<<discardPile[i].value<<"\n";
    f << drawPileCount << "\n";
    for (int i=0;i<drawPileCount;++i) f << drawPile[i].color<<" "<<drawPile[i].type<<" "<<drawPile[i].value<<"\n";
    cout << "Game saved to " << filename << "\n";
    return true;
}

bool loadGame(const char* filename,
              Card playerHand[], Card player2Hand[], Card drawPile[], Card discardPile[],
              int& playerCount, int& player2Count, int& drawPileCount, int& discardCount,
              int& currentColor, int& drawPenalty, int& currentPlayer,
              bool& saidUno, bool& saidUno2, bool& clockwise,
              mt19937& rng) {
    ifstream f(filename);
    if (!f) return false;
    f >> drawPenalty >> currentColor >> clockwise >> currentPlayer >> saidUno >> saidUno2;
    f >> playerCount;
    for(int i=0;i<playerCount;++i) f >> playerHand[i].color>>playerHand[i].type>>playerHand[i].value;
    f >> player2Count;
    for(int i=0;i<player2Count;++i) f >> player2Hand[i].color>>player2Hand[i].type>>player2Hand[i].value;
    f >> discardCount;
    for(int i=0;i<discardCount;++i) f >> discardPile[i].color>>discardPile[i].type>>discardPile[i].value;
    f >> drawPileCount;
    for(int i=0;i<drawPileCount;++i) f >> drawPile[i].color>>drawPile[i].type>>drawPile[i].value;
    cout << "Game loaded from "<< filename << "\n";
    return true;
}


bool gameOver(int playerCount, int player2Count) {
    if(playerCount==0){cout<<"\n*** PLAYER 1 WINS! ***\n"; return true;}
    if(player2Count==0){cout<<"\n*** PLAYER 2 WINS! ***\n"; return true;}
    return false;
}
void playerTurn(Card playerHand[], Card player2Hand[], Card drawPile[], Card discardPile[],
                int& playerCount, int& player2Count, int& drawPileCount, int& discardCount,
                int& currentColor, int& drawPenalty, int& currentPlayer,
                bool& saidUno, bool& saidUno2, bool& clockwise,
                mt19937& rng) {

    Card* hand = (currentPlayer == 0) ? playerHand : player2Hand;
    int& count = (currentPlayer == 0) ? playerCount : player2Count;
    bool& unoFlag = (currentPlayer == 0) ? saidUno : saidUno2;

    cout << "\n--- Player " << (currentPlayer+1) << " turn ---\n";

    if(drawPenalty>0){
        cout << "You must draw " << drawPenalty << " cards (penalty)\n";
        for(int i=0;i<drawPenalty && count<MAX_HAND;++i)
            hand[count++] = drawOne(drawPile, drawPileCount, discardPile, discardCount, rng);
        drawPenalty=0;
        currentPlayer = 1-currentPlayer;
        return;
    }

    printTop(discardPile, discardCount, currentColor);
    printHand(hand, count, currentPlayer);

    if(count==1 && !unoFlag)
        cout << "You have ONE card left! Type 'uno' before playing!\n";

    while(true){
        cout << "Enter index, 'draw', 'uno', 'save', or 'exit': ";
        string input;
        getline(cin,input);

        if(input=="exit"){
            char saveName[128];
            cout << "Enter save filename: ";
            cin.getline(saveName,128);
            saveGame(saveName, playerHand, player2Hand, drawPile, discardPile,
                     playerCount, player2Count, drawPileCount, discardCount,
                     currentColor, drawPenalty, currentPlayer, saidUno, saidUno2, clockwise, rng);
            cout << "Game saved. Exiting...\n";
            exit(0);
        }

        if(input=="save"){
            char saveName[128];
            cout << "Enter save filename: ";
            cin.getline(saveName,128);
            saveGame(saveName, playerHand, player2Hand, drawPile, discardPile,
                     playerCount, player2Count, drawPileCount, discardCount,
                     currentColor, drawPenalty, currentPlayer, saidUno, saidUno2, clockwise, rng);
            cout << "Game saved. Continue...\n";
            continue;
        }

        if(input=="uno"){
            if(count==1){unoFlag=true; cout<<"UNO called!\n";}
            else cout<<"You don't have 1 card - ignored.\n";
            continue;
        }

        if(input=="draw"){
            if(count<MAX_HAND){
                hand[count++] = drawOne(drawPile, drawPileCount, discardPile, discardCount, rng);
                char buf[16]; getShort(hand[count-1], buf);
                cout << "You drew: "<<buf<<"\n";
            }
            break;
        }

        if(strIsDigit(input.c_str())){
            int idx = stoi(input);
            if(idx<0 || idx>=count){
                cout << "Invalid choice. Draw 1 card.\n";
                hand[count++] = drawOne(drawPile, drawPileCount, discardPile, discardCount, rng);
                break;
            }

            Card chosen = hand[idx];
            if(!canPlay(chosen, discardPile[discardCount-1], currentColor)){
                cout << "Illegal card. Draw 1 card.\n";
                hand[count++] = drawOne(drawPile, drawPileCount, discardPile, discardCount, rng);
                break;
            }

            discardPile[discardCount++] = chosen;
            currentColor = chosen.color;
            for(int j=idx;j<count-1;++j) hand[j] = hand[j+1];
            count--;

            char buf[16]; getShort(chosen, buf);
            cout << "You played: "<<buf<<"\n";

            if(count==0 && !unoFlag){
                cout << "Forgot UNO! Draw 2 penalty.\n";
                if(count+2<=MAX_HAND){
                    hand[count++] = drawOne(drawPile, drawPileCount, discardPile, discardCount, rng);
                    hand[count++] = drawOne(drawPile, drawPileCount, discardPile, discardCount, rng);
                }
            }
            unoFlag=false;

            switch(chosen.type){
                case 1: currentPlayer=1-currentPlayer; cout<<"Skip!\n"; break;
                case 2: clockwise=!clockwise; cout<<"Reverse!\n"; break;
                case 3: drawPenalty=2; currentPlayer=1-currentPlayer; cout<<"+2 penalty\n"; break;
                case 5: drawPenalty=4; currentPlayer=1-currentPlayer; cout<<"Wild +4 penalty\n"; break;
                case 4: {
                    cout<<"Choose color (r/g/b/y): ";
                    char ch; cin>>ch; cin.ignore();
                    ch=tolower(ch);
                    currentColor=(ch=='r'?0:(ch=='g'?1:(ch=='b'?2:(ch=='y'?3:rng()%4))));
                    cout<<"Color is now "<<getColorName(currentColor)<<"\n";
                    break;
                }
            }

            if(chosen.type!=1 && chosen.type!=3 && chosen.type!=5) currentPlayer=1-currentPlayer;
            break;
        }

        cout << "Invalid input.\n";
    }
}


void newGame(Card playerHand[], Card player2Hand[], Card drawPile[], Card discardPile[],
             int& playerCount, int& player2Count, int& drawPileCount, int& discardCount,
             int& currentColor, int& drawPenalty, int& currentPlayer,
             bool& saidUno, bool& saidUno2, bool& clockwise,
             mt19937& rng) {

    createDeck(drawPile, drawPileCount, rng);
    playerCount = player2Count = discardCount = drawPenalty = 0;
    clockwise = true;
    saidUno = saidUno2 = false;

    for(int i=0;i<7;++i){
        playerHand[playerCount++] = drawOne(drawPile, drawPileCount, discardPile, discardCount, rng);
        player2Hand[player2Count++] = drawOne(drawPile, drawPileCount, discardPile, discardCount, rng);
    }

    discardPile[discardCount++] = drawOne(drawPile, drawPileCount, discardPile, discardCount, rng);
    currentColor = getColor(discardPile[0]);
    currentPlayer=0;

    cout<<"New game started!\n";
}


void gameLoop(Card playerHand[], Card player2Hand[], Card drawPile[], Card discardPile[],
              int& playerCount, int& player2Count, int& drawPileCount, int& discardCount,
              int& currentColor, int& drawPenalty, int& currentPlayer,
              bool& saidUno, bool& saidUno2, bool& clockwise,
              mt19937& rng) {

    while(!gameOver(playerCount, player2Count)){
        playerTurn(playerHand, player2Hand, drawPile, discardPile,
                   playerCount, player2Count, drawPileCount, discardCount,
                   currentColor, drawPenalty, currentPlayer,
                   saidUno, saidUno2, clockwise,
                   rng);
    }
}


int main() {
	Card drawPile[DECK_SIZE], discardPile[MAX_DISCARD];
    Card playerHand[MAX_HAND], player2Hand[MAX_HAND];
    int drawPileCount=0, discardCount=0, playerCount=0, player2Count=0;
    int currentColor=4, drawPenalty=0, currentPlayer=0;
    bool clockwise=true, saidUno=false, saidUno2=false;

    cout<<"--- UNO Game ---\n";

    while(true){
        cout<<"1. New Game\n2. Continue Saved Game\n3. Save & Exit\n4. Exit\n";
        int choice; cin>>choice; cin.ignore();

        if(choice==1){
            newGame(playerHand, player2Hand, drawPile, discardPile,
                    playerCount, player2Count, drawPileCount, discardCount,
                    currentColor, drawPenalty, currentPlayer,
                    saidUno, saidUno2, clockwise, randomGenerator);
            gameLoop(playerHand, player2Hand, drawPile, discardPile,
                     playerCount, player2Count, drawPileCount, discardCount,
                     currentColor, drawPenalty, currentPlayer,
                     saidUno, saidUno2, clockwise, randomGenerator);
        }
        else if(choice==2){
            char filename[128];
            cout<<"Enter save filename: ";
            cin.getline(filename,128);
            if(loadGame(filename, playerHand, player2Hand, drawPile, discardPile,
                        playerCount, player2Count, drawPileCount, discardCount,
                        currentColor, drawPenalty, currentPlayer,
                        saidUno, saidUno2, clockwise, randomGenerator)){
                gameLoop(playerHand, player2Hand, drawPile, discardPile,
                         playerCount, player2Count, drawPileCount, discardCount,
                         currentColor, drawPenalty, currentPlayer,
                         saidUno, saidUno2, clockwise, randomGenerator);
            }
        }
        else if(choice==3){
            char saveName[128];
            cout<<"Enter save filename: ";
            cin.getline(saveName,128);
            saveGame(saveName, playerHand, player2Hand, drawPile, discardPile,
                     playerCount, player2Count, drawPileCount, discardCount,
                     currentColor, drawPenalty, currentPlayer,
                     saidUno, saidUno2, clockwise, randomGenerator);
            cout<<"Game saved. Exiting...\n";
            break;
        }
        else if(choice==4){
            cout<<"Exiting without saving...\n";
            break;
        }
    }

    cout<<"Goodbye!\n";
    return 0;
}
