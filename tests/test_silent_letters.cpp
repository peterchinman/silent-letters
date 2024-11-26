#include <catch2/catch_test_macros.hpp>
#include "CMU_Dict.h"
#include "silent_letters.h"
#include <iostream>

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
        REQUIRE(graphemes.size() == 2);
        REQUIRE(std::find(graphemes.begin(), graphemes.end(), "B") != graphemes.end());
    }

    SECTION("check_next_phone_for_grapheme_match_and_get_silent_letters vowel case") {
        std::string phone{"AE1"};
        std::string word {"ACID"};
        std::size_t word_index {0};
        Grapheme_Match_Result match_result = check_next_phone_for_grapheme_match_and_get_silent_letters(phone, word, word_index);
        REQUIRE(match_result.valid == true);
        REQUIRE(match_result.found_silent_letters == false);
        REQUIRE(match_result.match_start == 0);
        REQUIRE(match_result.match_end == 0);
    }

    SECTION("check_next_phone_for_grapheme_match_and_get_silent_letters ER case") {
        std::string phone{"ER1"};
        // BIRTH  B ER1 TH
        std::string word {"BIRTH"};
        std::size_t word_index {1};
        Grapheme_Match_Result match_result = check_next_phone_for_grapheme_match_and_get_silent_letters(phone, word, word_index);
        REQUIRE(match_result.valid == true);
        REQUIRE(match_result.found_silent_letters == false);
        REQUIRE(match_result.match_start == 1);
        REQUIRE(match_result.match_end == 3);
    }

    SECTION("check_next_phone_for_grapheme_match_and_get_silent_letters simple match case") {
        std::string phone{"B"};
        std::string word {"BRAVE"};
        std::size_t word_index {0};
        Grapheme_Match_Result match_result = check_next_phone_for_grapheme_match_and_get_silent_letters(phone, word, word_index);
        REQUIRE(match_result.matched_grapheme == "B");
        REQUIRE(match_result.valid == true);
        REQUIRE(match_result.found_silent_letters == false);
        REQUIRE(match_result.match_start == 0);
        REQUIRE(match_result.match_end == 1);
    }

    SECTION("check_next_phone_for_grapheme_match_and_get_silent_letters optimizer check longest grapheme") {
        std::string phone{"F"};
        std::string word {"RIFFING"};
        std::size_t word_index {2};
        Grapheme_Match_Result match_result = check_next_phone_for_grapheme_match_and_get_silent_letters(phone, word, word_index);
        REQUIRE(match_result.matched_grapheme == "FF");
        REQUIRE(match_result.valid == true);
        REQUIRE(match_result.found_silent_letters == false);
        REQUIRE(match_result.match_start == 2);
        REQUIRE(match_result.match_end == 4);
    }

    SECTION("check_next_phone_for_grapheme_match_and_get_silent_letters optimizer check least silent letters") {
        std::string phone{"S"};
        std::string word {"PSYCH"};
        std::size_t word_index {0};
        Grapheme_Match_Result match_result = check_next_phone_for_grapheme_match_and_get_silent_letters(phone, word, word_index);
        REQUIRE(match_result.matched_grapheme == "S");
        REQUIRE(match_result.valid == true);
        REQUIRE(match_result.found_silent_letters == true);
        REQUIRE(match_result.silent_letter_start == 0);
        REQUIRE(match_result.silent_letter_end == 1);
        REQUIRE(match_result.match_start == 1);
        REQUIRE(match_result.match_end == 2);
    }

    SECTION("check_next_phone_for_grapheme_match_and_get_silent_letters optimizer") {
        // /F/ has multiple realizations: {"GH", "PH", "FF", "F"}. The one resulting in the fewest silent letters is "F", which is what should be used.
        std::string phone{"F"};
        std::string word {"FPHGH"};
        std::size_t word_index {0};
        Grapheme_Match_Result match_result = check_next_phone_for_grapheme_match_and_get_silent_letters(phone, word, word_index);
        REQUIRE(match_result.matched_grapheme == "F");
        REQUIRE(match_result.valid == true);
        REQUIRE(match_result.found_silent_letters == false);
        REQUIRE(match_result.match_start == 0);
        REQUIRE(match_result.match_end == 1);
    }

    SECTION("check_next_phone_for_grapheme_match_and_get_silent_letters with silent letters") {
        std::string phone{"B"};
        std::string word {"MMBBRAVE"};
        std::size_t word_index {0};
        Grapheme_Match_Result match_result = check_next_phone_for_grapheme_match_and_get_silent_letters(phone, word, word_index);
        REQUIRE(match_result.matched_grapheme == "BB");
        REQUIRE(match_result.valid == true);
        REQUIRE(match_result.found_silent_letters == true);
        REQUIRE(match_result.silent_letter_start == 0);
        REQUIRE(match_result.silent_letter_end == 2);
        REQUIRE(match_result.match_start == 2);
        REQUIRE(match_result.match_end == 4);
    }

    SECTION("check_next_phone_for_grapheme_match_and_get_silent_letters invalid") {
        std::string phone{"B"};
        std::string word {"SLURP"};
        std::size_t word_index {0};
        Grapheme_Match_Result match_result = check_next_phone_for_grapheme_match_and_get_silent_letters(phone, word, word_index);
        REQUIRE(match_result.valid == false);
    }

    SECTION("pronunciations_to_silent_letter_indices no silent letters") {
        std::string word{"SLURP"};
        Word_Silent_Letter_Results word_results = pronunciations_to_silent_letter_indices(word, dict.find_phones(word));
        REQUIRE(word_results.phones_not_found == false);
        REQUIRE(word_results.silent_letter_indices.size() == 0);
    }

    SECTION("pronunciations_to_silent_letter_indices silent letters") {
        std::string word{"YACHT"};
        Word_Silent_Letter_Results word_results = pronunciations_to_silent_letter_indices(word, dict.find_phones(word));
        REQUIRE(word_results.phones_not_found == false);
        REQUIRE(word_results.silent_letter_indices.size() == 1);
        REQUIRE(word_results.silent_letter_indices[0].first == 2);
        REQUIRE(word_results.silent_letter_indices[0].second == 4);
    }

    SECTION("pronunciations_to_silent_letter_indices silent letters ending vowel") {
        std::string word2{"RASPBERRY"};
        Word_Silent_Letter_Results word_results2 = pronunciations_to_silent_letter_indices(word2, dict.find_phones(word2));
        REQUIRE(word_results2.phones_not_found == false);
        REQUIRE(word_results2.silent_letter_indices.size() == 1);
        REQUIRE(word_results2.silent_letter_indices[0].first == 3);
        REQUIRE(word_results2.silent_letter_indices[0].second == 4);
    }



    // // TODO PROBLEM WITH: DUMBER
    // and any other word ending in vowel with silent letters before the vowel

    // Words not behaving as expected:
    // 

    // for QU == K W
    // complicated solution: when you get to a K, check if next phone is a W, if so, check for QU graphemes. If QU is cleanest fit, toss the next (W) phone and continue.
    // I did the easy solution of /K/ = 'Q', /W/ = 'U'
    // which might backfire if there's a /W/ word with U

    // Handkerchief  HANDKERCHIEF  HH AE1 NG K ER0 CH IH0 F
    // ^^ seems insoluble unless I hardcode it in, only instance of this pattern
    // SCHEDULE  S K EH1 JH UH0 L
    // ^^ seems insolubule?? JH for "DU" is only instance of this pattern

    // POIGNANCY  P OY1 N Y AH0 N S IY0
    // OPPOSITE of Silent Letters! too many sounds
    // KNOW, KNOWLEDGE

    SECTION("get_word_with_marked_silent_letters") {
        std::string word{"DUMB"};
        std::string prefix {"<span class='silent-letters'>"};
        std::string suffix {"</span>"};

        std::string result = get_word_with_marked_silent_letters(word, dict.find_phones(word), prefix, suffix, false, "");
        REQUIRE(result == "DUM<span class='silent-letters'>B</span>");
    }


}


