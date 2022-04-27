#include "token.hxx"
#include <cctype>
#include <iostream>
#include <lexer.hxx>

lexer::lexer(std::string_view text)
    :text(text), position(0)
{
}

std::vector<token> lexer::lex()
{
    std::vector<token> result;
    while(position != text.length())
    {
        std::optional<token> tok = next_token();
        if (tok.has_value())
            result.push_back(tok.value());
        next();
    }

    return result;
}

char lexer::next()
{
    return text[++position];
}

char lexer::current()
{
    return text[position];
}

char lexer::peek(int offset)
{
    return text[position + offset];
}

std::optional<token> lexer::next_token()
{
    char c = current();
    if(std::isalpha(c))
    {
        std::string value = { c };
        while(std::isalnum(peek(1)) || peek(1) == '_')
            value += next();
        return token(token_type::identifier, value);
    }
    else if (std::isdigit(c))
    {
        std::string value = { c };
        while(std::isdigit(peek(1)))
            value += next();
        return token(token_type::integer_literal, value);
    }
    else if (c == '(')
        return token(token_type::open_paren, "(");
    else if (c == ')')
        return token(token_type::close_paren, ")");
    else if (c == '[')
        return token(token_type::open_sq_bracket, "[");
    else if (c == ']')
        return token(token_type::close_sq_bracket, "]");
    else if (c == '{')
        return token(token_type::open_bracket, "{");
    else if (c == '}')
        return token(token_type::close_bracket, "}");
    else if (c == ';')
        return token(token_type::semicolon, ";");
    else if (c == '+')
        return token(token_type::plus, "+");
    else if (c == '-')
        return token(token_type::minus, "-");
    else if (c == '*')
        return token(token_type::star, "*");
    else if (c == '/')
        return token(token_type::slash, "/");
    return std::nullopt;
}