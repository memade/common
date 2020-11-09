#if !defined(AFX_SKSTU_COMMON_H__99521_015D8167_C643491C0FE0E3C_8660__HEAD__)
#define AFX_SKSTU_COMMON_H__99521_015D8167_C643491C0FE0E3C_8660__HEAD__

#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>
#include <memory>
#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <initializer_list>
#include <future>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <codecvt>
#include <chrono>
#include <unordered_map>
#include <map>
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <set>
#include <stack>
#include <tuple>
#include <variant>
#include <assert.h>
//C++ 17
#include <charconv>


#include "sk.h"
#include "apires.hpp"
#include "command.h"
#include "macro.h"
#include "container.hpp"
#include "convert_string.hpp"
#include "helper.hpp"
#include "uuid.hpp"
#include "object.h"
#include "thread.hpp"
#include "counter.hpp"
#include "time.hpp"
#include "sync.hpp"

namespace sk
{
	namespace {
		template <typename Functor>
		struct function_traits
			: public function_traits<decltype(&Functor::operator())> {
		};

		template <typename R, typename... Args>
		struct function_traits<R(Args...)> {
			typedef R result_type;

			typedef result_type return_type;

			typedef result_type function_type(Args...);

			enum {
				arity = sizeof...(Args)
			};

			typedef std::tuple<Args...> tuple_type;

			template <size_t i>
			struct arg {
				typedef typename std::tuple_element<i, tuple_type>::type type;
			};
		};

		template <typename R, typename... Args>
		struct function_traits<R(*)(Args...)>
			: public function_traits<R(Args...)> {
		};

		template <typename C, typename R, typename... Args>
		struct function_traits<R(C::*)(Args...)>
			: public function_traits<R(Args...)> {
			typedef C& owner_type;
		};

		template <typename C, typename R, typename... Args>
		struct function_traits<R(C::*)(Args...) const>
			: public function_traits<R(Args...)> {
			typedef const C& owner_type;
		};

		template <typename C, typename R, typename... Args>
		struct function_traits<R(C::*)(Args...) volatile>
			: public function_traits<R(Args...)> {
			typedef volatile C& owner_type;
		};

		template <typename C, typename R, typename... Args>
		struct function_traits<R(C::*)(Args...) const volatile>
			: public function_traits<R(Args...)> {
			typedef const volatile C& owner_type;
		};

#if defined(_GLIBCXX_FUNCTIONAL)
#define MEM_FN_SYMBOL std::_Mem_fn
#elif defined(_LIBCPP_FUNCTIONAL)
#define MEM_FN_SYMBOL std::__mem_fn
#endif

#ifdef MEM_FN_SYMBOL
		template <typename R, typename... Args>
		struct function_traits<MEM_FN_SYMBOL<R(*)(Args...)>>
			: public function_traits<R(Args...)> {
		};

		template <typename C, typename R, typename... Args>
		struct function_traits<MEM_FN_SYMBOL<R(C::*)(Args...)>>
			: public function_traits<R(Args...)> {
			typedef C& owner_type;
		};

		template <typename C, typename R, typename... Args>
		struct function_traits<MEM_FN_SYMBOL<R(C::*)(Args...) const>>
			: public function_traits<R(Args...)> {
			typedef const C& owner_type;
		};

		template <typename C, typename R, typename... Args>
		struct function_traits<MEM_FN_SYMBOL<R(C::*)(Args...) volatile>>
			: public function_traits<R(Args...)> {
			typedef volatile C& owner_type;
		};

		template <typename C, typename R, typename... Args>
		struct function_traits<MEM_FN_SYMBOL<R(C::*)(Args...) const volatile>>
			: public function_traits<R(Args...)> {
			typedef const volatile C& owner_type;
		};

#undef MEM_FN_SYMBOL
#endif

		template <typename Functor>
		struct function_traits<std::function<Functor>>
			: public function_traits<Functor> {
		};

		template <typename T>
		struct function_traits<T&> : public function_traits<T> {
		};

		template <typename T>
		struct function_traits<const T&> : public function_traits<T> {
		};

		template <typename T>
		struct function_traits<volatile T&> : public function_traits<T> {
		};

		template <typename T>
		struct function_traits<const volatile T&> : public function_traits<T> {
		};

		template <typename T>
		struct function_traits<T&&> : public function_traits<T> {
		};

		template <typename T>
		struct function_traits<const T&&> : public function_traits<T> {
		};

		template <typename T>
		struct function_traits<volatile T&&> : public function_traits<T> {
		};

		template <typename T>
		struct function_traits<const volatile T&&> : public function_traits<T> {
		};

		/*
		 * This seems to be slightly better than the standard library version as of now.
		 * */

		template <typename Functor>
		using fn_result_of = typename function_traits<Functor>::result_type;
	}


}///namespace sk





/// /*新生联创（上海）**/
/// /*2019年07月29日 9:17:55|562**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_COMMON_H__99521_015D8167_C643491C0FE0E3C_8660__TAIL__