#include <iostream>
#include <tilebox-core/version.hpp>

auto main() -> int
{
    std::cout << "Tilebox Core Version: " << tilebox::core::VERSION_STRING << '\n';
}
