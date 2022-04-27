#include "ast/expression/expression.hxx"
#include "ast/expression/literal/integer.hxx"
#include <emitter.hxx>

// helper type for the visitor #4
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

emitter::emitter()
{
}

std::string emitter::emit(prgm_node* root, std::string filename)
{
    std::string result = "\t.text\n";
    for(top_level_node node : root->children)
    {
        std::visit(overloaded {
            [](auto&& arg) { throw; },
            [&result, this](func_decl_node arg) { result += emit_function(arg); }
        }, node);
    }
    return result;
}

std::string emitter::emit_function(func_decl_node node)
{
    std::string result = "\t.globl " + node.name;
    result += "\n\t.type " + node.name + ", @function\n";
    result += node.name + ":\n\tpushq %rbp\n\tmovq %rsp, %rbp\n\t";

    for(statement stmt : node.body.statements)
    {
        std::visit(overloaded {
            [](auto&& arg) { throw; },
            [&result, &node, this](return_statement arg) { result += emit_return(arg, node.return_type); },
        }, stmt);
    }

    result += ".size " + node.name + ", .-" + node.name;
    return result + "\n";
}

std::string emitter::emit_return(return_statement stmt, type_info type)
{
    std::string result;
    char mov_instr[5] = {'m', 'o', 'v', 0, 0};

    if (type.name != types.at("void").name)
    {
        switch(type.size)
        {
            case 1:
                mov_instr[3] = 'b';
                break;
            case 2:
                mov_instr[3] = 'w';
                break;
            case 4:
                mov_instr[3] = 'l';
                break;
            case 8:
                mov_instr[3] = 'q';
                break;
            default:
                break;
        }

        std::visit(overloaded {
            [](auto&& arg) { throw; },
            [&result, &mov_instr](int_literal_expression arg) { result += std::string(mov_instr) + std::string(" $") + std::to_string(arg.value) + ", %eax\n\t"; },
            [&result, &mov_instr, this](binary_operator_expression arg) { result += std::string(mov_instr) + std::string(" $") + std::to_string(emit_binary_operator(arg)) + ", %eax\n\t"; }
        }, stmt.value);
    }
    result += "popq %rbp\n\tret\n\t";
    return result;
}

int emitter::emit_binary_operator(binary_operator_expression expr)
{
    int_literal_expression lhs;
    int_literal_expression rhs;
    std::visit(overloaded {
        [](auto&& arg) { throw; },
        [&lhs](int_literal_expression literal) { lhs = literal; },
        [&lhs, this](binary_operator_expression ex) { lhs = emit_binary_operator(ex); }
    }, *expr.lhs);
    std::visit(overloaded {
        [](auto&& arg) { throw; },
        [&rhs](int_literal_expression literal) { rhs = literal; },
        [&rhs, this](binary_operator_expression ex) { rhs = emit_binary_operator(ex); }
    }, *expr.rhs);

    int_literal_expression total;;
    switch(expr.type)
    {
        case binary_operand::addition:
            total = int_literal_expression(lhs.value + rhs.value);
            break;
        case binary_operand::subtraction:
            total = int_literal_expression(lhs.value - rhs.value);
            break;
        case binary_operand::multiplication:
            total = int_literal_expression(lhs.value * rhs.value);
            break;
        case binary_operand::division:
            total = int_literal_expression(lhs.value / rhs.value);
            break;
        default:
            break;
    }

    return total.value;
}