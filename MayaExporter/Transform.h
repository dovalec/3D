#pragma once

class Transform
{
public:
	Transform(void);
	~Transform(void);

	void Export();
	void WriteTransforms(MFnTransform & transform);
};
