#include "Loader.h"

NEWDEL_IMPL(Loader)
Loader::Loader(void)
{
	m_pData = NULL;
	m_dataSize = 0;
}

Loader::~Loader(void)
{
	
}

bool Loader::OpenFile(const char * pFileName)
{
	//LOGI("Openning file: %s",pFileName);
	FILE* pFile = fopen(pFileName, "rb");
	
	if (pFile == NULL)
	{
		LOGE("Failed to open file: %s",pFileName);
		//STOP;   
		return false;
	}
	
	// Get the file size
	if (fseek(pFile, 0, SEEK_END))
	{
		LOGE("Failed to SEEK_END file: %s",pFileName);
		STOP;
	}
	m_dataSize = ftell(pFile);
	if (fseek(pFile, 0, SEEK_SET))
	{
		LOGE("Failed to SEEK_SET file: %s",pFileName);
		STOP;
	}

	m_pData = (char*)HeapTlsf::GetTLSF()->TLSF_NEW(m_dataSize);
    size_t totalBytesRead = 0;
    while (totalBytesRead < m_dataSize)
    {
        size_t bytesRead = fread(m_pData + totalBytesRead, 1, 1024 , pFile);
       
        if (bytesRead <= 0)
            break;

        totalBytesRead+=bytesRead;
    }
    
    fclose(pFile);
    
    if (totalBytesRead < m_dataSize)
    {	
        LOGE("Failed to open file correctly totalBytesRead != m_dataSize: %s",pFileName);
        FreeData();
        STOP;
        return false;
    }

	m_ptrRead = m_pData;
	
	return true;
}

bool Loader::OpenTextFile(const char * pFileName)
{

	FILE* pFile = fopen(pFileName, "rb");
	//LOGI("Openning file: %s",pFileName);

	if (pFile == NULL)
	{
		LOGE("Failed to open file: %s",pFileName);
		STOP;
		return false;
	}
	
	// Get the file size
	// Get the file size
	if (fseek(pFile, 0, SEEK_END))
	{
		LOGE("Failed to SEEK_END file: %s",pFileName);
		STOP;
	}
	m_dataSize = ftell(pFile);
	if (fseek(pFile, 0, SEEK_SET))
	{
		LOGE("Failed to SEEK_SET file: %s",pFileName);
		STOP;
	}

	m_pData = (char*)HeapTlsf::GetTLSF()->TLSF_NEW(m_dataSize+1); // Adding 1 byte for \0
    size_t totalBytesRead = 0;
    while (totalBytesRead < m_dataSize)
    {
        size_t bytesRead = fread(m_pData + totalBytesRead, 1, 1024 , pFile);
        
        if (bytesRead <= 0)
            break;
        
        totalBytesRead+=bytesRead;
    }
    
    fclose(pFile);
    
    if (totalBytesRead < m_dataSize)
    {	
        LOGE("Failed to open file correctly totalBytesRead != m_dataSize: %s",pFileName);
        FreeData();
        STOP;
        return false;
    }
    
    m_pData[m_dataSize] = '\0';

	m_ptrRead = m_pData;
	
	return true;
}

void Loader::FreeData()
{
	if (m_pData!=NULL)
	{
		HeapTlsf::GetTLSF()->TLSF_DEL(m_pData);
	}

	m_dataSize = 0;
	m_pData = NULL; 
}

