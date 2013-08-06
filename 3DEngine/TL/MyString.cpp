
#include "MyString.h"

NEWDEL_IMPL(String);

String::String()
{
	mStr = NULL;
}

String::~String()
{
	HeapTlsf::GetTLSF()->TLSF_DEL(mStr);
}


String::String(const char * str)
{
	mStr = NULL;
	Copy(str);
}


String::String(const String & other)
{
	mStr = NULL;
	const char * str =  other.GetPtr();
	Copy(str);
}

String & String::operator= (const String & other)
{
	if (this != &other) // protect against invalid self-assignment
	{
		const char * str =  other.GetPtr();
		Copy(str);
	}
	return *this;
}

String & String::operator= (const char * str)
{
	Copy(str);
	return *this;
}

bool String::operator== (const String & other)
{
	if (this != &other) // protect against invalid self-assignment
	{
		const char * str =  other.GetPtr();
		return Compare(str);
	}
	return true;
}

bool String::operator== (const char * str)
{
	return Compare(str);
}


String & String::operator+= (const String & other)
{
	if (this != &other) // protect against invalid self-assignment
	{
		const char * str =  other.GetPtr();
		Append(str);
	}
	return *this;
}

String & String::operator+= (const char * str)
{
	Append(str);
	return *this;
}

String String::operator+ (const String & other)
{
	String newstr(mStr);

	const char * str =  other.GetPtr();
	newstr.Append(str);

	return newstr;
}

String String::operator+ (const char * str)
{
	String newstr(mStr);
	newstr.Append(str);
	return newstr;
}

void String::Copy(const char * str)
{
	int len = strlen(str);		
	if (mStr)
		HeapTlsf::GetTLSF()->TLSF_DEL(mStr);

	mStr = (char*)HeapTlsf::GetTLSF()->TLSF_NEW(len+1);
	strcpy(mStr, str);
}


void String::Append(const char * str)
{
	if (mStr == NULL)
	{
		Copy(str);
		return;
	}

	static char buff[512]={0};
	memset(buff,0,512);
	strcat(buff, mStr);
	strcat(buff, str);
	Copy(buff);
}

bool String::Compare(const char * str)
{
	if (!strcmp(mStr, str))
		return true;

	return false;
}

int String::Find(const char * str)
{
	const char * p = strstr(mStr, str);
	if (p == NULL)
		return -1;

	return str - mStr;
}

void String::InsertZeroAtChar(char c)
{
	int length = strlen(mStr);
	for (int n=0;n<length;n++)
	{
		if (mStr[n] == c)
		{
			InsertZeroAtPosition(n);
			break;
		}
	}
}


void String::InsertZeroAtPosition( int position )
{
	if (position >= strlen(mStr))
	{
		STOP;
	}

	mStr[position] = '\0';
}
