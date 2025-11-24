#pragma once
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
struct PrintC : Stmt { std::unique_ptr<Expr> value; }; // Print without newline
struct ExprStmt : Stmt {
  std::unique_ptr<Expr> expr;
  ExprStmt() = default;
  explicit ExprStmt(std::unique_ptr<Expr> e) : expr(std::move(e)) {}
};
struct CallStmt : Stmt { std::string name; std::vector<std::unique_ptr<Expr>> args; };
struct Break : Stmt {}; // Simple break statement
struct Continue : Stmt {}; // Continue current loop
struct Exit : Stmt {
    std::string target; // "FOR", "WHILE", "SUB", "FUNCTION"
};
struct Goto : Stmt { std::string label; }; // GOTO label
struct Gosub : Stmt { std::string label; }; // GOSUB label
struct Label : Stmt { std::string name; }; // label: (statement marker)
struct End : Stmt {}; // END statement
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
// Await statement: AWAIT expression
struct AwaitStmt : Stmt {
  std::unique_ptr<Expr> expression;
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

// Range literal: 1..10 or "a".."z"
struct RangeLiteral : Expr {
  std::unique_ptr<Expr> start;
  std::unique_ptr<Expr> end;
};

// Array comprehension: [expr FOR var IN collection IF cond]
struct ArrayComprehension : Expr {
  std::unique_ptr<Expr> expr;
  std::string var;
  std::unique_ptr<Expr> collection;
  std::unique_ptr<Expr> condition; // Optional IF condition
};

// Null-safe member access: obj?.property
struct NullSafeAccess : Expr {
  std::unique_ptr<Expr> object;
  std::string member;
  bool isIndex{false}; // ?. vs ?[
  std::unique_ptr<Expr> index; // For ?[index]
};

// Null coalescing: value ?? defaultValue
struct NullCoalesceExpr : Expr {
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;
};

// Tuple literal: (x, y, z)
struct TupleLiteral : Expr {
  std::vector<std::unique_ptr<Expr>> elements;
};

// Destructuring assignment: (x, y) = getPos()
struct DestructureAssign : Stmt {
  std::vector<std::string> names;
  std::unique_ptr<Expr> value;
};

// Using block: USING resource = ... END USING
struct UsingBlock : Stmt {
  std::string varName;
  std::unique_ptr<Expr> resource;
  std::vector<std::unique_ptr<Stmt>> body;
};

// Match expression: MATCH value CASE ... END MATCH
struct MatchExpr : Expr {
  std::unique_ptr<Expr> value;
  struct Case {
    std::unique_ptr<Expr> pattern;
    std::unique_ptr<Expr> result;
  };
  std::vector<Case> cases;
  std::unique_ptr<Expr> defaultCase; // CASE ELSE
};

// Try/Catch/Finally block: TRY ... CATCH error ... FINALLY ... END TRY
struct TryCatchStmt : Stmt {
  std::vector<std::unique_ptr<Stmt>> tryBody;
  std::string catchVar; // Variable name for caught exception
  std::vector<std::unique_ptr<Stmt>> catchBody;
  std::vector<std::unique_ptr<Stmt>> finallyBody;
  bool hasCatch{false};
  bool hasFinally{false};
};

// Throw statement: THROW error
struct ThrowStmt : Stmt {
  std::unique_ptr<Expr> error; // Error value to throw
};

// Enum declaration
struct EnumValue {
  std::string name;
  std::unique_ptr<Expr> value; // Optional custom value (nullptr = auto-assign)
};
struct EnumDecl : Stmt {
  std::string name;
  std::vector<EnumValue> values;
};

// Union type declaration
struct UnionDecl : Stmt {
  std::string name;
  std::vector<std::string> types;
};

// Spread operator: ...arr
struct SpreadExpr : Expr {
  std::unique_ptr<Expr> value;
};

// State system AST nodes
struct StateHook : Stmt {
    enum HookType { Enter, Exit, Update };
    HookType type;
    std::vector<std::unique_ptr<Stmt>> body;
};

struct TransitionDecl : Stmt {
    std::string fromState;
    std::string toState;
    std::unique_ptr<Expr> condition; // WHEN condition
    int priority{0}; // Optional priority
};

struct StateDecl : Stmt {
    std::string name;
    std::string animation; // Optional animation name
    double animationBlend{0.0}; // Optional blend time
    double waitTime{0.0}; // Optional minimum wait time
    std::vector<std::unique_ptr<Stmt>> body; // State body (hooks, transitions, etc.)
    std::vector<StateHook> hooks; // ON ENTER/EXIT/UPDATE hooks
    std::vector<TransitionDecl> transitions; // TRANSITION statements
};

struct ParallelStates : Stmt {
    std::vector<std::unique_ptr<StateDecl>> states;
};

struct StateGroup : Stmt {
    std::string name;
    std::vector<std::unique_ptr<StateDecl>> states;
};

struct StateSystemDecl : Stmt {
    std::string name;
    std::vector<std::unique_ptr<StateDecl>> states;
    std::vector<StateGroup> groups;
    std::vector<ParallelStates> parallelBlocks;
};

struct AttachSystemStmt : Stmt {
    std::string systemName;
    std::unique_ptr<Expr> target; // Entity or identifier
};

struct AddStateStmt : Stmt {
    std::string stateName;
    std::string systemName;
    std::unique_ptr<StateDecl> state;
};

struct RemoveStateStmt : Stmt {
    std::string stateName;
    std::string systemName;
};

struct OverrideStateStmt : Stmt {
    std::string stateName;
    std::vector<std::unique_ptr<Stmt>> body;
};

struct SetTransitionRuleStmt : Stmt {
    std::string fromState;
    std::string toState;
    int priority{0};
};

struct EnableDisableStateStmt : Stmt {
    std::string stateName;
    bool enable{true};
};

struct DebugStatesStmt : Stmt {
    std::string systemName; // Empty for all systems
};

struct ExportSystemStmt : Stmt {
    std::string systemName;
    std::string filePath;
};

struct ImportSystemStmt : Stmt {
    std::string systemName;
    std::string filePath;
};

struct EventDecl : Stmt {
    std::string name;
};

struct AnimationStmt : Stmt {
    std::string animationName;
    double blendTime{0.0};
};

struct WaitStmt : Stmt {
    double duration;
};

struct Program { std::vector<std::unique_ptr<Stmt>> stmts; };

} // namespace bas
