#pragma once

class Builder;

class Buildable {
	friend class Builder;
private:
	mutable int m_length;
	mutable int m_startPos;
	mutable int m_useCount;

public:
	Buildable();
	int GetBuiltSize() const;
	int GetUseCount() const;
	virtual ~Buildable();
protected:
	virtual void PreBuild(Builder*) const;
	virtual void Build(Builder*) const = 0;
	virtual void PostBuild(Builder*) const;
}; // class Buildable
