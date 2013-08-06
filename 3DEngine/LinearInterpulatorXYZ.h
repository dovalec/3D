
#ifndef __LinearInterpulatorXYZ__
#define __LinearInterpulatorXYZ__


#include "Config.h"
#include "Vector3.h"

class LinearInterpulatorXYZ
{

private:
	Vector3 * Start;
	Vector3 * end;
	Vector3 * position;

	float interpulator = 0.0f;
	float interpulatorAdvance = 0.0f;

	float dx = 0.0f;
	float dy = 0.0f;
	float dz = 0.0f;

protected:

	bool cycle = true;

public:
	LinearInterpulatorXYZ();

	virtual void setCycle(bool cycle);
	virtual bool getCycle();

private:
	void calculatePosition();

public:
	virtual void advance();
	virtual void reverse();
	virtual Vector3 * getStart();
	virtual void setStart(Vector3 * start);
	virtual void setStart(float x, float y, float z);
	virtual Vector3 * getEnd();
	virtual void setEnd(Vector3 * end);
	virtual void setEnd(float x, float y, float z);
	virtual Vector3 * getPosition();
	virtual float getInterpulator();
	virtual void setInterpulator(float interpulator);
	virtual float getInterpulatorAdvance();
	virtual void setInterpulatorAdvance(float interpulatorDelta);


};

#endif //__LinearInterpulatorXYZ__

