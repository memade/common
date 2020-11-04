#if !defined(AFX_SK_H__5899C5C4_0383_4499_BD3D_5C0971C0DAFE_2020114900__HEAD__)
#define AFX_SK_H__5899C5C4_0383_4499_BD3D_5C0971C0DAFE_2020114900__HEAD__

namespace sk {
	class Plugin final
	{
	private:
		using tf_api_plugin_init = int(__stdcall*)(
			char**, size_t&,/*插件名字*/
			char**,size_t&/*插件扩展方法签名|名字*/
			);
		using tf_api_plugin_uninit = int(__stdcall*)();
		using tf_api_plugin_start = int(__stdcall*)(const std::intptr_t&);
		using tf_api_plugin_stop = int(__stdcall*)();
		using tf_api_plugin_ext = void(__stdcall*)();

		const char* api_plugin_init = "api_plugin_init";
		const char* api_plugin_uninit = "api_plugin_uninit";
		const char* api_plugin_start = "api_plugin_start";
		const char* api_plugin_stop = "api_plugin_stop";
	public:
		Plugin(const sk::stringa& plugindir) :
			m_PluginDir(plugindir)
		{

		}
		~Plugin()
		{

		}
	public:
		bool Load(const HMENU& hMenuMain,const WPARAM& MenuFirstID)
		{
			if (!sk::Helper::CheckDirectory(m_PluginDir.c_str()))
				return false;

			std::map<std::string, _finddata_t> files;
			sk::Helper::EnumFile(m_PluginDir, files, "*.dll");

			for (const auto& file : files)
			{
				auto pathname = m_PluginDir + "\\" + file.first;
				auto hModule = ::LoadLibraryA(pathname.c_str());
				if (!hModule)
					continue;
				sk::container::map<sk::stringa, void*> fns;
				auto fn_api_plugin_init = (tf_api_plugin_init)::GetProcAddress(hModule, api_plugin_init);
				auto fn_api_plugin_uninit = (tf_api_plugin_uninit)::GetProcAddress(hModule, api_plugin_uninit);
				auto fn_api_plugin_start = (tf_api_plugin_start)::GetProcAddress(hModule, api_plugin_start);
				auto fn_api_plugin_stop = (tf_api_plugin_stop)::GetProcAddress(hModule, api_plugin_stop);

				if (!api_plugin_init || !fn_api_plugin_uninit || !fn_api_plugin_start || !fn_api_plugin_stop)
					continue;
				fns.push(api_plugin_init, fn_api_plugin_init);
				fns.push(api_plugin_uninit, fn_api_plugin_uninit);
				fns.push(api_plugin_start, fn_api_plugin_start);
				fns.push(api_plugin_stop, fn_api_plugin_stop);

				size_t plugin_sign_len = _MAX_PATH;
				auto plugin_sign = new char[plugin_sign_len];
				size_t plugin_sign_ext_len = plugin_sign_len << 4;
				auto plugin_sign_ext = new char[plugin_sign_ext_len];

				if (fn_api_plugin_init(&plugin_sign, plugin_sign_len, &plugin_sign_ext, plugin_sign_ext_len))
				{
					SK_DELETE_PTR_BUFFER(plugin_sign);
					SK_DELETE_PTR_BUFFER(plugin_sign_ext);
					continue;
				}

				if (m_hPluginFns.search(plugin_sign, [](const auto&, auto&) {}))
				{
					SK_DELETE_PTR_BUFFER(plugin_sign);
					SK_DELETE_PTR_BUFFER(plugin_sign_ext);
					continue;
				}

				sk::container::map<sk::stringa, sk::stringa> fnexts;
				std::vector<sk::stringa> ext_signs;
				sk::Helper::OneUnknownParseA(plugin_sign_ext, ',', ext_signs);
				for (const auto& sign : ext_signs)
				{
					std::vector<sk::stringa> pares;
					sk::Helper::OneUnknownParseA(sign, '@', pares);
					if (pares.size() >= 2)
					{
						auto fn_target = (tf_api_plugin_ext)::GetProcAddress(hModule, pares[0].c_str());
						fns.push(pares[0], fn_target);
						fnexts.push(pares[0], pares[1]);
					}
				}

				m_hPluginSigns.push(hModule, sk::stringa(plugin_sign, plugin_sign_len));
				m_hPluginFns.push(plugin_sign, fns);
				m_hPluginExtFns.push(plugin_sign, fnexts);

				SK_DELETE_PTR_BUFFER(plugin_sign);
				SK_DELETE_PTR_BUFFER(plugin_sign_ext);
			}





			auto first_id = MenuFirstID;

			m_hPluginSigns.iterate(
				[&](const auto&, auto& sign)
				{
					auto hPluginMenu = ::CreatePopupMenu();
					if (hPluginMenu)
					{
						auto hPluginSubMenu = ::CreatePopupMenu();
						m_hPluginExtFns.iterate(
							[&](const auto&, auto& submenus)
							{
								submenus.iterate(
									[&](const auto& sub_sign, auto& sub_name)
									{
										auto id = ++first_id;
										m_MenuMapQ.push(id, sub_sign);
										::AppendMenuA(hPluginSubMenu, MF_STRING, id, sub_name.c_str());
									});
							});
						::AppendMenuA(hPluginMenu, MF_STRING | MF_POPUP, (UINT_PTR)hPluginSubMenu, sign.c_str());
						::AppendMenuA(hMenuMain, MF_STRING | MF_POPUP, (UINT_PTR)hPluginMenu, "Plugins");
					}
				});





			return !m_hPluginSigns.empty() && !m_hPluginFns.empty();
		}
		void UnLoad()
		{
			m_hPluginFns.iterate(
				[&](const auto&, auto& fns)
				{
					fns.search(
						api_plugin_uninit,
						[&](const auto&, auto& fn)
						{
							((tf_api_plugin_uninit)fn)();
						});
				});
			m_hPluginSigns.iterate(
				[](const auto& __module, auto&)
				{
					::FreeLibrary(__module);
				});
			m_hPluginFns.clear();
			m_hPluginSigns.clear();
			m_hPluginExtFns.clear();
		}
		int PluginStart(const sk::stringa& plugin_sign, const std::intptr_t& spi)
		{
			int result = -1;
			m_hPluginFns.search(
				plugin_sign,
				[&](const auto&, auto& fns)
				{
					fns.search(
						api_plugin_start,
						[&](const auto&, auto& fn)
						{
							result = ((tf_api_plugin_start)fn)(spi);
						});
				});
			return result;
		}
		int PluginStop(const sk::stringa& plugin_sign)
		{
			int result = -1;
			m_hPluginFns.search(
				plugin_sign,
				[&](const auto&, auto& fns)
				{
					fns.search(
						api_plugin_stop,
						[&](const auto&, auto& fn)
						{
							result = ((tf_api_plugin_stop)fn)();
						});
				});
			return result;
		}
		bool OnMenu(const WPARAM& wParam)
		{
			return m_MenuMapQ.search(
				wParam,
				[&](const auto&, auto& sign)
				{
					m_hPluginExtFns.iterate(
						[&](const auto& plguin_sign, auto& exts)
						{
							if (exts.search(sign, [](const auto&, auto&) {}))
							{
								m_hPluginFns.search(
									plguin_sign,
									[&](const auto&, auto& fns)
									{
										fns.search(sign,
											[&](const auto&, auto& fn)
											{
												((tf_api_plugin_ext)fn)();
											});
									});
							}
						});
				});
		}
	public:
		sk::stringa m_PluginDir;
		sk::container::map<WPARAM/*menuid*/, sk::stringa/*plugin ext sign*/> m_MenuMapQ;
		sk::container::map<HMODULE, sk::stringa/*plugin sign*/> m_hPluginSigns;
		sk::container::map<sk::stringa/*plugin sign*/, sk::container::map<sk::stringa/*plugin fn sign*/, void* /*fn*/>> m_hPluginFns;
		sk::container::map<sk::stringa/*plugin sign*/, sk::container::map<sk::stringa/*plugin ext sign*/, sk::stringa/*plugin ext name*/>> m_hPluginExtFns;
	};
}///namespace sk



//!@ /*新生联创®（上海）*/
//!@ /*Fri Oct 9 15:01:31 UTC+0800 2020*/
//!@ /*___www.skstu.com___*/
#endif /*AFX_SK_H__5899C5C4_0383_4499_BD3D_5C0971C0DAFE_2020114900__HEAD__*/