#pragma once

#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


class CMU_Dict {
private:
    // map of words with their CMU ARPABET_pronunciations, separated by spaces
    std::unordered_map<std::string, std::vector<std::string>> m_dictionary {};

    // All vowels used in CMU
    const std::unordered_set<std::string> CMU_VOWELS = {
        "AA", "AE", "AH", "AO", "EH",
        "ER", "IH", "IY", "UH", "UW", // 10 monopthongs
        "AW", "AY", "EY", "OW", "OY" // 5 dipthongs
    };


public:
    // assumes filepath "../data/cmudict-0.7b"
    bool import_dictionary();

    /**
     * Get array of possible pronunciations from CMUDict.
     * 
     * Throws a std::exception if word not found.
     * 
     * @param (string) word: English word to look up.
     * @return Vector of strings, of the possible pronunciations recorded in the dictionary, each of which are a string of ARAPBET phones separated by spaces.
    */
    std::vector<std::string> find_phones(std::string word);

    /**
     * Get an array of possible pronuncitation of each word from a text.
     * 
     * If a word is not found, vector will contain the word that was searched for, and bool is marked false.
     * 
     * @param text (string): text to look up
     * @return An array of pairs, each pair has 1. array of possible pronunciations, 2. a bool flag indicating if word was found.
    */
    std::vector<std::pair<std::vector<std::string>, bool>> text_to_phones(const std::string & text);


    // Takes a string of phones separated by spaces, returns a string of the stresses. 0 no stress, 1 primary stress, 2 secondary stress.
    std::string phone_to_stress(const std::string& phones);

    // Takes an English word and returns a vector of possible stresses. 
    std::vector<std::string> word_to_stresses(const std::string& word);

    // Take a string of phones separeated by spaces, returns a int number of syllables
    int phone_to_syllable_count(const std::string& phones);

    // Takes an English word, returns a vector of possible stresses.
    std::vector<int> word_to_syllables(const std::string& word);

    /**
     * Convert meter in "x/x /x/x /" form to a vector of bools, where 'x' is false and '/' is true;
     * 
     * Also removes any whitespace.
     * 
     * @param meter (string): meter string containing 'x', '/' and possible white-space
     * @return Vector of Bools. 
    */
    std::vector<bool> meter_to_binary(const std::string& meter);


     /**
     * Convert meter in form of "x/x /x/(x /)" to a set of vector<int>, where 'x' is 0 and '/' is 1, where the set contains all the possible meters that could conform to the options.
     * 
     * Also removes any whitespace.
     * 
     * @param meter (string): meter string containing 'x', '/' and possible white-space
     * @return Set of Vector of Ints.
    */
    std::set<std::vector<int>> fuzzy_meter_to_binary_set(const std::string& meter);


    struct Check_Validity_Result {
        bool is_valid{};
        std::vector<std::string> unrecognized_words;
    };

    /**
     * Check whether a given text and meter combination is valid.
     * 
     * Checks all possible pronunciations of each word in a text against all possible meters given optional meters.
     * 
     * Single-syllable words automatically passed, given the ambiguity of determining their meter. Multi-syllabic words are checked to see whether their pattern of stresses matches the meter, with various exceptions coded in. E.g. a "secondarily stressed" syllable can function within a meter as an ustressed syllable. (TODO: consider the scenario where an unstressed syllable can function within a meter as stressed...)
     * 
     * 
     * @param text (string): string of english words to check against the meter
     * @param meter (string): meter string containing 'x', '/' and possible white-space
     * @return a struct containing a bool is_valid and a vector of strings of unrecognized words
    */
    Check_Validity_Result check_meter_validity(const std::string& text, const std::string& meter);


    /**
     * Check whether a given text and syllable count combination is valid.
     * 
     * Checks all possible pronunciations of each word in a text against the syllable count
     * 
     * @param text (string): string of english words
     * @param syllable_cout (int): number of syllables
     * @return a struct containing a bool is_valid and a vector of strings of unrecognized words
    */
    Check_Validity_Result check_syllable_validity(const std::string& text, int syllable_count);



    /**
     * Check whether a given text and syllable count combination is valid.
     * 
     * Checks all possible pronunciations of each word in a text against the syllable count
     * 
     * @param text (string): string of english words
     * @param syllable_cout (int): number of syllables
     * @return a struct containing a bool is_valid and a vector of strings of unrecognized words
    */
    Check_Validity_Result check_end_rhyme_validity(const std::string& line1, const std::string& line2, int vowel_distance, int consonant_difference);
};
