#include "CMU_Dict.h"
#include "silent_letters.h"
#include <iostream>
#include <sstream>

// ordered by longest string to shortest string so that they get checked in the at order
std::unordered_map<std::string, std::vector<std::string>> phone_realizations = {
    {"B", {"BB", "B"}},
    {"CH", {"TI", "CH"}},
    {"D", {"DD", "D"}},
    {"DH", {"TH"}},
    {"F", {"GH", "PH", "FF", "F"}},
    {"G", {"G"}},
    {"HH", {"H"}},
    {"JH", {"J", "G"}},
    {"K", {"Q", "K", "C", "X", "CH"}}, // quick, excite, schedule
    {"L", {"LL", "L"}},
    {"M", {"MM", "M"}},
    {"N", {"NN", "N"}},
    {"NG", {"NG", "NDK"}},
    {"P", {"P"}},
    {"R", {"RR", "R"}},
    {"S", {"SS", "C", "S"}},
    {"SH", {"TI", "CI", "SH", "CH"}},
    {"T", {"TT", "T", "D"}},
    {"TH", {"TH"}},
    {"V", {"V"}},
    {"W", {"W", "U"}}, // "U" for /K W/, e.g. "quick"
    {"Y", {"U", "Y", "I"}}, // "I" for "pneumonia"
    {"Z", {"Z", "S"}},
    {"ZH", {"Z", "S"}}
};

const std::unordered_set<char> ENGLISH_VOWELS = {
    'A', 'a', 'E', 'e', 'I', 'i', 'O', 'o', 'U', 'u', 'Y', 'y'
};

std::vector<std::string> phone_to_graphemes(const std::string& phone) {
    return phone_realizations[phone];
}

// struct Grapheme_Match_Result {
//     std::string matched_grapheme{}; 
//     bool valid{};
//     bool found_silent_letters{};
//     std::size_t match_start{};
//     std::size_t match_end{};
//     std::size_t silent_letter_start{};
//     std::size_t silent_letter_end{};
// };

// No more word_index. match_result.match_end now returns the same value.
Grapheme_Match_Result check_next_phone_for_grapheme_match_and_get_silent_letters(const std::string& phone, const std::string& word, const std::size_t & word_index) {
    Grapheme_Match_Result match_result{};

    // IF PHONE IS VOWEL, simpler logic
    if (isdigit(phone.back())){

        // Other than /ER/ we're going to leave vowels in place, because we don't know how long a string of vowels we might be.
        // Instead we will discard these vowels when we go looking for consonants
        // But we do want to return results

        match_result.match_start = word_index;
        match_result.match_end = word_index;
        match_result.valid = true;


        // If that vowel is an ER, we want to get the word_index to after the R, so that we don't read it in as a silent_letter
        if(phone.size() >= 2 && phone[0] == 'E' && phone[1] == 'R'){
            for(std::size_t i{word_index}; i < word.size(); ++i, ++match_result.match_end) {
                // TODO should we add from vowel up to R to match resuls?
                // TODO consider case of DUMBER
                // continue up to the R
                if(word[i] == 'R'){
                    ++i;
                    ++match_result.match_end;
                    break;
                }
            }
        }
    
        return match_result;

    }

    std::vector<std::string> possible_graphemes{phone_to_graphemes(phone)};

    // need a way of also including temp_index!!
    std::vector<Grapheme_Match_Result> successful_matches{};
    
    // run thru possible grapheme, finding matches
    for(const auto & grapheme : possible_graphemes) {

        Grapheme_Match_Result current_grapheme{};
        current_grapheme.matched_grapheme = grapheme;
        
        // sloppy programming bool flags that you should re-factor out
        bool found_first_letter_yet{};
        bool first_loop{true};
        bool failure{};
        
        std::size_t temp_word_index{word_index};

        // iterate thru characters in grapheme
        for( const auto & c : grapheme) {
            // we're using the for loop below to pick up any silent-letters and get us up to the first correct letter, so we only want to enter it if we haven't found the first correct letter yet

            // COMMENCE SILENT LETTER CHECK
            if(!found_first_letter_yet) {
                // iterate thru the word looking for the first char in grapheme
                for(; temp_word_index < word.size(); ++temp_word_index) {
                    // if we find a first match
                    if (word[temp_word_index] == c) {
                        current_grapheme.match_start = temp_word_index;
                        current_grapheme.match_end = temp_word_index + 1;
                        found_first_letter_yet = true;
                        ++temp_word_index;
                        break;
                    }
                    // otherwise: potential SILENT LETTER
                    // FIRST we want to chew thru any vowels that are at at the temp_word_index
                    if (ENGLISH_VOWELS.find(word[temp_word_index]) != ENGLISH_VOWELS.end()){
                        continue;
                    }
                    // OK we've gotten to a consonant
                    // if this is our first loop, start the silent_letter_index
                    else if(first_loop == true) {
                        current_grapheme.found_silent_letters = true;
                        current_grapheme.silent_letter_start = temp_word_index;
                        current_grapheme.silent_letter_end = temp_word_index + 1;
                        first_loop = false;
                    }
                    // else we found another silent letter, so we update silent letter index
                    else {
                        ++current_grapheme.silent_letter_end;
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
            // END SILENT LETTER CHECK

            // if we get here it means that we have found the first letter, and there are more letters, so we should continue checking the rest of the chars in the grapheme
            else {
                if(temp_word_index < word.size() && word[temp_word_index] == c){
                    ++current_grapheme.match_end;
                    ++temp_word_index;
                }
                // FAILURE! next grapheme pls
                else {
                    failure = true;
                    break;
                }
            }
            
        }
        // if we get here without setting the failure flag, we've succeeded in matching each letter of the grapheme, so we add it to out vector of successful matches
        if (!failure){
            current_grapheme.valid = true;
            successful_matches.emplace_back(current_grapheme);
        }

        
    }

    // if we have multiple successful matches we need to choose what the best one is

    if (successful_matches.size() > 0) {
        if (successful_matches.size() > 1) {
            // if multiple successful matches we want to choose the one with the minimum number of silent letters. If there are multiple of those, we want to choose whichever is the longest grapheme match

            auto min_it = std::min_element(successful_matches.begin(), successful_matches.end(), [](const Grapheme_Match_Result& a, const Grapheme_Match_Result& b) {
                auto length_a = a.silent_letter_end - a.silent_letter_start;
                auto length_b = b.silent_letter_end - b.silent_letter_start;

                if (length_a == length_b) {
                    return a.matched_grapheme.size() > b.matched_grapheme.size();
                }
                return length_a < length_b;
            });
            // if found minimum item, SEND IT
            if (min_it != successful_matches.end()) {
                return *min_it;
            }

            else {
                // we didn't find a minumum item, which should never happen
                throw std::runtime_error("No minimum item found");
            }

        }
        else return successful_matches[0];
        
    }

    // TODO is it possible to get here?

    match_result.valid = false;
    return match_result;

}

// struct Word_Silent_Letter_Results {
//     bool word_found_in_dict{};
//     bool phones_not_found{};
//     std::vector<std::pair<std::size_t, std::size_t>> silent_letter_indices{};
// };

Word_Silent_Letter_Results pronunciations_to_silent_letter_indices(const std::string & word, const std::vector<std::string> & possible_pronunciations){

    Word_Silent_Letter_Results results{};
    
    // TODO do we want to iterate thru these to be completist, or just grab the first one, which will be easy? If we iterate thru them, what are we even checking? 
    for (const std::string & pronunciation : possible_pronunciations) {
        std::vector<std::pair<std::size_t, std::size_t>> silent_letter_indices{};
        bool phones_not_found;

        std::istringstream iss{pronunciation};
        std::string phone{};

        std::size_t word_index{0};

        while (iss >> phone) {
            Grapheme_Match_Result phone_result =  check_next_phone_for_grapheme_match_and_get_silent_letters(phone, word, word_index);
            if (phone_result.valid) {
                // update the word_index
                word_index = phone_result.match_end;
                if (phone_result.found_silent_letters){
                    silent_letter_indices.emplace_back(std::make_pair(phone_result.silent_letter_start, phone_result.silent_letter_end));
                }
            }
            else {
                phones_not_found = true;
                break;
            }

        }

        // here if we successfully match all phones we immediately return the results
        // if we wanted to be a little fancier... we could collect all the successes and figure out which one seemed best
        if (!phones_not_found){

            // check if there's anything left at the end of the word

            std::pair<std::size_t, std::size_t> silent_ending{};
            for(; word_index < word.size(); ++word_index) {
                // skip vowels
                // otherwise it's a consonant, add it to the silent letters
                if(ENGLISH_VOWELS.find(word[word_index]) == ENGLISH_VOWELS.end() && (!silent_ending.first)) {
                    silent_ending.first = word_index;
                    silent_ending.second = word_index + 1;
                }
                else if(ENGLISH_VOWELS.find(word[word_index]) == ENGLISH_VOWELS.end()) {
                    silent_ending.second = word_index + 1;
                }
            }

            if (silent_ending.first) {
                silent_letter_indices.emplace_back(silent_ending);
            }
            
            results.silent_letter_indices = silent_letter_indices;
            results.phones_not_found = false;
            return results;
        }
    }

    results.phones_not_found = true;
    return results;
}

bool is_within_range(const std::vector<std::pair<std::size_t, std::size_t>>& ranges, const std::size_t i) {
    for (const auto& range : ranges) {
        if (i >= range.first && i < range.second) {
            return true;
        }
    }
    return false;
}

bool is_start_of_range(const std::vector<std::pair<std::size_t, std::size_t>>& ranges, const std::size_t i) {
    for (const auto& range : ranges) {
        if(i == range.first) {
            return true;
        }
    }

    return false;
}

bool is_end_of_range(const std::vector<std::pair<std::size_t, std::size_t>>& ranges, const std::size_t i) {
    for (const auto& range : ranges) {
        if(i == range.second) {
            return true;
        }
    }
    return false;
}

void print_word_with_underscores(std::string & word, const CMU_Dict & dict) {
    std::transform(word.begin(), word.end(), word.begin(), ::toupper);
    Word_Silent_Letter_Results results {pronunciations_to_silent_letter_indices(word, dict.find_phones(word))};
    if (results.phones_not_found){
        std::cout << word << "[err: phones_not_found]";
    }
    else {
        for (std::size_t i{}; i < word.size(); ++i){
            if (is_within_range(results.silent_letter_indices, i)){
                std::cout << '_';
            }
            else {
                std::cout << word[i];
            }
        }
    }
}

std::string get_word_with_marked_silent_letters(std::string & word, const std::vector<std::string> & pronunciations, const std::string& insertSilentLetterStart, const std::string& insertSilentLetterEnd, bool replaceSilentLetters, const std::string& silentLetterReplacement) {

    std::string word_uppercase{word};
    std::transform(word_uppercase.begin(), word_uppercase.end(), word_uppercase.begin(), ::toupper);

    Word_Silent_Letter_Results results {pronunciations_to_silent_letter_indices(word_uppercase, pronunciations)};

    if (results.phones_not_found == false) {
        std::cout << "found phones" << std::endl;

        std::string marked_up_word{word};
        std::size_t marked_up_word_index{};

        // note that here we are sending i one past the end of the word string, so that it triggers the end of word out_of_range check
        for (std::size_t i{}; i <= word.size(); ++i){

            std::cout << "current word: " << marked_up_word;

            // replace silent letters
            if (replaceSilentLetters) {    
                if(is_within_range(results.silent_letter_indices, i)){
                    marked_up_word.replace(marked_up_word_index, 1, silentLetterReplacement);
                    if (silentLetterReplacement.size() > 0) {
                        std::size_t replacementDisplacement = silentLetterReplacement.size() - 1;
                        std::cout << "replacementDisplacement: " << replacementDisplacement << std::endl;
                        marked_up_word_index += replacementDisplacement;
                    }
                    
                }
            }

            if (is_start_of_range(results.silent_letter_indices, i)){
                marked_up_word.insert(marked_up_word_index, insertSilentLetterStart);
                if (insertSilentLetterStart.size() > 0) {
                    std::size_t replacementDisplacement = insertSilentLetterStart.size() - 1;
                    std::cout << "start replacementDisplacement: " << replacementDisplacement << std::endl;
                    marked_up_word_index += replacementDisplacement;
                }
                

            }
            else if(is_end_of_range(results.silent_letter_indices, i)){
                marked_up_word.insert(marked_up_word_index + 1, insertSilentLetterEnd);
                if (insertSilentLetterEnd.size() > 0) {
                    std::size_t replacementDisplacement = insertSilentLetterEnd.size() - 1;
                    std::cout << "replacementDisplacement" << replacementDisplacement << std::endl;
                    marked_up_word_index += replacementDisplacement;
                }
            }

            ++marked_up_word_index;


        }

        return marked_up_word;
    }

    return word;
}



