#include <iostream>
#include <ranges>
#include <unordered_map>
#include <vector>
#include <cinttypes>
#include <span>

struct Equation {
    std::int64_t lhs;
    std::vector<std::int64_t> rhs;
};

std::ostream& operator<<(std::ostream& os, const Equation& equation) {
    os << equation.lhs << ": ";
    for (const auto& x : equation.rhs) {
        os << x << " ";
    }
    return os;
}

bool is_valid_equation(std::int64_t lhs, std::span<const std::int64_t> rhs, std::int64_t acc) {
    if (rhs.empty()) {
        return lhs == acc;
    }
    std::int64_t val = rhs[0];
    std::span<const std::int64_t> rem{rhs.begin() + 1, rhs.end()};
    return is_valid_equation(lhs, rem, acc != 0 ? val * acc : val) || is_valid_equation(lhs, rem, val + acc);
}

std::int64_t part1(const std::vector<Equation>& equations) {
    std::int64_t ans = 0;
    for (auto& equation : equations) {
        if (is_valid_equation(equation.lhs, {equation.rhs}, 0)) {
            ans += equation.lhs;
        }
    }
    return ans;
}

std::optional<std::int64_t> safe_concat(std::int64_t a, std::int64_t b) {
    std::string str = std::to_string(a) + std::to_string(b);
    try {
        return std::stoll(str);
    } catch (const std::exception& e) {
        return std::nullopt;
    }
}

bool is_valid_equation2(std::int64_t lhs, std::span<const std::int64_t> rhs, std::int64_t acc) {
    if (rhs.empty()) {
        return lhs == acc;
    }
    std::int64_t val = rhs[0];
    std::span<const std::int64_t> rem{rhs.begin() + 1, rhs.end()};
    bool ret = false;
    if (acc > 0) {
        std::optional<std::int64_t> maybe_concat = safe_concat(acc, val);
        if (maybe_concat.has_value()) {
            ret |= is_valid_equation2(lhs, rem, *maybe_concat);
        }
    }
    ret |= is_valid_equation2(lhs, rem, acc != 0 ? val * acc : val) || is_valid_equation2(lhs, rem, val + acc);
    return ret;
}

std::int64_t part2(const std::vector<Equation>& equations) {
    std::int64_t ans = 0;
    for (auto& equation : equations) {
        if (is_valid_equation2(equation.lhs, {equation.rhs}, 0)) {
            ans += equation.lhs;
        }
    }
    return ans;
}

int main() {
    std::vector<Equation> equations;
    std::string line;
    while (std::getline(std::cin, line)) {
        auto colon = line.find(':');
        std::int64_t lhs = std::stoll(line.substr(0, colon));
        auto sv = line.substr(colon + 2)
            | std::views::split(' ')
            | std::views::transform([](auto&& s) {
                return std::stoll(std::string(s.begin(), s.end()));
            });
        equations.push_back({.lhs = lhs, .rhs = {sv.begin(), sv.end()}});
    }

    // std::int64_t ans = part1(equations);
    std::int64_t ans = part2(equations);
    std::cout << ans << std::endl;
}