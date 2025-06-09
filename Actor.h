#ifndef ACTOR_H
#define ACTOR_H
#include"card.h"
#include<vector>
class Actor {
private:
	double hp;
	int emotionalPoint = 0;
	int emotionalLeval = 0;
	double resistance[3];
	int lightCount[2] = { 4,4 };
	int moveCount = 1;
	int speedScope[2] = { 0 };
	Card card[9];
	vector<int> hand = { 0,0,0,0,0,0,0,0,0 };//手牌
	vector<int> deck = { 1,1,1,1,1,1,1,1,1 };//牌庫
	int speed[4] = { 0 };
	int target[4] = { 0 };
	string useCard[4];
public:
	//void test();
	Actor(double, double, double, double, int, int, Card card[]);
	void printState();
	void printCard(int, int);
	int getEmotionalPoint()const;
	int getEmotionalLeval()const;
	double getHp()const;
	int getLightCount(int)const;
	int getMoveCount()const;
	const int* getSpeedScope()const;
	double getResistance(string)const;
	double damageCalculation(string, int);
	void cheakEmotionPoint();//更新情感等級and回光
	void cheakState();//回和開始時更新狀態
	int randNum(int, int);
	void action(Actor&);
	int nameTransInt(string CardName);
	void drawCards();//抽卡機制
	bool clearCut(int);//檢查牌庫(1)or手牌(0)是否淨空
	void autoAction(Actor&);//隨機指定敵方行動槽(0到3)
	int autoPlayCard();//隨機出牌(須符合光芒也就是費用的機制)
	void setSpeed(int,int);//設置每個行動槽的速度值
	int getSpeed(int);//回傳每個行動槽的速度值
	void relight();//回光
	int getTarget(int);
	void setTarget(int, int);//改變目標(位置，值)
	string getUseCard(int);
	Card& getCard(int);
	void costHp(double);
	void catchEmotionalPoint(int, int, int);
	bool canDiscard();
	void printRelightAndNextEmotionalLeval();
};
#endif