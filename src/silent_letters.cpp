#include "CMU_Dict.h"
#include "silent_letters.h"
#include <iostream>
#include <sstream>

std::unordered_map<std::string, std::vector<std::string>> phone_realizations = {
    {"B", {"B"}},
    {"CH", {"CH"}},
    {"D", {"D"}},
    {"DH", {"TH"}},
    {"F", {"F", "GH"}},
    {"G", {"G"}},
    {"HH", {"H"}},
    {"JH", {"J", "G"}},
    {"K", {"K", "CK", "C"}},
    {"L", {"L", "LL"}},
    {"M", {"M", "MM"}},
    {"N", {"N", "NN"}},
    {"NG", {"NG"}},
    {"P", {"P"}},
    {"R", {"R"}},
    {"S", {"S", "C"}},
    {"SH", {"SH", "TI"}},
    {"T", {"T"}},
    {"TH", {"TH"}},
    {"V", {"V"}},
    {"W", {"W"}},
    {"Y", {"Y", "U"}},
    {"Z", {"Z", "S"}},
    {"ZH", {"Z", "S"}}
};

std::vector<std::string> phone_to_graphemes(const std::string& phone) {
    return phone_realizations[phone];
}

// struct Match_Result {
//     bool valid{};
//     bool found_silent_letters{};
//     std::size_t match_start{};
//     std::size_t match_end{};
//     std::size_t silent_letter_start{};
//     std::size_t silent_letter_end{};
// };

// Note word_index is passed by reference. This will update the current word_index!
Match_Result find_phone_grapheme_in_word(const std::string& phone, const std::string& word, std::size_t& word_index) {
    Match_Result match_result{};
    std::vector<std::string> graphemes{phone_to_graphemes(phone)};
    // is there a grapheme that matches the word
    for(const auto & grapheme : graphemes) {
        // we'll need to know if this is our fist match
        bool found_first_letter_yet{};
        bool first_loop{true};
        bool failure{};
        bool found_silent_letters{};
        // Defining a temp index so that we can step thru the word, looking for grapheme, but knowing we might fail. If succcessful match we'll want to update word_index to temp_word_index;
        std::size_t temp_word_index{word_index};


        std::size_t temp_match_start{};
        std::size_t temp_match_end{};   

        // Store start/end indexes of any potential silent letters. If successful match, we'll want to send this in the result.  
        std::size_t temp_silent_letter_start{};
        std::size_t temp_silent_letter_end{};

        // iterate thru characters in grapheme
        for( const auto & c : grapheme) {
            // see if we can find a match in the word
           
            // we're using the for loop below to pick up any silent-letters and get us up to the first correct letter, so we only want to enter it if we haven't found the first correct letter yet
            // COMMENCE SILENT LETTER CHECK
            if(!found_first_letter_yet) {
                // word thru the word looking for the first char in grapheme
                for(; temp_word_index < word.size(); ++temp_word_index) {
                    // if we find a first match
                    if (word[temp_word_index] == c) {
                        temp_match_start = temp_word_index;
                        temp_match_end = temp_word_index;
                        found_first_letter_yet = true;
                        ++temp_word_index;
                        break;
                    }
                    // otherwise: potential SILENT LETTER
                    // (also potential that this grapheme doesn't match at all)
                    // if this is our first loop, start the silent_letter_index
                    else if(first_loop == true) {
                        found_silent_letters = true;
                        temp_silent_letter_start = temp_word_index;
                        temp_silent_letter_end = temp_word_index;
                        first_loop = false;
                    }
                    // else, not a match and not our first loop, update silent letter index
                    else {
                        temp_silent_letter_end = temp_word_index;
                    }
                   
                }
                if(found_first_letter_yet){
                    // continue to next character
                    continue;
                }
                else{
                    // if we get here, we haven't found the first char of the grapheme, so we've failed, break to next grapheme
                    failure = true;
                    break;
                }
                
            }
            // if we get here it means that we have found the first letter, and there are more letters, so we should continue checking the rest of the chars in the grapheme
            else {
                if(temp_word_index < word.size() && word[temp_word_index] == c){
                    temp_match_end = temp_word_index;
                    ++temp_word_index;
                }
                // FAILURE! next grapheme pls
                else {
                    failure = true;
                    break;
                }
            }
            
        }
        // if we get here witout setting the failure flag, we've succeeded in matching each letter of the grapheme, so we need to copy over all our temp variables.
        if (!failure){
            match_result.match_start = temp_match_start;
            match_result.match_end = temp_match_end;
            if (found_silent_letters) {
                match_result.found_silent_letters = true;
            }
            match_result.silent_letter_start = temp_silent_letter_start;
            match_result.silent_letter_end = temp_silent_letter_end;
            match_result.valid = true;
            word_index = temp_word_index;
            return match_result;
        }

        
    }

    match_result.valid = false;
    return match_result;

}

// std::string silent_letter_duplicator(const std::string& pronunciation, const std::string& word) {
//     // iterate thru the pronunciation
//     std::istringstream iss(pronunciation);
//     std::string symbol;
//     std::string new_word{};

//     int repeats{2};

//     std::size_t word_index{};
//     std::size_t start_silent_letters{};

//     while (iss >> symbol) {
//         // if its a vowel
//         std::cout << "symbol: " << symbol << std::endl;
//         std::cout << "word_index: " << word_index << std::endl;
//         std::cout << "start_silent_letters: " << start_silent_letters << std::endl;
//         std::cout << "new_word: " << new_word << std::endl;
//         if(std::isdigit(symbol.back())) {
//             // we want to clear the current silent letter cache
//             if (start_silent_letters != word_index) {
//                 for (int i{}; i < repeats; ++i){
//                     for (; start_silent_letters <= word_index; ++start_silent_letters) {
//                         new_word += word[start_silent_letters];
//                     }
//                 }
//             }
            

//             // and move the index up past the vowels
//             for(; word_index < word.size(); ++word_index) {
//                 if (word[word_index] == 'A' || word[word_index] == 'E' || word[word_index] == 'I' || word[word_index] == 'O' || word[word_index] == 'U' ) {
//                     new_word += word[word_index];
//                 }
//                 else {
//                     break;
//                 }
//             }

//             start_silent_letters = word_index;

//         }
//         // if next phone is not a vowel
//         else {
//             // we want to know if we find a match
//             bool match{true};
//             // get our list of possible realizations of this phone
//             std::vector<std::string> possible_graphemes{phone_realizations[symbol]};
//             // we want to check if the next characters in the word match any of the possible graphemes
//             for(const auto & grapheme : possible_graphemes) {
                
//                 std::size_t temp_index{word_index};
//                 // go letter by letter in each grapheme
//                 for (const auto & c : grapheme) {
//                     // check it against the word
//                     if (temp_index < word.size() && word[temp_index] != c) {
//                         // failed to match, we can break out of this for and go to next grapheme;
//                         match = false;
//                         break;
//                     }
//                     else {
//                         ++temp_index;
//                     }
                    
//                 }

//                 if (match) {
//                     // if we get here then we have a match
//                     // first let's clear the cache of ghost letters
//                     // we want to clear the current silent letter cache
//                     if (start_silent_letters != word_index) {
//                         for (int i{}; i < repeats; ++i){
//                             for (; start_silent_letters <= word_index; ++start_silent_letters) {
//                                 new_word += word[start_silent_letters];
//                             }
//                         }
//                     }

                    
//                     // we want add matched letters to new word and the number of letter in temp_index can be "protected"
//                     match = true;
//                     for(; word_index < temp_index; ++word_index) {
//                         new_word += word[word_index];
//                     }

                    

//                     // and we don't need to keep checking the graphemes
//                     break;
//                 }
                
                
//             }
//             // if we get here, no grapheme matched and so any letter from word_index until the next match are silent letters
//             start_silent_letters = word_index;

//         }
//     }
// }
