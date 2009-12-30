#pragma once




class TextCtx : public ArsenalCPP::ARContext
{

public:
		virtual void OnError(int level, const wchar_t *msg)
		{
				if(msg)MessageBoxW(NULL, msg, 0,0);

		}

		virtual void OnFatal(const wchar_t *msg)
		{
				if(msg)MessageBoxW(NULL, msg, 0,0);
		}

		virtual void OnPrint(const wchar_t *msg)
		{
				if(msg)MessageBoxW(NULL, msg, 0,0);
		}

		virtual ~TextCtx()
		{

		}
};



