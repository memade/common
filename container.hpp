#if !defined(AFX_SKSTU_container_H__8213A_D8902B1C_DDB8875B51CB106_7AB5__HEAD__)
#define AFX_SKSTU_container_H__8213A_D8902B1C_DDB8875B51CB106_7AB5__HEAD__

namespace sk {
	namespace container {

		class container_impl {
		public:
			container_impl() 
			{
				m_mutex = std::make_shared<std::mutex>();
				m_condition_variable = std::make_shared<std::condition_variable>();
			}
			virtual ~container_impl() {}
		protected:
			std::shared_ptr<std::mutex> m_mutex;
			std::shared_ptr < std::condition_variable> m_condition_variable;
		};

		class buffer final : public container_impl {
		public:
			buffer() {}
			~buffer() {}
		public:
			size_t push(const char* data, const size_t& ndata) {
				std::lock_guard<std::mutex> _lock(*m_mutex);
				m_buffer.append(data, ndata);
				return m_buffer.size();
			}
			size_t push(const std::string& data) {
				std::lock_guard<std::mutex> _lock(*m_mutex);
				m_buffer.append(data);
				return m_buffer.size();
			}
			std::shared_ptr<std::string> pop() {
				std::shared_ptr<std::string> result;
				std::lock_guard<std::mutex> _lock(*m_mutex);
				if (m_buffer.empty()) return result;
				result = std::make_shared<std::string>(m_buffer.data(), m_buffer.size());
				m_buffer.clear();
				return result;
			}
			std::shared_ptr<std::string> front() {
				std::shared_ptr<std::string> result;
				std::lock_guard<std::mutex> _lock(*m_mutex);
				if (m_buffer.empty()) return result;
				result = std::make_shared<std::string>(m_buffer.data(), m_buffer.size());
				return result;
			}
			void pop(const size_t& nClear) {
				std::lock_guard<std::mutex> _lock(*m_mutex);
				if (m_buffer.empty()) return;
				if (m_buffer.size() < nClear) { m_buffer.clear(); return; }
				m_buffer.erase(0, nClear);
			}
			size_t size() {
				std::lock_guard<std::mutex> _lock(*m_mutex);
				return m_buffer.size();
			}
			bool empty() {
				std::lock_guard<std::mutex> _lock(*m_mutex);
				return m_buffer.empty();
			}
		private:
			std::string m_buffer;
		};


		template<typename T>
		class queue final : public container_impl {
		public:
			queue() {}
			virtual ~queue() {}
		public:
			void push(const T& data)
			{
				std::lock_guard<std::mutex> _lock(*m_mutex);
				m_queue.push(data);
			}
			void push(const std::vector<T>& datas)
			{
				std::lock_guard<std::mutex> _lock(*m_mutex);
				for (const auto& data : datas)
				{
					m_queue.push(data);
				}
			}
			std::shared_ptr<T> pop()
			{
				std::shared_ptr<T> result;
				std::lock_guard<std::mutex> _lock(*m_mutex);
				if (!m_queue.empty())
				{
					result = std::make_shared<T>(m_queue.front());
					m_queue.pop();
				}
				return result;
			}
			bool empty()
			{
				bool result = false;
				std::lock_guard<std::mutex> _lock(*m_mutex);
				result = m_queue.empty();
				return result;
			}
			size_t size()
			{
				size_t result = 0;
				std::lock_guard<std::mutex> _lock(*m_mutex);
				result = m_queue.size();
				return result;
			}
			void clear()
			{
				std::lock_guard<std::mutex> _lock(*m_mutex);
				std::queue<T> empty;
				std::swap(m_queue, empty);
			}
		private:
			std::queue<T> m_queue;
		};

		template<typename T>
		class set final : public container_impl {
		public:
			set() {}
			virtual ~set() {}
			void operator=(const std::set<T>& stlset) 
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				m_set = stlset;
			}
			void operator=(const set<T>& skset) 
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				m_set = skset.m_set;
			}
		public:
			std::shared_ptr<T> begin() const 
			{
				std::shared_ptr<T> result;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (!m_set.empty()) 
				{
					return std::make_shared<T>(*m_set.begin());
				}
				return result;
			}
			std::shared_ptr<T> end() const
			{
				std::shared_ptr<T> result;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (!m_set.empty()) {
					return std::make_shared<T>(*std::prev(m_set.end()));
				}
				return result;
			}
			
			//!签名 : push
			//!说明 : 
			//!注意 : return true : 存在 false 不存在
			//!日期 : Sat Jun 13 20:11:38 UTC+0800 2020
			bool push(const T& data)
			{
				bool result = false;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (m_set.find(data) != m_set.end())
				{
					m_set.erase(data);
					result = true;
				}
				m_set.insert(data).second;
				return result;
			}

			//!签名 : pushpush
			//!说明 : 
			//!注意 : 存在回调
			//!日期 : Sat Jun 13 17:55:07 UTC+0800 2020
			bool pushpush(const T& data, const std::function<void(T&)>& pushpush_cb)
			{
				bool result = false;
				std::lock_guard<std::mutex> lock(*m_mutex);
				auto find = m_set.find(data);
				if (find != m_set.end())
				{
					auto val = *find;
					m_set.erase(find);
					pushpush_cb(val);
					result = m_set.insert(val).second;
				}
				else
				{
					result = m_set.insert(data).second;
				}
				return result;
			}
			bool pop(const T& key) 
			{
				bool result = false;
				std::lock_guard<std::mutex> lock(*m_mutex);
				auto find = m_set.find(key);
				if (find != m_set.end())
				{
					m_set.erase(find);
					result = true;
				}
				return result;
			}
			bool empty() const
			{
				bool result = false;
				std::lock_guard<std::mutex> lock(*m_mutex);
				result = m_set.empty();
				return result;
			}
			size_t size() 
			{
				size_t result = 0;
				std::lock_guard<std::mutex> lock(*m_mutex);
				result= m_set.size();
				return result;
			}
			void clear() 
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				m_set.clear();
			}
			void clearat(const std::function<void(const T&,bool&)>& iterate_cb)
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				bool isClear = false;
				for (auto it = m_set.begin(); it != m_set.end();)
				{
					isClear = false;
					iterate_cb((T&)(*it), isClear);
					if (true == isClear)
					{
						it = m_set.erase(it);
					}
					else
					{
						++it;
					}
				}
			}
			std::shared_ptr<T> search(const T& key)
			{
				std::shared_ptr<T> result;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (!m_set.empty()) {
					auto find = m_set.find(key);
					if (find != m_set.end())
					{
						return std::make_shared<T>(*find);
					}
				}
				return result;
			}
			bool search(const T& key,const std::function<void(const T&)>& search_cb) 
			{
				bool result = false;
				std::lock_guard<std::mutex> lock(*m_mutex);
				auto find = m_set.find(key);
				if (find != m_set.end())
				{
					search_cb(*find);
					result = true;
				}
				return result;
			}
			bool search(const T& key) const
			{
				bool result = false;
				std::lock_guard<std::mutex> lock(*m_mutex);
				auto find = m_set.find(key);
				if (find != m_set.end())
				{
					result = true;
				}
				return result;
			}
			void iterate(const std::function<void(T&)>& iteratecb) 
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				for (auto it = m_set.begin(); it != m_set.end(); ++it) 
				{
					iteratecb((T&)(*it));
				}
			}
			void iterate(const std::function<void(T&,bool&)>& iteratecb)
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				bool itbreak = false;
				for (auto it = m_set.begin(); it != m_set.end(); ++it) {				
					iteratecb((T&)(*it), itbreak);
					if (itbreak == true)
					{
						break;
					}
				}
			}
			void iterate(const std::function<void(const T&, bool&)>& iteratecb) const
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				bool itbreak = false;
				for (auto it = m_set.begin(); it != m_set.end(); ++it) {
					iteratecb(*it, itbreak);
					if (itbreak == true)
					{
						break;
					}
				}
			}
			std::vector<T> Vector()
			{
				std::vector<T> result;
				std::lock_guard<std::mutex> lock(*m_mutex);
				for (const auto& node : m_set)
				{
					result.emplace_back(node);
				}
				return result;
			}
			std::set<T> Source()
			{
				std::set<T> result;
				std::lock_guard<std::mutex> lock(*m_mutex);
				for (const auto& node : m_set)
				{
					result.insert(node);
				}
				return result;
			}
		private:
			std::set<T> m_set;
		};

		template<typename T>
		class list final : public container_impl 
		{
		public:
			list() : m_list() {}
			virtual ~list() {}
		public:
			void operator=(const std::list<T>& _Target) 
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				m_list.clear();
				m_list = _Target;
			}
			void operator=(const list<T>& _Target) 
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				m_list.clear();
				m_list = _Target.m_list;
			}
		public:
			void push_front(const T& data)
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				m_list.push_front(data);
			}
			void push_back(const T& data)
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				m_list.push_back(data);
			}
			std::shared_ptr<T> pop_back()
			{
				std::shared_ptr<T> result;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (m_list.empty()) { return result; }
				result = std::make_shared<T>(m_list.back());
				m_list.pop_back();
				return result;
			}
			std::shared_ptr<T> pop_front()
			{
				std::shared_ptr<T> result;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (m_list.empty()) { return result; }
				result = std::make_shared<T>(m_list.front());
				m_list.pop_front();
				return result;
			}
			T& back()
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				return m_list.back();
			}
			T& front()
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				return m_list.front();
			}
			std::size_t size()
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				return m_list.size();
			}
			bool empty()
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				return m_list.empty();
			}
			void clear() 
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				m_list.clear();
			}
			T* search(const T& data)
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				auto find = std::find(m_list.begin(), m_list.end(), data);
				if (find != m_list.end()) {
					return &(*find);
				}
				else
				{
					return nullptr;
				}
			}
			void iterate(const std::function<void(T&)>& iterate_cb) 
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				for (auto it = m_list.begin(); it != m_list.end(); ++it) 
				{
					if (iterate_cb)
					{
						iterate_cb(*it);
					}
				}
			}

			std::vector<T> Vector() const
			{
				std::vector<T> result;
				std::lock_guard<std::mutex> lock(*m_mutex);
				for (const auto& node : m_list) { result.emplace_back(node); }
				return result;
			}
		private:
			std::list<T> m_list;
		};

		
		template<typename K, typename V>
		class map final : public container_impl {
			size_t m_max_size = 0;
		public:
			map() {}
			map(const std::map<K, V>& src) : m_map(src) {}
			map(const container::map<K, V>& src) : m_map(src.m_map) {}
			virtual ~map() { m_map.clear(); }
			void MaxSize(const size_t& max) { m_max_size = max; }
		public:
			V& operator[](const K& key)
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				return m_map[m_map.insert(std::make_pair(key, V())).first->first];
			}
			void operator=(const std::map<K, V>& src) 
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				m_map = src;
			}
			void operator=(const container::map<K, V>& src) 
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				m_map = src.m_map;
			}
			void operator()(const std::map<K, V>& src)
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				m_map = src.m_map;
			}
			void operator()(const container::map<K, V>& src)
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				m_map = src.m_map;
			}
		public:
			//!@ 更新或替换 -- 如果不存在则回调
			bool push_insert_cb(const K& key, const V& val, const std::function<void(const V&)>& insert_cb)
			{
				bool result = false;
				std::lock_guard<std::mutex> lock(*m_mutex);
				auto find = m_map.find(key);
				if (find != m_map.end())
				{
					m_map.erase(find);
				}
				else
				{
					result = true;
					insert_cb(val);
				}
				m_map.insert(std::make_pair(key, val));
				return result;
			}
			//!@ 更新或替换 -- 如果存在则回调、不存在则插入(不回调)
			void push_exist_cb(const K& key, const V& val, const std::function<void(V&)>& push_exist_cb)
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				auto find = m_map.find(key);
				if (find != m_map.end())
				{
					push_exist_cb(find->second);
				}
				else
				{
					m_map.insert(std::make_pair(key, val));
				}
			}
			//!@ 更新或替换
			bool push(const K& key, const V& val) 
			{
				bool result = false;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (m_max_size > 0 && m_map.size() >= m_max_size)
				{
					m_map.erase(m_map.begin());
				}
				auto find = m_map.find(key);
				if (find != m_map.end()) 
				{
					m_map.erase(find);
				}
				else 
				{
					result = true;
				}
				m_map.insert(std::make_pair(key, val));
				return result;
			}
			//!@ 更新或替换、并提供更新前的值
			bool push(const K& key, const V& val, const std::function<void(V& /*prev*/)>& push_cb) 
			{
				bool result = false;
				std::lock_guard<std::mutex> lock(*m_mutex);
				auto find = m_map.find(key);
				if (find == m_map.end()) 
				{
					m_map.insert(std::make_pair(key, val));
					result = true;
				}
				else 
				{
					push_cb(find->second);
					m_map.erase(find);
					m_map.insert(std::make_pair(key, val));
				}
				return result;
			}
			//!@ 专用压栈
			void push_cb(const K& key, const V& val, const std::function<void(V&)>& push_result_cb)
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				auto find = m_map.find(key);
				if (find == m_map.end())
				{
					push_result_cb(m_map[m_map.insert(std::make_pair(key, val)).first->first]);
				}
				else
				{
					m_map.erase(find);
					push_result_cb(m_map[m_map.insert(std::make_pair(key, val)).first->first]);
				}
			}
			//!@ 嵌套队列的插入、提供值队列的引用
			bool pushpush(const K& key, const std::function<void(V&)>& push_cb) 
			{
				bool result = false;
				std::lock_guard<std::mutex> lock(*m_mutex);
				auto find = m_map.find(key);
				if (find == m_map.end()) 
				{
					V newV = V();
					m_map.insert(std::make_pair(key, newV));
				}
				else
				{
					result = true;
				}
				push_cb(m_map[key]);
				return result;
			}
			//!说明 : pushpush_cb
			//!注意 : 存在回调
			//!日期 : Thu Jun 11 17:32:35 UTC+0800 2020
			void pushpush_cb(const K& key, const V& val, const std::function<void(V&,bool&)>& push_result_cb)
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				auto find = m_map.find(key);
				if (find == m_map.end())
				{
					bool clear = false;
					push_result_cb(m_map[m_map.insert(std::make_pair(key, val)).first->first], clear);
					if (clear)
					{
						m_map.erase(key);
					}
				}
			}
			//!@ 释放并返回开始位置数据
			std::shared_ptr<std::tuple<K, V>> pop()
			{
				std::shared_ptr<std::tuple<K, V>> result;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (!m_map.empty())
				{
					auto begin = m_map.begin();
					if (begin != m_map.end())
					{
						result = std::make_shared<std::tuple<K, V>>(std::tie(begin->first, begin->second));
						m_map.erase(begin);
					}
				}
				return result;
			}
			//!@ 释放并返回指定位置的值
			void pop(const K& key)
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (!m_map.empty())
				{
					auto find = m_map.find(key);
					if (find != m_map.end())
					{
						m_map.erase(find);
					}
				}
			}
			//!@ 释放满足自定义条件的值的位置
			//!@ 返回释放的个数
			size_t pop(const std::function<bool(const V&)>& cbCompare) 
			{
				size_t pop_count = 0;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (!m_map.empty())
				{
					for (auto it = m_map.begin(); it != m_map.end();)
					{
						if (cbCompare(it->second))
						{
							it = m_map.erase(it);
							++pop_count;
							continue;
						}
						++it;
					}
				}
				return pop_count;
			}
			bool end(const std::function<void(const K&, const V&)>& end_cb) const
			{
				bool result = false;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (!m_map.empty())
				{
					auto itend = std::prev(m_map.end());
					end_cb(itend->first, itend->second);
					result = true;
				}
				return result;
			}
			bool begin(const std::function<void(const K&, const V&)>& begin_cb) const
			{
				bool result = false;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (!m_map.empty())
				{
					begin_cb(m_map.begin()->first, m_map.begin()->second);
					result = true;
				}
				return result;
			}
			//!@ 传递释放之前的数据
			bool pop(const K& key, const std::function<void(const K&, V&)>& pop_cb)
			{
				bool result = false;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (!m_map.empty())
				{
					auto find = m_map.find(key);
					if (find != m_map.end())
					{
						pop_cb(find->first, find->second);
						m_map.erase(find);
						result = true;
					}
				}
				return result;
			}
			//!@ 专用于某项目
			bool pop(const K& key, const std::function<void(const K&, V&)>& pop_cb, const std::function<void()>& un_pop_cb)
			{
				bool result = false;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (!m_map.empty())
				{
					auto find = m_map.find(key);
					if (find != m_map.end())
					{
						pop_cb(find->first, find->second);
						m_map.erase(find);
						result = true;
					}
				}
				if (!result)
				{
					un_pop_cb();
				}
				return result;
			}

			bool search(const K& key, const std::function<void(const K&, V&)>& found_cb, const std::function<void(std::map<K, V>&)>& notfound_cb)
			{
				bool result = false;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (!m_map.empty())
				{
					auto find = m_map.find(key);
					if (find != m_map.end())
					{
						found_cb(find->first, find->second);
						result = true;
					}
				}
				if (!result)
				{
					notfound_cb(m_map);
				}
				return result;
			}

			bool search(const K& key, const std::function<void(V&)>& found_cb)
			{
				bool result = false;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (!m_map.empty())
				{
					auto find = m_map.find(key);
					if (find != m_map.end())
					{
						found_cb(find->second);
						result = true;
					}
				}
				return result;
			}

			bool search_clear(const K& key, const std::function<void(V&,bool&)>& found_cb)
			{
				bool result = false;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (!m_map.empty())
				{
					auto find = m_map.find(key);
					if (find != m_map.end())
					{
						bool clear = false;
						found_cb(find->second, clear);
						if (clear)
						{
							m_map.erase(find);
						}
						result = true;
					}
				}
				return result;
			}

			bool search(const K& key, const std::function<void(const K&, V&)>& found_cb)
			{
				bool result = false;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (!m_map.empty()) 
				{
					auto find = m_map.find(key);
					if (find != m_map.end())
					{
						found_cb(find->first, find->second);
						result = true;
					}
				}
				return result;
			}

			std::shared_ptr<V> search(const K& key) const
			{
				std::shared_ptr<V> result;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (!m_map.empty()) 
				{
					auto find = m_map.find(key);
					if (find != m_map.end()) 
					{
						result = std::make_shared<V>(find->second);
					}
				}
				return result;
			}

			bool exists(const K& key) const
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				return m_map.find(key) == m_map.end() ? false : true;
			}
			void iterate(const std::function<void(const K&, V&, const size_t&/*迭代次数*/, bool&/*是否中断*/)>& iterate_cb)
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				size_t itcount = 0;
				bool itbreak_flag = false;
				if (!m_map.empty())
				{
					for (auto it = m_map.begin(); it != m_map.end(); ++it)
					{
						if (itbreak_flag == true)
						{
							break;
						}
						iterate_cb(it->first, it->second, ++itcount, itbreak_flag);
					}
				}
			}

			void iterate(const std::function<void(const K&, V&, bool&/*是否中断*/)>& iterate_cb)
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				bool itbreak_flag = false;
				if (!m_map.empty())
				{
					for (auto it = m_map.begin(); it != m_map.end(); ++it)
					{
						if (itbreak_flag == true)
						{
							break;
						}
						iterate_cb(it->first, it->second, itbreak_flag);
					}
				}
			}

			void iterate(const std::function<void(const K&, const V&, bool&/*是否中断*/)>& iterate_cb) const
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				bool itbreak_flag = false;
				if (!m_map.empty())
				{
					for (auto it = m_map.begin(); it != m_map.end(); ++it)
					{
						if (itbreak_flag == true)
						{
							break;
						}
						iterate_cb(it->first, it->second, itbreak_flag);
					}
				}
			}

			void riterate(const std::function<void(const K&, const V&, bool&/*是否中断*/)>& riterate_cb) const
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				bool ritbreak_flag = false;
				if (!m_map.empty())
				{
					for (auto rit = m_map.rbegin(); rit != m_map.rend(); ++rit)
					{
						if (ritbreak_flag == true)
						{
							break;
						}
						riterate_cb(rit->first, rit->second, ritbreak_flag);
					}
				}
			}

			void iterate_clear(const std::function<void(const K&, V&, bool&/*是否中断*/, bool&/*是否删除*/)>& iterate_cb)
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				size_t itcount = 0;
				bool itbreak_flag = false;
				bool itclear_flag = false;
				if (!m_map.empty())
				{
					for (auto it = m_map.begin(); it != m_map.end();)
					{
						if (itbreak_flag == true)
						{
							break;
						}
						itclear_flag = false;
						iterate_cb(it->first, it->second, itbreak_flag, itclear_flag);
						if (itclear_flag == true)
						{
							it = m_map.erase(it);
							continue;
						}
						++it;
					}
				}
			}

			void clearat(const std::function<void(const K&, V&,bool&)>& iterate_cb)
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (!m_map.empty())
				{
					bool isClear = false;
					for (auto it = m_map.begin(); it != m_map.end();)
					{
						isClear = false;
						iterate_cb(it->first, it->second, isClear);
						if (isClear == true)
						{
							it = m_map.erase(it);
						}
						else
						{
							++it;
						}
					}
				}
			}

			//!@ 统计满足指定条件的值的数量
			size_t count(const std::function<bool(const V&)>& compare_cb) 
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				size_t result = 0;
				if (!m_map.empty()) 
				{
					for (const auto& node : m_map)
					{
						if (compare_cb(node.second))
						{
							++result;
						}
					}
				}
				return result;
			}
			void iterate(const std::function<void(const K&, V&)>& iterate_cb)
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (!m_map.empty()) 
				{
					for (auto it = m_map.begin(); it != m_map.end(); ++it)
					{
						iterate_cb(it->first, it->second);
					}
				}
			}
			void riterate(const std::function<void(const K&, V&)>& riterate_cb)
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (!m_map.empty()) 
				{
					for (auto rit = m_map.rbegin(); rit != m_map.rend(); ++rit) 
					{
						riterate_cb(rit->first, rit->second);
					}
				}
			}
			size_t size() const
			{
				size_t result = 0;
				std::lock_guard<std::mutex> lock(*m_mutex);
				result = m_map.size();
				return result;
			}
			bool empty() const
			{
				bool result = true;
				std::lock_guard<std::mutex> lock(*m_mutex);
				result = m_map.empty();
				return result;
			}
			void clear() 
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				m_map.clear();
			}
			std::shared_ptr<std::map<K, V>> src()
			{
				std::shared_ptr<std::map<K, V>> result;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (!m_map.empty())
				{
					result = std::make_shared<std::map<K, V>>(m_map);
				}
				return result;
			}

			std::vector<V> Vector() const
			{
				std::vector<V> result;
				std::lock_guard<std::mutex> lock(*m_mutex);
				for (const auto& node : m_map)
				{
					result.emplace_back(node.second);
				}
				return result;
			}
		private:
			std::map<K, V> m_map;
		};


		template<typename K, typename V>
		class multimap final : public container_impl 
		{
		private:
			size_t m_max_size = 0;
		public:
			multimap(size_t max_size = 0) : m_max_size(max_size) {}
			virtual ~multimap() {}
			void SetMaxSize(const size_t& max_size)
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				m_max_size = max_size;
			}
		public:
			size_t push(const K& key, const V& val) 
			{
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (m_max_size > 0)
				{
					while (m_map.size() >= m_max_size) {
						m_map.erase(std::prev(m_map.end()));
					}
				}
				m_map.insert(std::make_pair(key, val));
				return m_map.size();
			}

			std::shared_ptr<std::list<V>> search(const std::function<bool(const V&)>& cbSerach)
			{
				std::shared_ptr<std::list<V>> result;
				std::lock_guard<std::mutex> lock(*m_mutex);
				for (const auto& node : m_map)
				{
					if (cbSerach(node.second))
					{
						if (!result)
						{
							result = std::make_shared<std::list<V>>();
						}
						result->push_front(node.second);
					}
					else
					{
						break;
					}
				}
				return result;
			}

			std::shared_ptr<std::list<V>> get(const size_t& count)
			{
				std::shared_ptr<std::list<V>> result;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (count > 0)
				{
					size_t pos = 0;
					for (const auto& node : m_map)
					{
						if (count < pos) break;
						if (!result)
						{
							result = std::make_shared<std::list<V>>();
						}
						result->push_front(node.second);
						++pos;
					}
				}
				return result;
			}







			std::shared_ptr<std::tuple<K, V>> pop() 
			{
				std::shared_ptr<std::tuple<K, V>> val;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (m_map.empty()) {
					return val;
				}
				auto it = m_map.begin();
				if (it == m_map.end()) {
					return val;
				}
				val = std::make_shared<std::tuple<K, V>>(std::tie(it->first, it->second));
				m_map.erase(it);
				return val;
			}
			void pop_back(size_t count = 1) {
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (m_map.empty()) {
					return;
				}
				if (m_map.size() <= count) {
					m_map.clear();
					return;
				}
				do {
					if (count <= 0) {
						break;
					}
					if (m_map.empty()) {
						break;
					}
					m_map.erase(m_map.end()--);
					--count;
				} while (1);

			}
			std::shared_ptr<V> pop(const K& key) {
				std::shared_ptr<V> val;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (m_map.empty()) {
					return val;
				}
				auto it = m_map.find(key);
				if (it == m_map.end()) {
					return val;
				}
				val = std::make_shared<V>(std::move(it->second));
				m_map.erase(it);
				return val;
			}
			std::shared_ptr<std::tuple<K, V>> pop(const std::function<void(const K& _key, V& _val)>& _cb) {
				std::shared_ptr<std::tuple<K, V>> val;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (m_map.empty()) {
					return val;
				}
				auto it = m_map.begin();
				if (it == m_map.end()) {
					return val;
				}
				val = std::make_shared<std::tuple<K, V>>(std::tie(it->first, it->second));
				_cb(it->first, it->second);
				m_map.erase(it);
				return val;
			}
			bool pop(const K& key, const std::function<void(const K& _key, V& _val)>& _cb) {
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (m_map.empty()) {
					return false;
				}
				auto it = m_map.find(key);
				if (it == m_map.end()) {
					return false;
				}
				_cb(it->first, it->second);
				m_map.erase(it);
				return true;
			}
			bool pop(const K& key, const V& value, const std::function<void(const K&, const V&)>& pop_cb) {
				if (m_map.empty()) {
					return false;
				}
				std::lock_guard<std::mutex> lock(*m_mutex);
				auto itFind = m_map.find(key);
				if (itFind != m_map.end()) {
					for (std::size_t i = 0; i < m_map.count(key); ++i, ++itFind) {
						if (itFind->second == value) {
							if (pop_cb) {
								pop_cb(itFind->first, itFind->second);
							}
							m_map.erase(itFind);
							return true;
						}
					}
				}
				return false;
			}
			void riterate(const std::function<void(const K&, const V&)>& riterate_cb) const {
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (m_map.empty()) {
					return;
				}
				for (auto rit = m_map.rbegin(); rit != m_map.rend(); ++rit) {
					riterate_cb(rit->first, rit->second);
				}
			}
			void iterate(const std::function<void(const K&, const V&, const int& cycle_index, bool& _iterate_break)>& cb) const {
				int cycle_index = 0;
				bool _iterate_break = false;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (m_map.empty()) {
					return;
				}
				for (auto it = m_map.begin(); it != m_map.end(); ++it) {
					if (!_iterate_break) {
						cb(it->first, it->second, ++cycle_index, _iterate_break);
					}
					else {
						break;
					}
				}
			}
			auto count(const K& key) {
				std::lock_guard<std::mutex> lock(*m_mutex);
				return m_map.count(key);
			}
			void iterate(const std::function<void(const K&, V&)>& cb) {
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (m_map.empty()) {
					return;
				}
				for (auto it = m_map.begin(); it != m_map.end(); ++it) {
					cb(it->first, it->second);
				}
			}
			unsigned long long size() {
				std::lock_guard<std::mutex> lock(*m_mutex);
				return m_map.size();
			}
			bool empty() {
				std::lock_guard<std::mutex> lock(*m_mutex);
				return m_map.empty();
			}
			void clear() {
				std::lock_guard<std::mutex> lock(*m_mutex);
				m_map.clear();
			}
			std::shared_ptr<std::map<K, V>> get_map() {
				return std::make_shared<decltype(m_map)>(m_map);
			}
		private:
			std::multimap<K, V, std::greater<K>> m_map;
		};

		template<typename KEY, typename VAL>
		class unordered_map final : public container_impl {
		protected:
			virtual size_t GetSize() { return size(); }
		public:
			unordered_map() {}
			virtual ~unordered_map() {}
		public:
			bool push(const KEY& key, const VAL& val)
			{
				std::lock_guard<std::mutex> _lock(*m_mutex);
				auto result = m_unordered_map.insert(std::make_pair(key, val));
				return result.second;
			}
			bool pop(const KEY& key)
			{
				bool result = false;
				std::lock_guard<std::mutex> _lock(*m_mutex);
				auto found = m_unordered_map.find(key);
				if (found != m_unordered_map.end())
				{
					result = true;
					m_unordered_map.erase(found);
				}
				return result;
			}
			bool search(const KEY& key, const std::function<void(const VAL&)>& search_cb) const
			{
				bool result = false;
				std::lock_guard<std::mutex> _lock(*m_mutex);
				auto found = m_unordered_map.find(key);
				if (found != m_unordered_map.end())
				{
					result = true;
					search_cb(found.second);
				}
				return result;
			}
			void iterate(const std::function<void(const KEY&, const VAL&, bool&)>& iterate_cb) const
			{
				std::lock_guard<std::mutex> _lock(*m_mutex);
				bool itbreak = false;
				for (const auto& node : m_unordered_map)
				{
					itbreak = false;
					iterate_cb(node.first, node.second, itbreak);
					if (itbreak)
						break;
				}
			}
		public:
			__inline void operator=(const std::unordered_map<KEY, VAL>& _Target) {
				std::lock_guard<std::mutex> _lock(*m_mutex);
				m_unordered_map.clear();
				m_unordered_map = _Target;
			}
		private:
			std::unordered_map<KEY, VAL> m_unordered_map;
		};

		template<typename KEY, typename VAL>
		class unordered_multimap final : public container_impl {
		protected:
			virtual size_t GetSize() { return size(); }
		public:
			unordered_multimap() {}
			virtual ~unordered_multimap() {}
		public:
			bool end(const std::function<void(const KEY&, const VAL&)>& end_cb) const
			{
				bool result = false;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (!m_unordered_multimap.empty())
				{
					auto itend = std::prev(m_unordered_multimap.end());
					end_cb(itend->first, itend->second);
					result = true;
				}
				return result;
			}
			bool begin(const std::function<void(const KEY&, const VAL&)>& begin_cb) const
			{
				bool result = false;
				std::lock_guard<std::mutex> lock(*m_mutex);
				if (!m_unordered_multimap.empty())
				{
					begin_cb(m_unordered_multimap.begin()->first, m_unordered_multimap.begin()->second);
					result = true;
				}
				return result;
			}
			void push(const KEY& _key, const VAL& _info) {
				std::lock_guard<std::mutex> _lock(*m_mutex);
				m_unordered_multimap.insert(std::make_pair(_key, _info));
			}
			std::shared_ptr<VAL> search(const KEY& _key) {
				std::shared_ptr<VAL> val;
				std::lock_guard<std::mutex> _lock(*m_mutex);
				if (m_unordered_multimap.empty())
					return val;
				auto aFind = m_unordered_multimap.find(_key);
				if (aFind != m_unordered_multimap.end()) {
					val = std::make_shared <VAL>(aFind->second);
				}
				return val;
			}
			void search(const KEY& _key, const std::function<void(VAL& _info)>& _callback) {
				std::lock_guard<std::mutex> _lock(*m_mutex);
				if (!m_unordered_multimap.empty()) {
					auto aFind = m_unordered_multimap.find(_key);
					if (aFind != m_unordered_multimap.end()) {
						_callback(aFind->second);
					}
				}
			}
			std::shared_ptr<VAL> pop(const KEY& _key) {
				std::shared_ptr<VAL> val;
				std::lock_guard<std::mutex> _lock(*m_mutex);
				if (m_unordered_multimap.empty())
					return val;
				auto aFind = m_unordered_multimap.find(_key);
				if (aFind != m_unordered_multimap.end()) {
					val = std::make_shared<VAL>(std::move(aFind->second));
					m_unordered_multimap.erase(aFind);
				}
				return val;
			}
			std::shared_ptr<std::tuple<KEY, VAL>> pop() {
				std::shared_ptr<std::tuple<KEY, VAL>> val;
				std::lock_guard<std::mutex> _lock(*m_mutex);
				if (m_unordered_multimap.empty())
					return val;
				auto aFind = m_unordered_multimap.begin();
				if (aFind == m_unordered_multimap.end()) {
					return val;
				}
				val = std::make_shared<std::tuple<KEY, VAL>>(std::tie(aFind->first, aFind->second));
				m_unordered_multimap.erase(aFind);
				return val;
			}
			bool pop(KEY& key, VAL& info) {
				std::lock_guard<std::mutex> _lock(*m_mutex);
				if (m_unordered_multimap.empty())
					return false;
				auto aFind = m_unordered_multimap.begin();
				key = aFind->first;
				info = aFind->second;
				m_unordered_multimap.erase(aFind);
				return true;
			}
			size_t size() {
				std::lock_guard<std::mutex> _lock(*m_mutex);
				return m_unordered_multimap.size();
			}
			bool empty() {
				std::lock_guard<std::mutex> _lock(*m_mutex);
				return m_unordered_multimap.empty();
			}
			void clear() {
				std::lock_guard<std::mutex> _lock(*m_mutex);
				m_unordered_multimap.clear();
			}
			void iterate(const std::function<void(const KEY&, VAL&)>& _callback) {
				std::lock_guard<std::mutex> _lock(*m_mutex);
				for (auto it = m_unordered_multimap.begin(); it != m_unordered_multimap.end(); ++it) {
					_callback(it->first, it->second);
				}
			}
			void iterate(const std::function<void(const KEY&, const VAL&)>& _callback) const {
				std::lock_guard<std::mutex> _lock(*m_mutex);
				for (auto it = m_unordered_multimap.begin(); it != m_unordered_multimap.end(); ++it) {
					_callback(it->first, it->second);
				}
			}
			void iterate(const std::function<void(const KEY&, VAL&)>& _callback, bool& _break) {
				std::lock_guard<std::mutex> _lock(*m_mutex);
				for (auto it = m_unordered_multimap.begin(); it != m_unordered_multimap.end(); ++it) {
					if (_break) {
						break;
					}
					_callback(it->first, it->second);
				}
			}
		public:
			__inline void operator=(const std::unordered_multimap<KEY, VAL>& _Target) {
				std::lock_guard<std::mutex> _lock(*m_mutex);
				m_unordered_multimap.clear();
				m_unordered_multimap = _Target;
			}
		private:
			std::unordered_multimap<KEY, VAL> m_unordered_multimap;
		};


		template<typename T>
		class queue_mpsc final {
		public:
			queue_mpsc() :
				_head(reinterpret_cast<buffer_node_t*>(new buffer_node_aligned_t)),
				_tail(_head.load(std::memory_order_relaxed)) {
				buffer_node_t* front = _head.load(std::memory_order_relaxed);
				front->next.store(NULL, std::memory_order_relaxed);
			}
			~queue_mpsc() {
				T output;
				while (this->pop(output)) {}
				buffer_node_t* front = _head.load(std::memory_order_relaxed);
				delete front;
			}
			void push(const T& input) {
				buffer_node_t* node = reinterpret_cast<buffer_node_t*>(new buffer_node_aligned_t);
				node->data = input;
				node->next.store(NULL, std::memory_order_relaxed);
				buffer_node_t* prev_head = _head.exchange(node, std::memory_order_acq_rel);
				prev_head->next.store(node, std::memory_order_release);
			}
			bool pop(T& output) {
				buffer_node_t* tail = _tail.load(std::memory_order_relaxed);
				buffer_node_t* next = tail->next.load(std::memory_order_acquire);
				if (next == NULL) {
					return false;
				}
				output = next->data;
				_tail.store(next, std::memory_order_release);
				delete tail;
				return true;
			}
		private:
			struct buffer_node_t {
				T data;
				std::atomic<buffer_node_t*> next;
			};
			typedef typename std::aligned_storage<sizeof(buffer_node_t), std::alignment_of<buffer_node_t>::value>::type buffer_node_aligned_t;
			std::atomic<buffer_node_t*> _head;
			std::atomic<buffer_node_t*> _tail;
			queue_mpsc(const queue_mpsc&) {}
			void operator=(const queue_mpsc&) {}
		};

		template<typename T>
		class queue_spsc final {
		public:
			queue_spsc() :
				_head(reinterpret_cast<node_t*>(new node_aligned_t)),
				_tail(_head) {
				_head->next = NULL;
			}
			~queue_spsc() {
				T output;
				while (this->pop(output)) {}
				delete _head;
			}
			void push(const T& input) {
				node_t* node = reinterpret_cast<node_t*>(new node_aligned_t);
				node->data = input;
				node->next = NULL;

				std::atomic_thread_fence(std::memory_order_acq_rel);
				_head->next = node;
				_head = node;
			}
			bool pop(T& output) {
				std::atomic_thread_fence(std::memory_order_consume);
				if (!_tail->next) {
					return false;
				}
				output = _tail->next->data;
				std::atomic_thread_fence(std::memory_order_acq_rel);
				_back = _tail;
				_tail = _back->next;
				delete _back;
				return true;
			}
		private:
			struct node_t {
				node_t* next;
				T       data;
			};
			typedef typename std::aligned_storage<sizeof(node_t), std::alignment_of<node_t>::value>::type node_aligned_t;
			node_t* _head;
			char    _cache_line[64];
			node_t* _tail;
			node_t* _back;
			queue_spsc(const queue_spsc&) {}
			void operator=(const queue_spsc&) {}
		};

				class Buffer final {
		public:
			Buffer(size_t _BufferSize = 0x200) :
				m_pData(nullptr),
				m_nDataLength(_BufferSize),
				m_nDataLengthInit(_BufferSize),
				m_nDataLengthCurrent(0)
			{
				m_pData = new char[_BufferSize];
				memset(m_pData, 0x00, _BufferSize);
			}
			~Buffer() {
				if (m_pData) {
					delete m_pData;
					m_pData = nullptr;
				}
			}

			//! @reset 复位缓冲区
			void reset() {
				lock();
				m_nDataLengthCurrent = 0;
				memset(m_pData, 0x00, m_nDataLength);
				unlock();
			}
			void resize(const size_t& _BufferSize) {
				lock();
				if (m_pData) {
					delete m_pData;
					m_pData = nullptr;
				}
				m_nDataLength = _BufferSize;
				m_nDataLengthCurrent = 0;
				m_pData = new char[_BufferSize];
				memset(m_pData, 0x00, _BufferSize);
				unlock();
			}
			//! @push 将数据插入到缓冲区尾部，如果缓冲区剩余长度不够则拒绝插入
			//! @***** 如果缓冲区已满，那么在未有消费者的情况下该方法会阻塞
			bool push(const void* _Data, size_t _DataLen) {
				lock();
				if (_DataLen > (m_nDataLength - m_nDataLengthCurrent))
				{
					auto newpData = new char[m_nDataLengthCurrent + _DataLen];
					if (!newpData)
					{
						unlock();
						return false;
					}
					m_nDataLength = m_nDataLengthCurrent + _DataLen;
					if (m_nDataLengthCurrent > 0 && m_pData)
					{
						memcpy(newpData, m_pData, m_nDataLengthCurrent);
						delete m_pData;
						m_pData = nullptr;
					}
					memcpy(newpData + m_nDataLengthCurrent, _Data, _DataLen);
					m_nDataLengthCurrent += _DataLen;
					m_pData = newpData;
				}
				else
				{
					memcpy(m_pData + m_nDataLengthCurrent, _Data, _DataLen);
					m_nDataLengthCurrent += _DataLen;
				}
				unlock();
				return true;
			}
			//! @pop 如果出栈字节数不够，则拒绝出栈
			std::shared_ptr<char> pop(size_t _Length) {
				std::shared_ptr<char> result;
				lock();
				if (0 == m_nDataLengthCurrent) {
					unlock();
					return result;
				}
				if (_Length > m_nDataLengthCurrent) {
					unlock();
					return result;
				}
				if (_Length <= m_nDataLengthCurrent) {
					std::shared_ptr<char> spBuffer(new char[_Length], [](char* pbuffer) { delete[] pbuffer; });
					result = spBuffer;
					memcpy(result.get(), m_pData, _Length);
					memmove(m_pData, m_pData + _Length, m_nDataLength - _Length);
					m_nDataLengthCurrent -= _Length;
					memset(m_pData + m_nDataLengthCurrent, 0x00, m_nDataLength - m_nDataLengthCurrent);
				}
				else {
					_Length = m_nDataLengthCurrent;
					std::shared_ptr<char> spBuffer(new char[_Length], [](char* pbuffer) { delete[] pbuffer; });
					result = spBuffer;
					memcpy(result.get(), m_pData, _Length);
					m_nDataLengthCurrent = 0;
					memset(m_pData, 0x00, m_nDataLength);
				}
				unlock();
				return result;
			}

			bool pop(size_t _Length, sk::packet& out) {
				bool result = false;
				out.clear();

				lock();
				if (0 == m_nDataLengthCurrent) {
					unlock();
					return result;
				}
				if (_Length > m_nDataLengthCurrent) {
					unlock();
					return result;
				}
				if (_Length <= m_nDataLengthCurrent) {
					result = true;
					out.append(m_pData, _Length);

					memmove(m_pData, m_pData + _Length, m_nDataLength - _Length);
					m_nDataLengthCurrent -= _Length;
					memset(m_pData + m_nDataLengthCurrent, 0x00, m_nDataLength - m_nDataLengthCurrent);
				}
				else {
					_Length = m_nDataLengthCurrent;
					result = true;
					out.append(m_pData, _Length);
					m_nDataLengthCurrent = 0;
					memset(m_pData, 0x00, m_nDataLength);
				}

				if ((m_nDataLength != m_nDataLengthInit) && (m_nDataLengthCurrent <= 0) && (m_pData != nullptr))
				{
					delete m_pData;
					m_pData = new char[m_nDataLengthInit];
					m_nDataLength = m_nDataLengthInit;
					m_nDataLengthCurrent = 0;
				}

				unlock();
				return result;
			}
			//! @front 反回首字节地址，一般用于封包预解析
			const char* front() {
				lock();
				if (m_nDataLengthCurrent) {
					unlock();
					return m_pData;
				}
				unlock();
				return nullptr;
			}
			//! @pop 缓冲区全部数据出栈
			//! @return 需要释放缓存
			const char* pop() {
				char* result = nullptr;
				lock();
				if (0 == m_nDataLengthCurrent) {
					unlock();
					return result;
				}
				result = new char[m_nDataLengthCurrent];
				memcpy(result, m_pData, m_nDataLengthCurrent);
				m_nDataLengthCurrent = 0;
				memset(m_pData, 0x00, m_nDataLength);
				unlock();
				return result;
			}
			size_t GetDataTotalSize() {
				size_t result = 0;
				lock();
				result = m_nDataLength;
				unlock();
				return result;
			}
			size_t GetDataCurrentSize() {
				size_t result = 0;
				lock();
				result = m_nDataLengthCurrent;
				unlock();
				return result;
			}
		public:
			bool batch(const UINT64& serial, const UINT& index, const sk::packet& in, const std::function<bool(container::map<UINT, sk::packet>&, bool&/* itclear*/)>& checkcb)
			{
				bool result = false;
				if (in.empty() || !checkcb) return result;
				lock();
				bool itclear = false;
				m_BatchQ.pushpush(
					serial,
					[&](auto& q)
					{
						q.push(index, in);
						result = checkcb(q, itclear);
					});
				if (itclear) { m_BatchQ.pop(serial, [&](const auto&, auto&) {}); }
				unlock();
				return result;
			}
		private:
			char* m_pData;
			const size_t m_nDataLengthInit;
			size_t m_nDataLength;
			size_t m_nDataLengthCurrent;
			std::atomic_flag m_lock = ATOMIC_FLAG_INIT;
			void lock() { while (m_lock.test_and_set(std::memory_order_acquire)); }
			void unlock() { m_lock.clear(std::memory_order_release); }
			container::map<UINT64, container::map<UINT, sk::packet>> m_BatchQ;
		};

	}///namespace container
}///namespace sk


/// /*新生联创（上海）**/
/// /*2019年05月18日 7:16:36|843**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_container_H__8213A_D8902B1C_DDB8875B51CB106_7AB5__TAIL__