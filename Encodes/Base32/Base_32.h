#pragma once
#include <string>
class CBase32
{
public:
	CBase32(void);
	~CBase32(void);
	std::string Encode(std::string strInput);
	std::string Decode(std::string strInput);
private:
	void ConvertInt64ToStringPer5Bit(INT64 nNum, char * str,int nCharLength);
    void ConvertInt64ToStringPer8Bit(INT64 nNum, char * str, int nCharLength);
private:
	std::string m_strAlphabet;
};
