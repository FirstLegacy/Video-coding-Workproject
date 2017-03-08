
#include "stdafx.h"
#include "OtherClass.h"

string OtherClass::helloWorld() {
	return "Hello world!";
}

int OtherClass::pointerExampleStack() {
	// Pointer to stack/automatic variable.
	int i = 0;
	int* p = &i;
	*p = 10;
	
	return i;
}

int OtherClass::pointerExampleHeap() {
	// Pointer to heap/dynamic variable, manual deletion.
	int* p = new int(0);
	*p = 20;

	int i = *p;

	delete p;
	p = nullptr;

	return i;
}

int OtherClass::pointerExampleUnique() {
	// Better pointer to heap/dynamic memory.
	// shared_ptr for more than one copy.
	// To reassign: p2 = move(p1);
	unique_ptr<int> p(new int(0));

	*p = 30;

	return *p;
}

int OtherClass::pointerExampleFunc() {
	// Can also be used in a function.
	// Also works with the other types of pointers.
	unique_ptr<int> p(new int(0));

	*p = 30;
	plusFive(*p);

	return *p;
}

void OtherClass::plusFive(int& val) {
	val += 5;
}

string OtherClass::arrayExample() {
	int exampleArr[2][3] = { { 1, 2, 3 },{ 4, 5, 6 } };
	string s = "";

	for (auto &y : exampleArr)
	{
		for (auto &x : y) {
			s = s + to_string(x) + " ";
		}
		s = s + "\n";
	}
	s = s.substr(0, s.length() - 2);
	return s;
}