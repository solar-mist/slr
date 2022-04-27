#include "emitter.hxx"
#include <fstream>
#include <sstream>
#include <iostream>
#include <lexer.hxx>
#include <parser.hxx>

// helper type for the visitor #4
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        std::ifstream file(argv[1]);
        std::ofstream out(std::string(argv[1]) + ".s");
        std::stringstream buf;
        buf << file.rdbuf();
        type_info::init_default_types();
        lexer lex(buf.str());
        parser parse(lex.lex());
        emitter em;
        /*auto tree = parse.parse();
        for(auto child : tree->children)
        {
            std::visit(overloaded {
                [](auto&& arg){},
                [](func_decl_node arg){std::cout << arg << std::endl;}
            }, child);
        }*/
        out << em.emit(parse.parse(), argv[1]);
        return 0;
    }
    return 1;
}