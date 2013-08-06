#pragma once


#include <string>
#include <map>
//#include <windows.h>
#include "Globals.h"

using namespace std;

class Saver
{
public:
	Saver(void);
public:
	virtual ~Saver(void);

	void SetFolder(string folder);
	string GetFolder();
    
    
	bool CreateFile(string fileName);

    bool AppendFile(string fileName);
   
    
	void writeInt(signed int v);
	void writeUInt(unsigned int v);
	
	void writeShort(short v);
	void writeUShort(unsigned short v);
	
	void writeFloat32(float f);
	void writeDouble(double d);  
	
	void writeChar(char v);

	void writeRGBA(int RGBA);
	void writeRGB(int RGB);
	
	void writeString(const char * txt);
	void writeTxt(const char * txt);

private: 
	FILE * mHandle;
	string mFolder;
};

