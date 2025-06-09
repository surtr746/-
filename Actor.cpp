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
	cheakState();
	cheakEmotionPoint();
	relight();
	cout << "HP:" << setw(4) << getHp() << ' '
		<< "emotionalLeval:" << setw(4) << getEmotionalLeval() << ' ' << setw(4) << getEmotionalPoint() << ' '
		<< "resistance:" << setw(4) << getResistance("slash") << setw(4) << getResistance("pierce") << setw(4) << getResistance("blunt") << ' '
		<< "lightCount:" << setw(4) << getLightCount(0) << ' ' << setw(4) << getLightCount(1) << ' '
		<< "moveCount:" << setw(4) << getMoveCount() << ' '
		<< "speedScope:" << setw(4) << getSpeedScope()[0] << setw(4) << getSpeedScope()[1] << endl;
	printRelightAndNextEmotionalLeval();
	for (int i = 0; i < getMoveCount();i++) {
		cout << "*******" << '|';
	}
	cout << endl;
	for (int i = 0; i < getMoveCount(); i++) {
		setSpeed(i,randNum(getSpeedScope()[0], getSpeedScope()[1]));
		cout << "*  " <<  getSpeed(i) << "  *" << '|';
	}
	cout << endl;
	for (int i = 0; i < getMoveCount(); i++) {
		cout << "*******" << '|';
	}
	cout << endl;
	for (int i = 0; i < 9;i++) {
		if (hand[i] == 1) {
			printCard(i, 0);
		}
	}
	if (clearCut(1)) {
		cout << "The hand is full. Five cards will be discarded randomly in the next round and put into the deck." << endl;
	}
	cout << endl;
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
//void Actor::test() {
//	cout << "hand: ";
//	for (int i = 0; i < 9;i++) {
//		cout << hand[i] << ' ';
//	}
//	cout << endl;
//	cout << "deck: ";
//	for (int i = 0; i < 9; i++) {
//		cout << deck[i] << ' ';
//	}
//	cout << endl;
//}
int Actor::getEmotionalLeval() const{
	return emotionalLeval;
}
double Actor::getResistance(string n) const{
	if (n == "slash")
		return resistance[0];
	if (n == "pierce")
		return resistance[1];
	if (n == "blunt")
		return resistance[2];
}
int Actor::getEmotionalPoint() const {
	return emotionalPoint;
}
double Actor::getHp()const {
	return hp;
}
int Actor::getLightCount(int i)const {
	return lightCount[i];
}
int Actor::getMoveCount()const {
	return moveCount;
}
const int* Actor::getSpeedScope()const {
	return speedScope;
}
double Actor::damageCalculation(string n, int num) {
	return getResistance(n) * num;
}
void  Actor::cheakState() {
	int num = getEmotionalLeval();
	if (num == 0) {
		lightCount[0] = 4;
		moveCount = 1;
	}
	if (num == 1) {
		lightCount[0] = 4;
		moveCount = 1;
	}
	if (num == 2) {
		lightCount[0] = 5;
		moveCount = 2;
	}
	if (num == 3) {
		lightCount[0] = 6;
		moveCount = 2;
		speedScope[0] = 2;
		speedScope[1] = 7;
	}
	if (num == 4) {
		lightCount[0] = 7;
		moveCount = 3;
	}
	if (num == 5) {
		lightCount[0] = 9;
		moveCount = 4;
		speedScope[0] = 3;
		speedScope[1] = 8;
	}
}
void Actor::cheakEmotionPoint() {
	int num= emotionalPoint;
	int oldLeval = emotionalLeval;
	if (num == 0) {
		emotionalLeval = 0;
	}
	if (num > 0 && num <= 3) {
		emotionalLeval = 1;
	}
	if (num >= 4 && num <= 8) {
		emotionalLeval = 2;
	}
	if (num >= 9 && num <= 15) {
		emotionalLeval = 3;
	}
	if (num >= 16 && num <= 24) {
		emotionalLeval = 4;
	}
	if (num >= 25 && num <= 30) {
		emotionalLeval = 5;
	}
	if (emotionalLeval > oldLeval) {
		cheakState();
		lightCount[1] = lightCount[0]; // 升級時補滿
	}
}
int Actor::randNum(int floor, int ceiling) {
	int num = 0;
	num = rand() % (ceiling - floor + 1);
	return num + floor;
}
void Actor::drawCards() {
	//cout << "hand	";
	//for (int i = 0; i < 9;i++) {
	//	cout << hand[i] << "+";
	//}
	//cout << endl;
	//cout << "deck	";
	//for (int i = 0; i < 9; i++) {
	//	cout << deck[i] << "+";
	//}
	//cout << endl;
	if (clearCut(0)) {//沒牌時抽五張
		cout << "沒手牌抽五張" << endl;
		for (int i = 0; i < 5;) {
			int number = randNum(0,8);
			if (deck[number]==1) {
				deck[number] = 0;
				hand[number] = 1;
				i++;
			}
			if (clearCut(1)) {
				int j = 5 - i;
				for (; j > 0;) {
					int num = randNum(0, 8);
					if (deck[num] == 0) {
						deck[num] = 1;
						j--;
					}
				}
			}
		}
	}
	else if (clearCut(1)) { //特殊抽牌:從手牌上棄牌六張
		cout << "牌庫沒牌抽，從手牌和使用過的牌中棄六張回牌庫" << endl;
		for (int i = 0; i < 6;) {
			int number = randNum(0, 8);
			if (deck[number] == 0 ) {
				deck[number] = 1;
				hand[number] = 0;
				i++;
			}
			if (clearCut(3)) 
				break;
			if (clearCut(0))//手牌都棄掉了
				break;
			if (!canDiscard()) 
				break;
		}
	}
	else if (emotionalLeval == 5) {//情感等級五，抽三張
		cout << "情感等級五，抽三張" << endl;
		for (int i = 0; i < 3;) {
			int number = randNum(0, 8);
			if (deck[number] == 1) {
				deck[number] = 0;
				hand[number] = 1;
				i++;
			}
			if (clearCut(1))
				break;
			if (clearCut(2))
				break;
		}
	}
	else {
		cout << "情感等級小於五，抽兩張" << endl;
		for (int i = 0; i < 2;) {//情感等級在五以下抽兩張
			int number = randNum(0, 8);
			if (deck[number] == 1) {
				deck[number] = 0;
				hand[number] = 1;
				i++;
			}
			if (clearCut(1))
				break;
			if (clearCut(2))
				break;
		}
	}
	//cout << "hand	";
	//for (int i = 0; i < 9; i++) {
	//	cout << hand[i] << "-";
	//}
	//cout << endl;
	//cout << "deck	";
	//for (int i = 0; i < 9; i++) {
	//	cout << deck[i] << "-";
	//}
	//cout << endl;
}
bool Actor::clearCut(int i) { 
	if (i == 1) {//牌庫都被抽完了
		for (int i = 0; i < 9; i++) {
			if (deck[i] == 1)
				return false;
		}
		return true;
	}
	if (i == 0) {//手牌都被抽完了
		for (int i = 0; i < 9; i++) {
			if (hand[i] == 1)
				return false;
		}
		return true;
	}
	if(i == 2) {//手牌都滿了
		for (int i = 0; i < 9; i++) {
			if (hand[i] == 0)
				return false;
		}
		return true;
	}
	if(i == 3) {//牌庫都滿了
		for (int i = 0; i < 9; i++) {
			if (deck[i] == 0)
				return false;
		}
		return true;
	}
}
void Actor::autoAction(Actor& other) { //指定敵方行動槽
	for (int i = 0; i < getMoveCount();i++) {	
		target[i] = randNum(0, other.moveCount-1);
		cout << i << '(' << speed[i] << ')' << "-->" << target[i] << '(' << other.getSpeed(target[i]) << ')' << endl;
		int count = autoPlayCard();
		if (count != 9) {
			useCard[i] = card[count].getName();
			printCard(count, 0);
		}
		else { 
			useCard[i] = "pass";
			cout << "pass" << endl; 
		}
	}
}
int Actor::autoPlayCard() { //隨機出牌
	for (int i = 0; i < 9; i++) {
		int count = randNum(0, 8);
		if (card[count].getLight() <= getLightCount(1) && hand[count] == 1) {
			lightCount[1] -= card[count].getLight();
			hand[count] = 0;
			return count;
		}
	}	
	return 9;//pass(光不夠)
}
void Actor::setSpeed(int count,int num) {
	speed[count] = num;
}
bool Actor::canDiscard() {
	for (int i = 0; i < 9; i++) {
		if (deck[i] == 0 && hand[i] == 1)
			return true;
	}
	return false;
}
int Actor::getSpeed(int count) {
	return speed[count];
}
void Actor::setTarget(int i , int value){
	target[i] = value;
}
void Actor::relight() {
	if (getLightCount(0) == 4) {
		lightCount[1] += 1;
		if (getLightCount(1) > 4)
			lightCount[1] = 4;
	}
	if (getLightCount(0) == 5) {
		lightCount[1] += 1;
		if (getLightCount(1) > 5)
			lightCount[1] = 5;
	}
	if (getLightCount(0) == 6) {
		lightCount[1] += 2;
		if (getLightCount(1) > 6)
			lightCount[1] = 6;
	}
	if (getLightCount(0) == 7) {
		lightCount[1] += 2;
		if (getLightCount(1) > 7)
			lightCount[1] = 7;
	}
	if (getLightCount(0) == 9) {
		lightCount[1] += 3;
		if (getLightCount(1) > 9)
			lightCount[1] = 9;
	}
}
void Actor::action(Actor& other) {
	int tar;
	string useC;
	for (int i = 0; i < getMoveCount();) {
		cout << "第" << i << "行動槽的目標是?" << "(請輸入數字)" << endl;
 		cin >> tar;
		if (tar >= other.getMoveCount() || tar < 0) {
			cout << "輸入位置錯誤，請重新輸入" << endl;
			continue;
		}
		target[i] = tar;
		i++;
		for (int j = i-1; j < getMoveCount();) {
			cout << "第" << j << "行動槽要使用的卡牌是?" << "(請輸入卡牌名稱或pass)" << endl;
			cin >> useC;
			if (cin.fail()) {//發現錯誤就清除狀態並丟棄緩衝區內容
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				cout << "輸入錯誤，請重新輸入" << endl;
				continue;
			}
			if (useC == "pass") {
				useCard[j] = "pass";
				break;
			}
			int cardIndex = nameTransInt(useC);
			if (cardIndex == 9) {
				cout << "卡牌名稱錯誤，請重新輸入" << endl;
				continue;
			}
			else if (hand[cardIndex] != 1) {
				cout << "手牌沒有抽到這張牌，請重新輸入" << endl;
				continue;
			}
			else if (card[cardIndex].getLight() > lightCount[1]) {
				cout << "光芒不足，無法使用這張牌，請重新輸入" << endl;
				continue;
			}
			else {
				useCard[j] = useC;
				lightCount[1] -= card[cardIndex].getLight();
				hand[cardIndex] = 0;
				break;
			}
		}
	}
	
	for (int i = 0; i < getMoveCount(); i++) {
		cout << i << '(' << speed[i] << ')' << "-->" << target[i] << '(' << other.getSpeed(target[i]) << ')' << endl;
		if (useCard[i] != "pass")
			printCard(nameTransInt(useCard[i]), 0);
		else
			cout << "pass" << endl;
	}
}
int Actor::nameTransInt(string CardName) {
	for (int i = 0; i < 9; i++) {
		if (CardName == card[i].getName()) {
			return i;
		}
	}
	return 9;
}
int Actor::getTarget(int i) {
	return target[i];
}
string Actor::getUseCard(int i) {
	return useCard[i];
}
Card& Actor::getCard(int i){
	return card[i];
}
void Actor::costHp(double num){
	hp -= num;
}
void Actor::catchEmotionalPoint(int floor,int ceiling,int point) {
	if (point == floor)
		emotionalPoint += 1;
	if (point == ceiling)
		emotionalPoint += 1;
}
void Actor::printRelightAndNextEmotionalLeval() {
	cout << "當前回合";
	if (getLightCount(0) == 4) {
		cout << "回" << 1 << "光 ";
	}
	if (getLightCount(0) == 5) {
		cout << "回" << 1 << "光 ";
	}
	if (getLightCount(0) == 6) {
		cout << "回" << 2 << "光 ";
	}
	if (getLightCount(0) == 7) {
		cout << "回" << 2 << "光 ";
	}
	if (getLightCount(0) == 9) {
		cout << "回" << 3 << "光 ";
	}
	if (emotionalLeval == 0) {
		cout << "還差" << setw(4) << 1 - getEmotionalPoint() << "  點到達I情感階段" << endl;
	}
	if (emotionalLeval == 1) {
		cout << "還差" << setw(4) << 4 - getEmotionalPoint() << "點到達II情感階段" << endl;
	}
	if (emotionalLeval == 2) {
		cout << "還差" << setw(4) << 9 - getEmotionalPoint() << "點到達III情感階段" << endl;
	}
	if (emotionalLeval == 3) {
		cout << "還差" << setw(4) << 16 - getEmotionalPoint() << "點到達IV情感階段" << endl;
	}
	if (emotionalLeval == 4) {
		cout << "還差" << setw(4) << 25 - getEmotionalPoint() << "點到達V情感階段" << endl;
	}
	if (emotionalLeval == 5) {
		cout << "到達V情感階段，會贏嗎?" << endl;
	}

}