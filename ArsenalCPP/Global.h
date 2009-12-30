#pragma once

#include "Arsenal.h"

#pragma comment(lib, "Arsenal.lib")


namespace ArsenalCPP{

using namespace ARSpace;

class NonCopyable
{
private:
		NonCopyable(const NonCopyable&);
		const NonCopyable& operator=(const NonCopyable&);
protected:
		NonCopyable() { }
		~NonCopyable() { }
}; 



class ARContext : private NonCopyable
{
public:
		virtual void OnError(int_t level, const wchar_t *msg) = 0;	
		virtual void OnPrint(const wchar_t *msg) = 0;
		virtual ~ARContext() = 0{}
};




class Arsenal : private NonCopyable
{
private:
		ARContext		*m_ctx;
private:
		Arsenal(ARContext		*ctx);
		~Arsenal();
public:
		ARContext*	Context()const;
public:
		static Arsenal* Instance(ARContext *ctx);
		static void		UnInstance();
		static Arsenal*	Get();
};






}





