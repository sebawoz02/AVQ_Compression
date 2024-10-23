#pragma once

#include <types/growing_point.hpp>
#include <cstddef>

typedef struct GP_pool_entry
{
    explicit GP_pool_entry(Growing_point* _gp):next(nullptr), gp(_gp){};
    ~GP_pool_entry();

    GP_pool_entry* next;
    Growing_point* gp;
} GP_pool_entry;

typedef struct GP_pool
{
    ~GP_pool();

    Growing_point* operator[](size_t index) const;
    [[nodiscard]] bool contains(size_t x, size_t y) const;

    void add(Growing_point* gp_new);
    void remove(Growing_point* gp_old);

    Growing_point* last();

private:
    GP_pool_entry* head;
} GP_pool;
