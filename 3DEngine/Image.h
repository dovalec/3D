#ifndef __Image__
#define __Image__


#include "Config.h"
#include "ImageFormat.h"


class Image
{
public:
	NEWDEL_DECL
	int mWidth;
	int mHeight;
	int mSize;
	bool mHasAlpha;

	GLubyte * m_pBuffer;

	ImageFormat mImageFormat;

	Image();
	virtual ~Image()
	{
		if (m_pBuffer)
			HeapTlsf::GetTLSF()->TLSF_DEL(m_pBuffer);
	}

	virtual void Allocate(int size);
	virtual void PutValues(GLubyte * values);
	
	inline GLubyte * GetValues()
	{
		return m_pBuffer;
	}

	virtual void PutValuesDirect(GLubyte * values);


	virtual int GetWidth(); 
	virtual void SetWidth(int width);
	virtual int GetHeight(); 
	virtual void SetHeight(int height); 

	bool IsReady()
	{
		return m_pBuffer != NULL;
	}


	virtual ImageFormat GetImageFormat();
	virtual void SetImageFormat(ImageFormat imageFormat);

};


#endif //__Image__
