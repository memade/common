#if !defined(AFX_SK_H__25D51CCE_507D_4F7C_844B_4A86D9B6E6A9__HEAD__)
#define AFX_SK_H__25D51CCE_507D_4F7C_844B_4A86D9B6E6A9__HEAD__

#include <commctrl.h> 
#pragma comment(lib, "Comctl32.lib")
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#include <shellapi.h>

#define DEF_FIRST_CHILD_ID 0xC350
#define WM_TASKBAR WM_USER+1001
#define WM_DESTROY_CHILD WM_TASKBAR+1
#define WM_TOOLBAR WM_TASKBAR+2

namespace sk {
	class IWindowFrame;
}

namespace sk {

	enum struct EnWindowChildType : unsigned long long
	{
		EN_WINDOW_CHILD_TYPE_STD = 0x10000000,
		EN_WINDOW_CHILD_TYPE_LOG = 0x10000001,
	};

	class IWindow
	{
	protected:
		IWindow(const HINSTANCE& hInstance, const HINSTANCE& hInstanceRes, sk::string AppName = _T(""), sk::string ClassName = _T("")) :
			m_hInstance(hInstance),
			m_hInstanceRes(hInstanceRes),
			m_NameApp(AppName),
			m_NameClass(ClassName.empty() ? sk::StringConvert::MBytesToTString(sk::UUID().uuid()) : ClassName)
		{
			m_hFont =
				::CreateFont(
					-15/*高度*/,
					-7/*宽度*/,
					0/*不用管*/,
					0/*不用管*/,
					400 /*一般这个值设为400*/,
					FALSE/*不带斜体*/,
					FALSE/*不带下划线*/,
					FALSE/*不带删除线*/,
					DEFAULT_CHARSET,  //这里我们使用默认字符集，还有其他以 _CHARSET 结尾的常量可用
					OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,  //这行参数不用管
					DEFAULT_QUALITY,  //默认输出质量
					FF_DONTCARE,  //不指定字体族*/
					L"微软雅黑"  //字体名
				);
		}
		~IWindow()
		{
			::DeleteObject(m_hFont);
		}
	protected:
		virtual void DoPaint() const = 0;
		sk::container::unordered_map<HWND, HWND> m_hCtrlQ;
	public:
		const HWND& GetHwnd() const { return m_hWnd; }
		const sk::string& ClassName() const { return m_NameClass; }
		const sk::string& AppName() const { return m_NameApp; }
		const HINSTANCE& GetHinstance() const { return m_hInstance; }
		const HINSTANCE& GetHinstanceRes() const { return m_hInstanceRes; }
		const HFONT& Font() const { return m_hFont; }
	protected:
		HFONT m_hFont = nullptr;
		HWND m_hWnd = nullptr;
		const HINSTANCE m_hInstance = nullptr;
		const HINSTANCE m_hInstanceRes = nullptr;
		const sk::string m_NameClass;
		const sk::string m_NameApp;
		WNDCLASS m_WndClass = { 0 };
	};

	class IWindowChild : public IWindow
	{
		const long m_Key;
		const EnWindowChildType m_ChildType;
	public:
		IWindowChild(const EnWindowChildType& ChildType, const long& Key, const HINSTANCE& hInstance, const HINSTANCE& hInstanceRes, sk::string AppName = _T(""), sk::string  ClassName = _T("")) :
			m_Key(Key),
			m_ChildType(ChildType),
			IWindow(hInstance, hInstanceRes, AppName, ClassName)
		{
			m_WndClass.style = CS_HREDRAW | CS_VREDRAW;
			m_WndClass.lpfnWndProc = WndProc;
			m_WndClass.cbClsExtra = 0;
			m_WndClass.cbWndExtra = sizeof(HANDLE);
			m_WndClass.hInstance = hInstance;
			m_WndClass.hIcon = \
				hInstanceRes ? \
				LoadIcon(hInstanceRes, MAKEINTRESOURCE(IDI_ICON_LOGO_CHILD_128px)) : \
				LoadIcon(NULL, IDI_APPLICATION);
			m_WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			m_WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			m_WndClass.lpszMenuName = NULL;
			m_WndClass.lpszClassName = __super::ClassName().c_str();
			::RegisterClass(&m_WndClass);

			m_MDICreateStruct.szClass = __super::ClassName().c_str();
			m_MDICreateStruct.szTitle = __super::AppName().c_str();
			m_MDICreateStruct.hOwner = hInstance;
			m_MDICreateStruct.x = CW_USEDEFAULT;
			m_MDICreateStruct.y = CW_USEDEFAULT;
			m_MDICreateStruct.cx = CW_USEDEFAULT;
			m_MDICreateStruct.cy = CW_USEDEFAULT;

			DWORD _style = WS_CLIPCHILDREN;
			_style &= ~CS_HREDRAW;
			_style &= ~CS_VREDRAW;

			m_MDICreateStruct.style = _style;
			m_MDICreateStruct.lParam = reinterpret_cast<LPARAM>(this);
		}
		~IWindowChild()
		{
			::UnregisterClass(ClassName().c_str(), m_hInstance);
		}

		virtual void Release() const { delete this; }
		virtual void DoPaint() const {}
	public:
		const MDICREATESTRUCT& MDICreateStruct() const { return m_MDICreateStruct; }
		const long& Key() const { return m_Key; }
		const EnWindowChildType& ChildType() const { return m_ChildType; }
		void SetHwnd(const HWND& hwnd) { m_hWnd = hwnd; }
		const HWND& ListViewHwndGet() const { return m_hListView; }
	private:
		MDICREATESTRUCT m_MDICreateStruct = { 0 };
		HWND m_hListView = nullptr;
		__inline static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	protected:
		virtual LRESULT OnChildMessage(HWND, UINT, WPARAM, LPARAM,BOOL&) { return LRESULT(0); };
	public:
		virtual LRESULT Function() { return LRESULT{ 0 }; }
	};

	class IWindowFrame : public IWindow
	{
		enum struct EnChildArrangementType : UINT
		{
			EN_CHILD_ARRANGEMENT_TYPE_TILE = 0,
			EN_CHILD_ARRANGEMENT_TYPE_CASCADE = 1,
		};
		friend class IWindowChild;
	public:
		IWindowFrame(const HINSTANCE& hInstance, const HINSTANCE& hInstanceRes, sk::string AppName = _T(""), sk::string ClassName = _T("")) :
			IWindow(hInstance, hInstanceRes, AppName, ClassName)
		{
			m_WndClass.style = CS_HREDRAW | CS_VREDRAW;
			m_WndClass.cbClsExtra = 0;
			m_WndClass.cbWndExtra = 0;
			m_WndClass.hInstance = m_hInstance;
			m_WndClass.hIcon = m_hInstanceRes ? LoadIcon(m_hInstanceRes, MAKEINTRESOURCE(IDI_ICON_LOGO_128PX)) : LoadIcon(NULL, IDI_APPLICATION);
			m_WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			m_WndClass.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);
			m_WndClass.lpszMenuName = NULL;
			m_WndClass.lpszClassName = __super::ClassName().c_str();
			m_WndClass.lpfnWndProc = WndProc;
			::RegisterClass(&m_WndClass);
			m_hAccel = LoadAccelerators(m_hInstance, __super::AppName().c_str());

			Gdiplus::GdiplusStartupInput gdiplusStartupInput;
			Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

			INITCOMMONCONTROLSEX icex;
			icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
			icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES | ICC_INTERNET_CLASSES | ICC_TAB_CLASSES;
			InitCommonControlsEx(&icex);
		}
		~IWindowFrame()
		{
			::UnregisterClass(ClassName().c_str(), m_hInstance);

			Gdiplus::GdiplusShutdown(m_gdiplusToken);

			m_HotKeyQ.iterate_clear(
				[&](const auto& hotKeyID, const auto&, auto&, auto& itclear)
				{
					::UnregisterHotKey(m_hWnd, hotKeyID);
					itclear = true;
				});

			FILE* pFileConin{ nullptr }, * pFileConout{ nullptr }, * pFileConerr{ nullptr };
			m_FilePtrCacheQ.iterate_clear(
				[&](const auto&, auto& pFile, auto&, auto& itclear)
				{
					::fclose(pFile);
					pFile = nullptr;
					itclear = true;
				});
		}
	public:
		inline int CreateFrame(const std::map<INT, LPARAM>& hotKeys)
		{
			m_hMenu = m_hInstanceRes ? ::LoadMenu(m_hInstanceRes, MAKEINTRESOURCE(IDR_MENU_MAIN)) : NULL;

			m_hWnd = ::CreateWindow(
				ClassName().c_str(),
				AppName().c_str(),
				WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				NULL,
				m_hMenu,
				m_hInstance,
				this);
			m_hWndClient = ::GetWindow(m_hWnd, GW_CHILD);

			::ShowWindow(m_hWnd, SW_SHOWNORMAL);
			::UpdateWindow(m_hWnd);

			for (const auto& keys : hotKeys)
			{
				if (::RegisterHotKey(m_hWnd, keys.first, HIWORD(keys.second), LOWORD(keys.second)))
					m_HotKeyQ.push(keys.first, keys.second);
			}
			return 0;
		}
		inline HWND AppendChild(IWindowChild* pChild)
		{
			HWND hChild = (HWND)::SendMessage(m_hWndClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&pChild->MDICreateStruct());
			if (!hChild)
			{
				SK_DELETE_PTR(pChild);
				return nullptr;
			}
			pChild->SetHwnd(hChild);
			m_ChildQ.push(pChild->Key(), pChild);
			return hChild;
		}
		inline HWND CreateChild(const EnWindowChildType& Type, const sk::string& AppName)
		{
			IWindowChild* pChild = new IWindowChild(Type, ChildID(), m_hInstance, m_hInstanceRes, AppName);
			MDICREATESTRUCT mdicreate = { 0 };
			mdicreate.szClass = pChild->ClassName().c_str();
			mdicreate.szTitle = pChild->AppName().c_str();
			mdicreate.hOwner = m_hInstance;
			mdicreate.x = CW_USEDEFAULT;
			mdicreate.y = CW_USEDEFAULT;
			mdicreate.cx = CW_USEDEFAULT;
			mdicreate.cy = CW_USEDEFAULT;

			DWORD _style = WS_CLIPCHILDREN;
			_style &= ~CS_HREDRAW;
			_style &= ~CS_VREDRAW;
			mdicreate.style = _style;
			mdicreate.lParam = reinterpret_cast<LPARAM>(pChild);
			HWND hChild = (HWND)::SendMessage(m_hWndClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&mdicreate);
			if (!hChild)
			{
				SK_DELETE_PTR(pChild);
				return nullptr;
			}


			pChild->SetHwnd(hChild);
			m_ChildQ.push(pChild->Key(), pChild);
			return hChild;
		}
		inline void DestroyChild(const long& ChildID)
		{
			m_ChildQ.pop(ChildID, [&](const auto&, auto& child)
				{
					::SetWindowLongPtr(child->GetHwnd(), GWLP_USERDATA, 0);
					child->Release();
				});
		}
		inline void DestroyChild()
		{
			m_ChildQ.iterate_clear(
				[&](const auto&, auto& child, auto&, auto& itclear)
				{
					::SetWindowLongPtr(child->GetHwnd(), GWLP_USERDATA, 0);
					child->Release();
					itclear = true;
				});
		}
		inline void ChildTile()
		{
			m_ChildArrangementType.store(EnChildArrangementType::EN_CHILD_ARRANGEMENT_TYPE_TILE);
			SendMessage(m_hWndClient, WM_MDITILE, 0, 0);
		}
		inline void ChildCascade()
		{
			m_ChildArrangementType.store(EnChildArrangementType::EN_CHILD_ARRANGEMENT_TYPE_CASCADE);
			SendMessage(m_hWndClient, WM_MDICASCADE, 0, 0);
		}
		inline bool CreateStatusBar()
		{
			m_hStatusBar = ::CreateWindowEx(
				0,
				STATUSCLASSNAME,
				NULL,
				WS_CHILD | SBARS_SIZEGRIP | WS_VISIBLE,
				0,
				0,
				0,
				0,
				m_hWnd,
				(HMENU)1,
				m_hInstance,
				NULL);
			return m_hStatusBar != nullptr;
		}
		inline bool LoadImageFromResource(const HINSTANCE& hResource, const int& ResID, const TCHAR* typeImage, Gdiplus::Image** _outImagePtr, Gdiplus::Bitmap** _outBitmapPtr) const
		{
			if (!hResource)
				return false;
			HRSRC findHRsrc = FindResource(hResource, MAKEINTRESOURCE(ResID), typeImage);
			if (!findHRsrc)
				return false;
			DWORD lenRsrc = SizeofResource(hResource, findHRsrc);
			BYTE* pRsrc = (BYTE*)LoadResource(hResource, findHRsrc);
			if (!pRsrc || lenRsrc <= 0)
				return false;
			HGLOBAL hMemBmp = GlobalAlloc(GMEM_FIXED, lenRsrc);
			if (!hMemBmp)
			{
				FreeResource(pRsrc);
				return false;
			}
			BYTE* pMem = (BYTE*)GlobalLock(hMemBmp);
			if (!pMem)
			{
				FreeResource(pRsrc);
				return false;
			}
			memcpy(pMem, pRsrc, lenRsrc);
			IStream* pStmBmp = NULL;

			bool result = false;
			if (S_OK == ::CreateStreamOnHGlobal(hMemBmp, FALSE, &pStmBmp))
			{
				if (_outImagePtr)
				{
					*_outImagePtr = Gdiplus::Image::FromStream(pStmBmp);
				}
				if (_outBitmapPtr)
				{
					*_outBitmapPtr = Gdiplus::Bitmap::FromStream(pStmBmp);
				}
				result = true;
			}
			GlobalUnlock(hMemBmp);
			pStmBmp->Release();
			FreeResource(pRsrc);
			return result;
		}

		inline bool CreateToolBar(const std::unordered_map<int, sk::string>& resTools)
		{
			if (resTools.empty())
				return false;

			std::vector<TBBUTTON> tbButtons;
			m_hImageList = ::ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, static_cast<int>(resTools.size()), 0);
			for (const auto& resTool : resTools)
			{
				Gdiplus::Bitmap* bitmap = nullptr;
				if (LoadImageFromResource(m_hInstanceRes, resTool.first, resTool.second.c_str(), nullptr, &bitmap) && bitmap)
				{
					HBITMAP _hBitmap;
					if (Gdiplus::Status::Ok == bitmap->GetHBITMAP(Gdiplus::Color::Transparent, &_hBitmap))
					{
						TBBUTTON tbButton = { 0 };
						tbButton.iBitmap = ::ImageList_Add(m_hImageList, _hBitmap, NULL);
						tbButton.idCommand = resTool.first;
						tbButton.fsState = TBSTATE_ENABLED;
						tbButton.fsStyle = TBSTYLE_BUTTON;
						tbButtons.emplace_back(tbButton);
					}
				}
			}

			if (tbButtons.empty())
				return false;
#if 0
			m_hToolBar = CreateWindowEx(
				0,
				TOOLBARCLASSNAME,
				NULL,
				WS_CHILD | WS_VISIBLE,
				0,
				0,
				0,
				0,
				m_hWnd,
				HMENU(1),
				m_hInstance,
				NULL);
			SendMessage(m_hToolBar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
			SendMessage(m_hToolBar, TB_ADDBUTTONS, static_cast<WPARAM>(0), (LPARAM)&tbButtons[0]);
#else
			m_hToolBar = ::CreateToolbarEx(
				m_hWnd,
				WS_CHILD | CCS_TOP | WS_VISIBLE | WS_BORDER,
				WM_TOOLBAR,
				static_cast<int>(tbButtons.size()),
				m_hInstance,
				reinterpret_cast<UINT_PTR>(m_hImageList),
				&tbButtons[0],
				static_cast<int>(tbButtons.size()),
				16, 16, 16, 16, sizeof(TBBUTTON));
#endif
			SendMessage(m_hToolBar, TB_SETIMAGELIST, 0, reinterpret_cast<LPARAM>(m_hImageList));
			SendMessage(m_hToolBar, TB_AUTOSIZE, 0, 0);
			ShowWindow(m_hToolBar, TRUE);
			return true;
		}

		inline void CreateTaskBar(const TCHAR* tip)
		{
			NOTIFYICONDATA nid;
			nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
			nid.hWnd = m_hWnd;
			nid.uID = WM_TASKBAR;
			nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
			nid.uCallbackMessage = WM_TASKBAR;
			nid.hIcon = LoadIcon(m_hInstanceRes, MAKEINTRESOURCE(IDI_ICON_LOGO_16PX));
			if (tip) { _stprintf_s(nid.szTip, TEXT("%s"), tip); }
			Shell_NotifyIcon(NIM_ADD, &nid);
		}

		inline void DestroyTaskBar()
		{
			NOTIFYICONDATA nid;
			nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
			nid.hWnd = m_hWnd;
			nid.uID = WM_TASKBAR;
			Shell_NotifyIcon(NIM_DELETE, &nid);
		}
	public:
		const HWND& GetClientHwnd() const { return m_hWndClient; }
		const HWND& GetStatusBar() const { return m_hStatusBar; }
		const HWND& GetToolBar() const { return m_hToolBar; }
		const HACCEL& GetHaccel() const { return m_hAccel; }
		const EnChildArrangementType ChildArrangementType() const { return m_ChildArrangementType.load(); }
	protected:
		HWND m_hWndClient = nullptr;
		HWND m_hToolBar = nullptr;
		HWND m_hStatusBar = nullptr;
		HACCEL m_hAccel = nullptr;
		HMENU m_hMenu = nullptr;
		HMENU m_hPopMenuTaskBar = nullptr;

		sk::container::map<long, IWindowChild*> m_ChildQ;
	private:
		sk::container::map<INT/*HotKeyID*/, LPARAM/*vk | fsModifiers*/> m_HotKeyQ;
		volatile long m_ChildID = 0;
		HIMAGELIST m_hImageList = nullptr;
		ULONG_PTR m_gdiplusToken = 0;
		std::atomic<EnChildArrangementType> m_ChildArrangementType = EnChildArrangementType::EN_CHILD_ARRANGEMENT_TYPE_CASCADE;
		sk::container::map<std::intptr_t, FILE*> m_FilePtrCacheQ;
	protected:
		long ChildID() { return ::_InterlockedExchangeAdd(&m_ChildID, 1); }
	private:
		virtual void DoPaint() const {}
		__inline static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	public:
		void MessageLoop() const
		{
			MSG msg = { 0 };
			do {
				if (!::GetMessage(&msg, NULL, 0, 0))
					break;
				if (!::TranslateMDISysAccel(m_hWndClient, &msg) && !::TranslateAccelerator(m_hWnd, m_hAccel, &msg))
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
			} while (1);
		}
	protected:
		virtual LRESULT OnFrameMessage(HWND, UINT, WPARAM, LPARAM, BOOL bHandle = false) = 0;
	};


	__inline LRESULT CALLBACK IWindowChild::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		BOOL bHandle = FALSE;
		IWindowChild* __this = reinterpret_cast<IWindowChild*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		HWND hWndClient = GetParent(hWnd);
		HWND hWndFrame = GetParent(hWndClient);
		switch (message)
		{
		case WM_CREATE:
		{
#if 0
			auto gwlStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
			gwlStyle &= ~WS_CAPTION;
			SetWindowLongPtr(hWnd, GWL_STYLE, gwlStyle);
			SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_DRAWFRAME);
#endif
			auto pCreateStruct = (CREATESTRUCT*)lParam;
			auto pMDICreateStruct = reinterpret_cast<MDICREATESTRUCT*>(pCreateStruct->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, static_cast<LONG_PTR>(pMDICreateStruct->lParam));

			__this = reinterpret_cast<IWindowChild*>(pMDICreateStruct->lParam);
		}break;
		case WM_MDIACTIVATE:
		{
		}break;
		case WM_QUERYENDSESSION:
			[[fallthrough]];
		case WM_CLOSE:
		{
		}break;
		case WM_DESTROY:
		{
			::SendMessage(hWndFrame, WM_DESTROY_CHILD, 0, MAKELPARAM(0, __this->Key()));
		}break;
		}///switch

		__this = reinterpret_cast<IWindowChild*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		LRESULT result = 0;
		if (__this)
			result = __this->OnChildMessage(hWnd, message, wParam, lParam, bHandle);
		return bHandle ? result : DefMDIChildProc(hWnd, message, wParam, lParam);
	}

	__inline LRESULT CALLBACK IWindowFrame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		BOOL bHandle = FALSE;

		IWindowFrame* __this = reinterpret_cast<IWindowFrame*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		switch (message)
		{
		case WM_CREATE:
		{
			auto pCreateStruct = (CREATESTRUCT*)lParam;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
			auto self = reinterpret_cast<IWindowFrame*>(pCreateStruct->lpCreateParams);

			CLIENTCREATESTRUCT clientcreate = { 0 };
			clientcreate.hWindowMenu = nullptr;
			clientcreate.idFirstChild = DEF_FIRST_CHILD_ID;
			self->m_hWndClient = ::CreateWindow(
				_T("MDICLIENT"),
				NULL,
				WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				hWnd,
				(HMENU)1,
				self->m_hInstance,
				(PSTR)&clientcreate);

			__this = reinterpret_cast<IWindowFrame*>(pCreateStruct->lpCreateParams);
			__this->m_hWnd = hWnd;
			__this->CreateStatusBar();
		}break;
		case WM_QUERYENDSESSION:
			[[fallthrough]];
		case WM_CLOSE:
		{
			if (IDOK != MessageBox(hWnd, _T("Are you sure to exit the main program?"), _T("Tip"), MB_ICONQUESTION | MB_OKCANCEL))
				return 0;

			if (__this)
			{
				__this->DestroyTaskBar();

				auto resultAllocConsole = ::AllocConsole();
				if (!resultAllocConsole)
				{
					::FreeConsole();
					resultAllocConsole = ::AllocConsole();
				}
				if (resultAllocConsole)
				{
					::ShowWindow(__this->GetHwnd(), SW_HIDE);
					HWND hConsole = ::GetConsoleWindow();
					HMENU hConsoleMenu = ::GetSystemMenu(hConsole, false);
					::RemoveMenu(hConsoleMenu, SC_CLOSE, MF_BYCOMMAND);
					::DestroyMenu(hConsoleMenu);

					FILE* pFileConin{ nullptr }, * pFileConout{ nullptr }, * pFileConerr{ nullptr };
					if (!::freopen_s(&pFileConin, "conin$", "w", stdin))
						__this->m_FilePtrCacheQ.push(TypeToPtr(pFileConin), pFileConin);
					if (!::freopen_s(&pFileConout, "conout$", "w", stdout))
						__this->m_FilePtrCacheQ.push(TypeToPtr(pFileConout), pFileConout);

					::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
					std::cout << "System is exiting , Please wait . . ." << std::endl;
				}

				::EnumChildWindows(
					__this->m_hWndClient,
					[](HWND hWnd, LPARAM lParam)->BOOL
					{
						if (::GetWindow(hWnd, GW_OWNER))
						{
							return TRUE;
						}
						::SendMessage(::GetParent(hWnd), WM_MDIRESTORE, (WPARAM)hWnd, 0);
						if (!::SendMessage(hWnd, WM_QUERYENDSESSION, 0, 0))
						{
							return TRUE;
						}
						::SendMessage(::GetParent(hWnd), WM_MDIDESTROY, (WPARAM)hWnd, 0);
						return TRUE;
					}, 0);
			}

		}break;
		case WM_SIZE:
		{
			if (__this)
			{
				if (__this->m_hStatusBar)
					::SendMessage(__this->m_hStatusBar, WM_SIZE, 0, 0);
				if (__this->m_hWndClient)
				{
					RECT rtWindowFrame, rtClientFrame;
					GetWindowRect(hWnd, &rtWindowFrame);
					GetClientRect(hWnd, &rtClientFrame);
					auto border = ((rtWindowFrame.right - rtWindowFrame.left) - (rtClientFrame.right - rtClientFrame.left)) / 2;
					auto menu_bar_height = GetSystemMetrics(SM_CYMENU);
					MENUBARINFO menuInfo = { 0 };
					menuInfo.cbSize = sizeof(MENUBARINFO);
					if (::GetMenuBarInfo(hWnd, OBJID_MENU, 0, &menuInfo))
					{
						menu_bar_height = menuInfo.rcBar.bottom - menuInfo.rcBar.top;
					}

					LONG status_bar_height = 0;
					RECT rtWindowStatusBar = { 0 };
					if (__this->m_hStatusBar)
					{
						GetWindowRect(__this->m_hStatusBar, &rtWindowStatusBar);
						status_bar_height = rtWindowStatusBar.bottom - rtWindowStatusBar.top;
					}

					RECT rtToolBarWindow = { 0 };
					RECT rtToolBarClient = { 0 };
					LONG tool_bar_height = 0;
					if (__this->m_hToolBar)
					{
						::SendMessage(__this->m_hToolBar, TB_AUTOSIZE, 0, 0);
						::GetWindowRect(__this->m_hToolBar, &rtToolBarWindow);
						::GetClientRect(__this->m_hToolBar, &rtToolBarClient);
						tool_bar_height = rtToolBarWindow.bottom - rtToolBarWindow.top;
					}
					::MoveWindow(
						__this->m_hWndClient,
						0,
						tool_bar_height,
						rtClientFrame.right - rtClientFrame.left,
						rtClientFrame.bottom - rtClientFrame.top - status_bar_height - tool_bar_height,
						TRUE);

					if (__this->m_hToolBar)
					{
						::SendMessage(__this->m_hToolBar, TB_AUTOSIZE, 0, 0);
					}

					__this->m_ChildQ.iterate(
						[&](const auto&, auto& child)
						{
							::SendMessage(child->GetHwnd(), WM_SIZE, 0, MAKELPARAM(rtClientFrame.right - rtClientFrame.left, rtClientFrame.bottom - rtClientFrame.top - status_bar_height - tool_bar_height));
						});

					switch (__this->ChildArrangementType())
					{
					case EnChildArrangementType::EN_CHILD_ARRANGEMENT_TYPE_TILE:
					{
						__this->ChildTile();
					}break;
					case EnChildArrangementType::EN_CHILD_ARRANGEMENT_TYPE_CASCADE:
					{
						__this->ChildCascade();
					}break;
					default:
					{
					}break;
					}

					bHandle = TRUE;
				}
			}

		}break;
		case WM_HOTKEY:
		{

		}break;
		case WM_TASKBAR:
		{

		}break;
		case WM_DESTROY_CHILD:
		{
			__this->DestroyChild(HIWORD(lParam));
		}break;
		case WM_DESTROY:
		{
			::PostQuitMessage(0);
			::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			std::cout << "Window process close complete." << std::endl;
		}break;
		}///switch

		LRESULT result = 0;
		if (__this)
			result = __this->OnFrameMessage(hWnd, message, wParam, lParam, bHandle);

		return bHandle ? result : ::DefFrameProc(hWnd, __this ? __this->m_hWndClient : nullptr, message, wParam, lParam);
	}


}///namespace sk


//!@ /*新生联创®（上海）*/
//!@ /*Tue Oct 6 16:07:48 UTC+0800 2020*/
//!@ /*___www.skstu.com___*/
#endif /*AFX_SK_H__25D51CCE_507D_4F7C_844B_4A86D9B6E6A9__HEAD__*/