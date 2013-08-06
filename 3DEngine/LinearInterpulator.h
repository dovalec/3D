#ifndef __LinearInterpulator__
#define __LinearInterpulator__


#include "Config.h"

class LinearInterpulator
{

private:
	float mStart;
	float mEnd;
	float mPosition;

	float mInterpulator = 0.0f;
	float mInterpulatorDelta = 0.0f;
	float mDx = 0.0f;


public:
	LinearInterpulator();

private:
	void calculatePosition();

public:
	virtual void advance();
	virtual float getStart();
	virtual void setStart(float start);
	virtual float getEnd();
	virtual void setEnd(float end);
	virtual float getPosition();
	virtual float getInterpulator();
	virtual void setInterpulator(float interpulator);
	virtual float getInterpulatorDelta();
	virtual void setInterpulatorDelta(float interpulatorDelta);


};
#endif //__LinearInterpulator__
