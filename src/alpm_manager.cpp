#include "alpm_manager.h"

AlpmManager::AlpmManager(const char* fsRootDirectory, const char* pacmanDBDirectory) : fsRootDirectory(fsRootDirectory), pacmanDBDirectory(pacmanDBDirectory) {}

map<string, vector<PackageDependency*>> AlpmManager::getPackages() {
    map<string, vector<PackageDependency*>> packages{};

    // Initializing connection to Pacman DB
    alpm_handle_t *handle = alpm_initialize(fsRootDirectory, pacmanDBDirectory, nullptr);

    if(handle == nullptr) {
        throw new runtime_error("Error while connecting to pacman database");
    }

    alpm_db_t *local_db = alpm_get_localdb(handle);
    alpm_list_t *cached_packages= alpm_db_get_pkgcache(local_db);

    while(cached_packages->data)
    {
        // MAIN PACKAGE

        alpm_pkg_t *pkg = static_cast<alpm_pkg_t *>(cached_packages->data);
        const char *package_name = alpm_pkg_get_name(pkg);

        // If package doesn't already exist, create a sublist for dependencies
        if(packages.find(package_name) == packages.end())
        {
            packages[package_name] = vector<PackageDependency*>{};
        }

        if(cached_packages->next == nullptr) break;
        cached_packages = cached_packages->next;

        fetch_required_dependencies(pkg, packages, package_name);
        fetch_optional_dependencies(pkg, packages, package_name);
    }

    alpm_release(handle);

    return packages;
}

void AlpmManager::fetch_required_dependencies(alpm_pkg_t *pkg, map<string, vector<PackageDependency*>> &packages)
{
    const char *package_name = alpm_pkg_get_name(pkg);
    fetch_required_dependencies(pkg, packages, package_name);
}

void AlpmManager::fetch_optional_dependencies(alpm_pkg_t *pkg, map<string, vector<PackageDependency*>> &packages)
{
    const char *package_name = alpm_pkg_get_name(pkg);
    fetch_optional_dependencies(pkg, packages, package_name);
}

void AlpmManager::fetch_required_dependencies(alpm_pkg_t *pkg, map<string, vector<PackageDependency*>> &packages, string const &package_name)
{
    alpm_list_t *dependencies = alpm_pkg_get_depends(pkg);

    if(dependencies == nullptr) return; // Don't do anything if there are no dependencies

    while(dependencies->data)
    {
        alpm_depend_t *dep = static_cast<alpm_depend_t *>(dependencies->data);
        packages[package_name].push_back(new PackageDependency{ dep->name }); // Storing dependency

        if(dependencies->next == nullptr) break;
        dependencies = dependencies->next;
    }
}

void AlpmManager::fetch_optional_dependencies(alpm_pkg_t *pkg, map<string, vector<PackageDependency*>> &packages, string const &package_name)
{
    alpm_list_t *dependencies = alpm_pkg_get_optdepends(pkg);

    if(dependencies == nullptr) return; // Don't do anything if there are no dependencies

    while(dependencies->data)
    {
        alpm_depend_t *dep = static_cast<alpm_depend_t *>(dependencies->data);
        packages[package_name].push_back(new PackageDependency{ dep->name, false }); // Storing dependency

        if(dependencies->next == nullptr) break;
        dependencies = dependencies->next;
    }
}
