#include"Card.h"
#include<iostream>
#include<iomanip>
using namespace std;
Card::Card() {
	light = 0;
	typeCount = 1;
	name = "unknown";
	type = new string[typeCount];
	scope = new int[typeCount * 2];
}
Card::Card(int newLight, int newTypeCount, string newName) {
	light = newLight;
	typeCount = newTypeCount;
	name = newName;
	type = new string[typeCount];
	scope = new int[typeCount * 2];
}
void Card::setType(string cardType,int i) {
	type[i] = cardType;
}
void Card::setScope(int floor,int ceiling,int i) {
	scope[2*i] = floor;
	scope[2*i + 1] = ceiling;
}
int Card::getScope(int num)const {
	return scope[num];
}
Card::Card(const Card& other) {
	light = other.light;
	typeCount = other.typeCount;
	name=other.name;
	type = new string[typeCount];
	scope = new int[typeCount * 2];
	for (int i = 0; i < typeCount; i++) {
		type[i] = other.type[i];
	}
	for (int i = 0; i < typeCount * 2; i++) {
		scope[i] = other.scope[i];
	}
}
Card& Card::operator=(const Card& other) {
	if ( this == &other)
		return *this;
	delete[] type;
	delete[] scope;
	light = other.light;
	typeCount = other.typeCount;
	name = other.name;
	type = new string[typeCount];
	scope = new int[typeCount * 2];
	for (int i = 0; i < typeCount; i++) {
		type[i] = other.type[i];
	}
	for (int i = 0; i < typeCount * 2; i++) {
		scope[i] = other.scope[i];
	}
	return *this;
}
Card::~Card() {
	delete[]type;
	delete[]scope;
}
void Card::printCard() {
	cout << right << setfill(' ');
	cout << light << ' ' << setw(7) << name << endl;
	for (int i = 0; i < typeCount; i++) {
		cout << "  " << setw(8) << type[i] << setw(3) << scope[2 * i] << ' ' << setw(3) << scope[2 * i + 1] << ' ';
	}
	cout << endl;
}
int Card::randNum(int floor,int ceiling) {
	int num=0;
	num = rand() % (ceiling - floor);
	return num + floor;
}
int Card::getTypeCount() const{
	return typeCount;
}