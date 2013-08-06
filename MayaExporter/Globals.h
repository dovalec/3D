#pragma once

class Globals
{
private:
	Globals(void);
	~Globals(void);

public:
	inline static Globals & GetGlobals()
	{
		static Globals globals;
		return globals;
	}

	string TARGET;
	float SCALE;

};
