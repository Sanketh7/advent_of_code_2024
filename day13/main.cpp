#include <iostream>
#include <complex>
#include <regex>
#include <cassert>
#include <vector>
#include <optional>
#include <climits>
#include <numeric>

struct Machine {
    std::complex<int64_t> a, b, prize;
};

std::ostream& operator<<(std::ostream& os, const Machine& machine) {
    os << '[' << machine.a << ", " << machine.b << ", " << machine.prize << ']';
    return os;
}

std::complex<int64_t> parse_line(std::string line) {
    static std::regex number_pat{"(\\d)+"};
    std::smatch matches;
    assert(std::regex_search(line, matches, number_pat));
    int64_t x = std::stoll(matches[0]);
    line = matches.suffix().str();
    assert(std::regex_search(line, matches, number_pat));
    int64_t y = std::stoll(matches[0]);
    return {x, y};
}

std::optional<int64_t> solve_machine(const Machine& machine) {
    int64_t best = INT64_MAX;
    for (int64_t m = 0; m <= 100; m++) {
        for (int64_t n = 0; n <= 100; n++) {
            if (m * machine.a + n * machine.b == machine.prize) {
                best = std::min(best, 3 * m + n);
            }
        }
    }
    if (best == INT64_MAX) {
        return {};
    }
    return best;
}

std::optional<int64_t> solve_machine_better(const Machine& machine) {
    int64_t a1 = machine.a.real(), a2 = machine.a.imag();
    int64_t b1 = machine.b.real(), b2 = machine.b.imag();
    int64_t p1 = machine.prize.real(), p2 = machine.prize.imag();

    int64_t n = (a1 * p2 - a2 * p1) / (a1 * b2 - a2 * b1);
    int64_t m = (p1 - b1 * n) / a1;
    if (m * machine.a + n * machine.b != machine.prize) return {};
    return 3 * m + n;
}

int64_t part1(const std::vector<Machine>& machines) {
    int64_t ans = 0;
    for (const Machine& machine : machines) {
        std::optional<int64_t> maybe_tokens = solve_machine(machine);
        if (maybe_tokens.has_value()) {
            ans += *maybe_tokens;
        }
    }
    return ans;
}

int64_t part2(std::vector<Machine> machines) {
    int64_t ans = 0;
    for (Machine& machine : machines) {
        machine.prize += std::complex<int64_t>{10000000000000, 10000000000000};
        std::optional<int64_t> maybe_tokens = solve_machine_better(machine);
        if (maybe_tokens.has_value()) {
            ans += *maybe_tokens;
        }
    }
    return ans;
}

int main() {
    std::vector<Machine> machines;
    std::string line;
    while (std::getline(std::cin, line)) {
        std::complex<int64_t> a = parse_line(line);
        std::getline(std::cin, line);
        std::complex<int64_t> b = parse_line(line);
        std::getline(std::cin, line);
        std::complex<int64_t> prize = parse_line(line);
        std::getline(std::cin, line); // blank line
        machines.emplace_back(Machine{.a = a, .b = b, .prize = prize});
    }

    // int64_t ans = part1(machines);
    int64_t ans = part2(machines);
    std::cout << ans << std::endl;
}