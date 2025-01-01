#include <iostream>
#include <ranges>
#include <vector>
#include <cassert>

int64_t calc_ways(const std::string& design, const std::vector<std::string>& patterns) {
    int n = design.size();
    if (n == 0) return 1;
    std::vector<int64_t> dp(n, 0);
    for (const auto& pattern : patterns) {
        if (design.starts_with(pattern)) {
            dp[pattern.size() - 1]++;
        }
    }
    for (int i = 0; i < n; i++) {
        for (const auto& pattern : patterns) {
            int j = i - static_cast<int>(pattern.size());
            if (j < 0) continue;
            std::string_view sv{design.begin() + j + 1, design.begin() + i + 1};
            if (sv == pattern) {
                dp[i] += dp[j];
            }
        }
    }
    return dp.back();
}

int64_t part1(const std::vector<std::string>& patterns, const std::vector<std::string>& designs) {
    int64_t ans = 0;
    for (const auto& design : designs) {
        if (calc_ways(design, patterns) > 0) {
            ++ans;
        }
    }
    return ans;
}

int64_t part2(const std::vector<std::string>& patterns, const std::vector<std::string>& designs) {
    int64_t ans = 0;
    for (const auto& design : designs) {
        ans += calc_ways(design, patterns);
    }
    return ans;
}

int main() {
    std::string line;
    std::getline(std::cin, line);
    auto sv = line 
        | std::views::split(std::string_view{", "})
        | std::views::transform([](auto&& s) { 
            return std::string(s.begin(), s.end());
        });
    std::vector<std::string> patterns(sv.begin(), sv.end());
    std::getline(std::cin, line); // blank line
    std::vector<std::string> designs;
    while (std::getline(std::cin, line)) {
        designs.emplace_back(std::move(line));
    }

    // int64_t ans = part1(patterns, designs);
    int64_t ans = part2(patterns, designs);
    std::cout << ans << std::endl;
}