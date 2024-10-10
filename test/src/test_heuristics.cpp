#include <heuristics.hpp>
#include <test_common.hpp>

static void test_match();
static void test_dict_init();
static void test_growing();
static void test_dict_deletion();
static void test_gp_update();
static void test_dict_update();

void test_heuristics()
{
    test_match();
    test_dict_init();
    test_growing();
    test_dict_update();
    test_dict_deletion();
    test_gp_update();
}

static void test_match()
{
    // MSE
    {
       std::vector<std::vector<uint8_t>> pixel(std::vector<std::vector<uint8_t>>(1, std::vector<uint8_t>(1, 0)));
       auto* b1 = new Block(1, 1, pixel);
       auto* b2 = new Block(1, 1, pixel);

       assert(heuristic::match::mse(b1, b2) == 0.0);

       delete b1;
       delete b2;
    }
}

static void test_dict_init()
{
    Dictionary* dict = heuristic::dict_init::range_0_to_255();

    assert(dict != nullptr);
    assert(dict->size == 256);

    for(size_t i = 0; i < 256; i++)
    {
        assert(dict->entries[i] != nullptr);
        assert(dict->entries[i]->pixels[0][0] == i);
    }

    delete dict;
}

static void test_growing()
{
    // {1, 2}, {3, 1}, {2, 1}, {0, 2}
    std::vector<Growing_point*> gpp;
    auto gp1 = new Growing_point(1, 2, new Block(1, 1, (std::vector<std::vector<uint8_t>>){0}));
    auto gp2 = new Growing_point(3, 1, new Block(1, 1, (std::vector<std::vector<uint8_t>>){0}));
    auto gp3 = new Growing_point(2, 1, new Block(1, 1, (std::vector<std::vector<uint8_t>>){0}));
    auto gp4 = new Growing_point(0, 2, new Block(1, 1, (std::vector<std::vector<uint8_t>>){0}));
    gpp.push_back(gp1);
    gpp.push_back(gp2);
    gpp.push_back(gp3);
    gpp.push_back(gp4);

    // WAVE
    {
        Growing_point *gp = heuristic::growing::wave(gpp);
        assert(gp != nullptr);
        assert(gp->x == 0);
        assert(gp->y == 2);
        assert(gp == gp4);
    }

    // DIAGONAL
    {
        Growing_point *gp = heuristic::growing::diagonal(gpp);
        assert(gp != nullptr);
        assert(gp->x == 1);
        assert(gp->y == 2);
        assert(gp == gp1);
    }

    // LIFO
    {
        Growing_point *gp = heuristic::growing::lifo(gpp);
        assert(gp != nullptr);
        assert(gp->x == 0);
        assert(gp->y == 2);
        assert(gp == gp4);
    }

    delete gp1;
    delete gp2;
    delete gp3;
    delete gp4;
}

static void test_dict_deletion()
{
    // implementation not finished
}

static void test_gp_update()
{
    // implementation not finished
}

static void test_dict_update()
{
    // One Row
    
}