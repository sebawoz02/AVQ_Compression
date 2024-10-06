#include <heuristics.hpp>
#include <test_common.hpp>

static void test_match();
static void test_dict_init();

void test_heuristics()
{
    test_match();
    test_dict_init();
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
