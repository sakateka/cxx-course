#ifndef HISTORY_CC
#define HISTORY_CC

#include <string>
#include <list>
#include <vector>
#include <stdexcept>

namespace NHistory {

    struct Record {
        const int radix1;
        const int radix2;
        const std::string number1;
        const std::string number2;

        std::string ToString() const {
            return "{{" + number1 + ", " + std::to_string(radix1) + "} =>" +
                   " {" + number2 + ", " + std::to_string(radix2) + "}}";
        }
        bool operator==(Record& rhs) const {
            return radix1 == rhs.radix1 && radix2 == rhs.radix2 &&
                   number1 == rhs.number1 && number2 == rhs.number2;
        }
    };

    class THistory {
    public:
        THistory() {
        }

        Record operator[](size_t i) {
            if (_history.size() > i) {
                for (auto it = _history.begin(); it != _history.end(); it++) {
                    if (i == 0) {
                        return *it;
                    }
                    i--;
                }
            }
            throw std::out_of_range("Index: " + std::to_string(i));
        }
        void AddRecord(int p1, int p2, const std::string& n1, const std::string& n2) {
            _history.push_back({p1, p2, n1, n2});
        }

        void Clear() {
            _history.clear();
        }

        std::vector<std::string> Get() {
            std::vector<std::string> l;
            for (auto& r : _history) {
                l.push_back(r.ToString());
            }
            return l;
        }

        int Count() {
            return _history.size();
        }

    private:
        std::list<Record> _history;
    }; // class THistory
} // namespace NHistory

#ifdef RUN_TESTS
#include "acutest.h"

void test_history() {
    using namespace std;
    using NHistory::Record;
    using NHistory::THistory;
    THistory h;
    TEST_CHECK(h.Count() == 0);
    Record r_A = {16, 13, "F.F000", "12.C259"};
    h.AddRecord(r_A.radix1, r_A.radix2, r_A.number1, r_A.number2);
    TEST_CHECK(h.Count() == 1);

    h.Clear();
    TEST_CHECK(h.Count() == 0);

    h.AddRecord(r_A.radix1, r_A.radix2, r_A.number1, r_A.number2);
    TEST_CHECK(h.Count() == 1);
    TEST_CHECK(h[0] == r_A);

    TEST_EXCEPTION(h[1], out_of_range);
    Record r_B = {8, 13, "17.7400", "12.C259"};
    h.AddRecord(r_B.radix1, r_B.radix2, r_B.number1, r_B.number2);
    TEST_CHECK(h[1] == r_B);

    Record r_C = {16, 7, "F.F000", "21.6364"};
    h.AddRecord(r_C.radix1, r_C.radix2, r_C.number1, r_C.number2);

    TEST_CHECK(h.Count() == 3);
    TEST_CHECK(h[0] == r_A);
    TEST_CHECK(h[2] == r_C);
    TEST_CHECK(h[1].ToString() == "{{17.7400, 8} => {12.C259, 13}}");
    TEST_CHECK(h[2].ToString() == "{{F.F000, 16} => {21.6364, 7}}");

    h.Clear();
    TEST_CHECK(h.Count() == 0);
}

#endif // #ifdef RUN_TESTS
#endif // #ifndef HISTORY_CC
