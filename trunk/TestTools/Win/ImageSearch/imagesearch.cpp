

#include "imagesearch.h"
#include "utilities.h"
#include "window_capture.h"


/**************************************************************************************************************/

static void AR_STDCALL __on_error(ar_int_t level, const wchar_t* msg, void *ctx)
{
		AR_UNUSED(level);
		AR_UNUSED(ctx);
        
        //printf("%ls", msg);
//#if defined(AR_DEBUG)
		::OutputDebugStringW(msg);
//#endif

}




static void AR_STDCALL __on_print(const wchar_t *msg, void *ctx)
{
		/*AR_UNUSED(msg);*/
		AR_UNUSED(ctx);
        
		//printf("%ls", msg);
//#if defined(AR_DEBUG)
		::OutputDebugStringW(msg);
//#endif

}



static USHORT (WINAPI *pfCaptureStackBackTrace)(ULONG FramesToSkip, ULONG FramesToCapture, PVOID *BackTrace,PULONG BackTraceHash) = NULL;

static void setup_debugtrace_api()
{
		HMODULE hk32 = ::LoadLibraryW(L"kernel32.dll");
		if(hk32 == NULL)
		{
				return;
		}

		pfCaptureStackBackTrace = (USHORT (WINAPI*)(ULONG,  ULONG, PVOID*,PULONG))::GetProcAddress(hk32, "RtlCaptureStackBackTrace");

		if(pfCaptureStackBackTrace == NULL)
		{
				
		}

}


static size_t AR_STDCALL __build_backtrace(void **callstack, size_t callstack_cnt)
{
		AR_ASSERT(callstack != NULL && callstack_cnt > 0);

		if(pfCaptureStackBackTrace)
		{
				return (size_t)pfCaptureStackBackTrace(1, (ULONG)callstack_cnt, callstack, NULL);
		}else
		{
				return 0;
		}
}

static size_t	AR_STDCALL __build_backtrace_symbol(void **callstack, size_t callstack_cnt, char *str, size_t len)
{
		unsigned int	i;
		SYMBOL_INFO		*symbol;
		HANDLE			process;
		char			*p;
		size_t			remain;
		
		AR_ASSERT(callstack != NULL && str != NULL && len > 0);
		
		if(callstack_cnt == 0)
		{
				str[0] = '\0';
                return 0;
		}

		process = GetCurrentProcess();

		SymInitialize( process, NULL, TRUE );
		symbol               = ( SYMBOL_INFO * )calloc( sizeof( SYMBOL_INFO ) + 256 * sizeof( char ), 1 );
		symbol->MaxNameLen   = 255;
		symbol->SizeOfStruct = sizeof( SYMBOL_INFO );

		p = str;
		remain = len - 1;

		for(i = 0; i < callstack_cnt; i++ )
		{
				size_t n;
				SymFromAddr( process, (DWORD64)(callstack[i]), 0, symbol );

				if(_snprintf(p, remain, "%s\r\n", symbol->Name) <= 0)
				{
						break;
				}
				
				n = strlen(p);
				
				if(n == 0)
				{
						break;
				}
				
				p += n;
				remain -= n;
		}
		
		*p = '\0';
		free(symbol);
		return (size_t)strlen(str);
}


static arInit_t __g_ar_ctx =
{
        {
                __on_error,
                __on_print,
                NULL,
        },
        
        {
				__build_backtrace,
                __build_backtrace_symbol,
        }
};




BOOL ExternalLibrary_Init()
{
		BOOL result = TRUE;
		ar_bool_t ar_is_init = false, basis_is_init = false, media_is_init = false;
		setup_debugtrace_api();


		arStatus_t      ar_status;
                
		ar_status = Arsenal_Init(&__g_ar_ctx);
		if(ar_status != AR_S_YES)
		{
				result = FALSE;
				goto END_POINT;
		}
		ar_is_init = true;


END_POINT:
		if(!result)
		{

				if(ar_is_init)
				{
						Arsenal_UnInit();
						ar_is_init = false;
				}
		}

		return result;
}


void ExternalLibrary_UnInit()
{
		Arsenal_UnInit();
}


/**************************************************************************************************************/


void ImageSearch::on_icon_activated(QSystemTrayIcon::ActivationReason reason)
{
		switch (reason) 
		{
		case QSystemTrayIcon::Trigger:
				break;
		case QSystemTrayIcon::DoubleClick:
		{
				if(!this->isVisible())
				{
						this->show();
				}else
				{
						this->hide();
				}
		}
				break;
		case QSystemTrayIcon::MiddleClick:
				
				break;
		case QSystemTrayIcon::Context:
		{
				if(!this->isVisible())
				{
						m_showAction->setEnabled(true);
						m_hideAction->setEnabled(false);

				}else
				{
						m_showAction->setEnabled(false);
						m_hideAction->setEnabled(true);
				}
		}
				break;

		default:
				;
		}
}



ImageSearch::ImageSearch(QWidget *parent, Qt::WFlags flags)	: QDialog(parent, flags)
{
		/**************************init**********************************/

		if(!ExternalLibrary_Init())
		{
				abort();
		}


		/**************************setup**********************************/
		ui.setupUi(this);


		QAction *showAction;
		QAction *hideAction;
		QAction *quitAction;

		QMenu *trayIconMenu;

		QFileIconProvider file_icon;
		
		m_tray = new QSystemTrayIcon(this);
		m_tray->setIcon(file_icon.icon(QFileIconProvider::Computer));
		m_tray->setToolTip(tr("ImageSearch"));
		
		connect(m_tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(on_icon_activated(QSystemTrayIcon::ActivationReason)));


		m_showAction = new QAction(tr("Show"), this);
		connect(m_showAction, SIGNAL(triggered()), this, SLOT(show()));

		m_hideAction = new QAction(tr("Hide"), this);
		connect(m_hideAction, SIGNAL(triggered()), this, SLOT(hide()));

		m_quitAction = new QAction(tr("Quit"), this);
		connect(m_quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));





		m_trayIconMenu = new QMenu(this);
		
		m_trayIconMenu->addAction(m_showAction);
		m_trayIconMenu->addAction(m_hideAction);
		m_trayIconMenu->addSeparator();
		m_trayIconMenu->addAction(m_quitAction);

		m_tray->setContextMenu(m_trayIconMenu);
		
		/**************************start**********************************/
		
		m_tray->show();



		m_check_foreground_timer = new QTimer(this);
		connect(m_check_foreground_timer,SIGNAL(timeout()),this,SLOT(on_check_foreground_window()));
		m_check_foreground_timer->start(1500);
		
}



ImageSearch::~ImageSearch()
{
		if(m_check_foreground_timer)
		{
				m_check_foreground_timer->stop();
				delete m_check_foreground_timer;
				m_check_foreground_timer = NULL;
		}

		ExternalLibrary_UnInit();
}






void ImageSearch::LoadSamples(const std::wstring &path)
{

		std::wstring samples_path;
		std::vector<std::wstring> samples_file_list;
		if(path.empty())
		{
				samples_path = UTIL_GetModulePath() + L"\\Samples";
		}else
		{
				samples_path = path;
		}



		AR_error(AR_ERR_MESSAGE, L"samples path : %ls\r\n", samples_path.c_str());
		
		arPathIter_t *iter = 	AR_CreatePathIterator();

		if(samples_path.back() == L'\\')
		{
				samples_path.pop_back();
		}

		if(AR_PathIteratorSetPath(iter, samples_path.c_str()) != AR_S_YES)
		{
				goto END_POINT;
		}

		

		while(!AR_PathIteratorIsDone(iter))
		{
				const wchar_t *name = AR_PathIteratorCurrent(iter);
				
				//AR_DPRINT(L"file name : %ls\r\n", name);

				const wchar_t *p = AR_reverse_wcschr(name, AR_wcslen(name), L'.');

				if(p == NULL || AR_wcsicmp(p, L".bmp") != 0)
				{
						continue;
				}

				std::wstring file_path = samples_path + L"\\" + std::wstring(name);

				samples_file_list.push_back(file_path);

				if(AR_PathIteratorNext(iter) != AR_S_YES)
				{
						break;
				}
		}


		
		for(size_t i = 0; i < samples_file_list.size(); ++i)
		{
				const std::wstring &sample_path = samples_file_list[i];
				const wchar_t *sample_name = AR_reverse_wcschr(sample_path.c_str(), sample_path.size(), L'\\');

				if(sample_name == NULL || AR_wcslen(sample_name) == 0)
				{
						AR_error(AR_ERR_WARNING, L"invalid sample file path : %ls\r\n", sample_path.c_str());
						continue;
				}

				sample_name++;

				std::string hash = "";

				if(!phash_image_file(sample_path.c_str(), hash))
				{
						AR_error(AR_ERR_WARNING, L"failed to phash image file : %ls\r\n", sample_path.c_str());
						continue;
				}

				AR_DPRINT(L"%ls -> %hs\r\n", sample_name, hash.c_str());

				this->m_sample_map[sample_name] = hash;
		}


END_POINT:
		if(iter)
		{
				AR_DestroyPathIterator(iter);
				iter = NULL;
		}

}


#define SAMPLE_SIZE						16
#define LIKE_THRESHOLD					95
#define MIN_MATCHED_COUNT				3

void ImageSearch::check_foreground_window()
{
		// 调用方法
		CByteImage_t img;
		DWORD pid = 0;
		
		HWND hwnd = GetForegroundWindow();

		if(hwnd == NULL)
		{
				AR_error(AR_ERR_WARNING, L"can't get foreground window handle : %hs\r\n", AR_FUNC_NAME);
				return;
		}

		::GetWindowThreadProcessId(hwnd,&pid);

		RECT rect;

		if(!::GetWindowRect(hwnd, &rect))
		{
				AR_error(AR_ERR_WARNING, L"failed to get foreground window rect ： %d\r\n", GetLastError());
				return;
		}

		size_t width = rect.right - rect.left;
		size_t height = rect.bottom - rect.top;

		if(width > 300 || height > 300 || width < 100 || height < 100)
		{
				AR_error(AR_ERR_WARNING, L"PID : %d, window size : (%d,%d) not matched\r\n",pid,  (int)width, (int)height);
				return;
		}

		
		if(!CaptureAnImageSaveToCImg(hwnd,img))
		{
				AR_error(AR_ERR_WARNING, L"failed to capture PID %d foreground window\r\n", pid);
				return;
		}

		CByteImage_t bak_img = img;

		std::string hash;

		if(phash_image(img, hash, SAMPLE_SIZE) != AR_S_YES)
		{
				AR_error(AR_ERR_WARNING, L"failed to phash PID %d, window image\r\n", pid);
				return;
		}

		AR_error(AR_ERR_MESSAGE, L"PID : %d window image phash : '%hs'\r\n", pid, hash.c_str());
		AR_ASSERT(hash.size() == SAMPLE_SIZE * SAMPLE_SIZE);


		std::vector<std::wstring>		matched_name_list;
		std::vector<size_t>				matched_distance_list;
		for(SampleMap_t::const_iterator cit = m_sample_map.begin(); cit != m_sample_map.end(); ++cit)
		{
				size_t distance = phash_hamming_distance(cit->second, hash);
				if(distance < LIKE_THRESHOLD)
				{
						AR_error(AR_ERR_MESSAGE, L"pid %d foreground window matched sample : %ls, distance : %d\r\n", pid, cit->first.c_str(), (int)distance);
						matched_name_list.push_back(cit->first);
						matched_distance_list.push_back(distance);
				}
		}


		size_t min_dist = 512;
		std::wstring min_name = L"";

		for(size_t i = 0; i < matched_distance_list.size(); ++i)
		{
				if(matched_distance_list[i] < min_dist)
				{
						min_dist = matched_distance_list[i];
						min_name = matched_name_list[i];
				}
		}

		ar_bool_t is_matched = (matched_distance_list.size() > MIN_MATCHED_COUNT || (m_sample_map.size() < MIN_MATCHED_COUNT && matched_distance_list.size() > 0));

		if(is_matched)
		{
				on_matched(pid, bak_img, min_name,min_dist);
		}
}


void ImageSearch::on_check_foreground_window()
{
		AR_DPRINT(L"On timer\r\n");
		check_foreground_window();
}


void ImageSearch::on_matched(DWORD pid, const CByteImage_t &img, const std::wstring &matched_sample_name, size_t distance)
{
		arString_t *tmp = AR_CreateString();
		if(tmp == NULL)
		{
				AR_error(AR_ERR_WARNING, L"low mem : %hs\r\n", AR_FUNC_NAME);
				return;
		}

		if(AR_GetTempPath(tmp) != AR_S_YES)
		{
				AR_error(AR_ERR_WARNING, L"failed to get temp path : %hs\r\n", AR_FUNC_NAME);
				AR_DestroyString(tmp);
				tmp = NULL;
				return;
		}

		
		wchar_t tmpfile[1024];
		AR_srand(time(NULL));
		do{
				
				ar_uint_64_t name = AR_rand64();
				if(name == 0)
				{
						name = 12345678;
				}

				AR_swprintf(tmpfile, 1024, L"%ls\\%qd.bmp", AR_CSTR(tmp), name);

		}while(AR_path_is_existed(tmpfile) == AR_S_YES);
		
		AR_error(AR_ERR_MESSAGE, L"temp file : %ls\r\n", tmpfile);
		
		std::string utf8 = UTIL_WToUTF8(tmpfile);

		try{
				if(!utf8.empty())
				{
						img.save(utf8.c_str());
						
						QPixmap pixmap;

						pixmap.load(utf8.c_str());
						ui.matched_image->setPixmap(pixmap);

						AR_path_remove(tmpfile);
				}
				
				static char msg[10240];

				AR_sprintf(msg, 10240, "PID : %d\r\nSample Image : %ls\r\nDistance : %d\r\n", pid, matched_sample_name.c_str(), (int)distance);
				m_tray->showMessage(tr("Matched image detected"), msg, QSystemTrayIcon::Information, 3000);

		}catch(...)
		{

				AR_error(AR_ERR_WARNING, L"failed to load temp image : %hs\r\n", AR_FUNC_NAME);
		}

		if(tmp)
		{
				AR_DestroyString(tmp);
				tmp = NULL;
		}
}

