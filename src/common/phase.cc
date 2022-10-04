#include "phase.h"

#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

Phase::Phase(const int id, const std::string& tool, const std::string& input,
             const std::vector<int>& dependencies, const std::string& output)
    : id(id),
      tool(tool),
      input(input),
      dependencies(dependencies),
      output(output) {}

std::string Phase::to_string() const {
  std::string result = std::to_string(this->id) + ": " + this->tool + ", ";
  if (this->input.empty()) {
    result += "{" + std::to_string(this->dependencies.front());
    for (int i = 1; i < this->dependencies.size(); ++i) {
      result += ", " + std::to_string(this->dependencies[i]);
    }
    result += "}, ";
  } else {
    result += "\"" + this->input + "\", ";
  }
  result += output;
  return result;
}

std::vector<Phase> Phase::parse_phases(const std::string& output) {
  std::vector<Phase> parse_result;
  static const auto& LINE_DELIMITER = std::regex("\\n");
  for (auto iter = std::sregex_token_iterator(output.begin(), output.end(),
                                              LINE_DELIMITER, /*submatch=*/-1);
       iter != std::sregex_token_iterator(); ++iter) {
    try {
      const auto& phase = Phase::parse_phase(iter->str());
      parse_result.push_back(phase);
    } catch (const std::exception& e) {
      // skip this line
    }
  }
  return parse_result;
}

bool Phase::has_link_stage(const std::vector<Phase>& phases) {
  for (const auto& phase : phases) {
    if (phase.tool == "linker") return true;
  }
  return false;
}

Phase Phase::parse_phase(const std::string& line) {
  Phase parse_result;
  static const auto& INPUT_PATTERN =
      std::regex("[\\|\\+\\- ]*(\\d+): ([^,]+), \\\"([^\\\"]+)\\\", (.+)");
  std::smatch match_result;
  std::regex_match(line, match_result, INPUT_PATTERN);
  if (match_result.size() == 5) {
    return Phase(
        /*id=*/std::stoi(match_result.str(1)),
        /*tool=*/match_result.str(2),
        /*input=*/match_result.str(3),
        /*dependencies=*/{},
        /*output=*/match_result.str(4));
  }
  static const auto& PHASE_PATTERN =
      std::regex("[\\|\\+\\- ]*(\\d+): ([^,]+), \\{([\\d, ]+)\\}, (.+)");
  std::regex_match(line, match_result, PHASE_PATTERN);
  if (match_result.size() == 5) {
    auto parse_result = Phase(
        /*id=*/std::stoi(match_result.str(1)),
        /*tool=*/match_result.str(2),
        /*input=*/"",
        /*dependencies=*/{},
        /*output=*/match_result.str(4));
    const auto& dependencies = match_result.str(3);
    static const auto& DELIMITER = std::regex(", ");
    for (auto iter = std::sregex_token_iterator(dependencies.begin(),
                                                dependencies.end(), DELIMITER,
                                                /*submatch=*/-1);
         iter != std::sregex_token_iterator(); ++iter) {
      parse_result.dependencies.push_back(std::stoi(iter->str()));
    }
    return parse_result;
  }
  throw std::runtime_error("invalid phase: " + line);
}
