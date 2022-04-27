#include <ast/expression/binary_operator.hxx>
#include <ast/expression/expr.hxx>

binary_operator_expression::binary_operator_expression(binary_operand type)
    :type(type)
{

}

binary_operator_expression::binary_operator_expression(token tok)
{
    switch (tok.type)
    {
        case token_type::plus:
            type = binary_operand::addition;
            break;
        case token_type::minus:
            type = binary_operand::subtraction;
            break;
        case token_type::star:
            type = binary_operand::multiplication;
            break;
        case token_type::slash:
            type = binary_operand::division;
            break;
        default:
            break;
    }
}

binary_operator_expression::binary_operator_expression(const binary_operator_expression& other)
{
    type = other.type;
    lhs = std::make_unique<expression>(*other.lhs);
    rhs = std::make_unique<expression>(*other.rhs);
}

binary_operator_expression& binary_operator_expression::operator=(const binary_operator_expression& other)
{
    type = other.type;
    lhs = std::make_unique<expression>(*other.lhs);
    rhs = std::make_unique<expression>(*other.rhs);
    return *this;
}