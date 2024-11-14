#pragma once

#include <unordered_map>
#include <vector>
#include <string>



std::vector<std::string> phone_to_graphemes(const std::string& phone);

struct Match_Result {
    bool valid{};
    bool found_silent_letters{};
    std::size_t match_start{};
    std::size_t match_end{};
    std::size_t silent_letter_start{};
    std::size_t silent_letter_end{};
};

Match_Result find_phone_grapheme_in_word(const std::string& phone, const std::string& word, std::size_t& word_index);


// std::string silent_letter_duplicator(const std::string& pronunciation, const std::string& word);
