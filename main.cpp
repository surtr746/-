#include"Actor.h"
#include"Card.h"
#include<iostream>
#include<ctime>
#include <iomanip>
//連接pass跟卡牌互動++
using namespace std;
void clashPoint(Actor&,Actor&);
void inClash(Actor&, Actor& ,Card&,Card&);
void oneSideAttack(Actor&, Actor&, Card&, int,string);
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
		cout << "enemy action:" << endl;
		enemy.autoAction(player);
		player.action(enemy);
		clashPoint(player,enemy);
		round++;
	}
	cout << "遊戲結束！" << endl;
	if (player.getHp() < 0 && enemy.getHp() < 0) {
		cout << "雙方同時倒下，平手！" << endl;
	}
	else if (player.getHp() < 0) {
		cout << "你輸了！" << endl;
	}
	else if (enemy.getHp() < 0) {
		cout << "你贏了！" << endl;
	}
	cout << "yl3h0 t 2l41l3";
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
	vector<int> sortOneSideAttack;//單方面攻擊順序
	for (int i = 0; i < player.getMoveCount(); i++) {
		if (i == enemy.getTarget(player.getTarget(i))) {
			cout << i << "<->" << player.getTarget(i) << endl;//我方第i行動槽和敵方第?行動槽拚點
			sortInClash.push_back(i);
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
	vector<int> sort = sortInClash;///拚點排序(兩方速度和相加)速度高的先拚點
	for (int i = 0; i < sort.size() / 2; i++) {
		int minIndex = i;
		for (int j = minIndex; j < sort.size() / 2; j++) {
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
	for (int i = 0; i < sort.size(); i++) {
		cout << sort[i] << '/';
	}
	cout << '*';
	cout << endl;
	for (int i = 0; i < sort.size() / 2; i++) {
		int idx = sort[2 * i]; // 直接取行動槽編號
		string p = player.getUseCard(idx);
		string n = enemy.getUseCard(player.getTarget(idx));
		if (p == "pass" && n != "pass") {
			oneSideAttack(player, enemy, enemy.getCard(enemy.nameTransInt(n)), 0, "enemy");
		}
		else if (p != "pass" && n == "pass") {
			oneSideAttack(player, enemy, player.getCard(player.nameTransInt(p)), 0, "player");
		}
		else if (p == "pass" && n == "pass") {
			cout << "雙方都pass，無事發生" << endl;
		}
		else {
			cout << p << "<->" << n << endl;
			inClash(player, enemy, player.getCard(player.nameTransInt(p)), enemy.getCard(enemy.nameTransInt(n)));
		}
	}
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
	for (int i = 0; i < sort.size(); i++) {
		cout << sort[i] << '/';
	}
	cout << '*';
	cout << endl;
	for (int i = 0; i < sort.size() / 2;i++) {
		if (sort[2*i] < player.getMoveCount()) {
			string p = player.getUseCard(sort[2*i]);
			if (p != "pass") {
				cout << p << endl;
				oneSideAttack(player, enemy, player.getCard(player.nameTransInt(p)), 0, "player");
			}
			else
				cout << "player pass無事發生" << endl;
		}
		if (sort[2 * i] >= player.getMoveCount()) {
			string p = enemy.getUseCard(sort[2 * i] - player.getMoveCount());
			if (p != "pass") {
				cout << p << endl;
				oneSideAttack(player, enemy, enemy.getCard(enemy.nameTransInt(p)), 0, "enemy");
			}
			else
				cout << "enemy pass無事發生" << endl;
		}
	}
}
void inClash(Actor& player, Actor& enemy,Card& playerCard, Card& enemyCard) {
	vector<int> playerList;
	vector<int> enemyList;
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
			oneSideAttack(player, enemy, enemyCard, i, "enemy");
			continue;
		}
		if (enemyList[i] == 0) {
			oneSideAttack(player, enemy, playerCard, i, "player");
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
				cout << "player回復" << -num << "點" << "血量" << endl;
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
				cout << "enemy回復" << -num << "點" << "血量" << endl;
				enemy.costHp(num);
			}
		}
	}
}
void oneSideAttack(Actor& player, Actor& enemy, Card& card, int i,string user) {
	vector <int> list;
	for (int j = 0; j < card.getTypeCount(); j++) {
		int n = card.randNum(card.getScope(2 * j), card.getScope(2 * j + 1));
		list.push_back(n);
		if (i == 0) {//由inClash傳入時不可額外獲得情感點數，不過卡牌點數會變
			if (user == "player")
				player.catchEmotionalPoint(card.getScope(2 * j), card.getScope(2 * j + 1), n);
			if (user == "enemy")
				enemy.catchEmotionalPoint(card.getScope(2 * j), card.getScope(2 * j + 1), n);
		}
	}
	if (i == 0 ) {
		for (int k=i; k < card.getTypeCount();k++) {
			if (user == "player") {
				cout << "player roll dice: " << card.getType(k) << ' ' << list.at(k) << " enemy roll dice: " << "空類型 " << "0" << endl;
				if (card.getType(k) == "slash" || card.getType(k) == "pierce" || card.getType(k) == "blunt") {
					double num = enemy.damageCalculation(card.getType(i), list[k]);
					cout << "player" << "對" << "enemy" << "單方面使用" << card.getType(k) << "造成" << num << "點傷害" << endl;
					enemy.costHp(num);
				}
				if (card.getType(k) == "defense" || card.getType(k) == "dodge") {
					cout << "player單方面使用" << card.getType(k) << "沒有作用" << endl;
				}
			}
			if (user == "enemy") {
				cout << "player roll dice: " << "空類型 " << "0" << " enemy roll dice: " << card.getType(k) << ' ' << list.at(k) << endl;
				if (card.getType(k) == "slash" || card.getType(k) == "pierce" || card.getType(k) == "blunt") {
					double num = player.damageCalculation(card.getType(k), list[k]);
					cout << "enemy" << "對" << "player" << "單方面使用" << card.getType(k) << "造成" << num << "點傷害" << endl;
					player.costHp(num);
				}
				if (card.getType(k) == "defense" || card.getType(k) == "dodge") {
					cout << "enemy單方面使用" << card.getType(k) << "沒有作用" << endl;
				}
			}
		}
	}
	else if ( i < card.getTypeCount() ) {
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