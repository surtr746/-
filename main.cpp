#include"Actor.h"
#include"Card.h"
#include<iostream>
#include<ctime>
#include <iomanip>
using namespace std;
int main() {
	srand(time(0));
	int light, typeCount,cardCeiling,cardFloor;
	string name,cardType;
	cout << "enter Card message" << endl;
	Card playerCard[9];
	Card enemyCard[9];
	for (int i = 0; i < 9; i++) {
		cin >> light >> typeCount >> name;
		playerCard[i] = { light,typeCount,name };
		for (int j = 0; j < typeCount; j++) {
			cin >> cardType;
			playerCard[i].setType(cardType, j);
			cin >> cardFloor >> cardCeiling;
			playerCard[i].setScope(cardFloor, cardCeiling, j);
		}
	}
	for (int i = 0; i < 9; i++) {
		cin >> light >> typeCount >> name;
		enemyCard[i] = { light,typeCount,name };
		for (int j = 0; j < typeCount; j++) {
			cin >> cardType;
			enemyCard[i].setType(cardType, j);
			cin >> cardFloor >> cardCeiling;
			enemyCard[i].setScope(cardFloor, cardCeiling, j);
		}
	}
	Actor player{ 100,0.5,1.0,2.0,1,6,playerCard };
	Actor enemy{ 100,1.0,0.5,2.0,1,6,enemyCard };
	player.printState();
	for (int i = 0; i < 9; i++) {
		player.printCard(i,1);
	}
}