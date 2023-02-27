#pragma once
#include "BasicBlock.h"
#include "Constant.h"
#include "DeadCode.h"
#include "FuncInfo.h"
#include "Function.h"
#include "Instruction.h"
#include "Module.h"
#include "PassManager.hpp"
#include "Value.h"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace GVNExpression {

// fold the constant value
class ConstFolder {
  public:
    ConstFolder(Module *m) : module_(m) {}
    Constant *compute(Instruction *instr, Constant *value1, Constant *value2);
    Constant *compute(Instruction *instr, Constant *value1);

  private:
    Module *module_;
};

/**
 * for constructor of class derived from `Expression`, we make it public
 * because `std::make_shared` needs the constructor to be publicly available,
 * but you should call the static factory method `create` instead the constructor itself to get the desired data
 */
class Expression {
  public:
    // TODO: you need to extend expression types according to testcases
    enum gvn_expr_t { e_constant, e_bin, e_phi, e_svar, e_cmp, e_fcmp, e_gep, e_call};
    Expression(gvn_expr_t t) : expr_type(t) {}
    virtual ~Expression() = default;
    virtual std::string print() = 0;
    gvn_expr_t get_expr_type() const { return expr_type; }

  private:
    gvn_expr_t expr_type;
};

bool operator==(const std::shared_ptr<Expression> &lhs, const std::shared_ptr<Expression> &rhs);
bool operator==(const GVNExpression::Expression &lhs, const GVNExpression::Expression &rhs);

class ConstantExpression : public Expression {
  public:
    static std::shared_ptr<ConstantExpression> create(Constant *c) { return std::make_shared<ConstantExpression>(c); }
    virtual std::string print() { return c_->print(); }
    // we leverage the fact that constants in lightIR have unique addresses
    bool equiv(const ConstantExpression *other) const { return c_ == other->c_; }
    ConstantExpression(Constant *c) : Expression(e_constant), c_(c) {}
    Constant* get_constant() { return c_; }


  private:
    Constant *c_;
};

// arithmetic expression
class BinaryExpression : public Expression {
  public:
    static std::shared_ptr<BinaryExpression> create(Instruction::OpID op,
                                                    std::shared_ptr<Expression> lhs,
                                                    std::shared_ptr<Expression> rhs) {
        return std::make_shared<BinaryExpression>(op, lhs, rhs);
    }
    virtual std::string print() {
        if(rhs_)
            return "(" + Instruction::get_instr_op_name(op_) +" " + lhs_->print() + " " + rhs_->print() + ")";
        else 
            return "(" + Instruction::get_instr_op_name(op_) +" " + lhs_->print() + ")";
    }

    bool equiv(const BinaryExpression *other) const {
      if(op_ == other->op_) {
          if(!rhs_ && !other->rhs_)
            return *lhs_ == *other->lhs_;
          else 
            return *lhs_ == *other->lhs_ && *rhs_ == *other->rhs_;
            
      }
      else 
          return false;
    }

    Instruction::OpID get_op_type() { return op_; }

    BinaryExpression(Instruction::OpID op, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
        : Expression(e_bin), op_(op), lhs_(lhs), rhs_(rhs) {}

    std::shared_ptr<Expression> get_lhs() { return lhs_; }
    std::shared_ptr<Expression> get_rhs() { return rhs_; }
    
  private:
    Instruction::OpID op_;
    std::shared_ptr<Expression> lhs_, rhs_;
    
};


class CmpExpression : public BinaryExpression {
  public:
    static std::shared_ptr<CmpExpression> create(CmpInst::CmpOp cmp_op,
                                                    std::shared_ptr<Expression> lhs,
                                                    std::shared_ptr<Expression> rhs) {
        return std::make_shared<CmpExpression>(cmp_op, lhs, rhs);
    }
    virtual std::string print() {
        return "(" + std::string("cmp") + " " + get_cmp_op_name() + " "+  get_lhs()->print() + " " + get_rhs()->print() + ")";
    }

    bool equiv(const CmpExpression *other) const {
      if(cmp_op == other->cmp_op) {
          return BinaryExpression::equiv(other);  
      }
      else 
          return false;
    }

    CmpExpression(CmpInst::CmpOp cmp_op, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
        : BinaryExpression(Instruction::cmp, lhs, rhs), cmp_op(cmp_op) {}

  private:
    std::string get_cmp_op_name() {
      switch (cmp_op) {

        case CmpInst::EQ:
          return "EQ";
          break;

        case CmpInst::NE:
          return "NE";
          break;

        case CmpInst::GT:
          return "GT";
          break;

        case CmpInst::GE:
          return "GE";
          break;

        case CmpInst::LT:
          return "LT";
          break;

        case CmpInst::LE:
          return "LE";
          break;
        default:
          break;

      }
    }

  private:
    CmpInst::CmpOp cmp_op;    
};

class FCmpExpression : public BinaryExpression {
  public:
    static std::shared_ptr<FCmpExpression> create(FCmpInst::CmpOp fcmp_op,
                                                    std::shared_ptr<Expression> lhs,
                                                    std::shared_ptr<Expression> rhs) {
        return std::make_shared<FCmpExpression>(fcmp_op, lhs, rhs);
    }
    virtual std::string print() {
        return "(" + std::string("fcmp") + " " + get_fcmp_op_name() + " "+  get_lhs()->print() + " " + get_rhs()->print() + ")";
    }

    bool equiv(const FCmpExpression *other) const {
      if(fcmp_op == other->fcmp_op) {
          return BinaryExpression::equiv(other);  
      }
      else 
          return false;
    }

    FCmpExpression(FCmpInst::CmpOp fcmp_op, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
        : BinaryExpression(Instruction::fcmp, lhs, rhs), fcmp_op(fcmp_op) {}


  private:
    std::string get_fcmp_op_name() {
      switch (fcmp_op) {

        case FCmpInst::EQ:
          return "EQ";
          break;

        case FCmpInst::NE:
          return "NE";
          break;

        case FCmpInst::GT:
          return "GT";
          break;

        case FCmpInst::GE:
          return "GE";
          break;

        case FCmpInst::LT:
          return "LT";
          break;

        case FCmpInst::LE:
          return "LE";
          break;
        default:
          break;

      }
    }

  private:
    FCmpInst::CmpOp fcmp_op;    
};


class PhiExpression : public Expression {
  public:
    static std::shared_ptr<PhiExpression> create(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs) {
        return std::make_shared<PhiExpression>(lhs, rhs);
    }
    virtual std::string print() { 
      if(rhs_.get() && lhs_.get()) {
        return "(phi " +  lhs_->print() + " " + rhs_->print() + ")";
      }
      else if(lhs_.get()) {
          return "(phi " +  lhs_->print() + " NULL)";
      }
      else if(rhs_.get()) {
          return "(phi NULL " +  rhs_->print() + " )";
      }  
    }
    bool equiv(const PhiExpression *other) const {
        if (*lhs_ == *other->lhs_ and *rhs_ == *other->rhs_)
            return true;
        else
            return false;
    }
    PhiExpression(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
        : Expression(e_phi), lhs_(lhs), rhs_(rhs) {}

    std::shared_ptr<Expression> get_lhs() { return lhs_; }
    std::shared_ptr<Expression> get_rhs() { return rhs_; }

    void set_lhs(std::shared_ptr<Expression> lhs) { lhs_ = lhs; }
    void set_rhs(std::shared_ptr<Expression> rhs) { rhs_ = rhs; }


  private:
    std::shared_ptr<Expression> lhs_, rhs_;
};

class VarExpression: public Expression {
  public:
    static std::shared_ptr<VarExpression> create(Value* val) {
        return std::make_shared<VarExpression>(val);
    }
    virtual std::string print() { return "(var " + val->get_name() + ")"; }

    bool equiv(const VarExpression *other) const {
        return val == other->val;
    }

    Value* get_val() { return val; }

    VarExpression(Value* val):Expression(e_svar),val(val) {}

  private:
    Value* val;
};

class GepExpression: public Expression {
  public:
    static std::shared_ptr<GepExpression> create(std::vector<std::shared_ptr<Expression>> operands) {
        return std::make_shared<GepExpression>(operands);
    }
    virtual std::string print() { 
      std::string sstring;
      sstring += "(gep ";

      for(auto &op_expr: operands_) {
        sstring += op_expr->print();
        sstring += " ";
      }
      sstring += ")";
      return sstring;
    }

    bool equiv(const GepExpression *other) const {
        auto iter1 = operands_.begin();
        auto iter2 = other->operands_.begin();
        while(iter1 != operands_.end()) {
          if(*(*iter1) == *(*iter2)) {
            iter1 ++;
            iter2 ++;
          }
          else 
            return false;
        }
        return iter1 == operands_.end() && iter2 == other->operands_.end();
    }

    GepExpression(std::vector<std::shared_ptr<Expression>> operands):Expression(e_gep),operands_(operands) {}

  private:
    std::vector<std::shared_ptr<Expression>> operands_;
};

class CallExpression: public Expression {
  public:
    static std::shared_ptr<CallExpression> create(Function *func,std::vector<std::shared_ptr<Expression>> args) {
        return std::make_shared<CallExpression>(func, args);
    }
    virtual std::string print() { 
      std::string sstring;
      sstring += "(call ";
      sstring += func_->get_name();
      for(auto &arg_expr: args_) {
        sstring += arg_expr->print();
        sstring += " ";
      }
      sstring += ")";
      return sstring;
    }

    bool equiv(const CallExpression *other) const {
        auto iter1 = args_.begin();
        auto iter2 = other->args_.begin();
        if(func_ != other->func_)
          return false;
        while(iter1 != args_.end()) {
          if(*(*iter1) == *(*iter2)) {
            iter1 ++;
            iter2 ++;
          }
          else 
            return false;
        }
        return iter1 == args_.end() && iter2 == other->args_.end();
    }

    CallExpression(Function* func, std::vector<std::shared_ptr<Expression>> args):Expression(e_call), func_(func), args_(args) {}

  private:
    Function* func_;
    std::vector<std::shared_ptr<Expression>> args_;
};


} // namespace GVNExpression


/**
 * Congruence class in each partitions
 * note: for constant propagation, you might need to add other fields
 * and for load/store redundancy detection, you most certainly need to modify the class
 */
struct CongruenceClass {
    size_t index_;
    // representative of the congruence class, used to replace all the members (except itself) when analysis is done
    Value *leader_;
    // value expression in congruence class
    std::shared_ptr<GVNExpression::Expression> value_expr_;
    // value φ-function is an annotation of the congruence class
    std::shared_ptr<GVNExpression::PhiExpression> value_phi_;
    // equivalent variables in one congruence class
    std::set<Value *> members_;

    CongruenceClass() = default;
    CongruenceClass(size_t index) : index_(index), leader_{}, value_expr_{}, value_phi_{}, members_{} {}

    bool operator<(const CongruenceClass &other) const { return this->index_ < other.index_; }
    bool operator==(const CongruenceClass &other) const;
};

namespace std {
template <>
// overload std::less for std::shared_ptr<CongruenceClass>, i.e. how to sort the congruence classes
struct less<std::shared_ptr<CongruenceClass>> {
    bool operator()(const std::shared_ptr<CongruenceClass> &a, const std::shared_ptr<CongruenceClass> &b) const {
        // nullptrs should never appear in partitions, so we just dereference it
        return *a < *b;
    }
};
} // namespace std

class GVN : public Pass {
  public:
    using partitions = std::set<std::shared_ptr<CongruenceClass>>;
    GVN(Module *m, bool dump_json) : Pass(m), dump_json_(dump_json) {}
    // pass start
    void run() override;
    // init for pass metadata;
    void initPerFunction();

    // fill the following functions according to Pseudocode, **you might need to add more arguments**
    void detectEquivalences();
    partitions join(const partitions &P1, const partitions &P2);
    std::shared_ptr<CongruenceClass> intersect(std::shared_ptr<CongruenceClass>, std::shared_ptr<CongruenceClass>);
    partitions transferFunction(Instruction *x, partitions pin, BasicBlock* bb);
    partitions transferFunction(partitions pin, BasicBlock* bb, bool init_global);
    std::shared_ptr<GVNExpression::PhiExpression> valuePhiFunc(std::shared_ptr<GVNExpression::Expression>,
                                                               const partitions &, BasicBlock* bb, Instruction* x);
    std::shared_ptr<GVNExpression::Expression> valueExpr(Instruction *instr, partitions p);
    std::shared_ptr<GVNExpression::Expression> getVN(const partitions &pout,
                                                     std::shared_ptr<GVNExpression::Expression> ve);
    std::shared_ptr<GVNExpression::Expression> getVarExpr(Value* val, partitions p);
    // replace cc members with leader
    void replace_cc_members();

    // note: be careful when to use copy constructor or clone
    partitions clone(const partitions &p);

    // create congruence class helper
    std::shared_ptr<CongruenceClass> createCongruenceClass(size_t index = 0) {
        return std::make_shared<CongruenceClass>(index);
    }

    bool is_top(const partitions& p);

    bool calculate_quivalences();

  private:
    bool dump_json_;
    std::uint64_t next_value_number_ = 1;
    Function *func_;
    std::map<BasicBlock *, partitions> pin_, pout_;
    std::unique_ptr<FuncInfo> func_info_;
    std::unique_ptr<GVNExpression::ConstFolder> folder_;
    std::unique_ptr<DeadCode> dce_;
    std::map<BasicBlock*,bool> visited;
};

bool operator==(const GVN::partitions &p1, const GVN::partitions &p2);

bool operator!=(const GVN::partitions &p1, const GVN::partitions &p2);

bool operator==(const CongruenceClass &c1, const CongruenceClass &c2);

bool operator!=(const CongruenceClass &c1, const CongruenceClass &c2);