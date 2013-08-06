#ifndef __LOADER__
#define __LOADER__


#include "Config.h"
#include "TL/MyString.h"

class Loader
{
public:
	NEWDEL_DECL
	Loader(void);
	virtual ~Loader(void);

	bool OpenFile(const char * pFileName);
	bool OpenTextFile(const char * pFileName);
	void FreeData();

	inline float ReadFloat()
	{
		float v;
		v = *((float*)m_ptrRead);
		m_ptrRead+=4;
		return v;
	}

	inline int ReadInt()
	{
		int v;
		v = *((int*)m_ptrRead);
		m_ptrRead+=4;
		return v;
	}

	inline unsigned int ReadUInt()
	{
		unsigned int v;
		v = *((unsigned int*)m_ptrRead);
		m_ptrRead+=4;
		return v;
	}

	inline short ReadShort()
	{
		short v;
		v = *((short*)m_ptrRead);
		m_ptrRead+=2;

		return v;
	}

	inline unsigned short ReadUShort()
	{
		unsigned short v;
		v = *((unsigned short*)m_ptrRead);
		m_ptrRead+=2;

		return v;
	}

	inline const String ReadString()
	{
		String str;
		int len = ReadInt();
		
		static char buff[256]={0};
		memset(buff,0,256);
		strncpy(buff, m_ptrRead, len);
		str.Copy(buff);
		m_ptrRead+=len;
		return str;
	}

	inline const char * GetReaderPtr()
	{
		return m_ptrRead;
	}

	inline const int GetDataSize()
	{
		return m_dataSize;
	}

	inline bool Progress(int size)
	{
		if ( (m_ptrRead + size - m_pData) >= m_dataSize )
		{
			return false;
		}

		m_ptrRead+=size;
		return true;
	}


private:
	char * m_pData;
	char * m_ptrRead;
	int m_dataSize;
};


#endif //__LOADER__