#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <functional>
#include "acutest.h"

// force test running
#ifndef RUN_TESTS
#define RUN_TESTS
#endif // RUN_TESTS

#include "fractional.cc"

using namespace std;
using NFrac::TFrac;

template <typename T>
class InOutDo {
public:
    InOutDo() {
    }

    void Input(istream& in, ostream& out) {
        //Вводим данные с клавиатуры и заносим в контейнер.
        out << "Input (one per line):" << endl;
        string frac;
        while (in >> frac) {
            try {
                T f(frac);
                out << "Add " << f << endl;
                data.push_back(f);
            } catch (invalid_argument& e) {
                out << "Failed to create TFrac. Invalid input: " << e.what() << endl;
            }
        }
    }
    void Output(ostream& out) {
        //Выводим содержимое контейнера на монитор.
        out << "{";
        for (int i = 0; i < data.size(); i++) {
            if (i != 0) {
                out << ", ";
            }
            out << data.at(i);
        }
        out << "}";
    }
    T Sum() {
        //Находим сумму
        T sum = accumulate(data.begin(), data.end(), T(), plus<T>());
        return sum;
    }

    void SortUp() {
        //Сортируем вектор по возрастанию
        sort(data.begin(), data.end(), less<T>());
    }
    void SortDown() {
        //Сортируем вектор по убыванию
        sort(data.begin(), data.end(), greater<T>());
    }

private:
    vector<T> data;
};

void test_InOutDo() {
    TEST_CASE("Input / Output");
    {
        stringstream input("9/1 8/1 7/lkjsdf 7/3");
        string expect = "{9/1, 8/1, 7/3}";
        stringstream inOut;
        InOutDo<TFrac> t1;
        t1.Input(input, inOut);
        stringstream out;
        t1.Output(out);
        if (not TEST_CHECK(out.str() == expect)) {
            TEST_MSG("%s", (out.str() + " != " + expect).c_str());
            TEST_MSG("Input output:%s", inOut.str().c_str());
        }
    }
    TEST_CASE("SortUp");
    {
        stringstream input("9/1 8/1 7/lkjsdf 7/3");
        string expect = "{7/3, 8/1, 9/1}";
        stringstream inOut;
        InOutDo<TFrac> t1;
        t1.Input(input, inOut);
        t1.SortUp();
        stringstream out;
        t1.Output(out);
        if (not TEST_CHECK(out.str() == expect)) {
            TEST_MSG("%s", (out.str() + " != " + expect).c_str());
            TEST_MSG("Input output:%s", inOut.str().c_str());
        }
    }
    TEST_CASE("SortDown");
    {
        stringstream input("9/1 8/1 3/3 7/3");
        string expect = "{9/1, 8/1, 7/3, 1/1}";
        stringstream inOut;
        InOutDo<TFrac> t1;
        t1.Input(input, inOut);
        t1.SortDown();
        stringstream out;
        t1.Output(out);
        if (not TEST_CHECK(out.str() == expect)) {
            TEST_MSG("%s", (out.str() + " != " + expect).c_str());
            TEST_MSG("Input output:%s", inOut.str().c_str());
        }
    }
}

void manualInOutDo() {
    cout << endl
         << "Run manual test programm for vector<TFrac>" << endl;
    InOutDo<TFrac> test_prog;
    test_prog.Input(cin, cout);
    cout << "Output: ";
    test_prog.Output(cout);
    cout << endl;

    cout << "SortUp: ";
    test_prog.SortUp();
    cout << "done" << endl;

    cout << "Output: ";
    test_prog.Output(cout);
    cout << endl;

    cout << "SortDown: ";
    test_prog.SortDown();
    cout << "done" << endl;

    cout << "Output: ";
    test_prog.Output(cout);
    cout << endl;

    cout << "Sum" << endl;
    TFrac sum = test_prog.Sum();
    cout << "TFrac sum: " << sum << endl;
}

TEST_LIST = {
    // Fractional
    {"fractional_constructor", test_fractional_construction},
    {"fractional_operations", test_fractional_operations},
    {"test_output", test_InOutDo},
    {"InOutDo", manualInOutDo},
    {NULL, NULL}};
