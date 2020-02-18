#include "alpm_manager.h"
#include <memory>

using namespace std;

AlpmManager::AlpmManager(const char* fsRootDirectory, const char* pacmanDBDirectory) : fsRootDirectory(fsRootDirectory), pacmanDBDirectory(pacmanDBDirectory) {}

auto AlpmManager::getPackages() -> map<string, vector<unique_ptr<PackageDependency>>> {
    map<string, vector<unique_ptr<PackageDependency>>> packages;

    // Initializing connection to Pacman DB
    alpm_handle_t *handle = alpm_initialize(fsRootDirectory, pacmanDBDirectory, nullptr);

    if(handle == nullptr) {
        throw runtime_error("Error while connecting to pacman database");
    }

    alpm_db_t *local_db = alpm_get_localdb(handle);
    alpm_list_t *cached_packages= alpm_db_get_pkgcache(local_db);

    while(cached_packages->data)
    {
        // MAIN PACKAGE

        auto *pkg = static_cast<alpm_pkg_t *>(cached_packages->data);
        const char *package_name = alpm_pkg_get_name(pkg);

        #warning "Check if really usefull"
        if(packages.find(package_name) == packages.end())
        {
            packages[package_name];
        }


        if(cached_packages->next == nullptr) break;
        cached_packages = cached_packages->next;

        fetch_required_dependencies(pkg, packages, package_name);
        fetch_optional_dependencies(pkg, packages, package_name);
    }

    alpm_release(handle);

    return packages;
}

void AlpmManager::fetch_required_dependencies(alpm_pkg_t *pkg, map<string, vector<unique_ptr<PackageDependency>>> &packages) const
{
    fetch_required_dependencies(pkg, packages, alpm_pkg_get_name(pkg));
}

void AlpmManager::fetch_optional_dependencies(alpm_pkg_t *pkg, map<string, vector<unique_ptr<PackageDependency>>> &packages) const
{
    fetch_optional_dependencies(pkg, packages, alpm_pkg_get_name(pkg));
}

void AlpmManager::fetch_required_dependencies(alpm_pkg_t *pkg, map<string, vector<unique_ptr<PackageDependency>>> &packages, string const &package_name) 
{
    alpm_list_t *dependencies = alpm_pkg_get_depends(pkg);

    if(dependencies == nullptr) return; // Don't do anything if there are no dependencies

    while(dependencies->data)
    {
        auto *dep = static_cast<alpm_depend_t *>(dependencies->data);
        packages[package_name].push_back(make_unique<PackageDependency>(dep->name)); // Storing dependency

        if(dependencies->next == nullptr) break;
        dependencies = dependencies->next;
    }
}

void AlpmManager::fetch_optional_dependencies(alpm_pkg_t *pkg,
                                              map<string, vector<unique_ptr<PackageDependency> > > &packages,
                                              string const &package_name) 
{
    alpm_list_t *dependencies = alpm_pkg_get_optdepends(pkg);

    if(dependencies == nullptr) return; // Don't do anything if there are no dependencies

    while(dependencies->data)
    {
        auto *dep = static_cast<alpm_depend_t *>(dependencies->data);
        packages[package_name].push_back(make_unique<PackageDependency>( dep->name, false )); // Storing dependency

        if(dependencies->next == nullptr) break;
        dependencies = dependencies->next;
    }
}
