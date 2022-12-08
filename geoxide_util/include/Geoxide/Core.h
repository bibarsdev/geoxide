
#ifndef __GX_CORE_H__
#define __GX_CORE_H__

namespace Geoxide {

	template <typename T>
	using Local = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Local<T> NewLocal(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	class Exception : public std::exception
	{
	public:
		Exception(const std::string& msg) : std::exception(msg.c_str()) {}
	};

	template<typename T>
	inline size_t Hash(const T& val)
	{
		return Hash((const uint8_t*)&val, sizeof(T));
	}

	inline size_t Hash(const uint8_t* data, uint32_t length) 
	{
		size_t hash = 14695981039346656037;

		for (size_t i = 0; i < length; i++)
		{
			hash ^= data[i];
			hash *= 1099511628211;
		}

		return hash;
	}
}

#define GX_THROW(str) throw Exception(str)
#define GX_FUNC_THROW(str)\
	GX_THROW(std::filesystem::path(__FILE__).filename().string() + "(" + std::to_string(__LINE__) + "): " str)

#endif // !__GX_CORE_H__

