#pragma once
#include <Windows.h>

class I2CInterface
{
public:
	I2CInterface(void);
	~I2CInterface(void);

	BOOL Write16(BYTE DevAddr, WORD InterAddr, WORD* pData, DWORD dwCount = 1);
	BOOL Read16(BYTE DevAddr, WORD InterAddr, WORD* pData, DWORD dwCount = 1);
	BOOL Write8(BYTE DevAddr, BYTE InterAddr, BYTE* pData, DWORD dwCount = 1);
	BOOL Read8(BYTE DevAddr, BYTE InterAddr, BYTE* pData, DWORD dwCount = 1);

protected:
	BOOL		m_SCL_T, m_SDA_T;
	BOOL		m_SCL_O, m_SDA_O;
	BOOL		m_SCL_I, m_SDA_I;

	void Set(DWORD scl, DWORD sda);
	void Get(void);
	void Start(void);
	void RepeatedStart(void);
	void Stop(void);
	BOOL SendByte(BYTE data);
	BOOL GetByte(BYTE& data, BOOL bLast = FALSE);

};

