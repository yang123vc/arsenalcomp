#pragma once

#include "Arsenal.h"
#pragma comment(lib, "Arsenal.lib")



#if defined(AR_SHARED_LIB)
		#if defined(AR_EXPORTS)
				#define ARAPI __declspec(dllexport)
		#else
				#define ARAPI __declspec(dllimport)	
		#endif
#else
		#define ARAPI
#endif




namespace ArsenalCPP{

using namespace ARSpace;

class ARAPI NonCopyable
{
private:
		NonCopyable(const NonCopyable&);
		const NonCopyable& operator=(const NonCopyable&);
protected:
		NonCopyable() { }
		~NonCopyable() { }
}; 



class ARAPI ARContext : private NonCopyable
{
public:
		virtual void OnError(int_t level, const wchar_t *msg) = 0;	
		virtual void OnPrint(const wchar_t *msg) = 0;
		virtual ~ARContext() = 0{}
};





class ARAPI Arsenal : private NonCopyable
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

/*******************************PreDefined**********************************/

class ARAPI DummyContext : public ARContext
{
public:
		virtual void OnError(int_t level, const wchar_t *msg)
		{

		}

		virtual void OnPrint(const wchar_t *msg)
		{

		}
public:
		DummyContext()
		{

		}

		virtual ~DummyContext()
		{

		}
};




}





