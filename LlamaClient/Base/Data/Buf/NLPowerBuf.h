#ifndef __NL_POWER_BUF_H__
#define __NL_POWER_BUF_H__
/**
 * 	createed by @cjxx 2019-08-20
 * 	与 NLBuf 一样拥有高效的缓冲操作能力.
 * 	但是比 NLBuf 多一个头部指针, 以便可以进行头部缓存的裁剪.能力更加强大
 * 	当缓存需要做头部裁剪时推荐使用 NLPowerBuf
 * 	
 * 	高级功能包括:
 * 		1. 高效率  头部裁剪
 * 		2. 高效率  尾部裁剪
 * 		3. 高效率  中间插入
 * 		4. 前后索引截取片段
 */
#include <assert.h>
#include <string>

//	use ralloc or use malloc
#define NL_POWER_BUFFER_UES_REALLOC 1

class NLPowerBuf {
public:
	//
	NLPowerBuf();
	//
	NLPowerBuf(const NLPowerBuf& othBuffer);
	//
    NLPowerBuf(NLPowerBuf&& rhs);
	//
	NLPowerBuf(const std::string& s);
	NLPowerBuf(const void* data, size_t size);
	// 任意简单类型构造
	template<class T, typename std::enable_if<std::is_trivially_copyable<T>::value, bool>::type = true>
	NLPowerBuf(const T& t) {
		this->Set(t);
	}

	// 判断相等
	bool operator==(const NLPowerBuf& oth) const {
		return this->size() == oth.size() && 0 == ::memcmp(this->data(), oth.data(), this->size());
	}	
	bool operator==(const char* p) const {
		return nullptr != p && this->size() == ::strlen(p) && 0 == ::memcmp(this->data(), p, this->size());
	}
	bool operator==(const std::string& s) const {
		return this->size() == s.size() && 0 == ::memcmp(this->data(), s.data(), this->size());
	}
	//
	bool operator!=(const NLPowerBuf& oth) const {
		return this->size() != oth.size() || 0 != ::memcmp(this->data(), oth.data(), this->size());
	}	
	bool operator!=(const char* p) const {
		return nullptr == p || this->size() != ::strlen(p) || 0 != ::memcmp(this->data(), p, this->size());
	}
	bool operator!=(const std::string& s) const {
		return this->size() != s.size() || 0 != ::memcmp(this->data(), s.data(), this->size());
	}	

	//
	void operator=(const std::string& s);
	// 
	void operator=(const NLPowerBuf& othBuffer);
	//
    void operator=(NLPowerBuf&& rhs);
	//
	void operator=(const char* p);	
	// 任意简单类型赋值
	template<class T, typename std::enable_if<std::is_trivially_copyable<T>::value, bool>::type = true>
	void operator=(const T& t) {
		this->Set<T>(t);
	}

	//
	char operator[](int index);

	// 类型转换
	operator std::string() const;

	// 任意简单类型转换
	template<class T, typename std::enable_if<std::is_trivially_copyable<T>::value || 
											  std::is_same<std::decay_t<T>, std::string>::value ||
											  std::is_same<std::decay_t<T>, std::string_view>::value ||
											  std::is_same<std::decay_t<T>, NLPowerBuf>::value, bool>::type = true>
	T conv() const {
        if constexpr(std::is_same<std::decay_t<T>, std::string>::value) {
            return str(this->c_str(), this->size());
        }
        if constexpr(std::is_same<std::decay_t<T>, std::string_view>::value) {
            return {this->c_str(), this->size()};
        }
        if constexpr(std::is_same<std::decay_t<T>, NLPowerBuf>::value) {
            return *this;
        } else {
			return *(std::remove_reference_t<T>*)this->data();
		}
	}
	template<class T, typename std::enable_if<std::is_trivially_copyable<T>::value, bool>::type = true>
	const T* convPoint() const {
		return (const std::remove_reference_t<T>*)this->data();
	}
	template<class T, typename std::enable_if<std::is_trivially_copyable<T>::value, bool>::type = true>
	const T& convRef() const {
		return *this->convPoint<T>();
	}	
	
	//	析构
	~NLPowerBuf();

public:
	//
	const char* Get() const;
	
	// data size
	int Size() const;

	//	clean data but not release memory
	void Clear();

public:
	// like std::array
	const char* data() const {
		return this->Get();
	}
	
	// like std::string
	const char* c_str() const noexcept {
		return this->Get();
	}	

	// like std::array
	size_t size() const {
		return this->Size();
	}

	// unsafe interface
	char* unsafe() const {
		return this->GetUnSafeBegin();
	}

	// begin pointer
	const char* begin() const {
		return this->GetUnSafeBegin();
	}

	// end pointer
	const char* end() const {
		return this->GetUnSafeEnd();
	}

	// like std::array
	void clear() {
		this->Clear();
	}

	// like std::array
	bool empty() const noexcept {
		return 0 == this->size();
	}

public:
	//	fast option

	//
	NLPowerBuf& Set(NLPowerBuf&& oth);
	NLPowerBuf& Set(const NLPowerBuf& oth) {
		this->CopyFromOthBuffer(oth);
		return *this;
	}
	
	NLPowerBuf& SetMust(const void* pszData, size_t iDataLength);	

	NLPowerBuf& Set(const void* pszData, size_t iDataLength);
	NLPowerBuf& Set(const std::string& s);
	//
    template<class T, typename std::enable_if<std::is_trivially_copyable<T>::value, bool>::type = true>
    NLPowerBuf& Set(const T& t) {
        return this->SetMust(&t, sizeof(T));
    }

	NLPowerBuf& AddMust(const void* pszData, size_t iDataLength);
	//
	NLPowerBuf& Add(const void* pszData, size_t iDataLength);
	
	NLPowerBuf& Add(const std::string& s) {
		return this->Add(s.data(), s.size());
	}
	//
    template<class T, typename std::enable_if<std::is_trivially_copyable<T>::value, bool>::type = true>
    NLPowerBuf& Add(const T& t) {
        return this->AddMust(&t, sizeof(T));
    }
	//
    template<class T, typename std::enable_if<std::is_trivially_copyable<T>::value, bool>::type = true>
    constexpr NLPowerBuf& AddMust(const T& t) {
        return this->AddMust(&t, sizeof(T));
    }

	//	O(1)
	NLPowerBuf& CutFront(int iCutNum);
	//	O(1)
	NLPowerBuf& CutBack(int iCutNum);
	
public:
	//	slow option

	/**
	 * O(1) - O(n)
	 * iBegin	: start cut pos - it can < 0 and it means the last of the buf 
	 * iLength 	: cut length    - must be > 0 and it can longer than the size of buf, it will be ok
	*/
	NLPowerBuf& Cut(int iCutBegin, int iLength = 987654321);

	/**
	 * O(n)
	 * pData		: insert buf begin address
	 * iDataLength	: insert buf length
	 * iInsertBegin : start insert pos - it can < 0 and it means the last of the buf 
	*/
	NLPowerBuf& Insert(const void* pData, size_t iDataLength, int iInsertBegin = 987654321);

	/**
	 * iStart	: copy from pos - it can < 0, = 0 and > 0 
	 * iEnd		: copy to pos 	- it can < 0, = 0 and > 0 
	 * if iStart >= iEnd it will return empty buf
	*/
	NLPowerBuf Sub(int iStart = 0, int iEnd = 987654321);

public:
	//
	void Prepare(int iBufferSize);

	//	*	prepare buffer but not clear the buffer and Only input size
	void PrepareOnly(size_t iBufferSize);

	//
	void PrepareCanUse(int iAddSize);
	//
	void ReCreate(int iSize);

	//
	void Destroy();

public:
	//	unsafe interface

	//
	char* GetUnSafeBegin() const {
		return m_pBufferBegin + m_iDataBegin;
	}
	char* GetUnSafeEnd() const {
		return m_pBufferBegin + m_iDataBegin + m_iDataSize;
	}
	//
	void SetDataSize(int iSize) {
		assert(iSize >= 0 && iSize <= m_iBufferSize);
		m_iDataSize = iSize;
	}
	//
	void AddDataSize(int iAddSize) {
		assert(iAddSize > 0);
		m_iDataSize += iAddSize;
		assert(m_iDataSize <= m_iBufferSize);
	}
	//
	void DelDataSize(int iDelSize) {
		assert(iDelSize > 0);
		m_iDataSize -= iDelSize;
		assert(m_iDataSize >= 0);
	}
	// buffer total size
	int GetCapacity() const {
		return this->m_iBufferSize;
	}	

private:
	//	100m
	enum { MAX_CACHE_SIZE = 1024 * 1024 * 100};

private:
	//
	void CopyFromOthBuffer(const NLPowerBuf& othBuffer);
	//
	void DestroyOnlyBuffer();
	//
	void ExpandBuffer(int iNewSize, size_t dataLength = 2000000000);
	//
	void ExpandBufferOnly(size_t iNewSize);	

private:
	char*	m_pBufferBegin{0};
	int		m_iBufferSize{0};
	int		m_iDataBegin{0};
	int		m_iDataSize{0};
};

//
inline 
NLPowerBuf::NLPowerBuf() {
	this->AddMust('\0');
	this->Clear();
}

//
inline
NLPowerBuf::NLPowerBuf(const NLPowerBuf& othBuffer) {
	this->CopyFromOthBuffer(othBuffer);
}

//
inline
NLPowerBuf::NLPowerBuf(NLPowerBuf&& rhs) :
m_pBufferBegin(rhs.m_pBufferBegin),
m_iBufferSize(rhs.m_iBufferSize),
m_iDataBegin(rhs.m_iDataBegin),
m_iDataSize(rhs.m_iDataSize) {

    rhs.m_iDataSize 	= 0;
	rhs.m_iDataBegin	= 0;
    rhs.m_iBufferSize 	= 0;
    rhs.m_pBufferBegin 	= 0;
}

//
inline
NLPowerBuf::NLPowerBuf(const std::string& s) {
	this->Set(s);
}

//
inline
NLPowerBuf::NLPowerBuf(const void* data, size_t size) {
	this->Set(data, size);
}

//
inline
NLPowerBuf::~NLPowerBuf() {
	this->Destroy();
}

//
inline
void NLPowerBuf::operator=(const std::string& s) {
	this->Set(s);
}

//
inline
void NLPowerBuf::operator=(const NLPowerBuf& othBuffer) {
	if (&othBuffer != this) {
		this->CopyFromOthBuffer(othBuffer);
	}
}

//
inline 
void NLPowerBuf::operator=(NLPowerBuf&& rhs) {
    this->Destroy();

	m_iDataSize 	= rhs.m_iDataSize;
	m_iDataBegin	= rhs.m_iDataBegin;
	m_iBufferSize	= rhs.m_iBufferSize;
	m_pBufferBegin	= rhs.m_pBufferBegin;

    rhs.m_iDataSize 	= 0;
	rhs.m_iDataBegin	= 0;
    rhs.m_iBufferSize 	= 0;
    rhs.m_pBufferBegin 	= 0;
}

inline
void NLPowerBuf::operator=(const char* p) {
	this->Set(p, ::strlen(p));
	this->AddMust('\0');
	this->CutBack(1);
}

//
inline
char NLPowerBuf::operator[](int index) {
	//	some fix params operation
	assert(index > -m_iDataSize && index < m_iDataSize);
	if (index < 0) {
		index += m_iDataSize;
	}

	return (m_pBufferBegin + m_iDataBegin)[index];
}

// 类型转换
inline
NLPowerBuf::operator std::string() const {
	return str(this->c_str(), this->size());
}

//
inline
const char* NLPowerBuf::Get() const {
	return m_pBufferBegin + m_iDataBegin;
}

//
inline
int NLPowerBuf::Size() const {
	return m_iDataSize;
}

//
inline
void NLPowerBuf::Clear() {
	if (m_iBufferSize > MAX_CACHE_SIZE) {
		this->DestroyOnlyBuffer();
	}
	m_iDataBegin= 0;
	m_iDataSize = 0;
}

//
inline
NLPowerBuf& NLPowerBuf::Set(NLPowerBuf&& oth) {
	this->Destroy();

	m_iDataSize 	= oth.m_iDataSize;
	m_iBufferSize	= oth.m_iBufferSize;
	m_pBufferBegin	= oth.m_pBufferBegin;
	m_iDataBegin	= oth.m_iDataBegin;

    oth.m_iDataSize 	= 0;
    oth.m_iBufferSize 	= 0;
    oth.m_pBufferBegin 	= 0;
	oth.m_iDataBegin 	= 0;
	return *this;
}
//
inline
NLPowerBuf& NLPowerBuf::Set(const std::string& s) {
	this->Set(s.data(), s.size());
	this->AddMust('\0');
	this->CutBack(1);
	
	return *this;
}

//
inline
NLPowerBuf& NLPowerBuf::Set(const void* pszData, size_t iDataLength) {
	if (0 == iDataLength || nullptr == pszData) {
		this->Clear();
		return *this;
	}
	this->ExpandBuffer((int)iDataLength, iDataLength);
	::memcpy(m_pBufferBegin, pszData, iDataLength);
	m_iDataBegin= 0;
	m_iDataSize = (int)iDataLength;
	return *this;
}

//
inline
NLPowerBuf& NLPowerBuf::SetMust(const void* pszData, size_t iDataLength) {
	assert(iDataLength != 0 && pszData != nullptr);

	this->ExpandBuffer((int)iDataLength, iDataLength);
	::memcpy(m_pBufferBegin, pszData, iDataLength);
	m_iDataBegin = 0;
	m_iDataSize = (int)iDataLength;
	return *this;
}

//
inline
NLPowerBuf& NLPowerBuf::Add(const void* pszData, size_t iDataLength) {
	if (0 == iDataLength || nullptr == pszData) {
		return *this;
	}

	this->ExpandBuffer(m_iDataSize + (int)iDataLength, iDataLength);
	::memcpy(m_pBufferBegin + m_iDataBegin + m_iDataSize, pszData, iDataLength);
	m_iDataSize += (int)iDataLength;

	return *this;
}

//
inline
NLPowerBuf& NLPowerBuf::AddMust(const void* pszData, size_t iDataLength) {
	assert(0 != iDataLength && nullptr != pszData);

	this->ExpandBuffer(m_iDataSize + (int)iDataLength, iDataLength);
	::memcpy(m_pBufferBegin + m_iDataBegin + m_iDataSize, pszData, iDataLength);
	m_iDataSize += (int)iDataLength;

	return *this;
}

//	O(1)
inline
NLPowerBuf& NLPowerBuf::CutFront(int iCutNum) {
	if (iCutNum <= 0) {
		return *this;
	}
	if (iCutNum >= m_iDataSize) {
		this->Clear();
		return *this;
	}
	m_iDataBegin += iCutNum;
	m_iDataSize  -= iCutNum;
	return *this;
}

//	O(1)
inline
NLPowerBuf& NLPowerBuf::CutBack(int iCutNum) {
	if (iCutNum <= 0) {
		return *this;
	}
	if (iCutNum >= m_iDataSize) {
		this->Clear();
		return *this;
	}
	m_iDataSize -= iCutNum;
	return *this;
}

//	O(n)
inline
NLPowerBuf& NLPowerBuf::Insert(const void* pData, size_t iDataLength, int iInsertBegin) {
	assert(0 != pData && iDataLength > 0);

	this->ExpandBuffer(m_iDataSize + (int)iDataLength, (int)iDataLength);

	if (iInsertBegin > m_iDataSize) {
		iInsertBegin = m_iDataSize;
	} else if (iInsertBegin < -m_iDataSize) {
		iInsertBegin = 0;
	} else if (iInsertBegin < 0) {
		iInsertBegin += m_iDataSize;
	}

	if (0 == m_iDataBegin) {
		::memmove(m_pBufferBegin + iInsertBegin + iDataLength, m_pBufferBegin + iInsertBegin, m_iDataSize - iInsertBegin);
	} else {
		int iCanLastMove = m_iBufferSize - m_iDataBegin - m_iDataSize;
		assert(iCanLastMove >= 0);

		if (iInsertBegin > (m_iDataSize >> 1)) {
			if (iCanLastMove >= iDataLength) {
				goto _move_from_end;
			} else if (m_iDataBegin >= iDataLength) {
				goto _move_from_begin;
			} else {
				goto _move_from_two_side;
			}
		} else {
			if (m_iDataBegin >= iDataLength) {
				goto _move_from_begin;
			} else if (iCanLastMove >= iDataLength) {
				goto _move_from_end;
			} else {
				goto _move_from_two_side;
			}
		}
		
	_move_from_begin:
		//
		::memmove(m_pBufferBegin + m_iDataBegin - iDataLength, m_pBufferBegin + m_iDataBegin, iInsertBegin);
		//
		m_iDataBegin -= (int)iDataLength;
		//
		assert(m_iDataBegin >= 0);
		//
		goto after_move_ok;

	_move_from_end:
		//
		::memmove(m_pBufferBegin + m_iDataBegin + iInsertBegin + iDataLength, m_pBufferBegin + iInsertBegin, m_iDataSize - iInsertBegin);
		//
		goto after_move_ok;

	_move_from_two_side:
		//
		::memmove(m_pBufferBegin, m_pBufferBegin + m_iDataBegin, iInsertBegin);
		//
		::memmove(m_pBufferBegin + iInsertBegin + iDataLength, m_pBufferBegin + m_iDataBegin + iInsertBegin, m_iDataSize - iInsertBegin);
		//
		m_iDataBegin = 0;
		//
		goto after_move_ok;
	}

after_move_ok:
	//
	::memcpy(m_pBufferBegin + m_iDataBegin + iInsertBegin, pData, iDataLength);
	//
	m_iDataSize += (int)iDataLength;

	return *this;
}

//	O(n)
inline
NLPowerBuf& NLPowerBuf::Cut(int iCutBegin, int iLength) {
	if (m_iDataSize < 1) {
		return *this;
	}

	if (iCutBegin > m_iDataSize) {
		iCutBegin = m_iDataSize;
	} else if (iCutBegin < -m_iDataSize) {
		iCutBegin = 0;
	} else if (iCutBegin < 0) {
		iCutBegin += m_iDataSize;
	}

	assert(iCutBegin >= 0 && iLength > 0 && iCutBegin < m_iDataSize);

	if (iCutBegin == 0) {
		return this->CutFront(iLength);
	} else {
		if (iCutBegin + iLength > m_iDataSize) {
			return this->CutBack(m_iDataSize - iCutBegin);
		} else {
			::memmove(m_pBufferBegin + m_iDataBegin + iCutBegin, m_pBufferBegin + m_iDataBegin + iCutBegin + iLength, m_iDataSize - iCutBegin - iLength);
			m_iDataSize -= iLength;
		}
	}
	return *this;
}

//
inline
NLPowerBuf NLPowerBuf::Sub(int iStart, int iEnd) {

	NLPowerBuf newBuf;

	if (m_iDataSize < 1) {
		return newBuf;
	}

	//	some fix params operation
	if (iEnd > m_iDataSize) {
		iEnd = m_iDataSize;
	} else if (iEnd <= -m_iDataSize) {
		iEnd = 0;
	} else if (iEnd < 0) {
		iEnd += m_iDataSize;
	}

	if (iStart > m_iDataSize) {
		iStart = m_iDataSize;
	} else if (iStart <= -m_iDataSize) {
		iStart = 0;
	} else if (iStart < 0) {
		iStart += m_iDataSize;
	}

	if (iStart >= iEnd) {
		//	do nothing
		return newBuf;
	}

	assert(iEnd - iStart <= m_iDataSize);

	newBuf.AddMust(m_pBufferBegin + m_iDataBegin + iStart, iEnd - iStart).AddMust('\0').CutBack(1);

	return newBuf;
}

//
inline	
void NLPowerBuf::Prepare(int iBufferSize) {
	assert(iBufferSize > 0);
	this->ExpandBuffer(iBufferSize, 2000000000);
}

//	*	prepare buffer but not clear the buffer
inline
void NLPowerBuf::PrepareOnly(size_t iBufferSize) {
	assert(iBufferSize > 0);
	this->ExpandBuffer(iBufferSize, iBufferSize);
}

//
inline
void NLPowerBuf::PrepareCanUse(int iAddSize) {
	assert(iAddSize > 0);
	this->ExpandBuffer(iAddSize + m_iDataSize, 2000000000);
}

//
inline
void NLPowerBuf::ReCreate(int iSize) {
	assert(iSize > 0);
	this->Clear();
	this->ExpandBuffer(iSize, 2000000000);
}

//
inline
void NLPowerBuf::Destroy() {
	this->DestroyOnlyBuffer();
	m_iDataSize	= 0;
	m_iDataBegin= 0;
}

//
inline
void NLPowerBuf::CopyFromOthBuffer(const NLPowerBuf& othBuffer) {
	assert(othBuffer.m_iBufferSize >= othBuffer.m_iDataSize);
	if (othBuffer.m_iDataSize > 0) {
		this->Set(othBuffer.m_pBufferBegin, othBuffer.m_iDataSize);
	} else {
		this->Clear();
	}
}

//
inline
void NLPowerBuf::DestroyOnlyBuffer() {
	if (0 != m_pBufferBegin) {
		assert(m_iBufferSize != 0);
		std::free(m_pBufferBegin);
		m_pBufferBegin = 0;
		m_iBufferSize = 0;
	}
}

//
inline
void NLPowerBuf::ExpandBuffer(int iNewSize, size_t dataLength/* = 2000000000*/) {
	assert(iNewSize > 0);
	assert(m_iDataSize <= m_iBufferSize);		
	assert(m_iBufferSize - m_iDataBegin >= 0);
	if (m_iBufferSize - m_iDataBegin < iNewSize) {
		if (m_iBufferSize < iNewSize) {
			if (iNewSize < MAX_CACHE_SIZE) {
				iNewSize <<= 1;
			}
		#if NL_POWER_BUFFER_UES_REALLOC				
			
			if (0 == m_iDataBegin) {
				m_pBufferBegin	= (char* )std::realloc(m_pBufferBegin, iNewSize);
				assert(0 != m_pBufferBegin);
				m_iBufferSize	= iNewSize;
			} else {
				char* pNewBuffer = (char* )std::malloc(iNewSize);
				assert(0 != pNewBuffer);
				if (0 != m_pBufferBegin) {
					if (m_iDataSize > 0) {
						::memcpy(pNewBuffer, m_pBufferBegin + m_iDataBegin, m_iDataSize);
					}
					assert(m_iBufferSize > 0);
					std::free(m_pBufferBegin);
				}
				m_pBufferBegin	= pNewBuffer;
				m_iBufferSize	= iNewSize;
				m_iDataBegin	= 0;
			}

		#else

			char* pNewBuffer = (char* )::malloc(iNewSize);
			assert(0 != pNewBuffer);
			if (0 != m_pBufferBegin) {
				if (m_iDataSize > 0) {
					::memcpy(pNewBuffer, m_pBufferBegin + m_iDataBegin, m_iDataSize);
				}
				assert(m_iBufferSize > 0);
				std::free(m_pBufferBegin);					
			}
			m_pBufferBegin	= pNewBuffer;
			m_iBufferSize	= iNewSize;
			m_iDataBegin	= 0;

		#endif

		} else {
			if (m_iDataSize > 0  && m_iDataBegin > 0 && m_iDataBegin + m_iDataSize + dataLength >= m_iBufferSize) {
				// 尽量不挪动数据, 直到数据即将buffer 填满.
				m_pBufferBegin = (char* )::memmove(m_pBufferBegin, m_pBufferBegin + m_iDataBegin, m_iDataSize);
				assert(0 != m_pBufferBegin);
				m_iDataBegin = 0;
			}
		}
	}
}	

#endif //__NL_POWER_BUF_H__