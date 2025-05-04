#ifndef CARD_H
#define CARD_H
#include<string>
using namespace std;
class Card {
private:
	int light=0;
	int typeCount=1;
	string name;
	string* type=nullptr;
	int* scope =nullptr;
public:
	Card();
	Card(int,int,string);
	void setType(string,int);
	void setScope(int, int,int);
	int getScope(int)const;
	Card(const Card& other);
	Card& operator=(const Card& other);
	~Card();
	void printCard();
	int randNum(int,int);
	int getTypeCount()const;
};
#endif
