#ifndef PROC_CC
#define PROC_CC
#include "pmemory.cc"
#include "pnumber.cc"
#include <exception>
#include <sstream>

enum struct TOperation { None, Add, Sub, Mul, Div };
enum struct TFunction { Revert, Sqrt };

class TProc {
public:
  TProc(unsigned r, unsigned p) {
    leftOpAndResult = TPNumber(0, r, p);
    rightOp = TPNumber(0, r, p);
  }

  void Reset(unsigned r, unsigned p) {
    leftOpAndResult = TPNumber(0, r, p);
    rightOp = TPNumber(0, r, p);
    operation = TOperation::None;
    error.clear();
  }
  void OperationClear() { operation = TOperation::None; }
  void OperationRun() {
    std::string op = "None";
    try {
      switch (operation) {
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
    } catch (const std::exception &e) {
      error = "Operation '" + op + "' threw exception";
      if (e.what() != NULL) {
        error += " ";
        error += e.what();
      }
    }
  }
  void FunctionRun(TFunction kind) {
    std::string fn = "Unknown";
    try {
      switch (kind) {
      case TFunction::Revert:
        fn = "Revert(!)";
        rightOp = !rightOp;
        break;
      case TFunction::Sqrt:
        fn = "sqrt";
        throw std::runtime_error("TODO: sqrt");
        break;
      default:
        error = "'" + fn + "' - function not suppoerted";
      }
    } catch (const std::exception &e) {
      error = "Function '" + fn + "' threw exception";
      if (e.what() != NULL) {
        error += " ";
        error += e.what();
      }
    }
  }
  TPNumber GetLeftOp() const { return leftOpAndResult; }
  void SetLeftOp(TPNumber p) { leftOpAndResult = p; }
  TPNumber GetRightOp() const { return rightOp; }
  void SetRightOp(TPNumber p) { rightOp = p; }
  TOperation GetOperation() const { return operation; }
  void SetOperation(TOperation o) { operation = o; }
  std::string GetError() const { return error; }
  void ClearError() { error = ""; }

private:
  TPNumber leftOpAndResult = TPNumber::default_();
  TPNumber rightOp = TPNumber::default_();
  TOperation operation = TOperation::None;
  std::string error = "";
};

#ifdef RUN_TESTS
// tests
#endif // #ifdef RUN_TESTS
#endif // #ifndef PROC_CC
