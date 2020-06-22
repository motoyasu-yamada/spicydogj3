#pragma once

#pragma warning(disable : 4100)
/**	
	SAFXMATH.H
*/
//	D���֐�������킷�\����
template <unsigned int N>
class function
{
private:
	double C[N+1];
public:
	function() // ������
	{
		for (int n = 0;n <= N;n ++)
			C[n] = 0;
	}
	double& operator[] (int n) // �W��
	{
		ASSERT(0 <= n && n <= N);
		return C[n];
	}
	const double& operator[] (int n) const // �W��
	{
		ASSERT(0 <= n && n <= N);
		return C[n];
	}
	double operator() (double x) const // �֐����Z
	{
		double r = 0;
		double xp = 1;
		for (int n = 0;n <= N;n ++)
		{
			r += C[n] * xp;
			xp *= x;
		}
		return r;
	}
}; // function

/**
	N���֐����������
*/
template <unsigned int N>
void diff_func(const function<N>& s,function<N-1>& d)
{
	for (int n = 1;n <= N;n++)
	{
		d[n-1] = n * s[n];
	}
} // diff_func
