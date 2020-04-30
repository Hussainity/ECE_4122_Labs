/* Author: Hussain Miyaziwala Class: ECE4122 Last Date Modified: 9/23/19
Description:

HW 2 Part 2 : MAX PRODUCT

*/

#pragma once
#include <iostream>
#include <string>
#include <deque>

using namespace std;
class prodBuff {
public:
	prodBuff();
	double MAX;
	deque<double> buffer;
	void process(double);
	void reset();
	void calcMax();
};