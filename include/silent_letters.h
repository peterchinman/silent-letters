#pragma once



#include <unordered_map>
#include <vector>
#include <string>



std::vector<std::string> phone_to_graphemes(const std::string& phone);

struct Grapheme_Match_Result {
    std::string matched_grapheme{};
    bool valid{};
    bool found_silent_letters{};
    std::size_t match_start{};
    // note: match_end is an index that points at the character AFTER the last matching grapheme
    std::size_t match_end{};
    std::size_t silent_letter_start{};
    // note: silent_letter_end is an index that points at the character AFTER the last silent letter 
    std::size_t silent_letter_end{};
};

Grapheme_Match_Result check_next_phone_for_grapheme_match_and_get_silent_letters(const std::string& phone, const std::string& word, const std::size_t& word_index);

struct Word_Silent_Letter_Results {
    bool phones_not_found{};
    std::vector<std::pair<std::size_t, std::size_t>> silent_letter_indices{};
};

bool is_within_range(const std::vector<std::pair<std::size_t, std::size_t>>& ranges, std::size_t i);
bool is_start_of_range(const std::vector<std::pair<std::size_t, std::size_t>>& ranges, const std::size_t i);
bool is_end_of_range(const std::vector<std::pair<std::size_t, std::size_t>>& ranges, const std::size_t i);


Word_Silent_Letter_Results pronunciations_to_silent_letter_indices(const std::string & word, const std::vector<std::string> & possible_pronunciations);

void print_word_with_underscores(std::string& word, const CMU_Dict & dict);

std::string get_word_with_marked_silent_letters(const std::string & word, const std::vector<std::string> & pronunciations, const std::string& insertSilentLetterStart, const std::string& insertSilentLetterEnd, bool replaceSilentLetters, const std::string& silentLetterReplacement);


// std::string get_word_with_marked_silent_letters(std::string & word, const std::vector<std::string> & pronunciations, const std::string& insertSilentLetterStart, const std::string& insertSilentLetterEnd, bool replaceSilentLetters = false, const std::string& silentLetterReplacement = "");
