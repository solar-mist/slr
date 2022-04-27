#include <typing/type_info.hxx>
#include <string>
#include <unordered_map>

std::unordered_map<std::string_view, type_info> types;

type_info::type_info()
{

}

type_info::type_info(std::string_view name, unsigned int size)
    :name(name), size(size)
{
}

void type_info::init_default_types()
{
    types["int"] =  type_info("int", 4);
    types["char"] = type_info("char", 1);
    types["i8"] =   type_info("i8", 1);
    types["i16"] =  type_info("i16", 2);
    types["i32"] =  type_info("i32", 4);
    types["i64"] =  type_info("i64", 8);
    types["void"] = type_info("void", 0);
}