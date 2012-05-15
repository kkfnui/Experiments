#include "StdAfx.h"
#include <exception>
#include "Base_32.h"

CBase32::CBase32(void):
m_strAlphabet("abcdefghijklmnopqrstuvwxyz234567")
{
}

CBase32::~CBase32(void)
{
}

std::string CBase32::Encode( std::string strInput )
{
	std::string strOutput;
	const int nGroupLength = 5;
	int nGroupCount = (strInput.length() + nGroupLength-1) / nGroupLength;
	for (int i = 0; i < nGroupCount; i++)
	{
		INT64 nSum = 0;
		char szBuffer[9];
		ZeroMemory(szBuffer, 9*sizeof(char));
		int rowCounter = 0;
		for (int j = 0; j < nGroupLength; j++)
		{
			if (i*nGroupLength+j+1 > strInput.length())
			{
				break;
			}
			nSum <<= 8;
			char c = strInput[i*nGroupLength + j];
			nSum += c;	
			rowCounter++;
		}
		ConvertInt64ToStringPer5Bit(nSum, szBuffer, rowCounter);
		strOutput.append(szBuffer);
	}
	return strOutput;
}

std::string CBase32::Decode( std::string strInput )
{
    std::string strOutput;
    const int nGroupLength = 8;
    int nGroupCount = (strInput.length() + nGroupLength-1) / nGroupLength;
    for (int i = 0; i < nGroupCount; i++)
    {
        INT64 nSum = 0;
        int rowCounter = 0;
        for (int j = 0; j < nGroupLength; j++)
        {
            if (i*nGroupLength+j+1 > strInput.length())
            {
                break;
            }
            nSum <<= 5;
            size_t nPos = m_strAlphabet.find(strInput[i*nGroupLength + j]);
            if (nPos == std::string::npos)
            {
                ATLASSERT(FALSE);
                throw std::exception("invalid code");
            }
            nSum += nPos;	
            rowCounter++;
        }
        char szBuffer[6];
        ZeroMemory(szBuffer, 6*sizeof(char));
        ConvertInt64ToStringPer8Bit(nSum, szBuffer, rowCounter);
        strOutput.append(szBuffer);
    }
    return strOutput;
}

void CBase32::ConvertInt64ToStringPer5Bit( INT64 nNum, char * str,int nCharLength)
{
	const INT64 nClip = 31;
	int nResultStringLenght = (nCharLength*8 + 4)/5;
	for (int i = 0; i <nResultStringLenght; i++)
	{
		int nMove = nCharLength*8-((i+1)*5);
        if (nMove >= 0)
        {	
            INT64 nResult = nNum & (nClip << nMove);
            int nIndex= (nResult >> nMove);
            str[i] = m_strAlphabet[nIndex];
        }
        else
        {
            INT64 nResult = nNum & (nClip >> (nMove));
            int nIndex = (nResult << (-nMove));
            str[i] = m_strAlphabet[nIndex];
        }
	}
	if (nResultStringLenght < 8)
	{
		str[nResultStringLenght] = 0;
	}
}

void CBase32::ConvertInt64ToStringPer8Bit( INT64 nNum, char * str, int nCharLength )
{
    const INT64 nClip = 255;
 
    int nResultStringLenght = nCharLength * 5 /8;
    for (int i = 0; i <nResultStringLenght; i++)
    {
        int nMove = nCharLength*5-((i+1)*8);
        INT64 nResult = nNum & (nClip << nMove);
        UINT nIndex= nResult >> nMove;
        str[i] = nIndex;
    }
    if (nResultStringLenght < 5)
    {
        str[nResultStringLenght] = 0;
    }
}


