#include <iostream>
#include <complex>
#include <vector>
#include <cmath>
#include <fstream>
#define PI 3.14159265358979323846
using namespace std;

class Signal { //Sinusoid 신호의 대한 정보를 담는 Signal 클래스 정의.
private: //진폭, 주파수, 위상에 대응하는 private 변수 선언.
    double amplitude;
    double frequency;
    double phase;
public:
    Signal(double amp, double freq, double ph) : amplitude(amp), frequency(freq), phase(ph) {} //생성자.
    double calculatesin(double t) const { //신호의 사인 함수 값을 도출하는 메소드.
        double singraph = amplitude * sin(2 * PI * frequency * t + phase);
        return singraph;
    }
    double calculatecos(double t) const { //신호의 코사인 함수 값을 도출하는 메소드.
        double cosgraph = amplitude * cos(2 * PI * frequency * t + phase);
        return cosgraph;
    }
    Signal operator+(Signal& x) const { //주파수가 같은 신호끼리 덧셈 연산자로 계산할 수 있도록 오버로딩.
        if (this->frequency == x.frequency) {
            double amp1 = this->amplitude;
            double amp2 = x.amplitude;
            double ph1 = this->phase;
            double ph2 = x.phase;
            double NewAmp = sqrt(pow((amp1 * cos(ph1) + amp2 * cos(ph2)), 2) + pow((amp1 * sin(ph1) + amp2 * sin(ph2)), 2));
            double NewPh = atan2((amp1 * sin(ph1) + amp2 * sin(ph2)), (amp1 * cos(ph1) + amp2 * cos(ph2)));
            Signal Add(NewAmp, this->frequency, NewPh);
            return Add;
        }
    }
};

int main() {
    Signal x1(5, 15, 0.5 * PI); //문제의 조건에 맞게 Signal x1 ~ x4 생성.
    Signal x2(5, 15, -0.25 * PI);
    Signal x3(5, 15, 0.4 * PI);
    Signal x4(5, 15, -0.9 * PI);
    Signal x5 = x1 + x2 + x3 + x4; //덧셈 연산자를 통해 x1 ~ x4를 더하여 x5 생성.

    vector<double> time_values; //시간 데이터를 담는 벡터 생성.
    vector<double> x1_values, x2_values, x3_values, x4_values, x5_values; 
    //시간 데이터에 따른 코사인 값 데이터를 담는 벡터 생성.

    double T = 1.0 / 15.0;  // 주기.
    double time_end = 3 * T; // 3주기.
    double time_step = 0.001; // 시간 간격.

    for (double t = 0; t <= time_end; t += time_step) { 
        time_values.push_back(t);
        x1_values.push_back(x1.calculatecos(t));
        x2_values.push_back(x2.calculatecos(t));
        x3_values.push_back(x3.calculatecos(t));
        x4_values.push_back(x4.calculatecos(t));
        x5_values.push_back(x5.calculatecos(t));
    }
    //주어진 시간 조건에 맞추어 계산해낸 데이터를 각각 알맞은 벡터의 요소로 추가.

    ofstream outfile("signals.txt"); 
    for (size_t i = 0; i < time_values.size(); ++i) {
        outfile << time_values[i] << "\t"
            << x1_values[i] << "\t" << x2_values[i] << "\t"
            << x3_values[i] << "\t" << x4_values[i] << "\t"
            << x5_values[i] << endl;
    }
    outfile.close();
    //.txt 파일을 생성하여 각각의 벡터의 요소 데이터를 알맞게 정렬하여 저장.
    cout << "signals.txt 완료" << endl;

    return 0;
}
