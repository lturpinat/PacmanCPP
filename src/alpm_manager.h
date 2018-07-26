#ifndef ALPMMANAGER_H

#include <alpm.h>
#include <alpm_list.h>

#include <vector>
#include <map>
#include <string>

#include <package_dependency.h>

#define ALPMMANAGER_H

using namespace std;

class AlpmManager
{
private:
    const char *fsRootDirectory;
    const char *pacmanDBDirectory;

public:
    AlpmManager(const char* fsRootDirectory, const char* pacmanDBDirectory);

    void fetch_required_dependencies(alpm_pkg_t *pkg, map<string, vector<PackageDependency*>> &packages);
    void fetch_optional_dependencies(alpm_pkg_t *pkg, map<string, vector<PackageDependency*>> &packages);

    void fetch_required_dependencies(alpm_pkg_t *pkg, map<string, vector<PackageDependency*>> &packages, string const &package_name);
    void fetch_optional_dependencies(alpm_pkg_t *pkg, map<string, vector<PackageDependency*>> &packages, string const &package_name);

    map<string, vector<PackageDependency*>> getPackages();
};

#endif // ALPMMANAGER_H
