#include "acutest.h"
#include <iostream>
#include <set>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <initializer_list>

using namespace std;

namespace NSet {

    class TSet {
    public:
        TSet() {
            _s = set<string, less<string>>();
        }

        TSet(initializer_list<string> in) {
            _s = set<string, less<string>>();
            _s.insert(in.begin(), in.end());
        }

        void Insert(string a) {
            _s.insert(a);
        }

        void Remove(string a) {
            _s.erase(a);
        }

        void Clear() {
            _s.clear();
        }

        size_t Size() const {
            return _s.size();
        }

        bool Has(string a) const {
            return _s.count(a);
        }

        bool Empty() const {
            return _s.empty();
        }

        bool operator==(const TSet& rhs) const {
            if (Size() == rhs.Size()) {
                for (size_t i = 0; i < Size(); i++) {
                    string el = Get(i);
                    if (!rhs.Has(el)) {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }

        bool operator!=(const TSet& rhs) const {
            return !(*this == rhs);
        }

        TSet operator*(const TSet& rhs) const {
            TSet t;
            for (size_t i = 0; i < Size(); i++) {
                string el = Get(i);
                if (rhs.Has(el)) {
                    t.Insert(el);
                }
            }
            return t;
        }

        TSet operator+(TSet& rhs) const {
            TSet t;
            for (size_t i = 0; i < Size(); i++) {
                t.Insert(Get(i));
            }
            for (size_t i = 0; i < rhs.Size(); i++) {
                t.Insert(rhs.Get(i));
            }
            return t;
        }

        TSet operator-(TSet& rhs) const {
            TSet t;
            for (size_t i = 0; i < Size(); i++) {
                string el = Get(i);
                if (!rhs.Has(el)) {
                    t.Insert(el);
                };
            }
            return t;
        }

        string Get(size_t idx) const {
            if (idx < _s.size() && idx >= 0) {
                auto start = _s.begin();
                for (size_t i = 0; i < idx; i++) {
                    start++;
                }
                return *start;
            }
            throw out_of_range("Index " + to_string(idx));
        }

        void Output(ostream& out = cout) const {
            out << ToString();
        }

        string ToString() const {
            stringstream out;
            out << "{";
            bool first = true;
            for (auto iter = _s.begin(); iter != _s.end(); iter++) {
                if (!first) {
                    out << ", ";
                } else {
                    first = false;
                }
                out << *iter;
            }
            out << "}";
            return out.str();
        }

    private:
        set<string, less<string>> _s;
    }; // class TSet
} // namespace NSet

void test_constructor() {
    NSet::TSet t;
    TEST_CHECK(t.Empty());

    t = {"1", "Z"}; // implicit constructor with initialization list
    TEST_CHECK(t.ToString() == "{1, Z}");

    NSet::TSet t2({"One", "Two"}); // explicit with initialization list
    TEST_CHECK(t2.ToString() == "{One, Two}");
}

void test_insert() {
    NSet::TSet t;
    t.Insert("1");
    t.Insert("2");
    t.Insert("2");
    t.Insert("3");
    TEST_CHECK(t == NSet::TSet({"1", "2", "3"}));
}

void test_remove() {
    NSet::TSet t({"1", "2", "3", "3"}), empty;
    t.Remove("2");
    TEST_CHECK(t == NSet::TSet({"1", "3"}));
    t.Remove("1");
    t.Remove("3");
    TEST_CHECK(empty.Empty());
    TEST_CHECK(t.Empty() && t == empty);
}

void test_clear() {
    NSet::TSet t;
    TEST_CHECK(t.Empty());
    t.Insert("1");
    t.Insert("2");
    t.Clear();
    TEST_CHECK(t.Empty() && t.Size() == 0);
    t.Clear();
    TEST_CHECK(t.Empty() && t.ToString() == "{}");
}

void test_size() {
    NSet::TSet t;
    TEST_CHECK(t.Size() == 0);
    t.Insert("1");
    t.Insert("2");
    TEST_CHECK(t.Size() == 2);
    t.Insert("2");
    TEST_CHECK(t.Size() == 2);
    t.Insert("3");
    TEST_CHECK(t.Size() == 3);
    t.Remove("3");
    TEST_CHECK(t.Size() == 2);
    t.Clear();
    TEST_CHECK(t.Size() == 0);
}

void test_has() {
    NSet::TSet t;
    TEST_CHECK(!t.Has("1"));

    t.Insert("1");
    TEST_CHECK(t.Has("1"));

    t.Insert("2");
    TEST_CHECK(t.Has("1") && t.Has("2"));

    t.Remove("1");
    TEST_CHECK(!t.Has("1") && t.Has("2"));

    t.Insert("1");
    TEST_CHECK(t.Has("1"));
    t.Insert("1");
    TEST_CHECK(t.Has("1"));
    t.Remove("1");
    TEST_CHECK(!t.Has("1"));
    TEST_CHECK(t.Has("2"));
}

void test_empty() {
    NSet::TSet t;
    TEST_CHECK(t.Empty());
    t.Insert("1");
    TEST_CHECK(!t.Empty());
    t.Remove("1");
    TEST_CHECK(t.Empty() && t.Size() == 0);
    t.Insert("1");
    TEST_CHECK(!t.Empty());
    t.Clear();
    TEST_CHECK(t.Empty() && t.ToString() == "{}");
}

void test_equal() {
    NSet::TSet a, b;
    TEST_CHECK(a == b);

    a.Insert("1");
    TEST_CHECK(a != b);
    a.Remove("1");
    TEST_CHECK(a == b);

    a.Insert("1");
    b.Insert("1");
    TEST_CHECK(a == b);

    b.Insert("2");
    TEST_CHECK(a != b);

    a.Insert("3");
    TEST_CHECK(a != b);

    b.Clear();
    TEST_CHECK(a != b);
    b.Insert("1");
    TEST_CHECK(a != b);
    b.Insert("3");
    TEST_CHECK(a == b);

    a = {"a", "b"};
    b = {"1", "2"};
    TEST_CHECK(a != b);
}

void test_intersection() {
    NSet::TSet t1({"1", "2"});
    NSet::TSet t2({"a", "b", "2"});
    NSet::TSet t3, empty;

    t3 = t3 * t3;
    TEST_CHECK(t3 == empty);

    t3 = t1 * t2;
    TEST_CHECK(t3 == NSet::TSet({"2"}));

    t3 = t1 * empty;
    TEST_CHECK(t3 == NSet::TSet({}));

    t3 = t2 * t2;
    TEST_CHECK(t3 == t2);
}

void test_union() {
    NSet::TSet t1({"1", "2"});
    NSet::TSet t2({"a", "b", "2"});
    NSet::TSet t3, empty;

    t3 = t1 + t2;
    TEST_CHECK(t3 == NSet::TSet({"1", "2", "a", "b"}));

    t3 = t3 + empty;
    TEST_CHECK(t3 == NSet::TSet({"1", "2", "a", "b"}));
    t3 = t3 + t3;
    TEST_CHECK(t3 == NSet::TSet({"1", "2", "a", "b"}));
}

void test_substraction() {
    NSet::TSet t1({"1", "2", "a", "c"});
    NSet::TSet t2({"a", "b", "2"});
    NSet::TSet t3, empty, expect;

    t3 = t1 - t2;
    expect = {"1", "c"};
    TEST_CHECK_(t3 == expect,
                "%s == %s", t3.ToString().c_str(), expect.ToString().c_str());

    t3 = t1 - empty;
    expect = {"1", "2", "a", "c"};
    TEST_CHECK_(t3 == expect,
                "%s == %s", t3.ToString().c_str(), expect.ToString().c_str());
    t3 = t1 - t1;
    expect = {};
    TEST_CHECK_(t3 == expect,
                "%s == %s", t3.ToString().c_str(), expect.ToString().c_str());

    t3 = empty - t1;
    expect = {};
    TEST_CHECK_(t3 == expect, "%s == %s",
                t3.ToString().c_str(), expect.ToString().c_str());
}

void test_get() {
    NSet::TSet t({"1", "2", "a", "c"}), z;

    for (size_t i = 0; i < t.Size(); i++) {
        z.Insert(t.Get(i));
    }
    TEST_CHECK(z.Size() == t.Size() && z.Size() == 4);
    TEST_CHECK(z == t);

    TEST_EXCEPTION(z.Get(5), out_of_range);
    TEST_EXCEPTION(z.Get(-1), out_of_range);

    NSet::TSet empty;
    TEST_EXCEPTION(empty.Get(2), out_of_range);
}

void test_to_string() {
    NSet::TSet t({"1", "2", "a", "c"});
    NSet::TSet g({"1", "1", "2", "2", "1", "2"});
    NSet::TSet z;

    TEST_CHECK(t.ToString() == "{1, 2, a, c}");
    TEST_CHECK(g.ToString() == "{1, 2}");
    TEST_CHECK(z.ToString() == "{}");
    // do output
    stringstream o;
    t.Output(o);
    TEST_CHECK(o.str() == "{1, 2, a, c}");
}

TEST_LIST = {
    {"constructor", test_constructor},
    {"insert", test_insert},
    {"remove", test_remove},
    {"clear", test_clear},
    {"size", test_size},
    {"has", test_has},
    {"empty", test_empty},
    {"equal", test_equal},
    {"intersection", test_intersection},
    {"union", test_union},
    {"substraction", test_substraction},
    {"get", test_get},
    {"to_string", test_to_string},
    {NULL, NULL}};
