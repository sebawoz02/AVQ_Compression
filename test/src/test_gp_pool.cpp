#include <test_common.hpp>
#include <types/gp_pool.hpp>
#include <iostream>

static void test_add();
static void test_contains();
static void test_remove();


void test_gp_pool()
{
    test_add();
    test_contains();
    test_remove();

    std::cout << "GP Pool struct tests finished successfully!" << std::endl;
}

static void test_add()
{
    auto* gpp = new GP_pool();
    assert(gpp != nullptr);
    assert((*gpp)[0] == nullptr);

    Growing_point* gp1 = new Growing_point(1, 1);
    gpp->add(gp1);

    assert((*gpp)[0] == gp1);
    assert((*gpp)[1] == nullptr);
    assert((*gpp)[0]->x == 1);
    assert((*gpp)[0]->y == 1);

    Growing_point* gp2 = new Growing_point(2, 6);
    gpp->add(gp2);
    assert((*gpp)[0] == gp1);
    assert((*gpp)[1] == gp2);
    assert((*gpp)[2] == nullptr);
    assert((*gpp)[0]->x == 1);
    assert((*gpp)[0]->y == 1);
    assert((*gpp)[1]->x == 2);
    assert((*gpp)[1]->y == 6);

    delete gpp;
}

static void test_contains()
{
    auto* gpp = new GP_pool();
    assert(gpp != nullptr);
    assert((*gpp)[0] == nullptr);

    const size_t gp1_x = 1;
    const size_t gp1_y = 1;
    Growing_point* gp1 = new Growing_point(gp1_x, gp1_y);
    assert(gpp->contains(gp1_x, gp1_y) == false);
    gpp->add(gp1);
    assert((*gpp)[0] == gp1);
    assert(gpp->contains(gp1_x, gp1_y) == true);

    const size_t gp2_x = 2;
    const size_t gp2_y = 6;
    Growing_point* gp2 = new Growing_point(gp2_x, gp2_y);
    assert(gpp->contains(gp2_x, gp2_y) == false);
    gpp->add(gp2);
    assert((*gpp)[0] == gp1);
    assert((*gpp)[1] == gp2);
    assert(gpp->contains(gp2_x, gp2_y) == true);

    delete gpp;
}

static void test_remove()
{
    auto* gpp = new GP_pool();
    assert(gpp != nullptr);
    assert((*gpp)[0] == nullptr);

    Growing_point* gp1 = new Growing_point(1, 1);
    gpp->add(gp1);
    assert((*gpp)[0] == gp1);
    Growing_point* gp2 = new Growing_point(6, 2);
    gpp->add(gp2);
    assert((*gpp)[0] == gp1);
    assert((*gpp)[1] == gp2);

    gpp->remove(gp1);
    assert((*gpp)[0] == gp2);
    assert((*gpp)[1] == nullptr);

    gpp->remove(gp2);
    assert((*gpp)[0] == nullptr);

    delete gpp;
}