#include <catch2/catch_test_macros.hpp>
#include "CMU_Dict.h"
#include "silent_letters.h"

struct Fixture {
    mutable CMU_Dict dict;
    bool import_success{};

    Fixture() {
        import_success = dict.import_dictionary();
    }
};

TEST_CASE_PERSISTENT_FIXTURE(Fixture, "silent_letters") {

    SECTION ("dictionary import success") {
        REQUIRE(import_success == true);
    }

    SECTION("get_grapheme") {
        std::vector<std::string> graphemes {phone_to_graphemes("B")};
        REQUIRE(graphemes.size() == 1);
        REQUIRE(graphemes[0] == "B");
    }

    SECTION("grapheme_match") {
        std::string phone{"B"};
        std::string word {"BRAVE"};
        REQUIRE(word[0] == phone[0]);
        std::size_t word_index {0};
        Match_Result match_result = find_phone_grapheme_in_word(phone, word, word_index);
        REQUIRE(match_result.valid == true);
        REQUIRE(match_result.found_silent_letters == false);
        REQUIRE(match_result.match_start == 0);
        REQUIRE(match_result.match_start == 0);
        REQUIRE(word_index == 1);
    }

}


