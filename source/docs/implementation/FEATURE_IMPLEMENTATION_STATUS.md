# # #pragma once
#include <memory>
#include <string>
#include <vector>
#include "token.hpp"

namespace bas {

struct Expr {
  virtual ~Expr() = default;
};

struct Literal : Expr {
  Token tok; explicit Literal(Token t): tok(std::move(t)) {}
};

struct Variable : Expr {
  std::string name; explicit Variable(std::string n): name(std::move(n)) {}
};

struct Unary : Expr {
  Tok op; std::unique_ptr<Expr> right;
  Unary(Tok o, std::unique_ptr<Expr> r): op(o), right(std::move(r)) {}
};

struct Binary : Expr {
  std::unique_ptr<Expr> left, right; Tok op;
  Binary(std::unique_ptr<Expr> l, Tok o, std::unique_ptr<Expr> r)
    : left(std::move(l)), right(std::move(r)), op(o) {}
};

// Named parameter in function calls
struct NamedArg {
  std::string name;
  std::unique_ptr<Expr> value;
};

struct Call : Expr {
  std::string callee; 
  std::vector<std::unique_ptr<Expr>> args;
  std::vector<NamedArg> namedArgs; // Named parameters
  Call(std::string c, std::vector<std::unique_ptr<Expr>> a)
    : callee(std::move(c)), args(std::move(a)) {}
};

struct Index : Expr {
  std::unique_ptr<Expr> target;
  std::unique_ptr<Expr> index;
  Index(std::unique_ptr<Expr> t, std::unique_ptr<Expr> i)
    : target(std::move(t)), index(std::move(i)) {}
};

struct MemberAccess : Expr {
  std::unique_ptr<Expr> object;
  std::string member;
  MemberAccess(std::unique_ptr<Expr> obj, std::string mem)
    : object(std::move(obj)), member(std::move(mem)) {}
};

struct MethodCall : Expr {
  std::unique_ptr<Expr> object;
  std::string method;
  std::vector<std::unique_ptr<Expr>> args;
  MethodCall(std::unique_ptr<Expr> obj, std::string m, std::vector<std::unique_ptr<Expr>> a)
    : object(std::move(obj)), method(std::move(m)), args(std::move(a)) {}
};

struct ArrayLiteral : Expr {
  std::vector<std::unique_ptr<Expr>> elements;
};

struct Stmt { virtual ~Stmt() = default; };
// OPTION EXPLICIT directive (enable strict undeclared-variable checks)
struct OptionExplicit : Stmt { bool enabled{true}; };
struct Let : Stmt { 
  std::string name; 
  std::unique_ptr<Expr> value;
  std::string typeName; // Optional type annotation
  bool hasType{false};
};
struct ConstDecl : Stmt { std::string name; std::unique_ptr<Expr> value; };
struct Assign : Stmt { std::string name; std::unique_ptr<Expr> value; };
struct LocalDecl : Stmt { std::vector<std::string> names; };
struct GlobalDecl : Stmt { std::vector<std::string> names; };
struct Print : Stmt { std::unique_ptr<Expr> value; };
struct ExprStmt : Stmt { std::unique_ptr<Expr> expr; };
struct CallStmt : Stmt { std::string name; std::vector<std::unique_ptr<Expr>> args; };
struct Break : Stmt {}; // Simple break statement
struct Continue : Stmt {}; // Continue current loop
// IMPORT "file.bas"
struct ImportStmt : Stmt { std::string path; };
struct IfChain : Stmt {
  struct Branch { std::unique_ptr<Expr> cond; std::vector<std::unique_ptr<Stmt>> body; };
  std::vector<Branch> branches; // evaluated in order
  std::vector<std::unique_ptr<Stmt>> elseBody; // optional, empty when absent
  bool hasElse{false};
};

// Simple IF-THEN-ENDIF structure
struct IfThenEndIf : Stmt { 
  std::unique_ptr<Expr> cond; 
  std::vector<std::unique_ptr<Stmt>> body; 
};

struct WhileWend : Stmt { std::unique_ptr<Expr> cond; std::vector<std::unique_ptr<Stmt>> body; };

// DO ... LOOP (infinite loop, exit via BREAK)
struct DoLoop : Stmt { std::vector<std::unique_ptr<Stmt>> body; };

// REPEAT ... UNTIL <cond> (post-test loop; executes body, then exits when cond is true)
struct RepeatUntil : Stmt { std::vector<std::unique_ptr<Stmt>> body; std::unique_ptr<Expr> cond; };

struct ForNext : Stmt {
  std::string var;
  std::unique_ptr<Expr> init;
  std::unique_ptr<Expr> limit;
  std::unique_ptr<Expr> step; // may be null -> default 1
  std::vector<std::unique_ptr<Stmt>> body;
};

struct SubDecl : Stmt {
  std::string name;
  std::vector<std::string> params;
  std::vector<std::unique_ptr<Stmt>> body;
};

// Forward declaration for FunctionParam
struct FunctionParam {
  std::string name;
  std::string typeName; // Optional type annotation
  bool hasType{false};
  std::unique_ptr<Expr> defaultValue; // Optional default value
  bool hasDefault{false};
};

// Enhanced function declaration with return types and optional parameters
struct FunctionDecl : Stmt {
  std::string name;
  std::vector<FunctionParam> params; // Enhanced with types and defaults
  std::string returnType; // Optional return type
  bool hasReturnType{false};
  std::vector<std::unique_ptr<Stmt>> body;
};

struct AssignIndex : Stmt { std::string name; std::vector<std::unique_ptr<Expr>> indices; std::unique_ptr<Expr> value; };
struct AssignMember : Stmt { std::unique_ptr<Expr> object; std::string member; std::unique_ptr<Expr> value; };

struct Return : Stmt { std::unique_ptr<Expr> value; };

// DIM declaration: DIM name(s1[, s2, ...])
struct Dim : Stmt { std::string name; std::vector<std::unique_ptr<Expr>> sizes; };

// REDIM declaration: REDIM [PRESERVE] name(s1[, s2, ...])
struct Redim : Stmt {
  std::string name;
  std::vector<std::unique_ptr<Expr>> sizes;
  bool preserve{false};
};

struct CaseBranch {
  bool isElse{false};
  std::vector<std::unique_ptr<Expr>> values; // empty when isElse
  std::vector<std::unique_ptr<Stmt>> body;
  // Optional relational form: CASE IS <op> expr
  bool isRel{false};
  Tok relOp{Tok::Eq};
  std::unique_ptr<Expr> relExpr;
};

struct SelectCaseStmt : Stmt {
  std::unique_ptr<Expr> selector;
  std::vector<CaseBranch> branches; // evaluated in order; first match wins
};

// User-Defined Type (UDT)
struct TypeField {
  std::string name;
  std::string typeName; // Optional type annotation
  bool hasType{false};
};

struct TypeDecl : Stmt {
  std::string name;
  std::string parentType; // For inheritance (EXTENDS)
  bool hasParent{false};
  std::vector<TypeField> fields;
  std::vector<std::unique_ptr<Stmt>> methods; // Functions/subroutines within type
};

// Type annotation for variables and function parameters
struct TypeAnnotation {
  std::string typeName;
  bool hasType{false};
};

// Lambda/Anonymous function
struct LambdaExpr : Expr {
  std::vector<FunctionParam> params;
  std::string returnType;
  bool hasReturnType{false};
  std::vector<std::unique_ptr<Stmt>> body;
};


// For-Each loop
struct ForEach : Stmt {
  std::string var;
  std::unique_ptr<Expr> collection;
  std::vector<std::unique_ptr<Stmt>> body;
};

// String interpolation expression
struct InterpolatedString : Expr {
  std::vector<std::unique_ptr<Expr>> parts; // String literals and expressions
};

// Dictionary/Map literal
struct MapLiteral : Expr {
  std::vector<std::pair<std::unique_ptr<Expr>, std::unique_ptr<Expr>>> entries; // key-value pairs
};

// Module declaration
struct ModuleDecl : Stmt {
  std::string name;
  bool isPublic{true};
  std::vector<std::unique_ptr<Stmt>> body;
};

// Operator overloading
struct OperatorDecl : Stmt {
  Tok op;
  std::string typeName;
  std::vector<FunctionParam> params;
  std::vector<std::unique_ptr<Stmt>> body;
};

// Coroutine/Yield
struct YieldStmt : Stmt {
  std::unique_ptr<Expr> value; // Optional value to yield
};

// Event handlers
struct EventHandler : Stmt {
  std::string eventType; // "KEY", "MOUSE", etc.
  std::string eventName; // "PRESSED", "CLICKED", etc.
  std::vector<std::unique_ptr<Stmt>> body;
};

// Debug statements
struct AssertStmt : Stmt {
  std::unique_ptr<Expr> condition;
  std::unique_ptr<Expr> message; // Optional error message
};

struct BreakpointStmt : Stmt {};

struct DebugPrintStmt : Stmt {
  std::unique_ptr<Expr> value;
};

// Reflection
struct TypeOfExpr : Expr {
  std::unique_ptr<Expr> value;
};

struct GetPropertiesExpr : Expr {
  std::unique_ptr<Expr> value;
};

struct GetMethodsExpr : Expr {
  std::unique_ptr<Expr> value;
};

// Super call (for inheritance)
struct SuperCall : Expr {
  std::string method;
  std::vector<std::unique_ptr<Expr>> args;
};

struct Program { std::vector<std::unique_ptr<Stmt>> stmts; };

} // namespace bas
namespace bas {

struct Expr {
  virtual ~Expr() = default;
};

struct Literal : Expr {
  Token tok; explicit Literal(Token t): tok(std::move(t)) {}
};

struct Variable : Expr {
  std::string name; explicit Variable(std::string n): name(std::move(n)) {}
};

struct Unary : Expr {
  Tok op; std::unique_ptr<Expr> right;
  Unary(Tok o, std::unique_ptr<Expr> r): op(o), right(std::move(r)) {}
};

struct Binary : Expr {
  std::unique_ptr<Expr> left, right; Tok op;
  Binary(std::unique_ptr<Expr> l, Tok o, std::unique_ptr<Expr> r)
    : left(std::move(l)), right(std::move(r)), op(o) {}
};

// Named parameter in function calls
struct NamedArg {
  std::string name;
  std::unique_ptr<Expr> value;
};

struct Call : Expr {
  std::string callee; 
  std::vector<std::unique_ptr<Expr>> args;
  std::vector<NamedArg> namedArgs; // Named parameters
  Call(std::string c, std::vector<std::unique_ptr<Expr>> a)
    : callee(std::move(c)), args(std::move(a)) {}
};

struct Index : Expr {
  std::unique_ptr<Expr> target;
  std::unique_ptr<Expr> index;
  Index(std::unique_ptr<Expr> t, std::unique_ptr<Expr> i)
    : target(std::move(t)), index(std::move(i)) {}
};

struct MemberAccess : Expr {
  std::unique_ptr<Expr> object;
  std::string member;
  MemberAccess(std::unique_ptr<Expr> obj, std::string mem)
    : object(std::move(obj)), member(std::move(mem)) {}
};

struct MethodCall : Expr {
  std::unique_ptr<Expr> object;
  std::string method;
  std::vector<std::unique_ptr<Expr>> args;
  MethodCall(std::unique_ptr<Expr> obj, std::string m, std::vector<std::unique_ptr<Expr>> a)
    : object(std::move(obj)), method(std::move(m)), args(std::move(a)) {}
};

struct ArrayLiteral : Expr {
  std::vector<std::unique_ptr<Expr>> elements;
};

struct Stmt { virtual ~Stmt() = default; };
// OPTION EXPLICIT directive (enable strict undeclared-variable checks)
struct OptionExplicit : Stmt { bool enabled{true}; };
struct Let : Stmt { 
  std::string name; 
  std::unique_ptr<Expr> value;
  std::string typeName; // Optional type annotation
  bool hasType{false};
};
struct ConstDecl : Stmt { std::string name; std::unique_ptr<Expr> value; };
struct Assign : Stmt { std::string name; std::unique_ptr<Expr> value; };
struct LocalDecl : Stmt { std::vector<std::string> names; };
struct GlobalDecl : Stmt { std::vector<std::string> names; };
struct Print : Stmt { std::unique_ptr<Expr> value; };
struct ExprStmt : Stmt { std::unique_ptr<Expr> expr; };
struct CallStmt : Stmt { std::string name; std::vector<std::unique_ptr<Expr>> args; };
struct Break : Stmt {}; // Simple break statement
struct Continue : Stmt {}; // Continue current loop
// IMPORT "file.bas"
struct ImportStmt : Stmt { std::string path; };
struct IfChain : Stmt {
  struct Branch { std::unique_ptr<Expr> cond; std::vector<std::unique_ptr<Stmt>> body; };
  std::vector<Branch> branches; // evaluated in order
  std::vector<std::unique_ptr<Stmt>> elseBody; // optional, empty when absent
  bool hasElse{false};
};

// Simple IF-THEN-ENDIF structure
struct IfThenEndIf : Stmt { 
  std::unique_ptr<Expr> cond; 
  std::vector<std::unique_ptr<Stmt>> body; 
};

struct WhileWend : Stmt { std::unique_ptr<Expr> cond; std::vector<std::unique_ptr<Stmt>> body; };

// DO ... LOOP (infinite loop, exit via BREAK)
struct DoLoop : Stmt { std::vector<std::unique_ptr<Stmt>> body; };

// REPEAT ... UNTIL <cond> (post-test loop; executes body, then exits when cond is true)
struct RepeatUntil : Stmt { std::vector<std::unique_ptr<Stmt>> body; std::unique_ptr<Expr> cond; };

struct ForNext : Stmt {
  std::string var;
  std::unique_ptr<Expr> init;
  std::unique_ptr<Expr> limit;
  std::unique_ptr<Expr> step; // may be null -> default 1
  std::vector<std::unique_ptr<Stmt>> body;
};

struct SubDecl : Stmt {
  std::string name;
  std::vector<std::string> params;
  std::vector<std::unique_ptr<Stmt>> body;
};

// Forward declaration for FunctionParam
struct FunctionParam {
  std::string name;
  std::string typeName; // Optional type annotation
  bool hasType{false};
  std::unique_ptr<Expr> defaultValue; // Optional default value
  bool hasDefault{false};
};

// Enhanced function declaration with return types and optional parameters
struct FunctionDecl : Stmt {
  std::string name;
  std::vector<FunctionParam> params; // Enhanced with types and defaults
  std::string returnType; // Optional return type
  bool hasReturnType{false};
  std::vector<std::unique_ptr<Stmt>> body;
};

struct AssignIndex : Stmt { std::string name; std::vector<std::unique_ptr<Expr>> indices; std::unique_ptr<Expr> value; };
struct AssignMember : Stmt { std::unique_ptr<Expr> object; std::string member; std::unique_ptr<Expr> value; };

struct Return : Stmt { std::unique_ptr<Expr> value; };

// DIM declaration: DIM name(s1[, s2, ...])
struct Dim : Stmt { std::string name; std::vector<std::unique_ptr<Expr>> sizes; };

// REDIM declaration: REDIM [PRESERVE] name(s1[, s2, ...])
struct Redim : Stmt {
  std::string name;
  std::vector<std::unique_ptr<Expr>> sizes;
  bool preserve{false};
};

struct CaseBranch {
  bool isElse{false};
  std::vector<std::unique_ptr<Expr>> values; // empty when isElse
  std::vector<std::unique_ptr<Stmt>> body;
  // Optional relational form: CASE IS <op> expr
  bool isRel{false};
  Tok relOp{Tok::Eq};
  std::unique_ptr<Expr> relExpr;
};

struct SelectCaseStmt : Stmt {
  std::unique_ptr<Expr> selector;
  std::vector<CaseBranch> branches; // evaluated in order; first match wins
};

// User-Defined Type (UDT)
struct TypeField {
  std::string name;
  std::string typeName; // Optional type annotation
  bool hasType{false};
};

struct TypeDecl : Stmt {
  std::string name;
  std::string parentType; // For inheritance (EXTENDS)
  bool hasParent{false};
  std::vector<TypeField> fields;
  std::vector<std::unique_ptr<Stmt>> methods; // Functions/subroutines within type
};

// Type annotation for variables and function parameters
struct TypeAnnotation {
  std::string typeName;
  bool hasType{false};
};

// Lambda/Anonymous function
struct LambdaExpr : Expr {
  std::vector<FunctionParam> params;
  std::string returnType;
  bool hasReturnType{false};
  std::vector<std::unique_ptr<Stmt>> body;
};


// For-Each loop
struct ForEach : Stmt {
  std::string var;
  std::unique_ptr<Expr> collection;
  std::vector<std::unique_ptr<Stmt>> body;
};

// String interpolation expression
struct InterpolatedString : Expr {
  std::vector<std::unique_ptr<Expr>> parts; // String literals and expressions
};

// Dictionary/Map literal
struct MapLiteral : Expr {
  std::vector<std::pair<std::unique_ptr<Expr>, std::unique_ptr<Expr>>> entries; // key-value pairs
};

// Module declaration
struct ModuleDecl : Stmt {
  std::string name;
  bool isPublic{true};
  std::vector<std::unique_ptr<Stmt>> body;
};

// Operator overloading
struct OperatorDecl : Stmt {
  Tok op;
  std::string typeName;
  std::vector<FunctionParam> params;
  std::vector<std::unique_ptr<Stmt>> body;
};

// Coroutine/Yield
struct YieldStmt : Stmt {
  std::unique_ptr<Expr> value; // Optional value to yield
};

// Event handlers
struct EventHandler : Stmt {
  std::string eventType; // "KEY", "MOUSE", etc.
  std::string eventName; // "PRESSED", "CLICKED", etc.
  std::vector<std::unique_ptr<Stmt>> body;
};

// Debug statements
struct AssertStmt : Stmt {
  std::unique_ptr<Expr> condition;
  std::unique_ptr<Expr> message; // Optional error message
};

struct BreakpointStmt : Stmt {};

struct DebugPrintStmt : Stmt {
  std::unique_ptr<Expr> value;
};

// Reflection
struct TypeOfExpr : Expr {
  std::unique_ptr<Expr> value;
};

struct GetPropertiesExpr : Expr {
  std::unique_ptr<Expr> value;
};

struct GetMethodsExpr : Expr {
  std::unique_ptr<Expr> value;
};

// Super call (for inheritance)
struct SuperCall : Expr {
  std::string method;
  std::vector<std::unique_ptr<Expr>> args;
};

struct Program { std::vector<std::unique_ptr<Stmt>> stmts; };

} // namespace bas
namespace bas {

struct Expr {
  virtual ~Expr() = default;
};

struct Literal : Expr {
  Token tok; explicit Literal(Token t): tok(std::move(t)) {}
};

struct Variable : Expr {
  std::string name; explicit Variable(std::string n): name(std::move(n)) {}
};

struct Unary : Expr {
  Tok op; std::unique_ptr<Expr> right;
  Unary(Tok o, std::unique_ptr<Expr> r): op(o), right(std::move(r)) {}
};

struct Binary : Expr {
  std::unique_ptr<Expr> left, right; Tok op;
  Binary(std::unique_ptr<Expr> l, Tok o, std::unique_ptr<Expr> r)
    : left(std::move(l)), right(std::move(r)), op(o) {}
};

// Named parameter in function calls
struct NamedArg {
  std::string name;
  std::unique_ptr<Expr> value;
};

struct Call : Expr {
  std::string callee; 
  std::vector<std::unique_ptr<Expr>> args;
  std::vector<NamedArg> namedArgs; // Named parameters
  Call(std::string c, std::vector<std::unique_ptr<Expr>> a)
    : callee(std::move(c)), args(std::move(a)) {}
};

struct Index : Expr {
  std::unique_ptr<Expr> target;
  std::unique_ptr<Expr> index;
  Index(std::unique_ptr<Expr> t, std::unique_ptr<Expr> i)
    : target(std::move(t)), index(std::move(i)) {}
};

struct MemberAccess : Expr {
  std::unique_ptr<Expr> object;
  std::string member;
  MemberAccess(std::unique_ptr<Expr> obj, std::string mem)
    : object(std::move(obj)), member(std::move(mem)) {}
};

struct MethodCall : Expr {
  std::unique_ptr<Expr> object;
  std::string method;
  std::vector<std::unique_ptr<Expr>> args;
  MethodCall(std::unique_ptr<Expr> obj, std::string m, std::vector<std::unique_ptr<Expr>> a)
    : object(std::move(obj)), method(std::move(m)), args(std::move(a)) {}
};

struct ArrayLiteral : Expr {
  std::vector<std::unique_ptr<Expr>> elements;
};

struct Stmt { virtual ~Stmt() = default; };
// OPTION EXPLICIT directive (enable strict undeclared-variable checks)
struct OptionExplicit : Stmt { bool enabled{true}; };
struct Let : Stmt { 
  std::string name; 
  std::unique_ptr<Expr> value;
  std::string typeName; // Optional type annotation
  bool hasType{false};
};
struct ConstDecl : Stmt { std::string name; std::unique_ptr<Expr> value; };
struct Assign : Stmt { std::string name; std::unique_ptr<Expr> value; };
struct LocalDecl : Stmt { std::vector<std::string> names; };
struct GlobalDecl : Stmt { std::vector<std::string> names; };
struct Print : Stmt { std::unique_ptr<Expr> value; };
struct ExprStmt : Stmt { std::unique_ptr<Expr> expr; };
struct CallStmt : Stmt { std::string name; std::vector<std::unique_ptr<Expr>> args; };
struct Break : Stmt {}; // Simple break statement
struct Continue : Stmt {}; // Continue current loop
// IMPORT "file.bas"
struct ImportStmt : Stmt { std::string path; };
struct IfChain : Stmt {
  struct Branch { std::unique_ptr<Expr> cond; std::vector<std::unique_ptr<Stmt>> body; };
  std::vector<Branch> branches; // evaluated in order
  std::vector<std::unique_ptr<Stmt>> elseBody; // optional, empty when absent
  bool hasElse{false};
};

// Simple IF-THEN-ENDIF structure
struct IfThenEndIf : Stmt { 
  std::unique_ptr<Expr> cond; 
  std::vector<std::unique_ptr<Stmt>> body; 
};

struct WhileWend : Stmt { std::unique_ptr<Expr> cond; std::vector<std::unique_ptr<Stmt>> body; };

// DO ... LOOP (infinite loop, exit via BREAK)
struct DoLoop : Stmt { std::vector<std::unique_ptr<Stmt>> body; };

// REPEAT ... UNTIL <cond> (post-test loop; executes body, then exits when cond is true)
struct RepeatUntil : Stmt { std::vector<std::unique_ptr<Stmt>> body; std::unique_ptr<Expr> cond; };

struct ForNext : Stmt {
  std::string var;
  std::unique_ptr<Expr> init;
  std::unique_ptr<Expr> limit;
  std::unique_ptr<Expr> step; // may be null -> default 1
  std::vector<std::unique_ptr<Stmt>> body;
};

struct SubDecl : Stmt {
  std::string name;
  std::vector<std::string> params;
  std::vector<std::unique_ptr<Stmt>> body;
};

// Forward declaration for FunctionParam
struct FunctionParam {
  std::string name;
  std::string typeName; // Optional type annotation
  bool hasType{false};
  std::unique_ptr<Expr> defaultValue; // Optional default value
  bool hasDefault{false};
};

// Enhanced function declaration with return types and optional parameters
struct FunctionDecl : Stmt {
  std::string name;
  std::vector<FunctionParam> params; // Enhanced with types and defaults
  std::string returnType; // Optional return type
  bool hasReturnType{false};
  std::vector<std::unique_ptr<Stmt>> body;
};

struct AssignIndex : Stmt { std::string name; std::vector<std::unique_ptr<Expr>> indices; std::unique_ptr<Expr> value; };
struct AssignMember : Stmt { std::unique_ptr<Expr> object; std::string member; std::unique_ptr<Expr> value; };

struct Return : Stmt { std::unique_ptr<Expr> value; };

// DIM declaration: DIM name(s1[, s2, ...])
struct Dim : Stmt { std::string name; std::vector<std::unique_ptr<Expr>> sizes; };

// REDIM declaration: REDIM [PRESERVE] name(s1[, s2, ...])
struct Redim : Stmt {
  std::string name;
  std::vector<std::unique_ptr<Expr>> sizes;
  bool preserve{false};
};

struct CaseBranch {
  bool isElse{false};
  std::vector<std::unique_ptr<Expr>> values; // empty when isElse
  std::vector<std::unique_ptr<Stmt>> body;
  // Optional relational form: CASE IS <op> expr
  bool isRel{false};
  Tok relOp{Tok::Eq};
  std::unique_ptr<Expr> relExpr;
};

struct SelectCaseStmt : Stmt {
  std::unique_ptr<Expr> selector;
  std::vector<CaseBranch> branches; // evaluated in order; first match wins
};

// User-Defined Type (UDT)
struct TypeField {
  std::string name;
  std::string typeName; // Optional type annotation
  bool hasType{false};
};

struct TypeDecl : Stmt {
  std::string name;
  std::string parentType; // For inheritance (EXTENDS)
  bool hasParent{false};
  std::vector<TypeField> fields;
  std::vector<std::unique_ptr<Stmt>> methods; // Functions/subroutines within type
};

// Type annotation for variables and function parameters
struct TypeAnnotation {
  std::string typeName;
  bool hasType{false};
};

// Lambda/Anonymous function
struct LambdaExpr : Expr {
  std::vector<FunctionParam> params;
  std::string returnType;
  bool hasReturnType{false};
  std::vector<std::unique_ptr<Stmt>> body;
};


// For-Each loop
struct ForEach : Stmt {
  std::string var;
  std::unique_ptr<Expr> collection;
  std::vector<std::unique_ptr<Stmt>> body;
};

// String interpolation expression
struct InterpolatedString : Expr {
  std::vector<std::unique_ptr<Expr>> parts; // String literals and expressions
};

// Dictionary/Map literal
struct MapLiteral : Expr {
  std::vector<std::pair<std::unique_ptr<Expr>, std::unique_ptr<Expr>>> entries; // key-value pairs
};

// Module declaration
struct ModuleDecl : Stmt {
  std::string name;
  bool isPublic{true};
  std::vector<std::unique_ptr<Stmt>> body;
};

// Operator overloading
struct OperatorDecl : Stmt {
  Tok op;
  std::string typeName;
  std::vector<FunctionParam> params;
  std::vector<std::unique_ptr<Stmt>> body;
};

// Coroutine/Yield
struct YieldStmt : Stmt {
  std::unique_ptr<Expr> value; // Optional value to yield
};

// Event handlers
struct EventHandler : Stmt {
  std::string eventType; // "KEY", "MOUSE", etc.
  std::string eventName; // "PRESSED", "CLICKED", etc.
  std::vector<std::unique_ptr<Stmt>> body;
};

// Debug statements
struct AssertStmt : Stmt {
  std::unique_ptr<Expr> condition;
  std::unique_ptr<Expr> message; // Optional error message
};

struct BreakpointStmt : Stmt {};

struct DebugPrintStmt : Stmt {
  std::unique_ptr<Expr> value;
};

// Reflection
struct TypeOfExpr : Expr {
  std::unique_ptr<Expr> value;
};

struct GetPropertiesExpr : Expr {
  std::unique_ptr<Expr> value;
};

struct GetMethodsExpr : Expr {
  std::unique_ptr<Expr> value;
};

// Super call (for inheritance)
struct SuperCall : Expr {
  std::string method;
  std::vector<std::unique_ptr<Expr>> args;
};

struct Program { std::vector<std::unique_ptr<Stmt>> stmts; };

} // namespace bas
namespace bas {

struct Expr {
  virtual ~Expr() = default;
};

struct Literal : Expr {
  Token tok; explicit Literal(Token t): tok(std::move(t)) {}
};

struct Variable : Expr {
  std::string name; explicit Variable(std::string n): name(std::move(n)) {}
};

struct Unary : Expr {
  Tok op; std::unique_ptr<Expr> right;
  Unary(Tok o, std::unique_ptr<Expr> r): op(o), right(std::move(r)) {}
};

struct Binary : Expr {
  std::unique_ptr<Expr> left, right; Tok op;
  Binary(std::unique_ptr<Expr> l, Tok o, std::unique_ptr<Expr> r)
    : left(std::move(l)), right(std::move(r)), op(o) {}
};

// Named parameter in function calls
struct NamedArg {
  std::string name;
  std::unique_ptr<Expr> value;
};

struct Call : Expr {
  std::string callee; 
  std::vector<std::unique_ptr<Expr>> args;
  std::vector<NamedArg> namedArgs; // Named parameters
  Call(std::string c, std::vector<std::unique_ptr<Expr>> a)
    : callee(std::move(c)), args(std::move(a)) {}
};

struct Index : Expr {
  std::unique_ptr<Expr> target;
  std::unique_ptr<Expr> index;
  Index(std::unique_ptr<Expr> t, std::unique_ptr<Expr> i)
    : target(std::move(t)), index(std::move(i)) {}
};

struct MemberAccess : Expr {
  std::unique_ptr<Expr> object;
  std::string member;
  MemberAccess(std::unique_ptr<Expr> obj, std::string mem)
    : object(std::move(obj)), member(std::move(mem)) {}
};

struct MethodCall : Expr {
  std::unique_ptr<Expr> object;
  std::string method;
  std::vector<std::unique_ptr<Expr>> args;
  MethodCall(std::unique_ptr<Expr> obj, std::string m, std::vector<std::unique_ptr<Expr>> a)
    : object(std::move(obj)), method(std::move(m)), args(std::move(a)) {}
};

struct ArrayLiteral : Expr {
  std::vector<std::unique_ptr<Expr>> elements;
};

struct Stmt { virtual ~Stmt() = default; };
// OPTION EXPLICIT directive (enable strict undeclared-variable checks)
struct OptionExplicit : Stmt { bool enabled{true}; };
struct Let : Stmt { 
  std::string name; 
  std::unique_ptr<Expr> value;
  std::string typeName; // Optional type annotation
  bool hasType{false};
};
struct ConstDecl : Stmt { std::string name; std::unique_ptr<Expr> value; };
struct Assign : Stmt { std::string name; std::unique_ptr<Expr> value; };
struct LocalDecl : Stmt { std::vector<std::string> names; };
struct GlobalDecl : Stmt { std::vector<std::string> names; };
struct Print : Stmt { std::unique_ptr<Expr> value; };
struct ExprStmt : Stmt { std::unique_ptr<Expr> expr; };
struct CallStmt : Stmt { std::string name; std::vector<std::unique_ptr<Expr>> args; };
struct Break : Stmt {}; // Simple break statement
struct Continue : Stmt {}; // Continue current loop
// IMPORT "file.bas"
struct ImportStmt : Stmt { std::string path; };
struct IfChain : Stmt {
  struct Branch { std::unique_ptr<Expr> cond; std::vector<std::unique_ptr<Stmt>> body; };
  std::vector<Branch> branches; // evaluated in order
  std::vector<std::unique_ptr<Stmt>> elseBody; // optional, empty when absent
  bool hasElse{false};
};

// Simple IF-THEN-ENDIF structure
struct IfThenEndIf : Stmt { 
  std::unique_ptr<Expr> cond; 
  std::vector<std::unique_ptr<Stmt>> body; 
};

struct WhileWend : Stmt { std::unique_ptr<Expr> cond; std::vector<std::unique_ptr<Stmt>> body; };

// DO ... LOOP (infinite loop, exit via BREAK)
struct DoLoop : Stmt { std::vector<std::unique_ptr<Stmt>> body; };

// REPEAT ... UNTIL <cond> (post-test loop; executes body, then exits when cond is true)
struct RepeatUntil : Stmt { std::vector<std::unique_ptr<Stmt>> body; std::unique_ptr<Expr> cond; };

struct ForNext : Stmt {
  std::string var;
  std::unique_ptr<Expr> init;
  std::unique_ptr<Expr> limit;
  std::unique_ptr<Expr> step; // may be null -> default 1
  std::vector<std::unique_ptr<Stmt>> body;
};

struct SubDecl : Stmt {
  std::string name;
  std::vector<std::string> params;
  std::vector<std::unique_ptr<Stmt>> body;
};

// Forward declaration for FunctionParam
struct FunctionParam {
  std::string name;
  std::string typeName; // Optional type annotation
  bool hasType{false};
  std::unique_ptr<Expr> defaultValue; // Optional default value
  bool hasDefault{false};
};

// Enhanced function declaration with return types and optional parameters
struct FunctionDecl : Stmt {
  std::string name;
  std::vector<FunctionParam> params; // Enhanced with types and defaults
  std::string returnType; // Optional return type
  bool hasReturnType{false};
  std::vector<std::unique_ptr<Stmt>> body;
};

struct AssignIndex : Stmt { std::string name; std::vector<std::unique_ptr<Expr>> indices; std::unique_ptr<Expr> value; };
struct AssignMember : Stmt { std::unique_ptr<Expr> object; std::string member; std::unique_ptr<Expr> value; };

struct Return : Stmt { std::unique_ptr<Expr> value; };

// DIM declaration: DIM name(s1[, s2, ...])
struct Dim : Stmt { std::string name; std::vector<std::unique_ptr<Expr>> sizes; };

// REDIM declaration: REDIM [PRESERVE] name(s1[, s2, ...])
struct Redim : Stmt {
  std::string name;
  std::vector<std::unique_ptr<Expr>> sizes;
  bool preserve{false};
};

struct CaseBranch {
  bool isElse{false};
  std::vector<std::unique_ptr<Expr>> values; // empty when isElse
  std::vector<std::unique_ptr<Stmt>> body;
  // Optional relational form: CASE IS <op> expr
  bool isRel{false};
  Tok relOp{Tok::Eq};
  std::unique_ptr<Expr> relExpr;
};

struct SelectCaseStmt : Stmt {
  std::unique_ptr<Expr> selector;
  std::vector<CaseBranch> branches; // evaluated in order; first match wins
};

// User-Defined Type (UDT)
struct TypeField {
  std::string name;
  std::string typeName; // Optional type annotation
  bool hasType{false};
};

struct TypeDecl : Stmt {
  std::string name;
  std::string parentType; // For inheritance (EXTENDS)
  bool hasParent{false};
  std::vector<TypeField> fields;
  std::vector<std::unique_ptr<Stmt>> methods; // Functions/subroutines within type
};

// Type annotation for variables and function parameters
struct TypeAnnotation {
  std::string typeName;
  bool hasType{false};
};

// Lambda/Anonymous function
struct LambdaExpr : Expr {
  std::vector<FunctionParam> params;
  std::string returnType;
  bool hasReturnType{false};
  std::vector<std::unique_ptr<Stmt>> body;
};


// For-Each loop
struct ForEach : Stmt {
  std::string var;
  std::unique_ptr<Expr> collection;
  std::vector<std::unique_ptr<Stmt>> body;
};

// String interpolation expression
struct InterpolatedString : Expr {
  std::vector<std::unique_ptr<Expr>> parts; // String literals and expressions
};

// Dictionary/Map literal
struct MapLiteral : Expr {
  std::vector<std::pair<std::unique_ptr<Expr>, std::unique_ptr<Expr>>> entries; // key-value pairs
};

// Module declaration
struct ModuleDecl : Stmt {
  std::string name;
  bool isPublic{true};
  std::vector<std::unique_ptr<Stmt>> body;
};

// Operator overloading
struct OperatorDecl : Stmt {
  Tok op;
  std::string typeName;
  std::vector<FunctionParam> params;
  std::vector<std::unique_ptr<Stmt>> body;
};

// Coroutine/Yield
struct YieldStmt : Stmt {
  std::unique_ptr<Expr> value; // Optional value to yield
};

// Event handlers
struct EventHandler : Stmt {
  std::string eventType; // "KEY", "MOUSE", etc.
  std::string eventName; // "PRESSED", "CLICKED", etc.
  std::vector<std::unique_ptr<Stmt>> body;
};

// Debug statements
struct AssertStmt : Stmt {
  std::unique_ptr<Expr> condition;
  std::unique_ptr<Expr> message; // Optional error message
};

struct BreakpointStmt : Stmt {};

struct DebugPrintStmt : Stmt {
  std::unique_ptr<Expr> value;
};

// Reflection
struct TypeOfExpr : Expr {
  std::unique_ptr<Expr> value;
};

struct GetPropertiesExpr : Expr {
  std::unique_ptr<Expr> value;
};

struct GetMethodsExpr : Expr {
  std::unique_ptr<Expr> value;
};

// Super call (for inheritance)
struct SuperCall : Expr {
  std::string method;
  std::vector<std::unique_ptr<Expr>> args;
};

struct Program { std::vector<std::unique_ptr<Stmt>> stmts; };

} // namespace bas
