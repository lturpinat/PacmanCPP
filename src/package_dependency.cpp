#include "package_dependency.h"

PackageDependency::PackageDependency(std::string package_name, bool isRequired) : package_name(package_name), required(isRequired) {}

std::string PackageDependency::getPackageName()
{
    return package_name;
}

bool PackageDependency::isRequired()
{
    return required;
}
