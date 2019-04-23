#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "acutest.h"
#include "fractional.cc"

using namespace std;
using NFrac::TFrac;

template <typename T>
class InOutDo {
public:
    InOutDo() {
        cout << endl
             << "Run test programm for vector<TFrac>" << endl;
    }

    void Input() {
        //Вводим данные с клавиатуры и заносим в контейнер.
        string frac;
        while (cin >> frac) {
            try {
                T f(frac);
                cout << "Add " << f << endl;
                data.push_back(f);
            } catch (invalid_argument& e) {
                cout << "Failed to create TFrac. Invalid input: " << e.what() << endl;
            }
        }
    }
    void Output() {
        //Выводим содержимое контейнера на монитор.
        cout << "{";
        for (int i = 0; i < data.size(); i++) {
            if (i != 0) {
                cout << ", ";
            }
            cout << data.at(i);
        }
        cout << "}" << endl;
    }
    T Sum() {
        //Находим сумму
    }
    void SortUp() {
        //Сортируем вектор по возрастанию
    }
    void SortDown() {
        //Сортируем вектор по убыванию
    }

private:
    vector<T> data;
};

void test_main() {
    InOutDo<TFrac> test_prog;
    test_prog.Input();
    test_prog.Output();
}

using namespace std;

TEST_LIST = {
    {"main", test_main},
    {NULL, NULL}};
