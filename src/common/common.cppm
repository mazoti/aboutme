module;

#include <iostream>
#include <sstream>
#include <string>

#include <span>

#include <map>
#include <set>
#include <vector>

export module common;

#if defined(ENABLE_ENVIRONMENT_VARIABLES)
	export void environment_variables(std::span<char*>& env_vars, char path_separator) noexcept;
#endif

#if defined(ENABLE_SYSTEM_CLOCK)
	export std::wostream& system_clock() noexcept;
#endif

export template<typename T1, typename T2, typename T3>
struct triple{
	T1 first;
	T2 second;
	T3 third;

	triple(const T1& a, const T2& b, const T3& c) : first(a), second(b), third(c) {}
};

export template<typename DATA>
std::wostream& operator<<(std::wostream& wos, const std::set<DATA>& dataset){
	for(typename std::set<DATA>::const_iterator it = dataset.begin(); it != dataset.end(); ++it)
		std::wcout << L'\t' << std::wstring(it->begin(), it->end()) << std::endl;
	return wos << std::endl;
}

export template<typename DATA>
std::wostream& operator<<(std::wostream& wos, const std::vector<DATA>& dataset){
	for(typename std::vector<DATA>::const_iterator it = dataset.begin(); it != dataset.end(); ++it)
		std::wcout << L'\t' << std::wstring(it->begin(), it->end()) << std::endl;
	return wos << std::endl;
}

export template<typename KEY, typename VALUE>
std::wostream& operator<<(std::wostream& wos, const std::multimap<KEY, VALUE>& string_multimap){
	KEY key;
	for(typename std::multimap<KEY, VALUE>::const_iterator it = string_multimap.begin();
		it != string_multimap.end(); ++it){
		if(key != it->first){
			key = it->first;
			std::wcout << L'\t' << std::wstring(it->first.begin(), it->first.end()) << L':' << std::endl;

			std::pair<typename std::multimap<KEY, VALUE>::const_iterator,
				typename std::multimap<KEY, VALUE>::const_iterator> range = string_multimap.equal_range(key);

			for(typename std::multimap<KEY, VALUE>::const_iterator iterator = range.first;
				iterator != range.second; ++iterator){
				std::wcout << L"\t\t" << std::wstring(iterator->second.begin(), iterator->second.end()) << std::endl;
			}
			std::wcout << std::endl;
		}
	}
	return wos;
}

export template<typename... Arguments>
std::wostream& print_largest(size_t tabs, size_t padding, size_t largest, wchar_t fill, Arguments... args){
	size_t i, largest_padding;
	std::wostringstream woss;
	std::vector<typename std::common_type<Arguments...>::type> arguments{args...};

	largest_padding = largest + padding;

	// Prints data: value
	for(i=0; i < arguments.size(); i +=2){
		std::wcout << std::wstring(tabs, L'\t') << arguments[i];

		woss.str(L"");
		woss << arguments[i] << arguments[i + 1];
		std::wcout << std::wstring(largest_padding - woss.str().size(), fill) << arguments[i + 1] << std::endl;
	}

	return std::wcout << std::endl;
}

export template<typename... Arguments>
std::wostream& print(size_t tabs, size_t padding, wchar_t fill, Arguments... args){
	size_t i, largest = 0;
	std::wostringstream woss;
	std::vector<typename std::common_type<Arguments...>::type> arguments{args...};

	// Finds the size of largest line
	for(i=0; i < arguments.size(); i +=2){
		woss.str(L"");
		woss << arguments[i] << arguments[i + 1];
		if(woss.str().size() > largest) largest = woss.str().size();
	}

	return print_largest(tabs, padding, largest, fill, args...);
}

export template<typename TITLE, typename... Arguments>
std::wostream& print_title_largest(size_t tabs, size_t padding, size_t largest, wchar_t fill, const TITLE& title, Arguments... args){
	std::wcout << std::wstring(tabs, L'\t') << title << std::endl;
	return print_largest(tabs + 1, padding, largest, fill, args...);
}

export template<typename TITLE, typename... Arguments>
std::wostream& print_title(size_t tabs, size_t padding, wchar_t fill, const TITLE& title, Arguments... args){
	std::wcout << std::wstring(tabs, L'\t') << title << std::endl;
	return print(tabs + 1, padding, fill, args...);
}

export template<typename KEY, typename VALUE>
void insert_if_unique(std::multimap<KEY, VALUE>& mm, const KEY& key, const VALUE& value){
	typename std::multimap<KEY, VALUE>::iterator it = mm.lower_bound(key);
	while(it != mm.end() && it->first == key){
		if(it->second == value) return; // Value exists
		++it;
	}
	mm.insert({key, value});
}

export template <typename T> struct releaser{ void operator()(T* ptr) const{if(ptr){ptr->Release();}}};
