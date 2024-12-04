#include <iostream>
#include <regex>
#include <cinttypes>
#include <string_view>
#include <variant>

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

std::uint64_t part1(const std::string& input) {
    std::uint64_t ans = 0;
    std::regex mul_regex{"mul\\((\\d{1,3}),(\\d{1,3})\\)"};
    std::smatch match;
    auto start = input.begin();
    while (std::regex_search(start, input.end(), match, mul_regex)) {
        int a = std::stoi(match[1]);
        int b = std::stoi(match[2]);
        ans += a * b;
        start = match.suffix().first;
    }
    return ans;
}

struct DoInstruction {};

struct DontInstruction {};

struct MulInstruction {
    static const std::regex pattern;
    int x, y;
};
const std::regex MulInstruction::pattern{"mul\\((\\d{1,3},(\\d{1,3})\\))"};

using Instruction = std::variant<DoInstruction, DontInstruction, MulInstruction>;

std::ostream& operator<<(std::ostream& os, const Instruction& instr) {
    std::visit(overloaded{
        [&os](const DoInstruction& do_instr) { 
            os << "do()";
        },
        [&os](const DontInstruction& dont_instr) {
            os << "don't()";
        },
        [&os](const MulInstruction& mul_instr) {
            os << "mul(" << mul_instr.x << "," << mul_instr.y << ")";
        }
    }, instr);
    return os;
}

std::uint64_t part2(const std::string& input) {
    static const std::regex combined_pattern{"(mul\\(\\d{1,3},\\d{1,3}\\))|(do\\(\\))|(don't\\(\\))"};
    std::smatch match;
    auto start = input.begin();
    std::vector<Instruction> instrs;
    while (std::regex_search(start, input.end(), match, combined_pattern)) {
        start = match.suffix().first;
        const std::string& raw_instr = match[0];
        if (raw_instr == "do()") {
            instrs.push_back(DoInstruction{});
        } else if (raw_instr == "don't()") {
            instrs.push_back(DontInstruction{});
        } else {
            std::regex_search(raw_instr, match, MulInstruction::pattern);
            instrs.push_back(MulInstruction{
                .x = std::stoi(match[1]), 
                .y = std::stoi(match[2])
            });
        }
    }
    bool active = true;
    std::uint64_t ans = 0;
    for (auto& instr : instrs) {
        std::visit(overloaded{
            [&active](const DoInstruction& do_instr) { 
                active = true;
            },
            [&active](const DontInstruction& dont_instr) {
                active = false;
            },
            [active,&ans](const MulInstruction& mul_instr) {
                if (active) {
                    ans += mul_instr.x * mul_instr.y; 
                }
            }
        }, instr);
    }
    return ans;
}

int main() {
    std::string input;
    std::string line;
    while (std::getline(std::cin, line)) {
        input += line;
    }
    // std::uint64_t ans = part1(input);
    std::uint64_t ans = part2(input);
    std::cout << ans << std::endl;
}