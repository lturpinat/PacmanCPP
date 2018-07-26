#ifndef PACKAGE_DEPENDENCY_H

#include <string>

#define PACKAGE_DEPENDENCY_H

class PackageDependency
{
private:
    std::string package_name;
    bool required;

public:
    PackageDependency(std::string package_name, bool isRequired = true);
    std::string getPackageName();
    bool isRequired();
};

#endif // PACKAGE_DEPENDENCY_H
