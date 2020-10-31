#if !defined(AFX_SKSTU_CONSOLE_H__735F1_006F0220_09F8FB4D5FDAE88_D23E__HEAD__)
#define AFX_SKSTU_CONSOLE_H__735F1_006F0220_09F8FB4D5FDAE88_D23E__HEAD__

namespace sk {

	class Console final {
	public:
		Console(const std::function<int(int,char**)>&) {

		}
		~Console() {

		}
	private:
		int Open() {
#if 0
			//int nScreenWidth, nScreenHeight;
			//nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
			//nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

			//system("mode con cols=120 lines=60");

			//HWND hConsole = GetConsoleWindow();
			//SetWindowLongPtr(
			//	hConsole,
			//	GWL_STYLE,
			//	GetWindowLongPtrA(GetConsoleWindow(), GWL_STYLE)
			//	& ~WS_SIZEBOX & ~WS_MAXIMIZEBOX /*& ~WS_MINIMIZEBOX*/
			//);

			//HWND hwnd = GetConsoleWindow();
			//HMENU hmenu = GetSystemMenu(hwnd, false);
			//RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
			//LONG style = GetWindowLong(hwnd, GWL_STYLE);
			//style &= ~(WS_MINIMIZEBOX);
			//SetWindowLong(hwnd, GWL_STYLE, style);
			//SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			//ShowWindow(hwnd, SW_SHOWNORMAL);
			//DestroyMenu(hmenu);
			//ReleaseDC(hwnd, NULL);

			//auto hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
			//CONSOLE_FONT_INFO consoleCurrentFont;

			//GetCurrentConsoleFont(hOutput, FALSE, &consoleCurrentFont);

			//printf("通过GetCurrentConsoleFont()函数获取到的控制台当前字体长度是%d, 高度是%d.\n"

			//	, consoleCurrentFont.dwFontSize.X

			//	, consoleCurrentFont.dwFontSize.Y

			//);
			//COORD currentFontCoord = GetConsoleFontSize(hOutput, consoleCurrentFont.nFont);

			//printf("通过GetConsoleFontSize()函数获取控制台当前字体长度是%d, 高度是%d.\n", currentFontCoord.X, currentFontCoord.Y);

			//CONSOLE_FONT_INFOEX ConsoleCurrentFontEx;
			//ConsoleCurrentFontEx.cbSize = sizeof(CONSOLE_FONT_INFOEX);
			//ConsoleCurrentFontEx.nFont = 0;
			//ConsoleCurrentFontEx.dwFontSize.X = 7;
			//ConsoleCurrentFontEx.dwFontSize.Y = 14;
			//ConsoleCurrentFontEx.FontFamily = TMPF_VECTOR;
			//ConsoleCurrentFontEx.FontWeight = 100;
			//wcscpy(ConsoleCurrentFontEx.FaceName, _T("黑体"));
			//SetCurrentConsoleFontEx(hOutput, FALSE, &ConsoleCurrentFontEx);
#endif
		}
		int Close() {

		}
	};





}




/// /*新生联创（上海）**/
/// /*2019年09月8日 22:8:31|583**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_CONSOLE_H__735F1_006F0220_09F8FB4D5FDAE88_D23E__TAIL__