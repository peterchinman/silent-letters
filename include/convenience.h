#pragma once

#include <iostream>
#include <regex>
#include <string>
#include <sstream>


// trim white space from start (in place)
inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim white space from end (in place)
inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// strip punctuation from a text, returning a vector of words in order
inline std::vector<std::string> strip_punctuation(const std::string& text){
   std::istringstream iss(text);
   std::vector<std::string> output{};
   std::string word{};
   while (iss >> word) {
      std::regex word_regex("[a-zA-Z]+([-']?[a-zA-Z]+)*");

      for (std::sregex_iterator it{word.begin(), word.end(), word_regex}, end{}; it != end; ++it) {
         std::smatch match = *it;
         std::string match_str = match.str();
         // put it in lower case
         output.emplace_back(match_str);
      }
   }
   return output;
}
