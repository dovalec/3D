#ifndef __String__h__
#define __String__h__

#include "../Config.h"
#include "../TLSF/HeapTlsf.h"

class String
{
	NEWDEL_DECL;

public:
	String();
	~String();

	String(const char * str);
	
	String(const String & other);

	String & operator= (const String & other);

	String & operator= (const char * str);

	bool operator== (const String & other);

	bool operator== (const char * str);

	String & operator+= (const String & other);

	String & operator+= (const char * str);


	String operator+ (const String & other);

	String operator+ (const char * str);


	void Copy(const char * str);

	void Append(const char * str);

	int Find(const char * str);

	bool Compare(const char * str);

	void InsertZeroAtChar( char c );
	void InsertZeroAtPosition( int position );

	inline const char * GetPtr() const
	{
		return mStr;
	}


private:
	char * mStr;
};




#endif //__String__h__