/* Author: Hussain Miyaziwala Class: ECE4122 Last Date Modified: 10/5/19

Description:

HW 3 Part 2 : MAX PRODUCT

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