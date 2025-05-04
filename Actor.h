#ifndef ACTOR_H
#define ACTOR_H
#include"card.h"
class Actor {
private:
	double hp;
	int emotionalPoint=0;
	int emotionalLeval=0;
	double resistance[3];
	int lightCount=4;
	int moveCount=1;
	int speedScope[2] = {0};
	Card card[9];
public:
	Actor(double, double, double, double,int,int,Card card[]);
	void printState();
	void printCard(int,int);
	int getEmotionalPoint()const;
	int getEmotionalLeval()const;
	int getHp()const;
	int getLightCount()const;
	int getMoveCount()const;
	int getSpeedScope()const;
	double getResistance(string)const;
	int damageCalculation(string, int);
	void cheakEmotionPoint();
	void cheakLightCount();
	void cheakMoveCount();
	void cheakSpeedRage();
};
#endif
