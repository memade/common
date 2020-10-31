#if !defined(AFX_SK_H__72F309CC_B408_44AB_A3EB_F74DCAC70A4C__HEAD__)
#define AFX_SK_H__72F309CC_B408_44AB_A3EB_F74DCAC70A4C__HEAD__

namespace sk {
	class MsgBoxPak final
	{
	public:
		MsgBoxPak(const sk::string& text, const sk::string& caption, UINT type = MB_OK, HWND hwnd = nullptr)
		{
			hWnd = hwnd;
			uType = type;
			Text = text;
			CaptionText = caption;
		}
	public:
		HWND hWnd;
		sk::string Text;
		sk::string CaptionText;
		UINT uType;
	};

}///namespace sk








//!@ /*新生联创®（上海）*/
//!@ /*Sun Aug 23 17:17:57 UTC+0800 2020*/
//!@ /*___www.skstu.com___*/
#endif /*AFX_SK_H__72F309CC_B408_44AB_A3EB_F74DCAC70A4C__HEAD__*/