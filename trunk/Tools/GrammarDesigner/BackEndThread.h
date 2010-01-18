#pragma once



// CBackEndThread

class CBackEndThread : public CWinThread
{
		DECLARE_DYNCREATE(CBackEndThread)
public:
		class CWorkerUnit
		{
		public:
				virtual void Run() = 0;
				virtual ~CWorkerUnit()
				{

				}
		};

private:
		CCriticalSection		m_mtx;
		CList<CWorkerUnit*>		m_work_queue;
public:
		CBackEndThread();           // protected constructor used by dynamic creation
		virtual ~CBackEndThread();
		
public:
		virtual BOOL InitInstance();
		virtual int ExitInstance();
public:
		//此函数不可在thread_func中调用
		BOOL EndThread();
public:
		void	PostWorker(CWorkerUnit *work);
		void	Clear();
protected:
		DECLARE_MESSAGE_MAP()

protected:
		void	OnWorkerUnit(WPARAM wparam, LPARAM lparam);
public:
		virtual int Run();
};


