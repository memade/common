#if !defined(AFX_SKSTU_1604620854_H__C9BAA_D4851DD6_86748960F5AC740_8C49__HEAD__)
#define AFX_SKSTU_1604620854_H__C9BAA_D4851DD6_86748960F5AC740_8C49__HEAD__

#ifdef DRIVER_DX9
#include "d3d9.h"
#define DIRECTINPUT_VERSION 0x0800
#include "../gpl/imgui/imgui.h"
#include "../gpl/imgui/backends/imgui_impl_dx9.h"
#include "../gpl/imgui/backends/imgui_impl_win32.h"
#pragma comment(lib,"../../gpl/dxsdk/lib/x64/d3d9.lib")
#endif

#ifdef DRIVER_DX12
#include <d3d12.h>
#include <dxgi1_4.h>
#include "../gpl/imgui/imgui.h"
#include "../gpl/imgui/backends/imgui_impl_win32.h"
#include "../gpl/imgui/backends/imgui_impl_dx12.h"
#ifdef _DEBUG
#define DX12_ENABLE_DEBUG_LAYER
#endif
#ifdef DX12_ENABLE_DEBUG_LAYER
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3d12.lib")
#endif

#ifdef DRIVER_GL3
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#if !defined(_DEBUG)
#pragma comment(lib,"../../bin/lib/x64/Release/glad.lib")
#else
#pragma comment(lib,"../../bin/lib/x64/Debug/glad.lib")
#endif

#if !defined(_DEBUG)
#pragma comment(lib,"../../bin/lib/x64/Release/glfw.lib")
#else
#pragma comment(lib,"../../bin/lib/x64/Debug/glfw.lib")
#endif
#endif

#ifdef DRIVER_GL2
#define IMGUI_IMPL_OPENGL_LOADER_GLAD2
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"

#if !defined(_DEBUG)
#pragma comment(lib,"../../bin/lib/x64/Release/glad.lib")
#else
#pragma comment(lib,"../../bin/lib/x64/Debug/glad.lib")
#endif

#if !defined(_DEBUG)
#pragma comment(lib,"../../bin/lib/x64/Release/glfw.lib")
#else
#pragma comment(lib,"../../bin/lib/x64/Debug/glfw.lib")
#endif
#endif

#ifdef DRIVER_GLUT_GL2
#include "backends/imgui_impl_opengl2.h"
#include "backends/imgui_impl_glut.h"
#include "GL/freeglut.h"
#endif

#if !defined(_DEBUG)
#pragma comment(lib,"imgui.lib")
#else
#pragma comment(lib,"imgui.lib")
#endif

namespace sk {

}




/// /*新生联创（上海）**/
/// /*2020年11月6日 9:39:51|311**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_1604620854_H__C9BAA_D4851DD6_86748960F5AC740_8C49__TAIL__