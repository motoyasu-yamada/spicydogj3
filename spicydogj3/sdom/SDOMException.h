#pragma once

class SDOMException : public CException {
public:
	static const int CAUSE_CORRUPTED;
	static const int CAUSE_UNKNOWN_VERSION;
	
	int GetCause() const;
	static __declspec(noreturn) void throwException(int cause);

private:
	SDOMException();
	static SDOMException s_exception;
	int m_cause;
};