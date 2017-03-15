// cppExample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "OtherClass.h"

#include <iostream>
#include <string>
using namespace std;


int main()
{
	cout << OtherClass::helloWorld() << endl;
	cout << OtherClass::pointerExampleStack() << endl;
	cout << OtherClass::pointerExampleHeap() << endl;
	cout << OtherClass::pointerExampleUnique() << endl;
	cout << OtherClass::pointerExampleFunc() << endl;
	cout << OtherClass::twoDArrayExample() << endl;
	cout << OtherClass::flattenedArrayExample() << endl;
}
