#include <types/gp_pool.hpp>

GP_pool::~GP_pool()
{
    GP_pool_entry* ptr = head;
    while (ptr != nullptr)
    {
        GP_pool_entry* to_delete = ptr;
        ptr = ptr->next;
        delete to_delete;
    }
}

Growing_point *GP_pool::operator[](size_t index) const
{
    size_t counter = 0;
    GP_pool_entry* ptr = head;
    while (counter != index && ptr != nullptr)
    {
        ptr = ptr->next;
        counter++;
    }
    if(ptr == nullptr)
    {
        return nullptr;
    }
    return ptr->gp;
}

bool GP_pool::contains(size_t x, size_t y) const {
    GP_pool_entry* ptr = head;
    while(ptr != nullptr)
    {
        if(ptr->gp->x == x && ptr->gp->y == y)
        {
            return true;
        }
        ptr = ptr->next;
    }
    return false;
}

void GP_pool::add(Growing_point* new_gp) {
    if(head == nullptr)
    {
        head = new GP_pool_entry(new_gp);
        return;
    }

    GP_pool_entry* ptr = head;
    while(ptr->next != nullptr) {
        ptr = ptr->next;
    }

    auto* gpp_new = new GP_pool_entry(new_gp);
    ptr->next = gpp_new;
}

void GP_pool::remove(Growing_point *gp_old) {

    if(head->gp == gp_old)
    {
        GP_pool_entry* to_delete = head;
        head = head->next;
        delete to_delete;
        return;
    }

    GP_pool_entry* prev = head;
    GP_pool_entry* ptr = prev->next;
    while(ptr->next != nullptr && ptr->gp != gp_old) {
        prev = ptr;
        ptr = ptr->next;
    }
    if(ptr->gp == gp_old)
    {
        prev->next = ptr->next;
        delete ptr;
    }
}

Growing_point* GP_pool::last() {
    if(head == nullptr)
        return nullptr;
    GP_pool_entry* ptr = head;
    while(ptr->next != nullptr)
    {
        ptr = ptr->next;
    }

    return ptr->gp;
}

GP_pool_entry::~GP_pool_entry() {
    delete gp;
}
