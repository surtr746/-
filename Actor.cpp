#include"Actor.h"
#include<iostream>
#include<iomanip>
using namespace std;
Actor::Actor(double newHp, double slash, double pierce, double blunt,int speedFloor,int speedCeiling,Card newCard[]) {
	hp = newHp;
	resistance[0] = slash;
	resistance[1] = pierce;
	resistance[2] = blunt;
	speedScope[0] = speedFloor;
	speedScope[1] = speedCeiling;
	for (int i = 0; i < 9; i++) {
		card[i] = newCard[i];
	}
}
void Actor::printState() {
	cheakEmotionPoint();
	cheakEmotionPoint();
	cheakMoveCount();
	cheakLightCount();
	cout << "HP:" << setw(4) << getHp() << ' '
		<< "emotionalLeval:" << setw(4) << getEmotionalLeval() << ' '
		<< "resistance:" << setw(4) << getResistance("slash") << setw(4) << getResistance("pierce") << setw(4) << getResistance("blunt") << ' '
		<< "lightCount:" << setw(4) << getLightCount() << ' '
		<< "moveCount:" << setw(4) << getMoveCount() << ' '
		<< "speedScope:" << setw(4) << getSpeedScope()-6 << setw(4) << getSpeedScope() -1<< endl;

}
void Actor::printCard(int num,int needRand) {
	card[num].printCard();
	if (needRand == 1) {
		for (int i = 0; i < card[num].getTypeCount(); i++) {
			cout << card[num].randNum(card[num].getScope(2*i), card[num].getScope(2*i+1)) << ' ';
		}
		cout << endl;
	}
}
int Actor::getEmotionalLeval() const{
	return emotionalLeval;
}
double Actor::getResistance(string n) const{
	if (n == "slash")
		return resistance[0];
	else if (n == "pierce")
		return resistance[1];
	else
		return resistance[2];
}
int Actor::getEmotionalPoint() const {
	return emotionalPoint;
}
int Actor::getHp()const {
	return hp;
}
int Actor::getLightCount()const {
	return lightCount;
}
int Actor::getMoveCount()const {
	return moveCount;
}
int Actor::getSpeedScope()const {
	return speedScope[0]+speedScope[1];
}
int Actor::damageCalculation(string n, int num) {
	return getResistance(n) * num;
}
void  Actor::cheakLightCount() {
	int num = getEmotionalLeval();
	if (num == 0)
		lightCount = 4;
	if (num == 1)
		lightCount = 5;
	if (num == 2)
		lightCount = 6;
	if (num == 3)
		lightCount = 7;
	if (num == 4)
		lightCount = 8;
	if (num == 5)
		lightCount = 10;
}
void  Actor::cheakMoveCount() {
	int num = getEmotionalLeval();
	if (num == 0)
		moveCount = 1;
	if (num == 1)
		moveCount = 1;
	if (num == 2)
		moveCount = 2;
	if (num == 3)
		moveCount = 2;
	if (num == 4)
		moveCount = 3;
	if (num == 5)
		moveCount = 4;
}
void  Actor::cheakSpeedRage() {
	int num = getEmotionalLeval();
	if (num == 3) {
		speedScope[0] += 1;
		speedScope[1] += 1;
	}
	if (num == 5) {
		speedScope[0] += 1;
		speedScope[1] += 1;
	}
}
void Actor::cheakEmotionPoint() {
	int num= emotionalPoint;
	if (num > 0 && num <= 3)
		emotionalLeval = 1;
	if (num >= 4 && num <= 8)
		emotionalLeval = 2;
	if (num >= 9 && num <= 15)
		emotionalLeval = 3;
	if (num >= 16 && num <= 24)
		emotionalLeval = 4;
	if (num >= 25 && num <= 30)
		emotionalLeval = 5;
}
