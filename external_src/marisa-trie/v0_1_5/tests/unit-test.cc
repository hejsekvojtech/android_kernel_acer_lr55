#include <cstdlib>
#include <ctime>

#include <marisa_alpha/base.h>
#include <marisa_alpha/popcount.h>
#include <marisa_alpha/rank.h>
#include <marisa_alpha/string.h>
#include <marisa_alpha/key.h>
#include <marisa_alpha/progress.h>
#include <marisa_alpha/range.h>
#include <marisa_alpha/query.h>
#include <marisa_alpha/container.h>
#include <marisa_alpha/cell.h>

#include "assert.h"

namespace {

void TestBase() {
  TEST_START();

  ASSERT(sizeof(marisa_alpha_uint8) == 1);
  ASSERT(sizeof(marisa_alpha_uint16) == 2);
  ASSERT(sizeof(marisa_alpha_uint32) == 4);
  ASSERT(sizeof(marisa_alpha_uint64) == 8);

  ASSERT(MARISA_ALPHA_UINT8_MAX == 0xFFU);
  ASSERT(MARISA_ALPHA_UINT16_MAX == 0xFFFFU);
  ASSERT(MARISA_ALPHA_UINT32_MAX == 0xFFFFFFFFU);
  ASSERT(MARISA_ALPHA_UINT64_MAX == 0xFFFFFFFFFFFFFFFFULL);

  ASSERT(sizeof(marisa_alpha::UInt8) == 1);
  ASSERT(sizeof(marisa_alpha::UInt16) == 2);
  ASSERT(sizeof(marisa_alpha::UInt32) == 4);
  ASSERT(sizeof(marisa_alpha::UInt64) == 8);

  int x = 100, y = 200;
  marisa_alpha::Swap(&x, &y);
  ASSERT(x == 200);
  ASSERT(y == 100);

  EXCEPT(marisa_alpha::Swap(static_cast<int *>(NULL), &y),
      MARISA_ALPHA_PARAM_ERROR);
  EXCEPT(marisa_alpha::Swap(&x, static_cast<int *>(NULL)),
      MARISA_ALPHA_PARAM_ERROR);

  TEST_END();
}

marisa_alpha::UInt32 NaivePopCount(marisa_alpha::UInt32 x) {
  marisa_alpha::UInt32 count = 0;
  while (x) {
    count += x & 1;
    x >>= 1;
  }
  return count;
}

void TestPopCount() {
  TEST_START();

  ASSERT(marisa_alpha::PopCount(0).lo8() == 0);
  ASSERT(marisa_alpha::PopCount(0).lo16() == 0);
  ASSERT(marisa_alpha::PopCount(0).lo24() == 0);
  ASSERT(marisa_alpha::PopCount(0).lo32() == 0);

  ASSERT(marisa_alpha::PopCount(0xFFFFFFFFU).lo8() == 8);
  ASSERT(marisa_alpha::PopCount(0xFFFFFFFFU).lo16() == 16);
  ASSERT(marisa_alpha::PopCount(0xFFFFFFFFU).lo24() == 24);
  ASSERT(marisa_alpha::PopCount(0xFFFFFFFFU).lo32() == 32);

  for (std::size_t i = 0; i < 1024; ++i) {
    marisa_alpha::UInt32 value = std::rand();
    marisa_alpha::PopCount popcount(value);
    ASSERT(popcount.lo8() == NaivePopCount(value & 0xFFU));
    ASSERT(popcount.lo16() == NaivePopCount(value & 0xFFFFU));
    ASSERT(popcount.lo24() == NaivePopCount(value & 0xFFFFFFU));
    ASSERT(popcount.lo32() == NaivePopCount(value));
  }

  TEST_END();
}

void TestRank() {
  TEST_START();

  marisa_alpha::Rank rank;

  ASSERT(rank.abs() == 0);
  ASSERT(rank.rel1() == 0);
  ASSERT(rank.rel2() == 0);
  ASSERT(rank.rel3() == 0);
  ASSERT(rank.rel4() == 0);
  ASSERT(rank.rel5() == 0);
  ASSERT(rank.rel6() == 0);
  ASSERT(rank.rel7() == 0);

  rank.set_abs(0xFFFFFFFFU);
  rank.set_rel1(64);
  rank.set_rel2(128);
  rank.set_rel3(192);
  rank.set_rel4(256);
  rank.set_rel5(320);
  rank.set_rel6(384);
  rank.set_rel7(448);

  ASSERT(rank.abs() == 0xFFFFFFFFU);
  ASSERT(rank.rel1() == 64);
  ASSERT(rank.rel2() == 128);
  ASSERT(rank.rel3() == 192);
  ASSERT(rank.rel4() == 256);
  ASSERT(rank.rel5() == 320);
  ASSERT(rank.rel6() == 384);
  ASSERT(rank.rel7() == 448);

  TEST_END();
}

void TestString() {
  TEST_START();

  marisa_alpha::String str;

  ASSERT(str.ptr() == NULL);
  ASSERT(str.length() == 0);

  marisa_alpha::RString rstr;

  ASSERT(rstr.ptr() == NULL);
  ASSERT(rstr.length() == 0);

  const char *s = "ab";
  str = marisa_alpha::String(s);

  ASSERT(str.ptr() == s);
  ASSERT(str.length() == 2);
  ASSERT(str[0] == s[0]);
  ASSERT(str[1] == s[1]);

  rstr = marisa_alpha::RString(str);
  ASSERT(rstr.ptr() == s);
  ASSERT(rstr.length() == 2);
  ASSERT(rstr[0] == s[1]);
  ASSERT(rstr[1] == s[0]);

  std::string s2 = "xyz";
  str = marisa_alpha::String(s2.c_str(), s2.length());

  ASSERT(str.ptr() == s2.c_str());
  ASSERT(str.length() == 3);
  ASSERT(str[0] == s2[0]);
  ASSERT(str[1] == s2[1]);
  ASSERT(str[2] == s2[2]);

  ASSERT(str.substr(0, 2).length() == 2);
  ASSERT(str.substr(0, 2)[0] == 'x');
  ASSERT(str.substr(0, 2)[1] == 'y');

  rstr = marisa_alpha::RString(str);

  ASSERT(rstr.ptr() == s2.c_str());
  ASSERT(rstr.length() == 3);
  ASSERT(rstr[0] == s2[2]);
  ASSERT(rstr[1] == s2[1]);
  ASSERT(rstr[2] == s2[0]);

  ASSERT(rstr.substr(1, 2).length() == 2);
  ASSERT(rstr.substr(1, 2)[0] == 'y');
  ASSERT(rstr.substr(1, 2)[1] == 'x');

  ASSERT(marisa_alpha::String("abc") == marisa_alpha::String("abc"));
  ASSERT(marisa_alpha::String("abc") != marisa_alpha::String("bcd"));
  ASSERT(marisa_alpha::String("abc") < marisa_alpha::String("bcd"));
  ASSERT(marisa_alpha::String("ab") < marisa_alpha::String("abc"));
  ASSERT(marisa_alpha::String("bcd") > marisa_alpha::String("abc"));
  ASSERT(marisa_alpha::String("abc") > marisa_alpha::String("ab"));

  ASSERT(marisa_alpha::String("abcde").substr(1, 2) ==
      marisa_alpha::String("bc"));

  TEST_END();
}

void TestKey() {
  TEST_START();

  marisa_alpha::Key<marisa_alpha::String> key;

  ASSERT(key.str().length() == 0);
  ASSERT(key.weight() == 0.0);
  ASSERT(key.id() == 0);
  ASSERT(key.terminal() == 0);

  key.set_str(marisa_alpha::String("abc"));
  key.set_weight(1.0);
  key.set_id(2);
  key.set_terminal(3);

  ASSERT(key.str() == marisa_alpha::String("abc"));
  ASSERT(key.weight() == 1.0);
  ASSERT(key.id() == 2);
  ASSERT(key.terminal() == 3);

  marisa_alpha::String str("string");
  marisa_alpha::Key<marisa_alpha::RString> rkey;

  ASSERT(rkey.str().length() == 0);
  ASSERT(rkey.weight() == 0.0);
  ASSERT(rkey.id() == 0);
  ASSERT(rkey.terminal() == 0);

  rkey.set_str(marisa_alpha::RString(str));
  rkey.set_weight(4.0);
  rkey.set_id(5);
  rkey.set_terminal(6);

  ASSERT(rkey.str() == marisa_alpha::RString(str));
  ASSERT(rkey.weight() == 4.0);
  ASSERT(rkey.id() == 5);
  ASSERT(rkey.terminal() == 6);

  TEST_END();
}
void TestProgress() {
  TEST_START();

  {
    marisa_alpha::Progress progress(0);

    ASSERT(progress.is_valid());
    while (!progress.is_last()) {
      ++progress;
    }
    ASSERT(progress.is_last());
    ASSERT(progress.flags() == MARISA_ALPHA_DEFAULT_FLAGS);
    ASSERT(progress.trie_id() == progress.num_tries() - 1);
    ASSERT(progress.total_size() == 0);

    progress.test_total_size(0);
    progress.test_total_size(1);
    EXCEPT(progress.test_total_size(MARISA_ALPHA_UINT32_MAX),
        MARISA_ALPHA_SIZE_ERROR);
    progress.test_total_size(MARISA_ALPHA_UINT32_MAX - 1);
    progress.test_total_size(0);
    EXCEPT(progress.test_total_size(1), MARISA_ALPHA_SIZE_ERROR);

    ASSERT(progress.num_tries() == MARISA_ALPHA_DEFAULT_NUM_TRIES);
    ASSERT(progress.trie() == MARISA_ALPHA_DEFAULT_TRIE);
    ASSERT(progress.tail() == MARISA_ALPHA_DEFAULT_TAIL);
    ASSERT(progress.order() == MARISA_ALPHA_DEFAULT_ORDER);
  }

  {
    marisa_alpha::Progress progress(MARISA_ALPHA_DEFAULT_FLAGS);

    ASSERT(progress.is_valid());
    ASSERT(!progress.is_last());
    ASSERT(progress.num_tries() == MARISA_ALPHA_DEFAULT_NUM_TRIES);
    ASSERT(progress.trie() == MARISA_ALPHA_DEFAULT_TRIE);
    ASSERT(progress.tail() == MARISA_ALPHA_DEFAULT_TAIL);
    ASSERT(progress.order() == MARISA_ALPHA_DEFAULT_ORDER);
  }

  {
    marisa_alpha::Progress progress(255 | MARISA_ALPHA_PREFIX_TRIE
        | MARISA_ALPHA_BINARY_TAIL | MARISA_ALPHA_LABEL_ORDER);

    ASSERT(progress.is_valid());
    ASSERT(!progress.is_last());
    ASSERT(progress.num_tries() == 255);
    ASSERT(progress.trie() == MARISA_ALPHA_PREFIX_TRIE);
    ASSERT(progress.tail() == MARISA_ALPHA_BINARY_TAIL);
    ASSERT(progress.order() == MARISA_ALPHA_LABEL_ORDER);
  }

  {
    marisa_alpha::Progress progress(~MARISA_ALPHA_FLAGS_MASK);

    ASSERT(!progress.is_valid());
  }

  TEST_END();
}

void TestRange() {
  TEST_START();

  marisa_alpha::Range range;

  ASSERT(range.begin() == 0);
  ASSERT(range.end() == 0);
  ASSERT(range.pos() == 0);

  range.set_begin(1);
  range.set_end(2);
  range.set_pos(3);

  ASSERT(range.begin() == 1);
  ASSERT(range.end() == 2);
  ASSERT(range.pos() == 3);

  marisa_alpha::WRange wrange;

  ASSERT(wrange.range().begin() == 0);
  ASSERT(wrange.range().end() == 0);
  ASSERT(wrange.range().pos() == 0);

  ASSERT(wrange.begin() == 0);
  ASSERT(wrange.end() == 0);
  ASSERT(wrange.pos() == 0);
  ASSERT(wrange.weight() == 0.0);

  wrange = marisa_alpha::WRange(range, 4.0);

  ASSERT(wrange.range().begin() == 1);
  ASSERT(wrange.range().end() == 2);
  ASSERT(wrange.range().pos() == 3);

  ASSERT(wrange.begin() == 1);
  ASSERT(wrange.end() == 2);
  ASSERT(wrange.pos() == 3);
  ASSERT(wrange.weight() == 4.0);

  wrange.set_begin(5);
  wrange.set_end(6);
  wrange.set_pos(7);
  wrange.set_weight(8.0);

  ASSERT(wrange.begin() == 5);
  ASSERT(wrange.end() == 6);
  ASSERT(wrange.pos() == 7);
  ASSERT(wrange.weight() == 8.0);

  TEST_END();
}

void TestQuery() {
  TEST_START();

  marisa_alpha::Query query("abc", 3);

  ASSERT(query[0] == 'a');
  ASSERT(!query.ends_at(0));

  ASSERT(query[1] == 'b');
  ASSERT(!query.ends_at(1));

  ASSERT(query[2] == 'c');
  ASSERT(!query.ends_at(2));

  ASSERT(query.ends_at(3));

  std::string str("str");

  query.insert(&str);
  ASSERT(str == "abcstr");

  marisa_alpha::CQuery cquery("xyz");

  ASSERT(cquery[0] == 'x');
  ASSERT(!cquery.ends_at(0));

  ASSERT(cquery[1] == 'y');
  ASSERT(!cquery.ends_at(1));

  ASSERT(cquery[2] == 'z');
  ASSERT(!cquery.ends_at(2));

  ASSERT(cquery.ends_at(3));

  cquery.insert(&str);
  ASSERT(str == "xyzabcstr");

  TEST_END();
}

void TestContainer() {
  TEST_START();

  int array[1024];
  marisa_alpha::Container<int *> array_container(array);

  ASSERT(array_container.is_valid());
  for (int i = 0; i < 1024; ++i) {
    int value = std::rand();
    array_container.insert(i, value);
    ASSERT(array[i] == value);
  }

  marisa_alpha::Container<int *> array_container2(NULL);

  ASSERT(!array_container2.is_valid());

  std::vector<int> vec;
  marisa_alpha::Container<std::vector<int> *> vec_container(&vec);

  ASSERT(vec_container.is_valid());
  for (int i = 0; i < 1024; ++i) {
    int value = std::rand();
    vec_container.insert(i, value);
    ASSERT(vec.back() == value);
    ASSERT(vec[i] == value);
  }
  ASSERT(vec.size() == 1024);

  marisa_alpha::Container<std::vector<int> *> vec_container2(&vec);

  ASSERT(vec_container2.is_valid());
  for (int i = 0; i < 1024; ++i) {
    int value = std::rand();
    vec_container2.insert(i, value);
    ASSERT(vec.back() == value);
    ASSERT(vec[i + 1024] == value);
  }
  ASSERT(vec.size() == 2048);

  marisa_alpha::Container<std::vector<int> *> vec_container3(NULL);
  ASSERT(!vec_container3.is_valid());

  TEST_END();
}

void TestCell() {
  TEST_START();

  marisa_alpha::Cell cell;

  ASSERT(cell.louds_pos() == 0);
  ASSERT(cell.node() == 0);
  ASSERT(cell.key_id() == 0);
  ASSERT(cell.length() == 0);

  cell.set_louds_pos(1);
  cell.set_node(2);
  cell.set_key_id(3);
  cell.set_length(4);

  ASSERT(cell.louds_pos() == 1);
  ASSERT(cell.node() == 2);
  ASSERT(cell.key_id() == 3);
  ASSERT(cell.length() == 4);

  TEST_END();
}

}  // namespace

int main() {
  std::srand((unsigned int)time(NULL));

  TestBase();
  TestPopCount();
  TestRank();
  TestString();
  TestKey();
  TestProgress();
  TestRange();
  TestQuery();
  TestContainer();
  TestCell();

  return 0;
}
