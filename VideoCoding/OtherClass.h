#pragma once

#include <iostream>
#include <memory>
#include <string>
using namespace std;

class OtherClass
{
public:
	static string helloWorld();
	static int pointerExampleStack();
	static int pointerExampleHeap();
	static int pointerExampleUnique();
	static int pointerExampleFunc();
	static string twoDArrayExample();
	static string flattenedArrayExample();
private:
	static void plusFive(int&);
};
