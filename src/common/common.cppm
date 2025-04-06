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
	export void environment_variables(const char* envp[], char separator) noexcept;
#endif

#if defined(ENABLE_SYSTEM_CLOCK)
	export std::wostream& system_clock() noexcept;
#endif

#if defined(ENABLE_SYSTEM_CLOCK) || defined(ENABLE_HARD_DISKS) || defined(ENABLE_MEMORY)
	// Prints key-value pairs with precomputed maximum width for alignment
	export template<typename... Arguments>
	std::wostream& print_largest(size_t tabs, size_t padding, size_t largest, wchar_t fill, Arguments... args){
		size_t i, largest_padding;
		std::wostringstream woss;
		std::vector<typename std::common_type<Arguments...>::type> arguments{args...};

		largest_padding = largest + padding;

		// Iterates over arguments in pairs (assumes even number of args)
		for(i=0; i < arguments.size(); i +=2){
			std::wcout << std::wstring(tabs, L'\t') << arguments[i];

			woss.str(L"");
			woss << arguments[i] << arguments[i + 1];
			std::wcout << std::wstring(largest_padding - woss.str().size(), fill) << arguments[i + 1] << L'\n';
		}

		return std::wcout << L'\n';
	}

	// Prints a title followed by key-value pairs with precomputed maximum width.
	// Increases indentation for the pairs relative to the title
	export template<typename TITLE, typename... Arguments>
	std::wostream& print_title_largest(size_t tabs, size_t padding, size_t largest, wchar_t fill, const TITLE& title, Arguments... args){
		std::wcout << std::wstring(tabs, L'\t') << title << L'\n';
		return print_largest(tabs + 1, padding, largest, fill, args...);
	}

	// Prints key-value pairs, dynamically calculating the largest width for alignment
	export template<typename... Arguments>
	std::wostream& print(size_t tabs, size_t padding, wchar_t fill, Arguments... args){
		static_assert(sizeof...(args) % 2 == 0, "Number of arguments must be even");

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

	// Prints a title followed by key-value pairs, dynamically calculating the width.
	// Combines title output with dynamic width calculation from print
	export template<typename TITLE, typename... Arguments>
	std::wostream& print_title(size_t tabs, size_t padding, wchar_t fill, const TITLE& title, Arguments... args){
		std::wcout << std::wstring(tabs, L'\t') << title << L'\n';
		return print(tabs + 1, padding, fill, args...);
	}
#endif

#if defined(ENABLE_BLUETOOTH) || defined(ENABLE_NETWORK) || defined(ENABLE_USB) || defined(ENABLE_WIFI) \
	|| defined(ENABLE_STARTUP) || defined(ENABLE_SHARED)
	// Overloads the output operator for std::multimap to print key-value pairs hierarchically
	export template<typename KEY, typename VALUE>
	std::wostream& operator<<(std::wostream& wos, const std::multimap<KEY, VALUE>& string_multimap){
		KEY key;

		// Iterates over all key-value pairs in the multimap
		for(typename std::multimap<KEY, VALUE>::const_iterator it = string_multimap.begin();
			it != string_multimap.end(); ++it){

			// Checks if the key has changed (first iteration or new key)
			if(key != it->first){
				key = it->first;
				wos << L'\t' << it->first << L':' << L'\n';

				std::pair<typename std::multimap<KEY, VALUE>::const_iterator,
					typename std::multimap<KEY, VALUE>::const_iterator> range = string_multimap.equal_range(key);

				// Iterates over all values associated with the current key
				for(typename std::multimap<KEY, VALUE>::const_iterator iterator = range.first;
					iterator != range.second; ++iterator){
					wos << L"\t\t" << iterator->second << L'\n';
				}
				wos << L'\n';
			}
		}
		return wos;
	}
#endif

#if defined(ENABLE_BATTERY) || defined(ENABLE_OPERATING_SYSTEM)
	// Static inline helper function to print a number with singular/plural descriptions
	// Avoids code duplication for uptime components (days, hours, minutes, seconds)
	export template<typename NUMBER>
	void print_plural(NUMBER time_number, const wchar_t* desc, const wchar_t* desc_plural, bool& is_first){
		if(time_number){
			std::wcout << (is_first ? L" " : L", ") << time_number << L" " << (time_number == 1? desc : desc_plural);
			is_first = false;
		}
	}
#endif

// Inserts a key-value pair into a multimap only if the value is unique for that key.
// Prevents duplicate values under the same key
#if defined(ENABLE_NETWORK)
	export template<typename KEY, typename VALUE>
	void insert_if_unique(std::multimap<KEY, VALUE>& mm, const KEY& key, const VALUE& value){
		typename std::multimap<KEY, VALUE>::iterator it = mm.lower_bound(key);
		// Iterates over all entries with the matching key
		for(;(it != mm.end() && it->first == key); ++it) if(it->second == value) return;
		mm.insert({key, value});
	}
#endif
