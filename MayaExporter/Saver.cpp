#include "StdAfx.h"
#include "Saver.h"




Saver::Saver(void)
{
	mHandle = 0;
}

Saver::~Saver(void)
{
	if (mHandle != 0)
	{
		fclose(mHandle);
	}
}

void Saver::SetFolder(string folder)
{
	mFolder = folder;
}

string Saver::GetFolder()
{
	return mFolder;
}

bool Saver::CreateFile(string fileName)
{
    mHandle = fopen(fileName.c_str(), "w");
    return true;
}

bool Saver::AppendFile(string fileName)
{
    mHandle = fopen(fileName.c_str(), "a");
    return true;
}

void Saver::writeInt(signed int v)
{
	char * p = (char*)&v;
	
	fwrite(  (const char*)p+0, 1, 1, mHandle);
	fwrite(  (const char*)p+1, 1, 1, mHandle);
	fwrite(  (const char*)p+2, 1, 1, mHandle);
	fwrite(  (const char*)p+3, 1, 1, mHandle);
}


void Saver::writeShort(short v)
{
	char * p = (char*)&v;
	
	fwrite(  (const char*)p+0, 1, 1, mHandle);
	fwrite(  (const char*)p+1, 1, 1, mHandle);
}

void Saver::writeUShort(unsigned short v)
{
	char * p = (char*)&v;
	
	fwrite(  (const char*)p+0, 1, 1, mHandle);
	fwrite(  (const char*)p+1, 1, 1, mHandle);
}


void Saver::writeUInt(unsigned int v)
{
	char * p = (char*)&v;
	
	fwrite(  (const char*)p+0, 1, 1, mHandle);
	fwrite(  (const char*)p+1, 1, 1, mHandle);
	fwrite(  (const char*)p+2, 1, 1, mHandle);
	fwrite(  (const char*)p+3, 1, 1, mHandle);
	
}

 void Saver::writeFloat32(float f)  
{
	

	char * p = (char*)&f;
	
	fwrite(  (const char*)p+0, 1, 1, mHandle);
	fwrite(  (const char*)p+1, 1, 1, mHandle);
	fwrite(  (const char*)p+2, 1, 1, mHandle);
	fwrite(  (const char*)p+3, 1, 1, mHandle);
	
}


 
 void Saver::writeDouble(double d)  
{
	char * p = (char*)&d;
	
	fwrite(  (const char*)p, sizeof(double), 1, mHandle);
}

 void Saver::writeChar(char v)
{
	char * p = (char*)&v;
	
	fwrite(  (const char*)p+0, 1, 1, mHandle);
}



 void Saver::writeRGBA(int RGBA)
{	
	

	char * p = (char*)&RGBA;
	
	fwrite(  (const char*)p+0, 1, 1, mHandle);
	fwrite(  (const char*)p+1, 1, 1, mHandle);
	fwrite(  (const char*)p+2, 1, 1, mHandle);
	fwrite(  (const char*)p+3, 1, 1, mHandle);
	
	
}


 void Saver::writeRGB(int RGB)
{	
	char dummy = 0;
	char * p = (char*)&RGB;
	
	fwrite(  (const char*)p+0, 1, 1, mHandle);
	fwrite(  (const char*)p+1, 1, 1, mHandle);
	fwrite(  (const char*)p+2, 1, 1, mHandle);

	// writing dummy char
	fwrite(  (const char*)&dummy, 1, 1, mHandle);
	


}

 void Saver::writeString(const char * txt)
{
	int len = strlen(txt);
	len = len + 4 -(len % 4);
	if (len >= 256)
	{
		err_stop("String too big");
	}

	static char buff[256];
	memset(buff,0,256);
	strcpy(buff, txt);

	writeInt(len);

	
	fwrite(  buff, len, 1, mHandle);
	
}

 void Saver::writeTxt(const char * txt)
{
	int len = strlen(txt);

	
	fwrite(  txt, len, 1, mHandle);
	
}
