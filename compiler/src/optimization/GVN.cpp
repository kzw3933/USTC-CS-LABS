#include "GVN.h"

#include "BasicBlock.h"
#include "Constant.h"
#include "DeadCode.h"
#include "FuncInfo.h"
#include "Function.h"
#include "Instruction.h"
#include "logging.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <memory>
#include <sstream>
#include <tuple>
#include <utility>
#include <vector>

using namespace GVNExpression;
using std::string_literals::operator""s;
using std::shared_ptr;

static auto get_const_int_value = [](Value *v) { return dynamic_cast<ConstantInt *>(v)->get_value(); };
static auto get_const_fp_value = [](Value *v) { return dynamic_cast<ConstantFP *>(v)->get_value(); };
// Constant Propagation helper, folders are done for you
Constant *ConstFolder::compute(Instruction *instr, Constant *value1, Constant *value2) {
    auto op = instr->get_instr_type();
    switch (op) {
    case Instruction::add: return ConstantInt::get(get_const_int_value(value1) + get_const_int_value(value2), module_);
    case Instruction::sub: return ConstantInt::get(get_const_int_value(value1) - get_const_int_value(value2), module_);
    case Instruction::mul: return ConstantInt::get(get_const_int_value(value1) * get_const_int_value(value2), module_);
    case Instruction::sdiv: return ConstantInt::get(get_const_int_value(value1) / get_const_int_value(value2), module_);
    case Instruction::fadd: return ConstantFP::get(get_const_fp_value(value1) + get_const_fp_value(value2), module_);
    case Instruction::fsub: return ConstantFP::get(get_const_fp_value(value1) - get_const_fp_value(value2), module_);
    case Instruction::fmul: return ConstantFP::get(get_const_fp_value(value1) * get_const_fp_value(value2), module_);
    case Instruction::fdiv: return ConstantFP::get(get_const_fp_value(value1) / get_const_fp_value(value2), module_);

    case Instruction::cmp:
        switch (dynamic_cast<CmpInst *>(instr)->get_cmp_op()) {
        case CmpInst::EQ: return ConstantInt::get(get_const_int_value(value1) == get_const_int_value(value2), module_);
        case CmpInst::NE: return ConstantInt::get(get_const_int_value(value1) != get_const_int_value(value2), module_);
        case CmpInst::GT: return ConstantInt::get(get_const_int_value(value1) > get_const_int_value(value2), module_);
        case CmpInst::GE: return ConstantInt::get(get_const_int_value(value1) >= get_const_int_value(value2), module_);
        case CmpInst::LT: return ConstantInt::get(get_const_int_value(value1) < get_const_int_value(value2), module_);
        case CmpInst::LE: return ConstantInt::get(get_const_int_value(value1) <= get_const_int_value(value2), module_);
        }  
    case Instruction::fcmp:
        switch (dynamic_cast<FCmpInst *>(instr)->get_cmp_op()) {
        case FCmpInst::EQ: return ConstantInt::get(get_const_fp_value(value1) == get_const_fp_value(value2), module_);
        case FCmpInst::NE: return ConstantInt::get(get_const_fp_value(value1) != get_const_fp_value(value2), module_);
        case FCmpInst::GT: return ConstantInt::get(get_const_fp_value(value1) > get_const_fp_value(value2), module_);
        case FCmpInst::GE: return ConstantInt::get(get_const_fp_value(value1) >= get_const_fp_value(value2), module_);
        case FCmpInst::LT: return ConstantInt::get(get_const_fp_value(value1) < get_const_fp_value(value2), module_);
        case FCmpInst::LE: return ConstantInt::get(get_const_fp_value(value1) <= get_const_fp_value(value2), module_);
        }
    default: return nullptr;
    }
}

Constant *ConstFolder::compute(Instruction *instr, Constant *value1) {
    auto op = instr->get_instr_type();
    switch (op) {
    case Instruction::sitofp: return ConstantFP::get((float)get_const_int_value(value1), module_);
    case Instruction::fptosi: return ConstantInt::get((int)get_const_fp_value(value1), module_);
    case Instruction::zext: return ConstantInt::get((int)get_const_int_value(value1), module_);
    default: return nullptr;
    }
}

namespace utils {
static std::string print_congruence_class(const CongruenceClass &cc) {
    std::stringstream ss;
    if (cc.index_ == 0) {
        ss << "top class\n";
        return ss.str();
    }
    ss << "\nindex: " << cc.index_ << "\nleader: " << cc.leader_->print()
       << "\nvalue phi: " << (cc.value_phi_ ? cc.value_phi_->print() : "nullptr"s)
       << "\nvalue expr: " << (cc.value_expr_ ? cc.value_expr_->print() : "nullptr"s) << "\nmembers: {";
    for (auto &member : cc.members_)
        ss << member->print() << "; ";
    ss << "}\n";
    return ss.str();
}

static std::string dump_cc_json(const CongruenceClass &cc) {
    std::string json;
    json += "[";
    for (auto member : cc.members_) {
        if (auto c = dynamic_cast<Constant *>(member))
            json += member->print() + ", ";
        else
            json += "\"%" + member->get_name() + "\", ";
    }
    json += "]";
    return json;
}

static std::string dump_partition_json(const GVN::partitions &p) {
    std::string json;
    json += "[";
    for (auto cc : p)
        json += dump_cc_json(*cc) + ", ";
    json += "]";
    return json;
}

static std::string dump_bb2partition(const std::map<BasicBlock *, GVN::partitions> &map) {
    std::string json;
    json += "{";
    for (auto [bb, p] : map)
        json += "\"" + bb->get_name() + "\": " + dump_partition_json(p) + ",";
    json += "}";
    return json;
}

// logging utility for you
static void print_partitions(const GVN::partitions &p) {
    if (p.empty()) {
        LOG_DEBUG << "empty partitions\n";
        return;
    }
    std::string log;
    for (auto &cc : p)
        log += print_congruence_class(*cc);
    LOG_DEBUG << log; // please don't use std::cout
}
} // namespace utils



GVN::partitions GVN::join(const partitions &P1, const partitions &P2) {
    //!TODO: do intersection pair-wise
    partitions p = {};

    //  打印待合并块的pout信息
    std::cout << "P1:" << std::endl;
    for(auto &conc: P1) {
        std::cout << utils::print_congruence_class(*conc);
    }
    std::cout << std::endl;
    std::cout << "P2:" << std::endl;
    for(auto &conc: P2) {
        std::cout << utils::print_congruence_class(*conc);
    }
    std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;

    if(is_top(P1)) {
        p = P2;
    }
    else if(is_top(P2)) {
        p = P1;
    }
    else {
        for(auto &pp1 : P1) {
            for(auto &pp2 : P2) {
                // std::cout << "=====================For Testing Begin======================" << std::endl;
                // std::cout << "pp1:" << std::endl;
                // std::cout << utils::print_congruence_class(*(pp1.get()));
                // std::cout << "pp2:" << std::endl;
                // std::cout << utils::print_congruence_class(*(pp2.get()));
                auto ck = intersect(pp1, pp2);
                if(ck.get()) {
                    p.insert(ck);
                    // std::cout << "p:" << std::endl;
                    // std::cout << utils::print_congruence_class(*(ck.get()));
                }
                // std::cout << "=====================For Testing END======================" << std::endl;
            }
        }
    }
    std::cout << "p:" << std::endl;
    for(auto &conc: p) {
        std::cout << utils::print_congruence_class(*conc);
    }
    std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl << std::endl;
    return p;
}



std::shared_ptr<CongruenceClass> GVN::intersect(std::shared_ptr<CongruenceClass> Ci,
                                                std::shared_ptr<CongruenceClass> Cj) {
    //!TODO:
    auto ConU = std::make_shared<CongruenceClass>();

    auto iter1 = Ci->members_.begin();

    while(iter1 != Ci->members_.end()) {
        auto iter2 = Cj->members_.begin();
        while(iter2 != Cj->members_.end()) {
            if(*iter1 == *iter2) {
                if(*iter1 == Ci->leader_) 
                    ConU->leader_ = Ci->leader_;
                ConU->members_.insert(*iter1);
                break;
            }
            iter2++;         
        }
        iter1++;
    }

    if(ConU->members_.size() > 0) {
        if(ConU->leader_) {
            ConU->index_ = Ci.get()->index_;
            ConU->leader_ = Ci.get()->leader_;
            ConU->value_expr_ =  Ci.get()->value_expr_;
            ConU->value_phi_ = Ci.get()->value_phi_;
        }
        else {
            ConU->index_ = next_value_number_++;
            ConU->leader_ = *(ConU->members_.begin());
            ConU->value_expr_ = PhiExpression::create(Ci.get()->value_expr_, Cj.get()->value_expr_);
            ConU->value_phi_ = PhiExpression::create(Ci.get()->value_expr_, Cj.get()->value_expr_);
        }
        return ConU;
    }

    return 0;
}




void GVN::detectEquivalences() {
    bool changed;

    // initialize pout with top
    // iterate until converge

    // pout_初始化: 以一个仅含有size_t=0值编号(正常使用的值编号不能为0)的等价类的partition代表Top
    for(auto &bb : func_->get_basic_blocks()) {
        auto conc_ptr = std::shared_ptr<CongruenceClass>(new CongruenceClass(0));
        partitions par = {};
        par.insert(conc_ptr); 
        pout_.insert({&bb, par});
        std::cout << &bb << ": "  << bb.get_name() << std::endl;
    }

    // 迭代计算各个partitions的等价类
    int iter_counts = 0;
    do {
        // see the pseudo code in documentation
        // you might need to visit the blocks in depth-first order
        // get PIN of bb by predecessor(s)
        // iterate through all instructions in the block
        // and the phi instruction in all the successors
        iter_counts++;
        
        changed = calculate_quivalences();

        // 打印所有的等价类
        std::cout << "***********第" << iter_counts << "轮迭代开始**************" << std::endl;
        std::cout << pout_.size() << std::endl; 
        for(auto [bb,p] : pout_) {
            
            std::cout << "==============================================================" << std::endl;
            std::cout << "PARTITION FOR " << bb->get_name() << std::endl;
            for(auto conc: p) {
                std::cout << utils::print_congruence_class(*conc);
            }
            std::cout << "==============================================================" << std::endl;
        }
        std::cout << "***********第" << iter_counts << "轮迭代结束**************" << std::endl;
    }while(changed);
    
}


shared_ptr<Expression> GVN::valueExpr(Instruction *instr, partitions p) {
    
    // TODO


    // 处理二元或一元操作
    Value* lval ;
    Value* rval ;

    std::shared_ptr<Expression> lvar_expr;
    std::shared_ptr<Expression> rvar_expr;
    std::shared_ptr<Expression> arg_expr;
    std::shared_ptr<Expression> operand_expr;
    Function* func;
    std::vector<std::shared_ptr<Expression>> operands;
    std::vector<std::shared_ptr<Expression>> args;

    if(instr->get_num_operand() ==  2) {
        lval = instr->get_operand(0);
        rval = instr->get_operand(1);
        lvar_expr = getVarExpr(lval, p);
        rvar_expr = getVarExpr(rval, p);
        if(!lvar_expr || !rvar_expr)
            return 0;
    }
    else if(instr->get_num_operand() ==  1){
        lval = instr->get_operand(0);
        lvar_expr = getVarExpr(lval, p);
        if(!lvar_expr)
            return 0;
    }

    switch (instr->get_instr_type()){

        case Instruction::add:

            if(lvar_expr->get_expr_type() == Expression::e_constant && rvar_expr->get_expr_type() == Expression::e_constant)  
                return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant(), (static_cast<ConstantExpression*>(rvar_expr.get()))->get_constant()));
            else 
                return BinaryExpression::create(Instruction::add, lvar_expr, rvar_expr);
            break;

        case Instruction::sub:

            if(lvar_expr->get_expr_type() == Expression::e_constant && rvar_expr->get_expr_type() == Expression::e_constant)  
                return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant(), (static_cast<ConstantExpression*>(rvar_expr.get()))->get_constant()));
            else 
                return BinaryExpression::create(Instruction::sub, lvar_expr, rvar_expr);
            break;

        case Instruction::mul:

            if(lvar_expr->get_expr_type() == Expression::e_constant && rvar_expr->get_expr_type() == Expression::e_constant)  
                return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant(), (static_cast<ConstantExpression*>(rvar_expr.get()))->get_constant()));
            else 
                return BinaryExpression::create(Instruction::mul, lvar_expr, rvar_expr);
            break;

        case Instruction::sdiv:
            if(lvar_expr->get_expr_type() == Expression::e_constant && rvar_expr->get_expr_type() == Expression::e_constant)  
                return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant(), (static_cast<ConstantExpression*>(rvar_expr.get()))->get_constant()));
            else 
                return BinaryExpression::create(Instruction::sdiv, lvar_expr, rvar_expr);
            break;

        case Instruction::fadd:
            if(lvar_expr->get_expr_type() == Expression::e_constant && rvar_expr->get_expr_type() == Expression::e_constant)  
                return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant(), (static_cast<ConstantExpression*>(rvar_expr.get()))->get_constant()));
            else 
                return BinaryExpression::create(Instruction::fadd, lvar_expr, rvar_expr);
            break;
        case Instruction::fsub:
            if(lvar_expr->get_expr_type() == Expression::e_constant && rvar_expr->get_expr_type() == Expression::e_constant)  
                return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant(), (static_cast<ConstantExpression*>(rvar_expr.get()))->get_constant()));
            else 
                return BinaryExpression::create(Instruction::fsub, lvar_expr, rvar_expr);
            break;
        case Instruction::fmul:
            if(lvar_expr->get_expr_type() == Expression::e_constant && rvar_expr->get_expr_type() == Expression::e_constant)  
                return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant(), (static_cast<ConstantExpression*>(rvar_expr.get()))->get_constant()));
            else 
                return BinaryExpression::create(Instruction::fmul, lvar_expr, rvar_expr);
            break;
        case Instruction::fdiv:
            if(lvar_expr->get_expr_type() == Expression::e_constant && rvar_expr->get_expr_type() == Expression::e_constant)  
                return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant(), (static_cast<ConstantExpression*>(rvar_expr.get()))->get_constant()));
            else 
                return BinaryExpression::create(Instruction::fdiv, lvar_expr, rvar_expr);
            break;

        case Instruction::cmp:
            switch(dynamic_cast<CmpInst *>(instr)->get_cmp_op()) {
                case CmpInst::EQ:
                    if(lvar_expr->get_expr_type() == Expression::e_constant && rvar_expr->get_expr_type() == Expression::e_constant)  
                        return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant(), (static_cast<ConstantExpression*>(rvar_expr.get()))->get_constant()));
                    else 
                        return CmpExpression::create(CmpInst::EQ, lvar_expr, rvar_expr);
                    break;
                case CmpInst::NE:
                    if(lvar_expr->get_expr_type() == Expression::e_constant && rvar_expr->get_expr_type() == Expression::e_constant)  
                        return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant(), (static_cast<ConstantExpression*>(rvar_expr.get()))->get_constant()));
                    else 
                        return CmpExpression::create(CmpInst::NE, lvar_expr, rvar_expr);
                    break;

                case CmpInst::GT:
                    if(lvar_expr->get_expr_type() == Expression::e_constant && rvar_expr->get_expr_type() == Expression::e_constant)  
                        return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant(), (static_cast<ConstantExpression*>(rvar_expr.get()))->get_constant()));
                    else 
                        return CmpExpression::create(CmpInst::GT, lvar_expr, rvar_expr);
                    break;
                case CmpInst::GE:
                    if(lvar_expr->get_expr_type() == Expression::e_constant && rvar_expr->get_expr_type() == Expression::e_constant)  
                        return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant(), (static_cast<ConstantExpression*>(rvar_expr.get()))->get_constant()));
                    else 
                        return CmpExpression::create(CmpInst::GE, lvar_expr, rvar_expr);
                    break;
                case CmpInst::LT:
                    if(lvar_expr->get_expr_type() == Expression::e_constant && rvar_expr->get_expr_type() == Expression::e_constant)  
                        return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant(), (static_cast<ConstantExpression*>(rvar_expr.get()))->get_constant()));
                    else 
                        return CmpExpression::create(CmpInst::LT, lvar_expr, rvar_expr);
                    break;
                case CmpInst::LE:
                    if(lvar_expr->get_expr_type() == Expression::e_constant && rvar_expr->get_expr_type() == Expression::e_constant)  
                        return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant(), (static_cast<ConstantExpression*>(rvar_expr.get()))->get_constant()));
                    else 
                        return CmpExpression::create(CmpInst::LE, lvar_expr, rvar_expr);
                    break;
            }
            break;
        case Instruction::fcmp:
            switch(dynamic_cast<FCmpInst *>(instr)->get_cmp_op()) {
                case FCmpInst::EQ:
                    if(lvar_expr->get_expr_type() == Expression::e_constant && rvar_expr->get_expr_type() == Expression::e_constant)  
                        return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant(), (static_cast<ConstantExpression*>(rvar_expr.get()))->get_constant()));
                    else 
                        return FCmpExpression::create(FCmpInst::EQ, lvar_expr, rvar_expr);
                    break;
                case FCmpInst::NE:
                    if(lvar_expr->get_expr_type() == Expression::e_constant && rvar_expr->get_expr_type() == Expression::e_constant)  
                        return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant(), (static_cast<ConstantExpression*>(rvar_expr.get()))->get_constant()));
                    else 
                        return FCmpExpression::create(FCmpInst::NE, lvar_expr, rvar_expr);
                    break;
                case FCmpInst::GT:
                    if(lvar_expr->get_expr_type() == Expression::e_constant && rvar_expr->get_expr_type() == Expression::e_constant)  
                        return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant(), (static_cast<ConstantExpression*>(rvar_expr.get()))->get_constant()));
                    else 
                        return FCmpExpression::create(FCmpInst::GT, lvar_expr, rvar_expr);
                    break;
                case FCmpInst::GE:
                    if(lvar_expr->get_expr_type() == Expression::e_constant && rvar_expr->get_expr_type() == Expression::e_constant)  
                        return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant(), (static_cast<ConstantExpression*>(rvar_expr.get()))->get_constant()));
                    else 
                        return FCmpExpression::create(FCmpInst::GE, lvar_expr, rvar_expr);
                    break;
                case FCmpInst::LT:
                    if(lvar_expr->get_expr_type() == Expression::e_constant && rvar_expr->get_expr_type() == Expression::e_constant)  
                        return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant(), (static_cast<ConstantExpression*>(rvar_expr.get()))->get_constant()));
                    else 
                        return FCmpExpression::create(FCmpInst::LT, lvar_expr, rvar_expr);
                    break;
                case FCmpInst::LE:
                    if(lvar_expr->get_expr_type() == Expression::e_constant && rvar_expr->get_expr_type() == Expression::e_constant)  
                        return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant(), (static_cast<ConstantExpression*>(rvar_expr.get()))->get_constant()));
                    else 
                        return FCmpExpression::create(FCmpInst::LE, lvar_expr, rvar_expr);
                    break;
            }

        case Instruction::zext:
            if(lvar_expr->get_expr_type() == Expression::e_constant)  
                return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant()));
            else 
                return BinaryExpression::create(Instruction::zext, lvar_expr, 0);
             
            break;

        case Instruction::fptosi:
            if(lvar_expr->get_expr_type() == Expression::e_constant)  
                return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant()));
            else 
                return BinaryExpression::create(Instruction::fptosi, lvar_expr, 0);
             
            break;

        case Instruction::sitofp:
            if(lvar_expr->get_expr_type() == Expression::e_constant)  
                return ConstantExpression::create(folder_->compute(instr, (static_cast<ConstantExpression*>(lvar_expr.get()))->get_constant()));
            else 
                return BinaryExpression::create(Instruction::sitofp, lvar_expr, 0);
             
            break;


        case Instruction::load:
            return 0;
            break;

        case Instruction::call:
            func = static_cast<Function*>(instr->get_operand(0));
            if(! (func_info_->is_pure_function(func))) 
                return 0;
            else {
                auto first = true;
                for(auto& operand: instr->get_operands()) {
                    if(first) {
                        first = false;
                        continue;
                    }
                    else {
                        arg_expr = getVarExpr(operand, p);
                        args.push_back(arg_expr);
                    }
                }
                return CallExpression::create(func, args);
            }
            break;

        case Instruction::getelementptr:
            
            
            for(auto& operand: instr->get_operands()) {

                operand_expr = getVarExpr(operand, p);
                operands.push_back(operand_expr);  
            }
            return GepExpression::create(operands);
            break;

        default:
            break;
    }
}

std::shared_ptr<GVNExpression::Expression> GVN::getVarExpr(Value* val, partitions p) {
    auto constant_ptr = dynamic_cast<Constant*>(val);
    if(constant_ptr) {
        return ConstantExpression::create(constant_ptr);
    }
    else {
        for(auto &conc: p) {
            if(((conc.get())->members_).find(val) != ((conc.get())->members_).end()) {
                return (conc.get())->value_expr_;
            }
        }
    } 
    return 0;
}

// instruction of the form `x = e`, mostly x is just e (SSA), but for copy stmt x is a phi instruction in the
// successor. Phi values (not copy stmt) should be handled in detectEquiv
/// \param bb basic block in which the transfer function is called
GVN::partitions GVN::transferFunction(Instruction *x, partitions pin, BasicBlock* bb) {
    partitions pout = clone(pin);

    // TODO: get different ValueExpr by Instruction::OpID, modify pout

    // std::cout << "************Before TransferFunction***********"<< std::endl;
    // for(auto &conc: pin) {
    //     std::cout << utils::print_congruence_class(*conc);
    // }
    // std::cout << std::endl;
    // 先删除指令
    auto iter = pout.begin();
    while(iter != pout.end()) {
        if(((*iter).get())->members_.find(x) != ((*iter).get())->members_.end()) {
            ((*iter).get())->members_.erase(x);
            if(((*iter).get())->members_.size() == 0) {
                pout.erase(*iter);
            }
        }
        iter++;
    }

    auto ve = valueExpr(x, pout); 
    shared_ptr<PhiExpression> vpf;
    if((bb->get_pre_basic_blocks()).size() == 2) 
        vpf = valuePhiFunc(ve, pin, bb, x);
    else 
        vpf = nullptr;

    // 打印指令对应的表达式
    std::cout << Instruction::get_instr_op_name(x->get_instr_type());
    if(ve)
        std::cout << ": ( " +  ve->print() + " ) ";
    else 
        std::cout << ": ( NOT FOUND ) ";
    std::cout << std::endl;

    // 已有等价类,直接插入
    iter = pout.begin();
    while(iter != pout.end()) {
        
        if(ve && *(ve.get()) == *(((*iter).get())->value_expr_)) {
            ((*iter).get())->members_.insert(x);
            break;
        }
        else if(vpf && ((*iter).get())->value_phi_ && *(vpf.get()) == *(((*iter).get())->value_phi_.get())) {
            ((*iter).get())->members_.insert(x);
            break;
        }
        iter ++;
    }

    // 没有对应的等价类,创建一个
    if(iter == pout.end()) {
        auto conc = std::make_shared<CongruenceClass>(next_value_number_++);  
        // 确定leader_
        if(ve && ve->get_expr_type() == Expression::e_constant) 
            conc->leader_ = (static_cast<ConstantExpression*>(ve.get()))->get_constant();
        else 
            conc->leader_ = x;
        // 插入members_
        conc->members_.insert(x);
        // 确定value_expr_
        if(!ve) {
            conc->value_expr_ = VarExpression::create(x);
        }
        else 
            conc->value_expr_ = ve;
        // 确定value_phi_
        if(vpf)
            conc->value_phi_ = vpf;
        pout.insert(conc);
    }

    // std::cout << "************After TransferFunction***********" << std::endl;
    // for(auto &conc: pout) {
    //     std::cout << utils::print_congruence_class(*conc);
    // }
    // std::cout << std::endl;
    return pout;
}

GVN::partitions GVN::transferFunction(partitions pin, BasicBlock* bb, bool init_global) {
    partitions pout = clone(pin);

    if(init_global) {
        // 初始化全局变量
        for(auto& global_val: m_->get_global_variable()) {
            auto conc = std::make_shared<CongruenceClass>(next_value_number_++);  
            // 确定leader_
            conc->leader_ = &global_val;
            // 插入members_
            conc->members_.insert(&global_val);
            // 确定value_expr_
            conc->value_expr_ = VarExpression::create(&global_val);
            // 确定value_phi_
            conc->value_phi_ = nullptr;
            pout.insert(conc);
        }
        // 初始化函数参数
        for(auto& fun_arg: func_->get_args()) {
            auto conc = std::make_shared<CongruenceClass>(next_value_number_++);  
            // 确定leader_
            conc->leader_ = fun_arg;
            // 插入members_
            conc->members_.insert(fun_arg);
            // 确定value_expr_
            conc->value_expr_ = VarExpression::create(fun_arg);
            // 确定value_phi_
            conc->value_phi_ = nullptr;
            pout.insert(conc);
        }
    }

    // 处理bb内的所有instructions
    for(auto &instr : bb->get_instructions()) {
        if(instr.is_void() || instr.is_phi() || instr.is_alloca()) {

            std::cout << Instruction::get_instr_op_name(instr.get_instr_type())  
                      << ": ( PASS )" 
                      << std::endl;

            continue;
        }
        pout = transferFunction(&instr, pout, bb);
    }

    // 处理bb后继块中所有的phi指令
    for(auto &bbs: bb->get_succ_basic_blocks()) {
        for(auto &instr :bbs->get_instructions()) {
            if(instr.is_phi()) {
                auto lvar_ptr = (static_cast<PhiInst*>(&instr))->get_operand(0);
                auto lvar_bb = (static_cast<PhiInst*>(&instr))->get_operand(1);
                auto rvar_ptr = (static_cast<PhiInst*>(&instr))->get_operand(2);

                for(auto &bb_pre: bbs->get_pre_basic_blocks()) {
                    Value* var_ptr;
                    if((static_cast<BasicBlock*>(lvar_bb)) == bb_pre) 
                        var_ptr = lvar_ptr;
                    else 
                        var_ptr = rvar_ptr;

                    if(bb_pre == bb) {
                        if(pout.empty()) {
                            if(dynamic_cast<Constant*>(var_ptr)){
                                auto conc = std::make_shared<CongruenceClass>(next_value_number_++);  
                                // 确定leader_
                                conc->leader_ = static_cast<Constant*>(var_ptr);
                                // 插入members_
                                conc->members_.insert(&instr);
                                // 确定value_expr_
                                conc->value_expr_ = ConstantExpression::create(static_cast<Constant*>(var_ptr));
                                // 确定value_phi_
                                conc->value_phi_ = nullptr;
                                pout.insert(conc);
                            }
                        }
                        else {
                            // 先在前驱块中含有该指令的等价类中去除该指令
                            for(auto &conc_ptr:pout) {
                                if(conc_ptr->members_.find(&instr) != conc_ptr->members_.end()) {
                                    conc_ptr->members_.erase(&instr);
                                    if(conc_ptr->members_.empty()) {
                                        pout.erase(conc_ptr);
                                    }
                                }
                            }

                            bool found = false;
                            for(auto &conc_ptr: pout) {
                                // lvar_ptr可能因为 mem2reg被上一个块删除, 这时要为上一个块补充创建等价类
                                if(conc_ptr->members_.find(var_ptr) != conc_ptr->members_.end()) {
                                    conc_ptr->members_.insert(&instr);
                                    found = true;
                                    break;
                                }
                                else if(var_ptr == conc_ptr->leader_) {
                                    conc_ptr->members_.insert(&instr);
                                    found = true;
                                    break;
                                }
                            }
                            if(!found && dynamic_cast<Constant*>(var_ptr)) {
                                
                                auto conc = std::make_shared<CongruenceClass>(next_value_number_++);  
                                // 确定leader_
                                conc->leader_ = static_cast<Constant*>(var_ptr);
                                // 插入members_
                                conc->members_.insert(&instr);
                                // 确定value_expr_
                                conc->value_expr_ = ConstantExpression::create(static_cast<Constant*>(var_ptr));
                                // 确定value_phi_
                                conc->value_phi_ = nullptr;
                                pout.insert(conc);
                            }
                        }
                    }

                    else {
                        auto &pout_pre = pout_[bb_pre];
                        
                        for(auto &conc_ptr:pout_pre) {
                            if(conc_ptr->members_.find(&instr) != conc_ptr->members_.end()) {
                                conc_ptr->members_.erase(&instr);
                                if(conc_ptr->members_.empty()) {
                                    pout.erase(conc_ptr);
                                }
                            }
                        }

                        for(auto &conc_ptr: pout_pre) {
                            // lvar_ptr可能因为 mem2reg被上一个块删除, 这时要为上一个块补充创建等价类
                            if(conc_ptr->members_.find(var_ptr) != conc_ptr->members_.end()) {
                                conc_ptr->members_.insert(&instr);
                                break;
                            }
                            else if(var_ptr == conc_ptr->leader_) {
                                conc_ptr->members_.insert(&instr);
                                break;
                            }
                        }
                    }
                }
            }  
            else 
                break;  
        }
    }
    return pout;
}

shared_ptr<PhiExpression> GVN::valuePhiFunc(shared_ptr<Expression> ve, const partitions &P, BasicBlock* bb, Instruction* x) {
    // TODO

    // 判断是否符合  φk(vi1, vj1) op φk(vi2, vj2)的形式

    if(! ve)
        return 0;

    BinaryExpression* bi_expression = dynamic_cast<BinaryExpression*>(ve.get());

    if(bi_expression) {
        PhiExpression* lhs_phi = dynamic_cast<PhiExpression*>(bi_expression->get_lhs().get());
        PhiExpression* rhs_phi = dynamic_cast<PhiExpression*>(bi_expression->get_rhs().get());

        if(lhs_phi && rhs_phi) {
            auto vi1 = lhs_phi->get_lhs();
            auto vj1 = lhs_phi->get_rhs();
            auto vi2 = rhs_phi->get_lhs();
            auto vj2 = rhs_phi->get_rhs();

            

            std::shared_ptr<Expression> vi,vj;

            if(!(vi1 && vj1 && vi2 && vj2))
                return 0;
            std::shared_ptr<BinaryExpression> bi_expression1,bi_expression2;
            std::shared_ptr<ConstantExpression> con_expression1,con_expression2;

            if(vi1->get_expr_type() == Expression::e_constant && vi2->get_expr_type() == Expression::e_constant) {
                con_expression1 = ConstantExpression::create(folder_->compute(x, (static_cast<ConstantExpression*>(vi1.get()))->get_constant(), (static_cast<ConstantExpression*>(vi2.get()))->get_constant()));
                vi = getVN(pout_[*((bb->get_pre_basic_blocks()).begin())], con_expression1);
            }
            else {
                bi_expression1 = BinaryExpression::create(bi_expression->get_op_type(), vi1, vi2);
                vi = getVN(pout_[*((bb->get_pre_basic_blocks()).begin())], bi_expression1);
            }

            if(!vi) {
                vi = valuePhiFunc(std::shared_ptr<BinaryExpression>(bi_expression1), pout_[*((bb->get_pre_basic_blocks()).begin())], *((bb->get_pre_basic_blocks()).begin()), x);
            } 

            if(vj1->get_expr_type() == Expression::e_constant && vj2->get_expr_type() == Expression::e_constant) {
                con_expression2 = ConstantExpression::create(folder_->compute(x, (static_cast<ConstantExpression*>(vj1.get()))->get_constant(), (static_cast<ConstantExpression*>(vj2.get()))->get_constant()));
                vj = getVN(pout_[*(++((bb->get_pre_basic_blocks()).begin()))], con_expression2);
            }
            else {
                bi_expression2 = BinaryExpression::create(bi_expression->get_op_type(), vj1, vj2);
                vj = getVN(pout_[*(++((bb->get_pre_basic_blocks()).begin()))], bi_expression2);
            }

            if(!vj ) {
                vj = valuePhiFunc(std::shared_ptr<BinaryExpression>(bi_expression2), pout_[*(++((bb->get_pre_basic_blocks()).begin()))], *(++((bb->get_pre_basic_blocks()).begin())), x);
            } 

            if(vi && vj)
                return std::shared_ptr<PhiExpression>(PhiExpression::create(vi, vj));
        }
    }
    return 0;
}

shared_ptr<Expression> GVN::getVN(const partitions &pout, shared_ptr<Expression> ve) {
    // TODO: return what?
    for (auto it = pout.begin(); it != pout.end(); it++)
        if ((*it)->value_expr_ and * (*it)->value_expr_ == *ve)
            return (*it)->value_expr_;
    return 0;
}

void GVN::initPerFunction() {
    next_value_number_ = 1;
    pin_.clear();
    pout_.clear();
}

void GVN::replace_cc_members() {
    for (auto &[_bb, part] : pout_) {
        auto bb = _bb; // workaround: structured bindings can't be captured in C++17
        for (auto &cc : part) {
            if (cc->index_ == 0)
                continue;
            // if you are planning to do constant propagation, leaders should be set to constant at some point
            for (auto &member : cc->members_) {
                bool member_is_phi = dynamic_cast<PhiInst *>(member);
                bool value_phi = cc->value_phi_ != nullptr;
                if (member != cc->leader_ and (value_phi or !member_is_phi)) {
                    // only replace the members if users are in the same block as bb
                    member->replace_use_with_when(cc->leader_, [bb](User *user) {
                        if (auto instr = dynamic_cast<Instruction *>(user)) {
                            auto parent = instr->get_parent();
                            auto &bb_pre = parent->get_pre_basic_blocks();
                            if (instr->is_phi()) // as copy stmt, the phi belongs to this block
                                return std::find(bb_pre.begin(), bb_pre.end(), bb) != bb_pre.end();
                            else
                                return parent == bb;
                        }
                        return false;
                    });
                }
            }
        }
    }
    return;
}

// top-level function, done for you
void GVN::run() {
    std::ofstream gvn_json;
    if (dump_json_) {
        gvn_json.open("gvn.json", std::ios::out);
        gvn_json << "[";
    }

    folder_ = std::make_unique<ConstFolder>(m_);
    func_info_ = std::make_unique<FuncInfo>(m_);
    func_info_->run();
    dce_ = std::make_unique<DeadCode>(m_);
    dce_->run(); // let dce take care of some dead phis with undef

    auto IRT = m_->print();
    std::string target_path = "test_mem2reg.ll";
    std::ofstream output_stream;
    output_stream.open(target_path, std::ios::out);
    output_stream << "; ModuleID = 'cminus'\n";
    output_stream << "source_filename = \"" << "test.cminus" << "\"\n\n";
    output_stream << IRT;
    output_stream.close();

    for (auto &f : m_->get_functions()) {
        if (f.get_basic_blocks().empty())
            continue;
        func_ = &f;
        initPerFunction();
        LOG_INFO << "Processing " << f.get_name();
        detectEquivalences();
        LOG_INFO << "===============pin=========================\n";
        for (auto &[bb, part] : pin_) {
            LOG_INFO << "\n===============bb: " << bb->get_name() << "=========================\npartitionIn: ";
            for (auto &cc : part)
                LOG_INFO << utils::print_congruence_class(*cc);
        }
        LOG_INFO << "\n===============pout=========================\n";
        for (auto &[bb, part] : pout_) {
            LOG_INFO << "\n=====bb: " << bb->get_name() << "=====\npartitionOut: ";
            for (auto &cc : part)
                LOG_INFO << utils::print_congruence_class(*cc);
        }
        if (dump_json_) {
            gvn_json << "{\n\"function\": ";
            gvn_json << "\"" << f.get_name() << "\", ";
            gvn_json << "\n\"pout\": " << utils::dump_bb2partition(pout_);
            gvn_json << "},";
        }
        replace_cc_members(); // don't delete instructions, just replace them
    }
    dce_->run(); // let dce do that for us
    if (dump_json_)
        gvn_json << "]";
}

template <typename T>
static bool equiv_as(const Expression &lhs, const Expression &rhs) {
    // we use static_cast because we are very sure that both operands are actually T, not other types.
    return static_cast<const T *>(&lhs)->equiv(static_cast<const T *>(&rhs));
}

bool GVNExpression::operator==(const Expression &lhs, const Expression &rhs) {
    if (lhs.get_expr_type() != rhs.get_expr_type())
        return false;
    switch (lhs.get_expr_type()) {
        case Expression::e_constant: return equiv_as<ConstantExpression>(lhs, rhs);
        case Expression::e_bin: return equiv_as<BinaryExpression>(lhs, rhs);
        case Expression::e_phi: return equiv_as<PhiExpression>(lhs, rhs);
        case Expression::e_svar: return equiv_as<VarExpression>(lhs, rhs);
        case Expression::e_cmp: return equiv_as<CmpExpression>(lhs, rhs);
        case Expression::e_fcmp: return equiv_as<FCmpExpression>(lhs, rhs);
        case Expression::e_gep: return equiv_as<GepExpression>(lhs, rhs);
        case Expression::e_call: return equiv_as<CallExpression>(lhs, rhs);
    }
}

bool GVNExpression::operator==(const shared_ptr<Expression> &lhs, const shared_ptr<Expression> &rhs) {
    if (lhs == nullptr and rhs == nullptr) // is the nullptr check necessary here?
        return true;
    return lhs and rhs and *lhs == *rhs;
}

GVN::partitions GVN::clone(const partitions &p) {
    partitions data;
    for (auto &cc : p) {
        data.insert(std::make_shared<CongruenceClass>(*cc));
    }
    return data;
}

bool operator==(const GVN::partitions &p1, const GVN::partitions &p2) {
    // TODO: how to compare partitions?
    bool flag = false;

    if(p1.size() == p2.size()) {
        auto iter1 = p1.begin();
        auto iter2 = p2.begin();
        while(iter1 != p1.end() && iter2 != p2.end()) {
            if(*((*iter1).get()) != *((*iter2).get()))
                break;
            ++iter1;
            ++iter2;
        }
        flag = (iter1 == p1.end()) && (iter2 == p2.end());
    }  

    return flag;
}


bool operator!=(const GVN::partitions &p1, const GVN::partitions &p2) {
    // TODO: how to compare partitions?
    return !(p1 == p2);
}


bool CongruenceClass::operator==(const CongruenceClass &other) const {
    // TODO: which fields need to be compared?
    bool flag = false;

    if((this->leader_ == other.leader_)) {
         
        auto iter1 = this->members_.begin();
        auto iter2 = other.members_.begin();
        while(iter1 != this->members_.end() && iter2 != other.members_.end()) {
            if((*iter1) != (*iter2))
                break;
            ++iter1;
            ++iter2;
        }
        flag = (iter1 == this->members_.end()) && (iter2 == other.members_.end());
        
    }
    return flag;
}

bool operator==(const CongruenceClass &c1, const CongruenceClass &c2) {
    return c1.operator==(c2);
}

bool operator!=(const CongruenceClass &c1, const CongruenceClass &c2) {
    return !(c1.operator==(c2));
}

bool GVN::is_top(const partitions& p) {
    return (p.size() == 1) && (((*p.begin()).get())->index_ == 0);
}

bool GVN::calculate_quivalences() {
    bool changed = false;
    partitions pin1 = {};

    partitions pout1 = transferFunction(pin1, func_->get_entry_block(), true);

    for(auto &bb : func_->get_basic_blocks()) {
        if(&bb == (func_->get_entry_block())) {
            pout_[&bb] = pout1;
            continue;
        } 

        partitions pin,pout;
        std::cout << "Instruction counts: " << bb.get_instructions().size() << std::endl;
        assert(bb.get_pre_basic_blocks().size() > 0);
        
        if(bb.get_pre_basic_blocks().size() == 2) {
            auto iter1 = bb.get_pre_basic_blocks().begin();
            auto iter2 = ++ bb.get_pre_basic_blocks().begin();
            std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
            std::cout << "FOR JOIN " << bb.get_name() << std::endl;
            pin = join(pout_[*iter1], pout_[*iter2]);
        }   
        else {
            auto iter1 = bb.get_pre_basic_blocks().begin();
            std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
            std::cout << "NO JOIN " << bb.get_name() << std::endl;
            std::cout << "P1:" << std::endl;
            for(auto &conc: (pout_[*iter1])) {
                std::cout << utils::print_congruence_class(*conc);
            }
            std::cout << std::endl;
   
            pin = pout_[*iter1];
        }
        pout = transferFunction(pin, &bb, false);
        if(pout != pout_[&bb]) {
            pout_[&bb].clear();
            pout_[&bb].insert(pout.begin(), pout.end());
            changed = true;
        }
    }
    return changed;
}