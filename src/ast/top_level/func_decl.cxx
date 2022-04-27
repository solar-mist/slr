#include <ast/top_level/func_decl.hxx>
#include <iostream>

// helper type for the visitor #4
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

func_decl_node::func_decl_node(type_info type, std::string_view name, std::vector<func_arg_node> args, func_body_node body)
    :return_type(type), name(name), args(args), body(body)
{
}

std::ostream& operator<<(std::ostream& stream, func_decl_node node)
{
    stream << "function-decl\n";
    stream << "\treturn-type: " << node.return_type.name << "\n";
    stream << "\targs\n";
    for (func_arg_node arg : node.args)
    {
        stream << "\t\tdecl\n";
        stream << "\t\t\ttype " << arg.type.name << "\n";
        stream << "\t\t\tname " << arg.name << "\n";
    }
    stream << "\tname " << node.name << "\n";
    stream << "\tbody\n";
    for (statement stmt : node.body.statements)
    {
        std::visit(overloaded {
            [](auto&& arg) {},
            [&stream](return_statement arg) { stream << arg; }
        }, stmt);
    }
    return stream;
}