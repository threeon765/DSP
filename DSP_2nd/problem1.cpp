#include <iostream>
#include <math.h>
#include <complex>
#include <fstream>

#define PI 3.141592
using namespace std;

void main()
{
	ofstream OutFile_n, OutFile_mag, OutFile_pha;

	int N = 100;
	double k0 = 40;
	double k1 = 25;
	// k = f_c * N / f_s
	// 여기서는 최대 주파수가 4 Hz이므로 f_s가 8보다 크면 OK, 따라서 10으로 설정

	// Continuous to Discrete
	complex<double>* x1 = new complex<double>[N];
	complex<double>* x2 = new complex<double>[N];
	complex<double>* x = new complex<double>[N];
	for (int n = 0; n < N; n++)
	{
		x1[n] = complex<double>(cos((2 * PI * n * k0) / (double)N), 0.0);
		x2[n] = complex<double>(sin((2 * PI * n * k1) / (double)N), 0.0);
		x[n] = 4.0 * x1[n] - 2.0 * x2[n];
	}

	//Discrete Fourier Transform
	complex<double>* X1 = new complex<double>[N];
	complex<double>* X2 = new complex<double>[N];
	complex<double>* X = new complex<double>[N];
	for (int k = 0; k < N; k++)
	{
		for (int n = 0; n < N; n++)
		{
			X1[k] += x1[n] * complex<double>(cos((-2 * PI * n * k) / (double)N), sin((-2 * PI * n * k) / (double)N));
			X2[k] += x2[n] * complex<double>(cos((-2 * PI * n * k) / (double)N), sin((-2 * PI * n * k) / (double)N));
		}
		X[k] = 4.0 * X1[k] - 2.0 * X2[k];
	}

	OutFile_n.open("n.txt");
	OutFile_mag.open("magnitude.txt");
	OutFile_pha.open("phase.txt");
	for (int n = 0; n < 100; n += 5) // 5개 간격으로 데이터 저장
	{
		OutFile_n << n << endl;
		OutFile_mag << abs(X[n]) << endl; // Magnitude
		if (n == 25 || n == 40 || n == 60 || n == 75) { // Phase, 유효한 샘플에서만 값 저장
			OutFile_pha << atan2(X[n].imag(), X[n].real()) << endl;
		}
		else {
			OutFile_pha << endl;
		}
	}

	system("pause");
	return;
}