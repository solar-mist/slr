#include "ast/expression/expression.hxx"
#include "ast/expression/literal/integer.hxx"
#include "token.hxx"
#include <exception>
#include <iostream>
#include <memory>
#include <optional>
#include <parser.hxx>
#include <typing/types.hxx>

parser::parser(const std::vector<token>& tokens)
    :tokens(tokens), position(0)
{
}

token parser::current()
{
    return tokens[position];
}
token parser::consume()
{
    return tokens[position++];
}
token parser::peek(int offset)
{
    return tokens[position + offset];
}

void parser::expect_token(token_type T)
{
    if (current().type != T)
    {
        token temp(T, "");
        std::cerr << "\u001b[31mUnexpected token: " << current() << ". Expected: " << temp.get_type() << "\u001b[0m" << std::endl;
        std::exit(1);
    }
}

prgm_node* parser::parse()
{
    prgm_node* root = new prgm_node();
    while(position < tokens.size())
    {
        root->children.push_back(parse_top_level());
    }
    return root;
}

top_level_node parser::parse_top_level()
{
    if (peek(2).type == open_paren)
        return parse_func();
    throw;
}

func_decl_node parser::parse_func()
{
    type_info type;
    expect_token(token_type::identifier);
    type = types.at(consume().text);
    
    std::string name;
    expect_token(token_type::identifier);
    name = consume().text;
    expect_token(token_type::open_paren);
    std::vector<func_arg_node> args;
    while(consume().type != close_paren)
    {
        std::optional<func_arg_node> arg = parse_arg();
        if(arg.has_value())
            args.push_back(arg.value());
    }
    expect_token(token_type::open_bracket);
    func_body_node body;
    while(consume().type != close_bracket)
    {
        body.statements.push_back(parse_statement());
    }

    func_decl_node result = func_decl_node(type, name, args, body);
    return result;
}

std::optional<func_arg_node> parser::parse_arg()
{
    if(current().type == token_type::close_paren)
        return std::nullopt;
    /*bool is_array = false;
    std::string base_type = consume().text;
    if(current().type == open_sq_bracket)
    {
        consume();
        if (current().type == close_sq_bracket)
        {
            consume();
            is_array = true;
        }
    }
    if (base_type == "string")
    {
        return is_array ? func_arg_node(string_array, consume().text) : func_arg_node(string, consume().text);
    }
    else if (base_type == "int")
    {
        return is_array ? func_arg_node(integer_array, consume().text) : func_arg_node(integer, consume().text);
    }
    throw;*/
    return std::nullopt;
}

statement parser::parse_statement()
{
    if(current().text == "return")
    {
        consume();
        return_statement stmt = return_statement(parse_expression());
        expect_token(token_type::semicolon);
        consume();
        return stmt;
    }
    throw;
}

expression parser::parse_expression()
{
    expression res;
    if(current().type == token_type::open_paren)
    {
        consume();
        res = parse_expression();
        expect_token(token_type::close_paren);
        consume();
    }
    else
        res = parse_muldiv();
    if(current().type == token_type::plus || current().type == token_type::minus || current().type == token_type::star || current().type == token_type::slash)
    {
        token operand = consume();
        expression rhs = parse_expression();
        binary_operator_expression* ex = new binary_operator_expression(operand);
        ex->lhs = std::make_unique<expression>(res);
        ex->rhs = std::make_unique<expression>(rhs);
        res = *ex;
    }
    return res;
}

expression parser::parse_muldiv()
{
    int sign = 1;
    if(current().type == token_type::minus)
    {
        consume();
        sign = -1;
    }
    expression lhs = int_literal_expression(std::stoi(consume().text) * sign);;
    switch(current().type)
    {
        case token_type::open_paren:
            consume();
            lhs = parse_expression();
            expect_token(token_type::close_paren);
            consume();
            break;
        case token_type::star:
        case token_type::slash:
        {
            token operand = consume();
            expression rhs = parse_expression();
            binary_operator_expression* ex = new binary_operator_expression(operand);
            ex->lhs = std::make_unique<expression>(lhs);
            ex->rhs = std::make_unique<expression>(rhs);
            lhs = *ex;
            break;
        }
        default:
            break;
    }
    return lhs;
}