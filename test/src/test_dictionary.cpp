#include <test_common.hpp>
#include <types/dictionary.hpp>
#include <iostream>

static void test_init();
static void test_insert();
static void test_remove();

void test_dictionary()
{
  test_init();
  test_insert();
  test_remove();

  std::cout << "Dictionary struct tests finished successfully!" << std::endl;
}

static void test_init()
{
  auto* d = new Dictionary();
  assert(d != nullptr);

  assert(d->size() == 0);

  delete d;
}

static void test_insert()
{
  auto* dict_list = new Dictionary();
  assert(dict_list != nullptr);

  std::vector<std::vector<uint8_t>> dummy_pixel(
    std::vector<std::vector<uint8_t>>(1, std::vector<uint8_t>(1, 0)));
  auto* ent1 = new Block(1, 2, dummy_pixel);
  auto* ent2 = new Block(2, 1, dummy_pixel);
  auto* ent3 = new Block(4, 3, dummy_pixel);
  auto* ent4 = new Block(2, 2, dummy_pixel);

  dict_list->insert(ent1);
  assert(dict_list->size() == 1);
  assert((*dict_list)[0] == ent1);

  dict_list->insert(ent2);
  assert(dict_list->size() == 2);
  assert((*dict_list)[0] == ent1);
  assert((*dict_list)[1] == ent2);

  dict_list->insert(ent3);
  assert(dict_list->size() == 3);
  assert((*dict_list)[0] == ent1);
  assert((*dict_list)[1] == ent2);
  assert((*dict_list)[2] == ent3);

  dict_list->insert(ent4);
  assert((dict_list->size() == 4));
  assert((*dict_list)[0] == ent1);
  assert((*dict_list)[1] == ent2);
  assert((*dict_list)[3] == ent3);
  assert((*dict_list)[2] == ent4);

  delete dict_list;
}

static void test_remove()
{
  auto* dict_list = new Dictionary();
  assert(dict_list != nullptr);

  std::vector<std::vector<uint8_t>> dummy_pixel(
    std::vector<std::vector<uint8_t>>(1, std::vector<uint8_t>(1, 0)));
  auto* ent1 = new Block(1, 2, dummy_pixel);
  auto* ent2 = new Block(2, 1, dummy_pixel);
  auto* ent3 = new Block(4, 3, dummy_pixel);
  auto* ent4 = new Block(2, 2, dummy_pixel);

  dict_list->insert(ent1);
  dict_list->insert(ent2);
  dict_list->insert(ent3);
  dict_list->insert(ent4);
  assert(dict_list->size() == 4);
  assert((*dict_list)[0] == ent1);
  assert((*dict_list)[1] == ent2);
  assert((*dict_list)[3] == ent3);
  assert((*dict_list)[2] == ent4);

  dict_list->remove(ent4);
  assert(dict_list->size() == 3);
  assert((*dict_list)[0] == ent1);
  assert((*dict_list)[1] == ent2);
  assert((*dict_list)[2] == ent3);

  dict_list->remove(ent2);
  assert(dict_list->size() == 2);
  assert((*dict_list)[0] == ent1);
  assert((*dict_list)[1] == ent3);

  dict_list->remove(ent3);
  assert(dict_list->size() == 1);
  assert((*dict_list)[0] == ent1);

  dict_list->remove(ent1);
  assert(dict_list->size() == 0);

  delete dict_list;
}
