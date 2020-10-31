#if !defined(AFX_SKSTU_sqlite3_H__6A083_39EE40CA_B1BFEF913B375BE_A141__HEAD__)
#define AFX_SKSTU_sqlite3_H__6A083_39EE40CA_B1BFEF913B375BE_A141__HEAD__
/*
* CppSQLite
* Developed by Rob Groves <rob.groves@btinternet.com>
* Maintained by NeoSmart Technologies <http://neosmart.net/>
* See LICENSE file for copyright and license info
*/

#include "../Common/gpl/sqlite3/sqlite3.h"
#if defined _M_IX86
#if defined(_DEBUG)
#pragma comment(lib,"../lib/Win32/Debug/db.lib")
#else
#pragma comment(lib,"../lib/Win32/Release/db.lib")
#endif
#elif defined _M_X64
#if defined(_DEBUG)
#pragma comment(lib,"../lib/x64/Debug/db.lib")
#else
#pragma comment(lib,"../lib/x64/Release/db.lib")
#endif
#endif
#include <cstdio>
#include <cstring>

#include <cstdlib>
#include <utility>

#define CPPSQLITE_ERROR 1000

namespace sk {
	namespace sqlite3_helper {
		// Named constant for passing to CppSQLite3Exception when passing it a string
		// that cannot be deleted.
		static const bool DONT_DELETE_MSG = false;

		// Error message used when throwing CppSQLite3Exception when allocations fail.
		static const char* const ALLOCATION_ERROR_MESSAGE = "Cannot allocate memory";
		/*
	** Encode a binary buffer "in" of size n bytes so that it contains
	** no instances of characters '\'' or '\000'.  The output is
	** null-terminated and can be used as a string value in an INSERT
	** or UPDATE statement.  Use sqlite3_decode_binary() to convert the
	** string back into its original binary.
	**
	** The result is written into a preallocated output buffer "out".
	** "out" must be able to hold at least 2 +(257*n)/254 bytes.
	** In other words, the output will be expanded by as much as 3
	** bytes for every 254 bytes of input plus 2 bytes of fixed overhead.
	** (This is approximately 2 + 1.0118*n or about a 1.2% size increase.)
	**
	** The return value is the number of characters in the encoded
	** string, excluding the "\000" terminator.
	*/
		static int sqlite3_encode_binary(const unsigned char* in, int n, unsigned char* out) {
			int i, j, e, m;
			int cnt[256];
			if (n <= 0) {
				out[0] = 'x';
				out[1] = 0;
				return 1;
			}
			memset(cnt, 0, sizeof(cnt));
			for (i = n - 1; i >= 0; i--) { cnt[in[i]]++; }
			m = n;
			for (i = 1; i < 256; i++) {
				int sum;
				if (i == '\'') continue;
				sum = cnt[i] + cnt[(i + 1) & 0xff] + cnt[(i + '\'') & 0xff];
				if (sum < m) {
					m = sum;
					e = i;
					if (m == 0) break;
				}
			}
			out[0] = e;
			j = 1;
			for (i = 0; i < n; i++) {
				int c = (in[i] - e) & 0xff;
				if (c == 0) {
					out[j++] = 1;
					out[j++] = 1;
				}
				else if (c == 1) {
					out[j++] = 1;
					out[j++] = 2;
				}
				else if (c == '\'') {
					out[j++] = 1;
					out[j++] = 3;
				}
				else {
					out[j++] = c;
				}
			}
			out[j] = 0;
			return j;
		}
		/*
	** Decode the string "in" into binary data and write it into "out".
	** This routine reverses the encoding created by sqlite3_encode_binary().
	** The output will always be a few bytes less than the input.  The number
	** of bytes of output is returned.  If the input is not a well-formed
	** encoding, -1 is returned.
	**
	** The "in" and "out" parameters may point to the same buffer in order
	** to decode a string in place.
	*/
		static int sqlite3_decode_binary(const unsigned char* in, unsigned char* out) {
			int i, c, e;
			e = *(in++);
			i = 0;
			while ((c = *(in++)) != 0) {
				if (c == 1) {
					c = *(in++);
					if (c == 1) {
						c = 0;
					}
					else if (c == 2) {
						c = 1;
					}
					else if (c == 3) {
						c = '\'';
					}
					else {
						return -1;
					}
				}
				out[i++] = (c + e) & 0xff;
			}
			return i;
		}




		typedef class CppSQLite3Exception
		{
		public:
			CppSQLite3Exception(const int nErrCode,
				const char* szErrMess,
				bool bDeleteMsg = true) :
				mnErrCode(nErrCode)
			{
				mpszErrMess = sqlite3_mprintf("%s[%d]: %s",
					errorCodeAsString(nErrCode),
					nErrCode,
					szErrMess ? szErrMess : "");

				if (bDeleteMsg && szErrMess)
				{
					sqlite3_free((void*)szErrMess);
				}
			}


			CppSQLite3Exception(const CppSQLite3Exception& e) :
				mnErrCode(e.mnErrCode)
			{
				mpszErrMess = 0;
				if (e.mpszErrMess)
				{
					mpszErrMess = sqlite3_mprintf("%s", e.mpszErrMess);
				}
			}


			static const char* errorCodeAsString(int nErrCode)
			{
				switch (nErrCode)
				{
				case SQLITE_OK: return "SQLITE_OK";
				case SQLITE_ERROR: return "SQLITE_ERROR";
				case SQLITE_INTERNAL: return "SQLITE_INTERNAL";
				case SQLITE_PERM: return "SQLITE_PERM";
				case SQLITE_ABORT: return "SQLITE_ABORT";
				case SQLITE_BUSY: return "SQLITE_BUSY";
				case SQLITE_LOCKED: return "SQLITE_LOCKED";
				case SQLITE_NOMEM: return "SQLITE_NOMEM";
				case SQLITE_READONLY: return "SQLITE_READONLY";
				case SQLITE_INTERRUPT: return "SQLITE_INTERRUPT";
				case SQLITE_IOERR: return "SQLITE_IOERR";
				case SQLITE_CORRUPT: return "SQLITE_CORRUPT";
				case SQLITE_NOTFOUND: return "SQLITE_NOTFOUND";
				case SQLITE_FULL: return "SQLITE_FULL";
				case SQLITE_CANTOPEN: return "SQLITE_CANTOPEN";
				case SQLITE_PROTOCOL: return "SQLITE_PROTOCOL";
				case SQLITE_EMPTY: return "SQLITE_EMPTY";
				case SQLITE_SCHEMA: return "SQLITE_SCHEMA";
				case SQLITE_TOOBIG: return "SQLITE_TOOBIG";
				case SQLITE_CONSTRAINT: return "SQLITE_CONSTRAINT";
				case SQLITE_MISMATCH: return "SQLITE_MISMATCH";
				case SQLITE_MISUSE: return "SQLITE_MISUSE";
				case SQLITE_NOLFS: return "SQLITE_NOLFS";
				case SQLITE_AUTH: return "SQLITE_AUTH";
				case SQLITE_FORMAT: return "SQLITE_FORMAT";
				case SQLITE_RANGE: return "SQLITE_RANGE";
				case SQLITE_ROW: return "SQLITE_ROW";
				case SQLITE_DONE: return "SQLITE_DONE";
				case CPPSQLITE_ERROR: return "CPPSQLITE_ERROR";
				default: return "UNKNOWN_ERROR";
				}
			}


			virtual ~CppSQLite3Exception()
			{
				if (mpszErrMess)
				{
					sqlite3_free(mpszErrMess);
					mpszErrMess = 0;
				}
			}
		public:
			const int errorCode() const { return mnErrCode; }
			const char* errorMessage() const { return mpszErrMess; }
		private:
			int mnErrCode;
			char* mpszErrMess;
		}_SQLITEEXCEPTION, sqliteexception;

		namespace detail
		{
			/**
			* RAII class for managing memory allocated by sqlite
			*/
			class SQLite3Memory
			{
			public:
				SQLite3Memory() :
					mnBufferLen(0),
					mpBuf(nullptr)
				{
				}

				SQLite3Memory(int nBufferLen) :
					mnBufferLen(nBufferLen),
					mpBuf(sqlite3_malloc(nBufferLen))
				{
					if (!mpBuf && mnBufferLen > 0)
					{
						throw CppSQLite3Exception(CPPSQLITE_ERROR,
							ALLOCATION_ERROR_MESSAGE,
							DONT_DELETE_MSG);
					}
				}

				SQLite3Memory(const char* szFormat, va_list list) :
					mnBufferLen(0),
					mpBuf(sqlite3_vmprintf(szFormat, list))
				{
					if (!mpBuf)
					{
						throw CppSQLite3Exception(CPPSQLITE_ERROR,
							ALLOCATION_ERROR_MESSAGE,
							DONT_DELETE_MSG);
					}
					mnBufferLen = (int)std::strlen(static_cast<char const*>(mpBuf)) + 1;
				}

				~SQLite3Memory()
				{
					clear();
				}

				SQLite3Memory(SQLite3Memory const& other) :
					mnBufferLen(other.mnBufferLen),
					mpBuf(sqlite3_malloc(other.mnBufferLen))
				{
					if (!mpBuf && mnBufferLen > 0)
					{
						throw CppSQLite3Exception(CPPSQLITE_ERROR,
							ALLOCATION_ERROR_MESSAGE,
							DONT_DELETE_MSG);
					}
					std::memcpy(mpBuf, other.mpBuf, mnBufferLen);
				}

				SQLite3Memory& operator=(SQLite3Memory const& lhs)
				{
					SQLite3Memory tmp(lhs);
					swap(tmp);
					return *this;
				}

				SQLite3Memory(SQLite3Memory&& other) :
					mnBufferLen(other.mnBufferLen),
					mpBuf(other.mpBuf)
				{
					other.mnBufferLen = 0;
					other.mpBuf = nullptr;
				}

				SQLite3Memory& operator=(SQLite3Memory&& lhs)
				{
					swap(lhs);
					return *this;
				}

				void swap(SQLite3Memory& other)
				{
					std::swap(mnBufferLen, other.mnBufferLen);
					std::swap(mpBuf, other.mpBuf);
				}
				void clear()
				{
					sqlite3_free(mpBuf);
					mpBuf = nullptr;
					mnBufferLen = 0;
				}

				int getLength() const { return mnBufferLen; }
				void* getBuffer() const { return mpBuf; }
			private:
				int mnBufferLen;
				void* mpBuf;
			};
		}///namespace detail









		typedef class CppSQLite3Buffer
		{
		public:
			const char* format(const char* szFormat, ...)
			{
				clear();
				va_list va;
				try
				{
					va_start(va, szFormat);
					mBuf = detail::SQLite3Memory(szFormat, va);
					va_end(va);
					return static_cast<const char*>(mBuf.getBuffer());
				}
				catch (CppSQLite3Exception&)
				{
					va_end(va);
					throw;
				}
			}

			operator const char* () { return static_cast<char const*>(mBuf.getBuffer()); }

			void clear()
			{
				mBuf.clear();
			}
		private:
			detail::SQLite3Memory mBuf;
		}_SQLITEBUFFER, sqlitebuffer;


		typedef class CppSQLite3Binary
		{
		public:
			CppSQLite3Binary() :
				mpBuf(0),
				mnBinaryLen(0),
				mnBufferLen(0),
				mnEncodedLen(0),
				mbEncoded(false)
			{
			}


			~CppSQLite3Binary()
			{
				clear();
			}


			void setBinary(const unsigned char* pBuf, int nLen)
			{
				mpBuf = allocBuffer(nLen);
				memcpy(mpBuf, pBuf, nLen);
			}


			void setEncoded(const unsigned char* pBuf)
			{
				clear();

				mnEncodedLen = (int)strlen((const char*)pBuf);
				mnBufferLen = mnEncodedLen + 1; // Allow for NULL terminator

				mpBuf = (unsigned char*)malloc(mnBufferLen);

				if (!mpBuf)
				{
					throw CppSQLite3Exception(CPPSQLITE_ERROR,
						ALLOCATION_ERROR_MESSAGE,
						DONT_DELETE_MSG);
				}

				memcpy(mpBuf, pBuf, mnBufferLen);
				mbEncoded = true;
			}


			const unsigned char* getEncoded()
			{
				if (!mbEncoded)
				{
					unsigned char* ptmp = (unsigned char*)malloc(mnBinaryLen);
					if (ptmp) {
						memcpy(ptmp, mpBuf, mnBinaryLen);
						mnEncodedLen = sqlite3_encode_binary(ptmp, mnBinaryLen, mpBuf);
						free(ptmp);
						mbEncoded = true;
					}
				}

				return mpBuf;
			}


			const unsigned char* getBinary()
			{
				if (mbEncoded)
				{
					// in/out buffers can be the same
					mnBinaryLen = sqlite3_decode_binary(mpBuf, mpBuf);

					if (mnBinaryLen == -1)
					{
						throw CppSQLite3Exception(CPPSQLITE_ERROR,
							"Cannot decode binary",
							DONT_DELETE_MSG);
					}

					mbEncoded = false;
				}

				return mpBuf;
			}


			int getBinaryLength()
			{
				getBinary();
				return mnBinaryLen;
			}


			unsigned char* allocBuffer(int nLen)
			{
				clear();

				// Allow extra space for encoded binary as per comments in
				// SQLite encode.c See bottom of this file for implementation
				// of SQLite functions use 3 instead of 2 just to be sure ;-)
				mnBinaryLen = nLen;
				mnBufferLen = 3 + (257 * nLen) / 254;

				mpBuf = (unsigned char*)malloc(mnBufferLen);

				if (!mpBuf)
				{
					throw CppSQLite3Exception(CPPSQLITE_ERROR,
						ALLOCATION_ERROR_MESSAGE,
						DONT_DELETE_MSG);
				}

				mbEncoded = false;

				return mpBuf;
			}


			void clear()
			{
				if (mpBuf)
				{
					mnBinaryLen = 0;
					mnBufferLen = 0;
					free(mpBuf);
					mpBuf = 0;
				}
			}
		private:
			unsigned char* mpBuf;
			int mnBinaryLen;
			int mnBufferLen;
			int mnEncodedLen;
			bool mbEncoded;
		}_SQLITEBINARY, sqlitebinary;



		typedef class CppSQLite3Query
		{
		public:
			CppSQLite3Query() {
				mpVM = 0;
				mbEof = true;
				mnCols = 0;
				mbOwnVM = false;
			}
			CppSQLite3Query(const CppSQLite3Query& rQuery) {
				mpVM = rQuery.mpVM;
				// Only one object can own the VM
				const_cast<CppSQLite3Query&>(rQuery).mpVM = 0;
				mbEof = rQuery.mbEof;
				mnCols = rQuery.mnCols;
				mbOwnVM = rQuery.mbOwnVM;
			}
			CppSQLite3Query(sqlite3* pDB,
				sqlite3_stmt* pVM,
				bool bEof,
				bool bOwnVM = true) {
				mpDB = pDB;
				mpVM = pVM;
				mbEof = bEof;
				mnCols = sqlite3_column_count(mpVM);
				mbOwnVM = bOwnVM;
			}
			CppSQLite3Query& operator=(const CppSQLite3Query& rQuery) {
				try
				{
					finalize();
				}
				catch (...)
				{
				}
				mpVM = rQuery.mpVM;
				// Only one object can own the VM
				const_cast<CppSQLite3Query&>(rQuery).mpVM = 0;
				mbEof = rQuery.mbEof;
				mnCols = rQuery.mnCols;
				mbOwnVM = rQuery.mbOwnVM;
				return *this;
			}
			virtual ~CppSQLite3Query() {
				try
				{
					finalize();
				}
				catch (...)
				{
				}
			}
			int numFields() const {
				checkVM();
				return mnCols;
			}
			int fieldIndex(const char* szField) const {
				checkVM();
				if (szField)
				{
					for (int nField = 0; nField < mnCols; nField++)
					{
						const char* szTemp = sqlite3_column_name(mpVM, nField);
						if (strcmp(szField, szTemp) == 0)
						{
							return nField;
						}
					}
				}
				throw CppSQLite3Exception(CPPSQLITE_ERROR,
					"Invalid field name requested",
					DONT_DELETE_MSG);
			}
			const char* fieldName(int nCol) const {
				checkVM();
				if (nCol < 0 || nCol > mnCols - 1)
				{
					throw CppSQLite3Exception(CPPSQLITE_ERROR,
						"Invalid field index requested",
						DONT_DELETE_MSG);
				}
				return sqlite3_column_name(mpVM, nCol);
			}
			const char* fieldDeclType(int nCol) const {
				checkVM();
				if (nCol < 0 || nCol > mnCols - 1)
				{
					throw CppSQLite3Exception(CPPSQLITE_ERROR,
						"Invalid field index requested",
						DONT_DELETE_MSG);
				}
				return sqlite3_column_decltype(mpVM, nCol);
			}
			int fieldDataType(int nCol) const {
				checkVM();
				if (nCol < 0 || nCol > mnCols - 1)
				{
					throw CppSQLite3Exception(CPPSQLITE_ERROR,
						"Invalid field index requested",
						DONT_DELETE_MSG);
				}
				return sqlite3_column_type(mpVM, nCol);
			}
			const char* fieldValue(int nField) const {
				checkVM();

				if (nField < 0 || nField > mnCols - 1)
				{
					throw CppSQLite3Exception(CPPSQLITE_ERROR,
						"Invalid field index requested",
						DONT_DELETE_MSG);
				}
				return (const char*)sqlite3_column_text(mpVM, nField);
			}
			const char* fieldValue(const char* szField) const {
				int nField = fieldIndex(szField);
				return (const char*)sqlite3_column_text(mpVM, nField);
			}
			int getIntField(int nField, int nNullValue = 0) const {
				if (fieldDataType(nField) == SQLITE_NULL)
				{
					return nNullValue;
				}
				else
				{
					return sqlite3_column_int(mpVM, nField);
				}
			}
			int getIntField(const char* szField, int nNullValue = 0) const {
				int nField = fieldIndex(szField);
				return getIntField(nField, nNullValue);
			}
			long long getInt64Field(int nField, long long nNullValue = 0) const {
				if (fieldDataType(nField) == SQLITE_NULL)
				{
					return nNullValue;
				}
				else
				{
					return sqlite3_column_int64(mpVM, nField);
				}
			}
			long long getInt64Field(const char* szField, long long nNullValue = 0) const {
				int nField = fieldIndex(szField);
				return getInt64Field(nField, nNullValue);
			}
			double getFloatField(int nField, double fNullValue = 0.0) const {
				if (fieldDataType(nField) == SQLITE_NULL)
				{
					return fNullValue;
				}
				else
				{
					return sqlite3_column_double(mpVM, nField);
				}
			}
			double getFloatField(const char* szField, double fNullValue = 0.0) const {
				int nField = fieldIndex(szField);
				return getFloatField(nField, fNullValue);
			}
			const char* getStringField(int nField, const char* szNullValue = "") const {
				if (fieldDataType(nField) == SQLITE_NULL)
				{
					return szNullValue;
				}
				else
				{
					return (const char*)sqlite3_column_text(mpVM, nField);
				}
			}
			const char* getStringField(const char* szField, const char* szNullValue = "") const {
				int nField = fieldIndex(szField);
				return getStringField(nField, szNullValue);
			}
			const unsigned char* getBlobField(int nField, int& nLen) const {
				checkVM();
				if (nField < 0 || nField > mnCols - 1)
				{
					throw CppSQLite3Exception(CPPSQLITE_ERROR,
						"Invalid field index requested",
						DONT_DELETE_MSG);
				}
				nLen = sqlite3_column_bytes(mpVM, nField);
				return (const unsigned char*)sqlite3_column_blob(mpVM, nField);
			}
			const unsigned char* getBlobField(const char* szField, int& nLen) const {
				int nField = fieldIndex(szField);
				return getBlobField(nField, nLen);
			}
			bool fieldIsNull(int nField) const {
				return (fieldDataType(nField) == SQLITE_NULL);
			}
			bool fieldIsNull(const char* szField) const {
				int nField = fieldIndex(szField);
				return (fieldDataType(nField) == SQLITE_NULL);
			}
			bool eof() const {
				checkVM();
				return mbEof;
			}
			void nextRow() {
				checkVM();

				int nRet = sqlite3_step(mpVM);

				if (nRet == SQLITE_DONE)
				{
					// no rows
					mbEof = true;
				}
				else if (nRet == SQLITE_ROW)
				{
					// more rows, nothing to do
				}
				else
				{
					nRet = sqlite3_finalize(mpVM);
					mpVM = 0;
					const char* szError = sqlite3_errmsg(mpDB);
					throw CppSQLite3Exception(nRet,
						(char*)szError,
						DONT_DELETE_MSG);
				}
			}
			void finalize() {
				if (mpVM && mbOwnVM)
				{
					int nRet = sqlite3_finalize(mpVM);
					mpVM = 0;
					if (nRet != SQLITE_OK)
					{
						const char* szError = sqlite3_errmsg(mpDB);
						throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
					}
				}
			}
		private:
			void checkVM() const {
				if (mpVM == 0) {
					throw CppSQLite3Exception(CPPSQLITE_ERROR,
						"Null Virtual Machine pointer",
						DONT_DELETE_MSG);
				}
			}
		private:
			sqlite3* mpDB;
			sqlite3_stmt* mpVM;
			bool mbEof;
			int mnCols;
			bool mbOwnVM;
		}_SQLITEQUERY, sqlitequery;




		typedef class CppSQLite3Table
		{
		public:
			CppSQLite3Table() {
				mpaszResults = 0;
				mnRows = 0;
				mnCols = 0;
				mnCurrentRow = 0;
			}
			CppSQLite3Table(const CppSQLite3Table& rTable) {
				mpaszResults = rTable.mpaszResults;
				// Only one object can own the results
				const_cast<CppSQLite3Table&>(rTable).mpaszResults = 0;
				mnRows = rTable.mnRows;
				mnCols = rTable.mnCols;
				mnCurrentRow = rTable.mnCurrentRow;
			}
			CppSQLite3Table(char** paszResults, int nRows, int nCols) {
				mpaszResults = paszResults;
				mnRows = nRows;
				mnCols = nCols;
				mnCurrentRow = 0;
			}
			virtual ~CppSQLite3Table() {
				try
				{
					finalize();
				}
				catch (...)
				{
				}
			}
			CppSQLite3Table& operator=(const CppSQLite3Table& rTable) {
				try
				{
					finalize();
				}
				catch (...)
				{
				}
				mpaszResults = rTable.mpaszResults;
				// Only one object can own the results
				const_cast<CppSQLite3Table&>(rTable).mpaszResults = 0;
				mnRows = rTable.mnRows;
				mnCols = rTable.mnCols;
				mnCurrentRow = rTable.mnCurrentRow;
				return *this;
			}
			int numFields() const {
				checkResults();
				return mnCols;
			}
			int numRows() const {
				checkResults();
				return mnRows;
			}
			const char* fieldName(int nCol) const {
				checkResults();

				if (nCol < 0 || nCol > mnCols - 1)
				{
					throw CppSQLite3Exception(CPPSQLITE_ERROR,
						"Invalid field index requested",
						DONT_DELETE_MSG);
				}

				return mpaszResults[nCol];
			}
			const char* fieldValue(int nField) const {
				checkResults();

				if (nField < 0 || nField > mnCols - 1)
				{
					throw CppSQLite3Exception(CPPSQLITE_ERROR,
						"Invalid field index requested",
						DONT_DELETE_MSG);
				}

				int nIndex = (mnCurrentRow * mnCols) + mnCols + nField;
				return mpaszResults[nIndex];
			}
			const char* fieldValue(const char* szField) const {
				checkResults();

				if (szField)
				{
					for (int nField = 0; nField < mnCols; nField++)
					{
						if (strcmp(szField, mpaszResults[nField]) == 0)
						{
							int nIndex = (mnCurrentRow * mnCols) + mnCols + nField;
							return mpaszResults[nIndex];
						}
					}
				}

				throw CppSQLite3Exception(CPPSQLITE_ERROR,
					"Invalid field name requested",
					DONT_DELETE_MSG);
			}
			int getIntField(int nField, int nNullValue = 0) const {
				if (fieldIsNull(nField))
				{
					return nNullValue;
				}
				else
				{
					return atoi(fieldValue(nField));
				}
			}
			int getIntField(const char* szField, int nNullValue = 0) const {
				if (fieldIsNull(szField))
				{
					return nNullValue;
				}
				else
				{
					return atoi(fieldValue(szField));
				}
			}
			double getFloatField(int nField, double fNullValue = 0.0) const {
				if (fieldIsNull(nField))
				{
					return fNullValue;
				}
				else
				{
					return atof(fieldValue(nField));
				}
			}
			double getFloatField(const char* szField, double fNullValue = 0.0) const {
				if (fieldIsNull(szField))
				{
					return fNullValue;
				}
				else
				{
					return atof(fieldValue(szField));
				}
			}
			const char* getStringField(int nField, const char* szNullValue = "") const {
				if (fieldIsNull(nField))
				{
					return szNullValue;
				}
				else
				{
					return fieldValue(nField);
				}
			}
			const char* getStringField(const char* szField, const char* szNullValue = "") const {
				if (fieldIsNull(szField))
				{
					return szNullValue;
				}
				else
				{
					return fieldValue(szField);
				}
			}
			bool fieldIsNull(int nField) const {
				checkResults();
				return (fieldValue(nField) == 0);
			}
			bool fieldIsNull(const char* szField) const {
				checkResults();
				return (fieldValue(szField) == 0);
			}
			void setRow(int nRow) {
				checkResults();

				if (nRow < 0 || nRow > mnRows - 1)
				{
					throw CppSQLite3Exception(CPPSQLITE_ERROR,
						"Invalid row index requested",
						DONT_DELETE_MSG);
				}

				mnCurrentRow = nRow;
			}
			void finalize() {
				if (mpaszResults)
				{
					sqlite3_free_table(mpaszResults);
					mpaszResults = 0;
				}
			}
		private:
			void checkResults() const {
				if (mpaszResults == 0)
				{
					throw CppSQLite3Exception(CPPSQLITE_ERROR,
						"Null Results pointer",
						DONT_DELETE_MSG);
				}
			}
			int mnCols;
			int mnRows;
			int mnCurrentRow;
			char** mpaszResults;
		}_SQLITETABLE, sqlitetable;





		typedef class CppSQLite3Statement
		{
		public:
			CppSQLite3Statement() {
				mpDB = 0;
				mpVM = 0;
			}
			CppSQLite3Statement(const CppSQLite3Statement& rStatement) {
				mpDB = rStatement.mpDB;
				mpVM = rStatement.mpVM;
				// Only one object can own VM
				const_cast<CppSQLite3Statement&>(rStatement).mpVM = 0;
			}
			CppSQLite3Statement(sqlite3* pDB, sqlite3_stmt* pVM) {
				mpDB = pDB;
				mpVM = pVM;
			}
			virtual ~CppSQLite3Statement() {
				try
				{
					finalize();
				}
				catch (...)
				{
				}
			}
			CppSQLite3Statement& operator=(const CppSQLite3Statement& rStatement) {
				mpDB = rStatement.mpDB;
				mpVM = rStatement.mpVM;
				// Only one object can own VM
				const_cast<CppSQLite3Statement&>(rStatement).mpVM = 0;
				return *this;
			}
			int execDML() {
				checkDB();
				checkVM();

				const char* szError = 0;

				int nRet = sqlite3_step(mpVM);

				if (nRet == SQLITE_DONE)
				{
					int nRowsChanged = sqlite3_changes(mpDB);

					nRet = sqlite3_reset(mpVM);

					if (nRet != SQLITE_OK)
					{
						szError = sqlite3_errmsg(mpDB);
						throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
					}

					return nRowsChanged;
				}
				else
				{
					nRet = sqlite3_reset(mpVM);
					szError = sqlite3_errmsg(mpDB);
					throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
				}
			}
			CppSQLite3Query execQuery() {
				checkDB();
				checkVM();

				int nRet = sqlite3_step(mpVM);

				if (nRet == SQLITE_DONE)
				{
					// no rows
					return CppSQLite3Query(mpDB, mpVM, true/*eof*/, false);
				}
				else if (nRet == SQLITE_ROW)
				{
					// at least 1 row
					return CppSQLite3Query(mpDB, mpVM, false/*eof*/, false);
				}
				else
				{
					nRet = sqlite3_reset(mpVM);
					const char* szError = sqlite3_errmsg(mpDB);
					throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
				}
			}
			void bind(int nParam, const char* szValue) {
				checkVM();
				int nRes = sqlite3_bind_text(mpVM, nParam, szValue, -1, SQLITE_TRANSIENT);

				if (nRes != SQLITE_OK)
				{
					throw CppSQLite3Exception(nRes,
						"Error binding string param",
						DONT_DELETE_MSG);
				}
			}
			void bind(int nParam, const int nValue) {
				checkVM();
				int nRes = sqlite3_bind_int(mpVM, nParam, nValue);

				if (nRes != SQLITE_OK)
				{
					throw CppSQLite3Exception(nRes,
						"Error binding int param",
						DONT_DELETE_MSG);
				}
			}
			void bind(int nParam, const unsigned int nValue) {
				checkVM();
				int nRes = sqlite3_bind_int(mpVM, nParam, static_cast<unsigned int>(nValue));

				if (nRes != SQLITE_OK)
				{
					throw CppSQLite3Exception(nRes,
						"Error binding int param",
						DONT_DELETE_MSG);
				}
			}
			void bind(int nParam, const long long nValue) {
				checkVM();
				int nRes = sqlite3_bind_int64(mpVM, nParam, nValue);

				if (nRes != SQLITE_OK)
				{
					throw CppSQLite3Exception(nRes,
						"Error binding int64 param",
						DONT_DELETE_MSG);
				}
			}
			void bind(int nParam, const unsigned long long nValue) {
				checkVM();
				int nRes = sqlite3_bind_int64(mpVM, nParam, static_cast<long long>(nValue));

				if (nRes != SQLITE_OK)
				{
					throw CppSQLite3Exception(nRes,
						"Error binding int64 param",
						DONT_DELETE_MSG);
				}
			}
			void bind(int nParam, const double dValue) {
				checkVM();
				int nRes = sqlite3_bind_double(mpVM, nParam, dValue);

				if (nRes != SQLITE_OK)
				{
					throw CppSQLite3Exception(nRes,
						"Error binding double param",
						DONT_DELETE_MSG);
				}
			}
			void bind(int nParam, const unsigned char* blobValue, int nLen) {
				checkVM();
				int nRes = sqlite3_bind_blob(mpVM, nParam,
					(const void*)blobValue, nLen, SQLITE_TRANSIENT);

				if (nRes != SQLITE_OK)
				{
					throw CppSQLite3Exception(nRes,
						"Error binding blob param",
						DONT_DELETE_MSG);
				}
			}
			void bindNull(int nParam) {
				checkVM();
				int nRes = sqlite3_bind_null(mpVM, nParam);

				if (nRes != SQLITE_OK)
				{
					throw CppSQLite3Exception(nRes,
						"Error binding NULL param",
						DONT_DELETE_MSG);
				}
			}
			void reset() {
				if (mpVM)
				{
					int nRet = sqlite3_reset(mpVM);

					if (nRet != SQLITE_OK)
					{
						const char* szError = sqlite3_errmsg(mpDB);
						throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
					}
				}
			}
			void finalize() {
				if (mpVM)
				{
					int nRet = sqlite3_finalize(mpVM);
					mpVM = 0;

					if (nRet != SQLITE_OK)
					{
						const char* szError = sqlite3_errmsg(mpDB);
						throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
					}
				}
			}
		private:
			void checkDB() const {
				if (mpDB == 0)
				{
					throw CppSQLite3Exception(CPPSQLITE_ERROR,
						"Database not open",
						DONT_DELETE_MSG);
				}
			}
			void checkVM() const {
				if (mpVM == 0)
				{
					throw CppSQLite3Exception(CPPSQLITE_ERROR,
						"Null Virtual Machine pointer",
						DONT_DELETE_MSG);
				}
			}
			sqlite3* mpDB;
			sqlite3_stmt* mpVM;
		}_SQLITESTATEMENT, sqlitestatement;









		typedef class CppSQLite3DB
		{
		public:
			CppSQLite3DB() {
				mpDB = 0;
				mnBusyTimeoutMs = 60000; // 60 seconds
			}
			virtual ~CppSQLite3DB() {
				close();
			}
			void open(const char* szFile) {
				int nRet = sqlite3_open(szFile, &mpDB);
				if (nRet != SQLITE_OK)
				{
					const char* szError = sqlite3_errmsg(mpDB);
					throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
				}
				setBusyTimeout(mnBusyTimeoutMs);
			}
			void close() {
				if (mpDB)
				{
					sqlite3_close(mpDB);
					mpDB = 0;
				}
			}
			bool tableExists(const char* szTable) {
				CppSQLite3Buffer sql;
				sql.format("select count(*) from sqlite_master where type='table' and name=%Q",
					szTable);
				int nRet = execScalar(sql);
				return (nRet > 0);
			}
			int execDML(const char* szSQL) {
				checkDB();

				char* szError = 0;

				int nRet = sqlite3_exec(mpDB, szSQL, 0, 0, &szError);

				if (nRet == SQLITE_OK)
				{
					return sqlite3_changes(mpDB);
				}
				else
				{
					throw CppSQLite3Exception(nRet, szError);
				}
			}
			CppSQLite3Query execQuery(const char* szSQL) {
				checkDB();

				sqlite3_stmt* pVM = compile(szSQL);

				int nRet = sqlite3_step(pVM);

				if (nRet == SQLITE_DONE)
				{
					// no rows
					return CppSQLite3Query(mpDB, pVM, true/*eof*/);
				}
				else if (nRet == SQLITE_ROW)
				{
					// at least 1 row
					return CppSQLite3Query(mpDB, pVM, false/*eof*/);
				}
				else
				{
					nRet = sqlite3_finalize(pVM);
					const char* szError = sqlite3_errmsg(mpDB);
					throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
				}
			}
			int execScalar(const char* szSQL) {
				CppSQLite3Query q = execQuery(szSQL);

				if (q.eof() || q.numFields() < 1)
				{
					throw CppSQLite3Exception(CPPSQLITE_ERROR,
						"Invalid scalar query",
						DONT_DELETE_MSG);
				}

				return atoi(q.fieldValue(0));
			}
			CppSQLite3Table getTable(const char* szSQL) {
				checkDB();
				char* szError = 0;
				char** paszResults = 0;
				int nRet;
				int nRows(0);
				int nCols(0);
				nRet = sqlite3_get_table(mpDB, szSQL, &paszResults, &nRows, &nCols, &szError);
				if (nRet == SQLITE_OK)
				{
					return CppSQLite3Table(paszResults, nRows, nCols);
				}
				else
				{
					throw CppSQLite3Exception(nRet, szError);
				}
			}
			CppSQLite3Statement compileStatement(const char* szSQL) {
				checkDB();

				sqlite3_stmt* pVM = compile(szSQL);
				return CppSQLite3Statement(mpDB, pVM);
			}
			sqlite_int64 lastRowId() const {
				return sqlite3_last_insert_rowid(mpDB);
			}
			void interrupt() { sqlite3_interrupt(mpDB); }
			void setBusyTimeout(int nMillisecs) {
				mnBusyTimeoutMs = nMillisecs;
				sqlite3_busy_timeout(mpDB, mnBusyTimeoutMs);
			}
			static const char* SQLiteVersion() { return SQLITE_VERSION; }
		private:
			CppSQLite3DB(const CppSQLite3DB& db) {
				mpDB = db.mpDB;
				mnBusyTimeoutMs = 60000; // 60 seconds
			}
			CppSQLite3DB& operator=(const CppSQLite3DB& db) {
				mpDB = db.mpDB;
				mnBusyTimeoutMs = 60000; // 60 seconds
				return *this;
			}
			sqlite3_stmt* compile(const char* szSQL) {
				checkDB();

				char* szError = 0;
				const char* szTail = 0;
				sqlite3_stmt* pVM;

				int nRet = sqlite3_prepare(mpDB, szSQL, -1, &pVM, &szTail);

				if (nRet != SQLITE_OK)
				{
					throw CppSQLite3Exception(nRet, szError);
				}

				return pVM;
			}
			void checkDB() const {
				if (!mpDB)
				{
					throw CppSQLite3Exception(CPPSQLITE_ERROR,
						"Database not open",
						DONT_DELETE_MSG);
				}
			}
			sqlite3* mpDB;
			int mnBusyTimeoutMs;
		}_SQLITEDB, sqlitedb;
	}///namespace sqlite3
}///namespace sk





/// /*新生联创（上海）**/
/// /*2019年01月17日 15:51:03|819**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_sqlite3_H__6A083_39EE40CA_B1BFEF913B375BE_A141__TAIL__


