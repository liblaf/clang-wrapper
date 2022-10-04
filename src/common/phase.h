#ifndef PHASE_H_
#define PHASE_H_

#include <string>
#include <vector>

class Phase {
 public:
  Phase(const int id = -1, const std::string& tool = "",
        const std::string& input = "",
        const std::vector<int>& dependencies = {},
        const std::string& output = "");

 public:
  std::string to_string() const;

 public:
  static std::vector<Phase> parse_phases(const std::string& output);
  static bool has_link_stage(const std::vector<Phase>& phases);

 protected:
  static Phase parse_phase(const std::string& line);

 public:
  int id;
  std::string tool;
  std::string input;
  std::vector<int> dependencies;
  std::string output;
};

#endif  // PHASE_H_
