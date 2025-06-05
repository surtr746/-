#include"Actor.h"
#include"Card.h"
#include<iostream>
#include<ctime>
#include <iomanip>
//連接pass跟卡牌互動++
using namespace std;
void clashPoint(Actor&,Actor&);
void inClash(Actor&, Actor& ,Card&,Card&);
void oneSideAttack(Actor&, Actor&, Card&, int,vector<int> ,string);
int main() {
	srand(time(0));
	int light, typeCount,cardCeiling,cardFloor,touch;
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
	int round = 0;//回合計數
	while (player.getHp() >= 0 && enemy.getHp() >= 0) {
		cout << "round" << round << endl;
		player.drawCards();
		enemy.drawCards();
		cout << "player ";
		player.printState();
		cout << "enemy ";
		enemy.printState();
		//enemy.test();
		cout << "enemy action:";
		enemy.autoAction(player);
		player.action(enemy);
		clashPoint(player,enemy);
		round++;
	}
}
void clashPoint(Actor& player, Actor& enemy) {
	cout << "----------------------------------------------------------------" << endl;
	for (int i = 0; i < player.getMoveCount();i++) {//階段一:改變敵人目標
		if (player.getSpeed(i) > enemy.getSpeed(player.getTarget(i))) {
			//cout << enemy.getTarget(player.getTarget(i)) << "*****";
			enemy.setTarget(player.getTarget(i),i);
			//cout << enemy.getTarget(player.getTarget(i)) << "*****";
		}
	}
	//cout << endl;
	//for (int i = 0; i < player.getMoveCount(); i++) {
	//	cout << player.getTarget(i) << "**";
	//}
	//cout << endl;
	//for (int i = 0; i < enemy.getMoveCount();i++) {
	//	cout << enemy.getTarget(i) << "**";
	//}
	//cout << endl;
	vector<int> sortInClash;//拚點順序
	vector<int> sortOneSideAttack;
	for (int i = 0; i < player.getMoveCount(); i++) {
		if (i == enemy.getTarget(player.getTarget(i))) {
			cout << player.getTarget(i) << "<->" << enemy.getTarget(player.getTarget(i)) << endl;
			sortInClash.push_back(player.getSpeed(i)+enemy.getSpeed(player.getTarget(i)));
		}
	}
	for (int i = 0; i < player.getMoveCount() + enemy.getMoveCount();i++) {
		if (i < player.getMoveCount()) {
			if (i != enemy.getTarget(player.getTarget(i))) {
				sortOneSideAttack.push_back(i);
				sortOneSideAttack.push_back(player.getSpeed(i));
			}
		}
		if (i >= player.getMoveCount()) {
			if (i - player.getMoveCount() != player.getTarget(enemy.getTarget(i - player.getMoveCount()))) {
				sortOneSideAttack.push_back(i);
				sortOneSideAttack.push_back(enemy.getSpeed(i- player.getMoveCount()));
			}
		}
	}
	for (int i = 0; i < sortInClash.size();i++) {
		cout << sortInClash[i] << '/';
	}
	cout << '*';
	cout << endl;
	for (int i = 0; i < sortOneSideAttack.size(); i++) {
		cout << sortOneSideAttack[i] << '/';
	}
	cout << '*';
	cout << endl;
	vector<int> sort = sortInClash;
	for (int i = 0; i < sort.size(); i++) {
		int minIndex = i;
		for (int j = minIndex; j < sort.size();j++) {
			if (sort[minIndex] < sort[j]) {
				int temp = sort[minIndex];
				sort[minIndex] = sort[j];
				sort[j] = temp;
			}
		}
	}
	//for (int i = 0; i < sort.size(); i++) {
	//	cout << sort[i] << '/';
	//}
	//cout << '*';
	//cout << endl;
	sort = sortOneSideAttack;
	for (int i = 0; i < sort.size()/2; i++) {
		int minIndex = i;
		for (int j = minIndex; j < sort.size()/2; j++) {
			if (sort[minIndex * 2 + 1] < sort[j * 2 + 1]) {
				int temp0 = sort[minIndex * 2];
				int temp1 = sort[minIndex * 2 + 1];
				sort[minIndex * 2] = sort[j * 2];
				sort[minIndex * 2 + 1] = sort[j * 2 + 1];
				sort[j * 2] = temp0;
				sort[j * 2 + 1] = temp1;
			}
		}
	}
	/*for (int i = 0; i < sort.size(); i++) {
		cout << sort[i] << '/';
	}
	cout << '*';
	cout << endl;*/
}
void inClash(Actor& player, Actor& enemy,Card& playerCard, Card& enemyCard) {
	vector<int> playerList;
	vector<int> enemyList;
	cout << playerCard.getName() << "<->" << enemyCard.getName() << endl;
	for (int i = 0; i < playerCard.getTypeCount();i++) {
		int n = playerCard.randNum(playerCard.getScope(2 * i), playerCard.getScope(2 * i + 1));
		playerList.push_back(n);
		player.catchEmotionalPoint(playerCard.getScope(2 * i), playerCard.getScope(2 * i + 1),n);
	}
	for (int i = 0; i < enemyCard.getTypeCount(); i++) {
		int n = enemyCard.randNum(enemyCard.getScope(2 * i), enemyCard.getScope(2 * i + 1));
		enemyList.push_back(n);
		enemy.catchEmotionalPoint(enemyCard.getScope(2 * i), enemyCard.getScope(2 * i + 1), n);
	}
	if (playerList.size() > enemyList.size()) {
		for (int i = 0; i < playerCard.getTypeCount() - enemyCard.getTypeCount(); i++) {
			enemyList.push_back(0);
		}
		for (int i = 0; i < playerCard.getTypeCount(); i++) {
			cout << playerList[i] << '*';
		}
		cout << endl;
		for (int i = 0; i < playerCard.getTypeCount(); i++) {
			cout << enemyList[i] << '*';
		}
	}
	if (playerList.size() < enemyList.size()) {
		for (int i = 0; i < enemyCard.getTypeCount() - playerCard.getTypeCount(); i++) {
			playerList.push_back(0);
		}
		for (int i = 0; i < enemyCard.getTypeCount(); i++) {
			cout << playerList[i] << '*';
		}
		cout << endl;
		for (int i = 0; i < enemyCard.getTypeCount(); i++) {
			cout << enemyList[i] << '*';
		}
	}
	cout << endl;
	for (int i = 0; i < playerList.size(); i++) {
		if (playerList[i] == 0) {
			oneSideAttack(player, enemy, enemyCard, i, enemyList, "enemy");
			continue;
		}
		if (enemyList[i] == 0) {
			oneSideAttack(player, enemy, playerCard, i, playerList, "player");
			continue;
		}
		cout << "player roll dice: " << playerCard.getType(i) << ' ' << playerList.at(i) << " enemy roll dice: " << enemyCard.getType(i) << ' ' << enemyList.at(i) << endl;
		if (playerList[i] == enemyList[i]) {
			cout << "點數相等，什麼都沒發生" << endl;
			continue;
		}
		if (playerCard.getType(i) == "defense" && (enemyCard.getType(i) == "defense" || enemyCard.getType(i) == "dodge")) {
			cout << "雙方使用守備，什麼都沒發生" << endl;
			continue;
		}
		if (playerCard.getType(i) == "dodge" && (enemyCard.getType(i) == "defense" || enemyCard.getType(i) == "dodge")) {
			cout << "雙方使用守備，什麼都沒發生" << endl;
			continue;
		}
		if (playerCard.getType(i) == "defense" && (enemyCard.getType(i) == "slash" || enemyCard.getType(i) == "pierce" || enemyCard.getType(i) == "blunt")) {
			if (playerList[i] < enemyList[i]) {
				double num = player.damageCalculation(enemyCard.getType(i), enemyList[i] - playerList[i]);
				cout << "enemy對player造成" << num << "點" << enemyCard.getType(i) << "減免傷害" << endl;
				player.costHp(num);
			}
			if (playerList[i] > enemyList[i]) {
				double num = 2.0 * (playerList[i] - enemyList[i]);
				cout << "player對enemy造成" << num << "點" << "counterAttack" << "傷害" << endl;
				enemy.costHp(num);
			}
		}
		if (playerCard.getType(i) == "dodge" && (enemyCard.getType(i) == "slash" || enemyCard.getType(i) == "pierce" || enemyCard.getType(i) == "blunt")) {
			if (playerList[i] < enemyList[i]) {
				double num = player.damageCalculation(enemyCard.getType(i), enemyList[i]);
				cout << "enemy對player造成" << num << "點" << enemyCard.getType(i) << "傷害" << endl;
				player.costHp(num);
			}
			if (playerList[i] > enemyList[i]) {
				double num = -2.0 * (playerList[i] - enemyList[i]);
				cout << "player回復" << num << "點" << "血量" << endl;
				player.costHp(num);
			}
		}
		if ((playerCard.getType(i) == "slash" || playerCard.getType(i) == "pierce" || playerCard.getType(i) == "blunt") && (enemyCard.getType(i) == "slash" || enemyCard.getType(i) == "pierce" || enemyCard.getType(i) == "blunt")) {
			if (playerList[i] > enemyList[i]) {
				double num = enemy.damageCalculation(playerCard.getType(i), playerList[i]);
				cout << "player對enemy造成" << num << "點" << playerCard.getType(i) << "傷害" << endl;
				enemy.costHp(num);
			}
			if (playerList[i] < enemyList[i]) {
				double num = player.damageCalculation(enemyCard.getType(i), enemyList[i]);
				cout << "enemy對player造成" << num << "點" << enemyCard.getType(i) << "傷害" << endl;
				player.costHp(num);
			}
		}
		if ((playerCard.getType(i) == "slash" || playerCard.getType(i) == "pierce" || playerCard.getType(i) == "blunt") && enemyCard.getType(i) == "defense") {
			if (playerList[i] > enemyList[i]) {
				double num = enemy.damageCalculation(playerCard.getType(i), playerList[i] - enemyList[i]);
				cout << "player對enemy造成" << num << "點" << playerCard.getType(i) << "減免傷害" << endl;
				enemy.costHp(num);
			}
			if (playerList[i] < enemyList[i]) {
				double num = 2.0 * (enemyList[i] - playerList[i]);
				cout << "enemy對player造成" << num << "點" << "counterAttack" << "傷害" << endl;
				player.costHp(num);
			}
		}
		if ((playerCard.getType(i) == "slash" || playerCard.getType(i) == "pierce" || playerCard.getType(i) == "blunt") && enemyCard.getType(i) == "dodge") {
			if (playerList[i] > enemyList[i]) {
				double num = enemy.damageCalculation(playerCard.getType(i), playerList[i]);
				cout << "player對enemy造成" << num << "點" << playerCard.getType(i) << "傷害" << endl;
				enemy.costHp(num);
			}
			if (playerList[i] < enemyList[i]) {
				double num = -2.0 * (enemyList[i] - playerList[i]);
				cout << "enemy回復" << num << "點" << "血量" << endl;
				enemy.costHp(num);
			}
		}
	}
}
void oneSideAttack(Actor& player, Actor& enemy, Card& card, int i,vector<int> list,string user) {
	for (; i < card.getTypeCount(); i++) {
		if ( user == "player") {
			cout << "player roll dice: " << card.getType(i) << ' ' << list.at(i) << " enemy roll dice: " << "空類型 " << "0" << endl;
			if (card.getType(i) == "slash" || card.getType(i) == "pierce" || card.getType(i) == "blunt") {
				double num= enemy.damageCalculation(card.getType(i), list[i]);
				cout << "player" << "對" << "enemy" << "單方面使用" << card.getType(i) << "造成" << num << "點傷害" << endl;
				enemy.costHp(num);
			}
			if (card.getType(i)=="defense" || card.getType(i) == "dodge") {
				cout << "player單方面使用" << card.getType(i) << "沒有作用" << endl;
			}
		}
		if (user == "enemy") {
			cout << "player roll dice: " << "空類型 " << "0" << " enemy roll dice: " << card.getType(i) << ' ' << list.at(i) << endl;
			if (card.getType(i) == "slash" || card.getType(i) == "pierce" || card.getType(i) == "blunt") {
				double num = player.damageCalculation(card.getType(i), list[i]);
				cout << "enemy" << "對" << "player" << "單方面使用" << card.getType(i) << "造成" << num << "點傷害" << endl;
				player.costHp(num);
			}
			if (card.getType(i) == "defense" || card.getType(i) == "dodge") {
				cout << "enemy單方面使用" << card.getType(i) << "沒有作用" << endl;
			}
		}
	}
}
