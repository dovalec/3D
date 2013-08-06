#include "LinearInterpulatorXYZ.h"



LinearInterpulatorXYZ::LinearInterpulatorXYZ()
			{
		Start = new Vector3();
		end = new Vector3();
		position = new Vector3();
			}

void LinearInterpulatorXYZ::setCycle(bool cycle)
			{
		this->cycle = cycle;
			}

bool LinearInterpulatorXYZ::getCycle()
			{
		return this->cycle;
			}

void LinearInterpulatorXYZ::calculatePosition()
			{
		position->X = Start::X + dx*interpulator;
		position->Y = Start::Y + dy*interpulator;
		position->Z = Start::Z + dz*interpulator;
			}

void LinearInterpulatorXYZ::advance()
			{
		interpulator+=interpulatorAdvance;
	
		if (cycle && (interpulator > 1.0f))
		{
			interpulator-= Math::Floor(interpulator);
		}
	
		calculatePosition();
			}

void LinearInterpulatorXYZ::reverse()
			{
		interpulator-=interpulatorAdvance;
	
		if (cycle && (interpulator < 0.0f))
		{
			interpulator += -1.0f*Math::Floor(interpulator);
		}
	
		calculatePosition();
			}

Vector3 * LinearInterpulatorXYZ::getStart()
			{
		return Start;
			}

void LinearInterpulatorXYZ::setStart(Vector3 * start)
			{
		this->Start->Set(Start);
	
		dx = (this->end->X - this->Start->X);
		dy = (this->end->Y - this->Start->Y);
		dz = (this->end->Z - this->Start->Z);
			}

void LinearInterpulatorXYZ::setStart(float x, float y, float z)
			{
		this->Start->Set(x,y,z);
	
		dx = (this->end->X - this->Start->X);
		dy = (this->end->Y - this->Start->Y);
		dz = (this->end->Z - this->Start->Z);
			}

Vector3 * LinearInterpulatorXYZ::getEnd()
			{
		return end;
			}

void LinearInterpulatorXYZ::setEnd(Vector3 * end)
			{
		this->end->Set(end);
	
		dx = (this->end->X - this->Start->X);
		dy = (this->end->Y - this->Start->Y);
		dz = (this->end->Z - this->Start->Z);
			}

void LinearInterpulatorXYZ::setEnd(float x, float y, float z)
			{
		this->end->Set(x,y,z);
	
		dx = (this->end->X - this->Start->X);
		dy = (this->end->Y - this->Start->Y);
		dz = (this->end->Z - this->Start->Z);
			}

Vector3 * LinearInterpulatorXYZ::getPosition()
			{
		return position;
			}

float LinearInterpulatorXYZ::getInterpulator()
			{
		return interpulator;
			}

void LinearInterpulatorXYZ::setInterpulator(float interpulator)
			{
		this->interpulator = interpulator;
		calculatePosition();
			}

float LinearInterpulatorXYZ::getInterpulatorAdvance()
			{
		return interpulatorAdvance;
			}

void LinearInterpulatorXYZ::setInterpulatorAdvance(float interpulatorDelta)
			{
		this->interpulatorAdvance = interpulatorDelta;
			}
}