// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0) throw "negative_length";
	BitLen = len;
	MemLen = (BitLen - 1) / (sizeof(TELEM) * 8) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	if (pMem != NULL)
	{
		delete[] pMem;
		pMem = NULL;
		BitLen = 0;
		MemLen = 0;
	}
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n >= 0)
		return(n / (sizeof(TELEM) * 8));
	else
		throw 1;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return (1 << (n - GetMemIndex(n) * sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return  BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n >= BitLen || n < 0) throw "index_wrong";
	pMem[GetMemIndex(n)] = (pMem[GetMemIndex(n)] | GetMemMask(n));
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n >= BitLen || n < 0) throw "index_wrong";
	pMem[GetMemIndex(n)] = (pMem[GetMemIndex(n)] & ~GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= BitLen || n < 0) throw "negative_index/too_large_index";

	if ((pMem[GetMemIndex(n)] & GetMemMask(n)) != 0)
		return 1;
	else
		return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if ((BitLen == bf.BitLen) && (MemLen == bf.MemLen))
	{
		for (int i = 0; i < MemLen; i++)
		{
			 if (pMem[i] != bf.pMem[i])
				return 0;
		}
		return 1;
	}
	return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (BitLen == bf.BitLen)
	{
		for (int i = 0; i < MemLen; i++)
		{
			if (pMem[i] == bf.pMem[i])
				return 0;
		}
	}
	return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int tmpLen = BitLen;
	if (BitLen < bf.BitLen)
	{
		tmpLen = bf.BitLen;
	}

	TBitField tmp(tmpLen);
	for (int i = 0; i < MemLen; i++)
	{
		tmp.pMem[i] = pMem[i];
	}
	for (int i = 0; i < MemLen; i++)
	{
		(tmp.pMem[i] |= bf.pMem[i]);
	}
	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int tmpLen = BitLen;
	if (BitLen < bf.BitLen)
	{
		tmpLen = bf.BitLen;
	}

	TBitField tmp(tmpLen);
	for (int i = 0; i < MemLen; i++)
	{
		tmp.pMem[i] = pMem[i];
	}
	for (int i = 0; i < MemLen; i++)
	{
		(tmp.pMem[i] &= bf.pMem[i]);
	}
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(BitLen);
	for (int i = 0; i < MemLen; i++)
	{
		if (GetBit(i) == 0)
		{
			tmp.SetBit(i);
		}
		else
		{
			tmp.ClrBit(i);
		}
    }
	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int bit;
	int tmpLen;
	istr >> tmpLen;
	bf = TBitField(tmpLen);
	for (int i = 0; i < bf.BitLen; i++)
	{
		istr >> bit;
		if (bit)
			bf.SetBit(i);
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	int length = bf.GetLength();
	ostr << length << " ";
	for (int i = 0; i < length; i++)
	{
		ostr << bf.GetBit(i) << " ";
	}
	return ostr;
}
