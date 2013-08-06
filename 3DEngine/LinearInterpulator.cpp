#include "LinearInterpulator.h"

LinearInterpulator::LinearInterpulator()
{

}

void LinearInterpulator::calculatePosition()
{
	position = Start + dx*interpulator;
}

void LinearInterpulator::advance()
{
	interpulator+=interpulatorDelta;

	if (interpulator > 1.0f)
		
	interpulator-=1.0f;

	calculatePosition();
}

float LinearInterpulator::getStart()
{
	return Start;
}

void LinearInterpulator::setStart(float start)
{
	this->Start = Start;

	dx = (this->end - this->Start);
}

float LinearInterpulator::getEnd()
{
	return end;
}

void LinearInterpulator::setEnd(float end)
{
	this->end = end;

	dx = (this->end - this->Start);
}

float LinearInterpulator::getPosition()
{
	return position;
}

float LinearInterpulator::getInterpulator()
{
	return interpulator;
}

void LinearInterpulator::setInterpulator(float interpulator)
{
	this->interpulator = interpulator;
	calculatePosition();
}

float LinearInterpulator::getInterpulatorDelta()
{
	return interpulatorDelta;
}

void LinearInterpulator::setInterpulatorDelta(float interpulatorDelta)
{
	this->interpulatorDelta = interpulatorDelta;
}
}