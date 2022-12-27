#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <stack>
#include <string>
#include <variant>

#include "y22.h"

namespace {

using namespace std;
using Number = int64_t;

// struct Monkey {
//   string name = "";
//   string friend1 = "";
//   string friend2 = "";
//   std::function<Number && (Number, Number)> op;
// };
//
// Number&& add(Number a, Number b) { return a + b; }
// Number&& subtract(Number a, Number b) { return a - b; }
// Number&& multiply(Number a, Number b) { return a * b; }
// Number&& divide(Number a, Number b) { return a / b; }
//
// void parseWrong(istream& in, list<Monkey>& monkeys,
//                 map<string, Number>& numbers)
//{
//   string line;
//   Number number;
//   char op;
//   while (getline(in, line)) {
//     stringstream ss(line);
//     Monkey m;
//     ss >> m.name;
//     m.name.erase(m.name.end() - 1, m.name.end());
//     if (ss >> number) {
//       numbers[m.name] = number;
//     } else {
//       ss.clear();
//       ss >> m.friend1;
//       ss >> op;
//       ss >> m.friend2;
//       switch (op) {
//         case '+':
//           m.op = add;
//           break;
//         case '-':
//           m.op = subtract;
//           break;
//         case '*':
//           m.op = multiply;
//           break;
//         case '/':
//           m.op = divide;
//           break;
//       }
//       monkeys.push_back(m);
//     }
//   }
// }
//
// enum class Operation { kAdd, kSubtract, kMultiply, kDivide };
//
// char toString(Operation op)
//{
//   switch (op) {
//     case Operation::kAdd:
//       return '+';
//     case Operation::kSubtract:
//       return '-';
//     case Operation::kMultiply:
//       return '*';
//     case Operation::kDivide:
//       return '/';
//   }
// }
//
// struct Token {
//   string arg1;
//   Operation op;
//   string arg2;
// };
//
// void parseCorrect(istream& in, map<string, variant<Token, Number>>& tokens)
//{
//   string line;
//   Number number;
//   char op;
//   while (getline(in, line)) {
//     stringstream ss(line);
//     string name;
//     ss >> name;
//     name.erase(name.end() - 1, name.end());
//     if (ss >> number) {
//       tokens[name] = number;
//     } else {
//       Token t;
//       ss.clear();
//       ss >> t.arg1;
//       ss >> op;
//       ss >> t.arg2;
//       switch (op) {
//         case '+':
//           t.op = Operation::kAdd;
//           break;
//         case '-':
//           t.op = Operation::kSubtract;
//           break;
//         case '*':
//           t.op = Operation::kMultiply;
//           break;
//         case '/':
//           t.op = Operation::kDivide;
//           break;
//       }
//       tokens[name] = t;
//     }
//   }
// }

inline std::string part1(std::ifstream& in)
{
  // list<Monkey> monkeys;
  // map<string, Number> numbers;
  // parseWrong(in, monkeys, numbers);
  // Number result = 0;

  // auto mit = monkeys.begin();
  // while (mit != monkeys.end()) {
  //   if (numbers.contains(mit->friend1) && numbers.contains(mit->friend2)) {
  //     numbers[mit->name] =
  //         mit->op(numbers[mit->friend1], numbers[mit->friend2]);
  //     if (mit->name == "root") {
  //       result = numbers[mit->name];
  //       break;
  //     }
  //     mit = monkeys.erase(mit);
  //   } else {
  //     mit++;
  //   }

  //  if (mit == monkeys.end()) {
  //    mit = monkeys.begin();
  //  }
  //}
  // return std::to_string(result);
  return "";
}

// struct TokenVisitor {
//   TokenVisitor(map<string, variant<Token, Number>>& map) : map_(map) {}
//
//   void operator()(Token token)
//   {
//     auto arg1 = map_[token.arg1];
//     auto arg2 = map_[token.arg2];
//
//     cout << "( ";
//     // cout << "[" << token.arg1 << "]";
//     if (token.arg1 == "humn") {
//       cout << "X";
//     } else {
//       std::visit(TokenVisitor(map_), arg1);
//     }
//     cout << " ) ";
//     cout << toString(token.op);
//     cout << " ( ";
//     // cout << "[" << token.arg2 << "]";
//     if (token.arg2 == "humn") {
//       cout << "X";
//     } else {
//       std::visit(TokenVisitor(map_), arg2);
//     }
//     cout << " )";
//   }
//   void operator()(Number number) { std::cout << number; }
//
//   map<string, variant<Token, Number>>& map_;
// };
//
// ostream& operator<<(ostream& out, map<string, variant<Token, Number>> tokens)
//{
//   // stack<Token> current;
//   // current.push(tokens["root"].);
//   Token root = get<Token>(tokens["root"]);
//   std::visit(TokenVisitor(tokens), tokens[root.arg1]);
//   out << endl;
//   out << endl;
//   std::visit(TokenVisitor(tokens), tokens[root.arg2]);
//   return out;
// }
//
// bool canConsume(map<string, variant<Token, Number>>& tokens, string token,
//                 Operation op);
//
// void consume(map<string, variant<Token, Number>>& tokens, string token,
//              Operation op, Number arg)
//{
//   auto current = tokens[token];
//   if (token == "humn") {
//     return;
//   }
//   if (holds_alternative<Number>(current)) {
//     Number n = get<Number>(current);
//     switch (op) {
//       case Operation::kAdd:
//         n += arg;
//         break;
//       case Operation::kSubtract:
//         n -= arg;
//         break;
//       case Operation::kMultiply:
//         n *= arg;
//         break;
//       case Operation::kDivide:
//         n /= arg;
//         break;
//     }
//     return;
//   } else {
//     Token t = get<Token>(current);
//     if (canConsume(tokens, t.arg1, t.op)) {
//       consume(tokens, t.arg1, t.op, arg);
//     } else if (canConsume(tokens, t.arg2, t.op)) {
//       consume(tokens, t.arg2, t.op, arg);
//     }
//   }
// }
//
// bool canConsume(map<string, variant<Token, Number>>& tokens, string token,
//                 Operation op)
//{
//   auto current = tokens[token];
//   if (token == "humn") {
//     return false;
//   }
//   if (holds_alternative<Number>(current)) {
//     return true;
//   } else {
//     Token t = get<Token>(current);
//     if (t.op == op && (op == Operation::kAdd || op == Operation::kMultiply))
//     {
//       return canConsume(tokens, t.arg1, op) || canConsume(tokens, t.arg2,
//       op);
//     }
//     return false;
//   }
// }
//
// bool reduce(map<string, variant<Token, Number>>& tokens, string token)
//{
//   auto current = tokens[token];
//   if (token == "humn") {
//     return false;
//   }
//   if (holds_alternative<Number>(current)) {
//     return true;
//   } else {
//     Token t = get<Token>(current);
//     bool r1 = reduce(tokens, t.arg1);
//     bool r2 = reduce(tokens, t.arg2);
//     if (r1 && r2) {
//       Number n;
//       switch (t.op) {
//         case Operation::kAdd:
//           n = get<Number>(tokens[t.arg1]) + get<Number>(tokens[t.arg2]);
//           break;
//         case Operation::kSubtract:
//           n = get<Number>(tokens[t.arg1]) - get<Number>(tokens[t.arg2]);
//           break;
//         case Operation::kMultiply:
//           n = get<Number>(tokens[t.arg1]) * get<Number>(tokens[t.arg2]);
//           break;
//         case Operation::kDivide:
//           n = get<Number>(tokens[t.arg1]) / get<Number>(tokens[t.arg2]);
//           break;
//       }
//       tokens[token] = n;
//       return true;
//     } else if (r1) {
//       //if (canConsume(tokens, t.arg2, t.op)) {
//       //  consume(tokens, t.arg2, t.op, get<Number>(tokens[t.arg1]));
//       //}
//       return false;
//     } else if (r2) {
//       //if (canConsume(tokens, t.arg1, t.op)) {
//       //  consume(tokens, t.arg1, t.op, get<Number>(tokens[t.arg2]));
//       //}
//       return false;
//     } else {
//       return false;
//     }
//   }
// }

// class Operation;
//
//// struct ValueVisitor {
////   optional<Number> operator()(shared_ptr<Operation> op) const
////   {
////     return op->result();
////   }
////
////   optional<Number> operator()(Number n) const { return n; }
////
////   optional<Number> operator()(Variable v) const { return nullopt; }
//// };
//
// struct Variable {};
//
// class Operation {
//  struct ValueVisitor {
//    optional<Number> operator()(shared_ptr<Operation> op) const
//    {
//      return op->result();
//    }
//
//    optional<Number> operator()(Number n) const { return n; }
//
//    optional<Number> operator()(Variable v) const { return nullopt; }
//  };
//  using Argument = variant<shared_ptr<Operation>, Number, Variable>;
//
// public:
//  enum class Type { kAdd, kSubtract, kMultiply, kDivide };
//
//  array<Argument, 2> arguments;
//  Type operation;
//
//  optional<Number> result() const
//  {
//    if (auto a = std::visit(ValueVisitor{}, arguments[0])) {
//      if (auto b = std::visit(ValueVisitor{}, arguments[1])) {
//        switch (operation) {
//          case Type::kAdd:
//            return *a + *b;
//            break;
//          case Type::kSubtract:
//            return *a - *b;
//            break;
//          case Type::kMultiply:
//            return *a * *b;
//            break;
//          case Type::kDivide:
//            return *a / *b;
//            break;
//        }
//      }
//    }
//    return nullopt;
//  }
//
// public:
//  void simplify(Operation& parent)
//  {
//    for (int i )
//    if (auto arg = get_if<shared_ptr<Operation>>(&(arguments[0]))) {
//      if (auto r = (*arg)->result()) {
//        parent.arguments[0] = *r;
//        (*arg)->simplify(*this);
//      } else {
//        (*arg)->simplify(*this);
//      }
//    }
//  }
//
//  template <size_t N>
//  void integrate(Operation& parent)
//  {
//    constexpr size_t self = N;
//    constexpr size_t other = (N + 1) % 2;
//
//    for (size_t i = 0; arguments.size(); ++i) {
//      if (auto arg = get_if<shared_ptr<Operation>>(&(arguments[i]))) {
//        (*arg)->integrate<i>(*this);
//      }
//    }
//
//    Number* numeric_arg;
//    Argument* other_arg;
//    if (holds_alternative<Number>(arguments[0])) {
//      numeric_arg = &get<Number>(arguments[0]);
//      other_arg = arguments[1];
//    } else if (holds_alternative<Number>(arguments[1])) {
//      other_arg = arguments[0];
//      numeric_arg = &get<Number>(arguments[1]);
//    } else {
//      return;
//    }
//
//    // Get the other argument from parent if its numeric
//    if (auto parent_numeric_arg = get_if<Number>(&(parent.arguments[other])))
//    {
//      if (parent.operation == Type::kAdd) {
//        if (operation == Type.kAdd) {
//          parent.arguments[other] = *parent_numeric_arg + *numeric_arg;
//          parent.arguments[self] = *other_arg;
//        } else if (operation == Type::kSubtract) {
//          parent.arguments[other] = *parent_numeric_arg + *numeric_arg;
//          parent.operation = Type::kSubtract;
//          parent.arguments[self] = *other_arg;
//        }
//      } else if (parent.operation == Type::kSubtract) {
//        if (operation == Type.kAdd) {
//          parent.arguments[other] = *parent_numeric_arg - *numeric_arg;
//          parent.arguments[self] = *other_arg;
//        } else if (operation == Type::kSubtract) {
//          parent.arguments[other] = *parent_numeric_arg - *numeric_arg;
//          parent.operation = Type::kAdd;
//          parent.arguments[self] = *other_arg;
//        }
//      } else if (parent.operation == Type::kMultiply) {
//        if (operation == Type::kMultiply) {
//          parent.arguments[other] = *parent_numeric_arg * *numeric_arg;
//          parent.arguments[self] = *other_arg;
//        } else if (operation == Type::kDivide) {
//          parent.arguments[other] = *parent_numeric_arg * *numeric_arg;
//          parent.operation = Type::kDivide;
//          parent.arguments[self] = *other_arg;
//        }
//      } else if (parent.operation == Type::kDivide) {
//        if (operation == Type::kMultiply) {
//          parent.arguments[other] = *parent_numeric_arg / *numeric_arg;
//          parent.arguments[self] = *other_arg;
//        } else if (operation == Type::kDivide) {
//          parent.arguments[other] = *parent_numeric_arg / *numeric_arg;
//          parent.operation = Type::kMultiply;
//          parent.arguments[self] = *other_arg;
//        }
//      }
//    }
//  }
//};

inline std::string part2(std::ifstream& in)
{
  // Operation root;

  // auto tmp = make_shared<Operation>();
  // tmp->arguments[0] = 1;
  // tmp->operation = Operation::Type::kAdd;
  // tmp->arguments[1] = 5;

  // root.arguments[0] = tmp;

  // tmp = make_shared<Operation>();
  // tmp->arguments[0] = 3;
  // tmp->operation = Operation::Type::kMultiply;
  // tmp->arguments[1] = 4;

  // root.arguments[1] = tmp;
  // root.operation = Operation::Type::kAdd;

  // cout << *root.result() << endl;
  // root.simplify<0>(root);
  // root.simplify<1>(root);
  // cout << *root.result() << endl;
  //// list<Monkey> monkeys;
  // map<string, variant<Token, Number>> tokens;
  // parseCorrect(in, tokens);
  //// cout << tokens << endl;

  // Token root = get<Token>(tokens["root"]);

  // stack<Token> current;
  // cout << tokens << endl;
  // reduce(tokens, root.arg1);
  // reduce(tokens, root.arg2);
  // cout << tokens << endl;

  // Number result = 0;
  return "";
}

}  // namespace

string y22day21(ifstream& in, int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}