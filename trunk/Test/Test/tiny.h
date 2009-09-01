#pragma once

#include "Arsenal.h"

#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <exception>
using  namespace Arsenal;




class TinyException
{
private:
		std::wstring m_msg;
public:
		TinyException(const std::wstring &msg):m_msg(msg)
		{

		}
		
		virtual ~TinyException()
		{

		}

		const std::wstring& GetMessage()const
		{
				return m_msg;
		}
};