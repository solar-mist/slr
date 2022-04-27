#include <token.hxx>

constexpr const char* types[] = { "identifier", "open_paren", "close_paren", "open_sq_bracket", "close_sq_bracket", "open_bracket", "close_bracket", "int_literal", "comma", "semicolon", "plus", "minus", "star", "slash" };

token::token(token_type type, std::string_view text)
    :type(type), text(text)
{
}

std::string token::get_type()
{
    return types[type];
}

std::ostream& operator<<(std::ostream& stream, token tok)
{
    stream << tok.get_type() << ": " << tok.text;
    return stream;
}