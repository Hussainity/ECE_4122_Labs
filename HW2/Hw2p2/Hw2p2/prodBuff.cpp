/* Author: Hussain Miyaziwala Class: ECE4122 Last Date Modified: 10/5/19

Description:

HW 3 Part 2 : MAX PRODUCT

*/

#include "prodBuff.h"
#include <limits>


prodBuff::prodBuff()
{
	MAX = -1 * std::numeric_limits<double>::max();
}

void prodBuff::process(double d)
{
	buffer.push_back(d);

	if (buffer.size() > 4)
	{
		buffer.pop_front();
	}
	if (buffer.size() == 4)
	{
		calcMax();
	}

}

void prodBuff::reset()
{
	buffer.clear();
}


void prodBuff::calcMax()
{
	double localMax = 1;
	for (int i = 0; i < 4; i++)
	{
		localMax *= buffer[i];
	}
	MAX = localMax > MAX ? localMax : MAX;
}

