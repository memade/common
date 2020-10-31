#if !defined(AFX_SKSTU_network_command_H__DBC03_D68871FD_10A60021454B8DA_FFED__HEAD__)
#define AFX_SKSTU_network_command_H__DBC03_D68871FD_10A60021454B8DA_FFED__HEAD__

#if 0//! 编译器判断
MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)
MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
MSVC++ 10.0 _MSC_VER == 1600 (Visual Studio 2010)
MSVC++ 9.0  _MSC_VER == 1500 (Visual Studio 2008)
MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
MSVC++ 7.1  _MSC_VER == 1310 (Visual Studio 2003)
MSVC++ 7.0  _MSC_VER == 1300
MSVC++ 6.0  _MSC_VER == 1200
MSVC++ 5.0  _MSC_VER == 1100
#endif
#if 0 //! OS判断
//******************************************************************************
// Automated platform detection
//******************************************************************************

// _WIN32 is used by
// Visual C++
#ifdef _WIN32
#define __NT__
#endif

// Define __MAC__ platform indicator
#ifdef macintosh
#define __MAC__
#endif

// Define __OSX__ platform indicator
#ifdef __APPLE__
#define __OSX__
#endif

// Define __WIN16__ platform indicator 
#ifdef _Windows_
#ifndef __NT__
#define __WIN16__
#endif
#endif

// Define Windows CE platform indicator
#ifdef WIN32_PLATFORM_HPCPRO
#define __WINCE__
#endif

#if (_WIN32_WCE == 300) // for Pocket PC
#define __POCKETPC__
#define __WINCE__
//#if (_WIN32_WCE == 211) // for Palm-size PC 2.11 (Wyvern)
//#if (_WIN32_WCE == 201) // for Palm-size PC 2.01 (Gryphon)  
//#ifdef WIN32_PLATFORM_HPC2000 // for H/PC 2000 (Galileo)
#endif
#endif


//!@ 网络模块版本号
#define NETWORK_MODULE_IDENTIFICATION	"新生联创(R) network(c) module"
#define NETWORK_NUMBER_MAJOR_VERSION				0	//! 主版本号
#define NETWORK_NUMBER_MINOR_VERSION				1	//! 次要版本号
#define NETWORK_NUMBER_UPDATE_VERSION				0	//! 补丁或更新版本号
/**
*	通信路由码
*/
#define NET_COMMAND_BEGIN 0x00000000
#define NET_COMMAND_END		0xFFFFFFFF

#define NET_COMMAND_TCP_BEGIN	0x00100001
#define NET_COMMAND_TCP_END		0x001FFFFF
/**服务器端针对客户端的初次连接，发送第一条通告消息
* 如果客户端有回执信息，应该以此编号做为命令码，以便服务器端识别*/
#define		NET_COMMAND_TCP_100200		0x00100200		//! 
#define		NET_COMMAND_TCP_100201		0x00100201		//! 
#define		NET_COMMAND_TCP_100300		0x00100300		//! 
#define		NET_COMMAND_TCP_100301		0x00100301		//! 
#define		NET_COMMAND_TCP_100302		0x00100302		//! 
#define		NET_COMMAND_TCP_100303		0x00100303		//! 
#define		NET_COMMAND_TCP_100400		0x00100400		//! 
#define		NET_COMMAND_TCP_100401		0x00100401		//! 
#define		NET_COMMAND_TCP_100402		0x00100402		//! 
#define		NET_COMMAND_TCP_100403		0x00100403		//! 
#define		NET_COMMAND_TCP_100404		0x00100404		//! 
#define		NET_COMMAND_TCP_100405		0x00100405		//! 
#define		NET_COMMAND_TCP_100500		0x00100500		//! 
#define		NET_COMMAND_TCP_100600		0x00100600		//! 
#define		NET_COMMAND_TCP_100601		0x00100601		//! 
#define		NET_COMMAND_TCP_100602		0x00100602		//! 
#define		NET_COMMAND_TCP_100603		0x00100603		//! 
#define		NET_COMMAND_TCP_100700		0x00100700		//! 
#define		NET_COMMAND_TCP_100701		0x00100701		//!  
#define		NET_COMMAND_TCP_100702		0x00100702		//! 
#define		NET_COMMAND_TCP_100703		0x00100703		//! 
#define		NET_COMMAND_TCP_100704		0x00100704		//! 
#define		NET_COMMAND_TCP_100705		0x00100705		//! 
#define		NET_COMMAND_TCP_100706		0x00100706		//! 
#define		NET_COMMAND_TCP_100707		0x00100707		//! 
#define		NET_COMMAND_TCP_100800		0x00100800		//! 
#define		NET_COMMAND_TCP_100900		0x00100900		//! 


//!@ MetaQuotes 5 business
#define NET_COMMAND_TCP_110000		0x00110000		//! 
#define NET_COMMAND_TCP_11FFFF			0x0011FFFF		//! 

#define  NET_COMMAND_TCP_110001		0x00110001
#define  NET_COMMAND_TCP_110002		0x00110002
#define  NET_COMMAND_TCP_110003		0x00110003
#define  NET_COMMAND_TCP_110004		0x00110004
#define  NET_COMMAND_TCP_110005		0x00110005
#define  NET_COMMAND_TCP_110006		0x00110006
#define  NET_COMMAND_TCP_110007		0x00110007



#define NET_COMMAND_UDP_BEGIN		0x00200001
#define NET_COMMAND_UDP_END		0x002FFFFF
#define		NET_COMMAND_UDP_200200	0x00200200
#define		NET_COMMAND_UDP_200300	0x00200300
#define		NET_COMMAND_UDP_200400	0x00200400		//! 
#define		NET_COMMAND_UDP_200500	0x00200500
#define		NET_COMMAND_UDP_200600	0x00200600
#define		NET_COMMAND_UDP_200700	0x00200700
#define		NET_COMMAND_UDP_200800	0x00200800
#define		NET_COMMAND_UDP_200900	0x00200900

#define NET_COMMAND_HTTP_BEGIN	0x00300001
#define NET_COMMAND_HTTP_END		0x003FFFFF
#define		NET_COMMAND_HTTP_300200	0x00300200		//! 
#define		NET_COMMAND_HTTP_300201	0x00300201		//! 
#define		NET_COMMAND_HTTP_300202	0x00300202		//! 


#define		NET_COMMAND_HTTP_300300	0x00300300		
#define		NET_COMMAND_HTTP_300301	0x00300301		//! 

#define		NET_COMMAND_HTTP_300400	0x00300400		//! 
#define		NET_COMMAND_HTTP_300500	0x00300500
#define		NET_COMMAND_HTTP_300600	0x00300600
#define		NET_COMMAND_HTTP_300700	0x00300700
#define		NET_COMMAND_HTTP_300800	0x00300800
#define		NET_COMMAND_HTTP_300900	0x00300900

#define		NET_COMMAND_HTTP_500300	0x00500300		
#define		NET_COMMAND_HTTP_500301	0x00500301		
#define		NET_COMMAND_HTTP_500302	0x00500302		
#define		NET_COMMAND_HTTP_500303	0x00500303		
#define		NET_COMMAND_HTTP_500304	0x00500304		
#define		NET_COMMAND_HTTP_500305	0x00500305	
#define		NET_COMMAND_HTTP_500306	0x00500306		
#define		NET_COMMAND_HTTP_500307	0x00500307	
#define		NET_COMMAND_HTTP_500308	0x00500308	
#define		NET_COMMAND_HTTP_500309	0x00500309	

#define		NET_COMMAND_HTTP_500400	0x00500400		
#define		NET_COMMAND_HTTP_500500	0x00500500
#define		NET_COMMAND_HTTP_500600	0x00500600
#define		NET_COMMAND_HTTP_500700	0x00500700
#define		NET_COMMAND_HTTP_500800	0x00500800
#define		NET_COMMAND_HTTP_500900	0x00500900


#define NET_COMMAND_WEBSOCKET_BEGIN		0x00400001
#define NET_COMMAND_WEBSOCKET_END		0x004FFFFF


/// /*新生联创（上海）**/
/// /*2018年10月15日 13:25:13|193**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_network_command_H__DBC03_D68871FD_10A60021454B8DA_FFED__TAIL__

