
#ifndef __GX_CORE_H__
#define __GX_CORE_H__

namespace Geoxide {

	template <typename T>
	using Local = std::unique_ptr<T>;

	template <typename T>
	using Shared = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Local<T> NewLocal(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename ... Args>
	constexpr Shared<T> NewShared(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	using hash_t = size_t;

	template<typename T>
	inline hash_t Hash(const T& val)
	{
		return Hash((const uint8_t*)&val, sizeof(T));
	}

	inline hash_t Hash(const uint8_t* data, uint32_t length)
	{
		hash_t hash = 14695981039346656037;

		for (size_t i = 0; i < length; i++)
		{
			hash ^= data[i];
			hash *= 1099511628211;
		}

		return hash;
	}

	class Exception : public std::exception
	{
	public:
		Exception(const std::string& msg) : std::exception(msg.c_str()) {}
	};

	template<typename T>
	class View
	{
	public:
		View(T* begin, T* end) : mBegin(begin), mEnd(end) {}

		T* begin() const { return mBegin; }
		T* end() const { return mEnd; }

		const T* cbegin() const { return mBegin; }
		const T* cend() const { return mEnd; }

		size_t size() const { return mEnd - mBegin; }

		T& operator[](size_t i) { return mBegin[i]; }
		const T& operator[](size_t i) const { return mBegin[i]; }

	private:
		T* mBegin;
		T* mEnd;
	};

	class Internal
	{
	public:
		virtual void release() = 0;

		uint32_t getRefCount()
		{ 
			return mRefCount; 
		}

		void addRef()
		{
			mRefCount++;
		}

	protected:
		Internal() : mRefCount(0) {}

		uint32_t mRefCount;
	};

	template <typename T>
	class Ref
	{
	public:
		Ref() : mObj(0) {}
		Ref(Ref&& other) : mObj(other.mObj) {}

		Ref(T* obj) : mObj(obj)
		{ 
			mObj->addRef();
		}

		Ref(const Ref& other) : mObj(other.mObj)
		{
			mObj->addRef();
		}

		T* get() { return mObj; }

		operator T* () { return get(); }
		T* operator->() { return get(); }

		Ref& operator=(T* obj)
		{
			reset();
			mObj = obj;
			mObj->addRef();
			return *this;
		}

		Ref& operator=(const Ref& other)
		{
			reset();
			mObj = other.mObj;
			mObj->addRef();
			return *this;
		}

		Ref& operator=(Ref&& other)
		{
			mObj = other.mObj;
			return *this;
		}

		void reset()
		{
			if (mObj)
			{
				mObj->release();
				mObj = 0;
			}
		}

		template <typename T2>
		T2* getAs()
		{
			return (T2*)mObj;
		}

	protected:
		T* mObj;
	};

}

#define GX_THROW(str) throw Exception(str)
#define GX_FUNC_THROW(str)\
	GX_THROW(std::filesystem::path(__FILE__).filename().string() + "(" + std::to_string(__LINE__) + "): " str)

#define GX_SAFE_DELETE(p) if(p) {delete p; p = 0;}

#define GX_SUCCESS 0
#define GX_FAILURE -1

#define GX_IMPLEMENT_INTERNAL() void release() override {assert(mRefCount != 0); if (--mRefCount == 0) delete this;}

#endif // !__GX_CORE_H__

