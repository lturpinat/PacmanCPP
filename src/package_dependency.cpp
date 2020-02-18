#include "package_dependency.h"

PackageDependency::PackageDependency(const std::string &package_name, bool isRequired) : package_name(package_name), required(isRequired) {}

auto PackageDependency::getPackageName() -> std::string
{
    return package_name;
}

auto PackageDependency::isRequired() -> bool
{
    return required;
}
