#ifndef ALPMMANAGER_H
#define ALPMMANAGER_H

#include <alpm.h>
#include <alpm_list.h>

#include <vector>
#include <map>
#include <string>
#include <memory>

#include <package_dependency.h>



using namespace std;

class AlpmManager
{
private:
    const char *fsRootDirectory;
    const char *pacmanDBDirectory;

public:
    AlpmManager(const char* fsRootDirectory, const char* pacmanDBDirectory);

    void fetch_required_dependencies(alpm_pkg_t *pkg, map<string, vector<unique_ptr<PackageDependency> > > &packages);
    void fetch_optional_dependencies(alpm_pkg_t *pkg, map<string, vector<unique_ptr<PackageDependency> > > &packages);

    void fetch_required_dependencies(alpm_pkg_t *pkg, map<string, vector<unique_ptr<PackageDependency> > > &packages, string const &package_name);
    void fetch_optional_dependencies(alpm_pkg_t *pkg, map<string, vector<unique_ptr<PackageDependency> > > &packages, string const &package_name);

    map<string, vector<unique_ptr<PackageDependency> > > getPackages();
};

#endif // ALPMMANAGER_H
