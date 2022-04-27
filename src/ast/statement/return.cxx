#include <ast/statement/return.hxx>

// helper type for the visitor #4
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

return_statement::return_statement()
{
}

return_statement::return_statement(expression expr)
    :value(expr)
{
}

return_statement::return_statement(const return_statement& other)
{
    value = other.value;
}

std::ostream& operator<<(std::ostream& stream, return_statement stmt)
{
    stream << "return-statement\n\tvalue ";
    std::visit(overloaded {
        [](auto&& arg) {},
        [&stream](int_literal_expression arg) { stream << arg.value; }
    }, stmt.value);
    return stream;
}