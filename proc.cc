#ifndef PROC_CC
#define PROC_CC
#include "pmemory.cc"
#include "pnumber.cc"
#include <exception>
#include <sstream>

namespace NProc {
    using TPNumber = NPNumber::TPNumber;

    enum struct TOperation { None,
                             Add,
                             Sub,
                             Mul,
                             Div };
    enum struct TFunction { Revert,
                            Sqr };

    class TProc {
    public:
        TProc(int r, int p) {
            Reset(r, p);
        }

        void Reset(int r, int p) {
            leftOpAndResult = TPNumber(0, r, p);
            rightOp = TPNumber(0, r, p);
            operation = TOperation::None;
            error.clear();
        }
        TPNumber GetLeftOpRes() const {
            return leftOpAndResult;
        }
        void SetLeftOp(TPNumber p) {
            leftOpAndResult = p;
        }
        TPNumber GetRightOp() const {
            return rightOp;
        }
        void SetRightOp(TPNumber p) {
            rightOp = p;
        }
        void FunctionRun(TFunction kind) {
            std::string fn = "Unknown";
            try {
                switch (kind) {
                    case TFunction::Revert:
                        fn = "Revert(!)";
                        rightOp = !rightOp;
                        break;
                    case TFunction::Sqr:
                        fn = "Sqr";
                        rightOp = rightOp.Sqr();
                        break;
                    default:
                        error = "'" + fn + "' - function not suppoerted";
                }
            } catch (const NPNumber::division_by_zero&) {
                error = "Division by zero";
            } catch (const std::exception& e) {
                error = "Function '" + fn + "' threw exception";
                if (e.what() != NULL) {
                    error += " ";
                    error += e.what();
                }
            }
        }
        TOperation GetOperation() const {
            return operation;
        }
        void SetOperation(TOperation o) {
            operation = o;
        }
        void OperationClear() {
            operation = TOperation::None;
        }
        void OperationRun() {
            std::string op = "None";
            try {
                switch (operation) {
                    case TOperation::None:
                        break;
                    case TOperation::Add:
                        op = "+";
                        leftOpAndResult += rightOp;
                        break;
                    case TOperation::Sub:
                        op = "-";
                        leftOpAndResult -= rightOp;
                        break;
                    case TOperation::Mul:
                        op = "*";
                        leftOpAndResult *= rightOp;
                        break;
                    case TOperation::Div:
                        op = "/";
                        leftOpAndResult /= rightOp;
                        break;
                    default:
                        error = "Unsupported operation: '" + op + "'";
                }
            } catch (const NPNumber::division_by_zero&) {
                error = "Division by zero";
            } catch (const std::exception& e) {
                error = "Operation '" + op + "' threw exception";
                if (e.what() != NULL) {
                    error += " ";
                    error += e.what();
                }
            }
        }
        std::string GetError() const {
            return error;
        }
        void ClearError() {
            error = "";
        }

    private:
        TPNumber leftOpAndResult;
        TPNumber rightOp;
        TOperation operation = TOperation::None;
        std::string error = "";
    };
}; // namespace NProc

#ifdef RUN_TESTS
void test_proc_construction() {
    using namespace NProc;
    using TPNumber = NPNumber::TPNumber;

    TEST_CASE("Constructor & Reset");
    {
        TEST_EXCEPTION(TProc(-2, 7), NPNumber::invalid_radix);
        TEST_EXCEPTION(TProc(2, -7), NPNumber::invalid_precision);
        TProc p1 = TProc(2, 7);
        TProc p2 = TProc(3, 3);
        TEST_CHECK(p2.GetError() == "");
        p2.Reset(2, 7); // reset to a state equal to p1

        TEST_CHECK(p1.GetLeftOpRes().GetRadix() == 2);
        TEST_CHECK(p1.GetLeftOpRes().Repr() == p2.GetLeftOpRes().Repr());
        TEST_CHECK(p1.GetLeftOpRes().GetPrecision() == 7);
        TEST_CHECK(p1.GetRightOp().Repr() == p2.GetRightOp().Repr());
        TEST_CHECK(p1.GetOperation() == TOperation::None);
        TEST_CHECK(p1.GetOperation() == p2.GetOperation());
        TEST_CHECK(p1.GetError() == "");
        TEST_CHECK(p1.GetError() == p2.GetError());
    }
    TEST_CASE("Get/Set Operands");
    {
        TProc p = TProc(2, 0);
        TEST_CHECK(p.GetLeftOpRes().GetNumber() == 0);
        TEST_CHECK(p.GetRightOp().GetNumber() == 0);
        p.SetLeftOp(TPNumber(3, 14, 15));
        TEST_CHECK(p.GetLeftOpRes().GetNumber() == 3);
        TEST_CHECK(p.GetLeftOpRes().GetRadix() == 14);
        TEST_CHECK(p.GetLeftOpRes().GetPrecision() == 15);
        p.SetRightOp(TPNumber(1, 6, 18));
        TEST_CHECK(p.GetRightOp().GetNumber() == 1);
        TEST_CHECK(p.GetRightOp().GetRadix() == 6);
        TEST_CHECK(p.GetRightOp().GetPrecision() == 18);
    }
}
void test_proc_functions() {
    using namespace NProc;
    using TPNumber = NPNumber::TPNumber;

    TEST_CASE("Functions");
    {
        TProc p = TProc(2, 2);
        p.SetLeftOp(TPNumber(42.55, 14, 8));
        string leftOpStr = p.GetLeftOpRes().ToString();
        TEST_CHECK_(leftOpStr == "30.79B2B2B3", "%s != 30.79B2B2B3", leftOpStr.c_str());

        p.SetRightOp(TPNumber(4, 2, 2));
        TEST_CASE("Revert");
        p.FunctionRun(TFunction::Revert);
        TEST_CHECK(p.GetRightOp().GetNumber() == 0.25);
        TEST_CHECK(p.GetLeftOpRes().ToString() == "30.79B2B2B3"); // unchanged
        p.FunctionRun(TFunction::Revert);
        TEST_CHECK(p.GetRightOp().GetNumber() == 4);
        TEST_CHECK(p.GetLeftOpRes().ToString() == "30.79B2B2B3"); // unchanged

        TEST_CASE("Sqr");
        p.FunctionRun(TFunction::Sqr); // 4^2
        TEST_CHECK(p.GetRightOp().GetNumber() == 16);
        p.SetRightOp(TPNumber(-0.25, 2, 2));
        p.FunctionRun(TFunction::Sqr);
        TEST_CHECK(p.GetRightOp().GetNumber() == 0.0625); // -0.25^2

        TEST_CASE("Reverse");
        p.FunctionRun(TFunction::Revert);
        TEST_CHECK(p.GetRightOp().GetNumber() == 16);
        p.SetRightOp(TPNumber(0, 2, 2));
        p.FunctionRun(TFunction::Revert);
        if (not TEST_CHECK(p.GetError().find("Division by zero") != string::npos)) {
            if (p.GetError() == "") {
                TEST_MSG("No error");
            } else {
                TEST_MSG("Unexpected error: %s", p.GetError().c_str());
            }
        }
        p.SetRightOp(TPNumber(numeric_limits<double>::infinity(), 2, 2));
        p.FunctionRun(TFunction::Sqr);
        TEST_CHECK(p.GetRightOp().ToString() == "inf"); // inf^2
    }
}
void test_proc_operations() {
    using namespace NProc;
    using TPNumber = NPNumber::TPNumber;
    TEST_CASE("Get/Set Operations");
    {
        TEST_CASE("Get/Set");
        TProc p = TProc(4, 4);
        TEST_CHECK(p.GetOperation() == TOperation::None);
        p.SetOperation(TOperation::Add);
        TEST_CHECK(p.GetOperation() == TOperation::Add);

        TEST_CASE("Operation None NOP");
        p.SetLeftOp(TPNumber(8, 2, 2));
        p.SetRightOp(TPNumber(8, 2, 2));
        p.OperationClear();
        TEST_CHECK(p.GetOperation() == TOperation::None);
        p.OperationRun();
        TEST_CHECK(p.GetRightOp().Repr() == "TPNumber(8.00, 2, 2)");
        TEST_CHECK(p.GetRightOp().Repr() == p.GetLeftOpRes().Repr());

        TEST_CASE("Operation Add");
        p.SetOperation(TOperation::Add);
        p.OperationRun();
        TEST_CHECK(p.GetLeftOpRes().Repr() == "TPNumber(16.00, 2, 2)");
        TEST_CHECK(p.GetRightOp().Repr() == "TPNumber(8.00, 2, 2)");

        TEST_CASE("Operation Sub");
        p.SetOperation(TOperation::Sub);
        p.OperationRun();
        TEST_CHECK(p.GetRightOp().Repr() == p.GetLeftOpRes().Repr());
        TEST_CHECK(p.GetRightOp().Repr() == "TPNumber(8.00, 2, 2)");

        TEST_CASE("Operation Mul");
        p.SetOperation(TOperation::Mul);
        p.OperationRun();
        TEST_CHECK(p.GetLeftOpRes().Repr() == "TPNumber(64.00, 2, 2)");
        TEST_CHECK(p.GetRightOp().Repr() == "TPNumber(8.00, 2, 2)");

        TEST_CASE("Operation Div");
        p.SetOperation(TOperation::Div);
        p.OperationRun();
        TEST_CHECK(p.GetLeftOpRes().Repr() == "TPNumber(8.00, 2, 2)");
        TEST_CHECK(p.GetRightOp().Repr() == "TPNumber(8.00, 2, 2)");
        p.OperationRun();
        TEST_CHECK(p.GetLeftOpRes().Repr() == "TPNumber(1.00, 2, 2)");
        TEST_CHECK(p.GetRightOp().Repr() == "TPNumber(8.00, 2, 2)");
        p.OperationRun();
        TEST_CHECK(p.GetLeftOpRes().Repr() == "TPNumber(0.12, 2, 2)");
        TEST_CHECK(p.GetRightOp().Repr() == "TPNumber(8.00, 2, 2)");
        p.SetRightOp(p.GetLeftOpRes());
        p.FunctionRun(TFunction::Revert);
        TEST_CHECK(p.GetRightOp().Repr() == "TPNumber(8.00, 2, 2)");

        p.SetRightOp(TPNumber(0, 2, 2));
        p.OperationRun();
        if (not TEST_CHECK(p.GetError().find("Division by zero") != string::npos)) {
            if (p.GetError() == "") {
                TEST_MSG("No error");
            } else {
                TEST_MSG("Unexpected error: %s", p.GetError().c_str());
            }
        }
        p.ClearError();
        TEST_CHECK(p.GetError() == "");
    }
}

#endif // #ifdef RUN_TESTS
#endif // #ifndef PROC_CC
