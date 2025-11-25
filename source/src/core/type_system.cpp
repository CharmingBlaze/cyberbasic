#include "bas/type_system.hpp"

namespace bas {

static TypeRegistry* g_type_registry = nullptr;

TypeRegistry* get_type_registry() {
    return g_type_registry;
}

void set_type_registry(TypeRegistry* registry) {
    g_type_registry = registry;
}

} // namespace bas

