#include <iostream>
#include <vector>
#include <ranges>

bool is_safe(const std::vector<int>& report) {
    if (report.size() < 2) return true;
    if (report[0] == report[1]) return false;
    bool incr = report[1] > report[0];
    for (int i = 1; i < report.size(); i++) {
        int diff = report[i] - report[i-1];
        if (incr && diff <= 0) return false;
        if (!incr && diff >= 0) return false;
        if (abs(diff) < 1 || abs(diff) > 3) return false;
    }
    return true;
}

int part1(const std::vector<std::vector<int>>& reports) {
    int ans = 0;
    for (const auto& report : reports) {
        if (is_safe(report)) {
            ++ans;
        }
    }
    return ans;
}

int part2(const std::vector<std::vector<int>>& reports) {
    int ans = 0;
    for (const auto& report : reports) {
        if (is_safe(report)) {
            ++ans;
            goto next_report;
        }
        for (int i = 0; i < report.size(); i++) {
            std::vector<int> tmp;
            for (int j = 0; j < report.size(); j++) {
                if (j != i) {
                    tmp.push_back(report[j]);
                }
            }
            if (is_safe(tmp)) {
                ++ans;
                goto next_report;
            }
        }
        next_report: ;
    }
    return ans;
}

int main() {
    std::vector<std::vector<int>> reports;
    std::string line;
    while (std::getline(std::cin, line)) {
        auto sv = line 
            | std::views::split(std::string_view{" "}) 
            | std::views::transform([](auto&& s) { 
                return std::stoi(std::string(s.begin(), s.end())); 
            });
        reports.emplace_back(sv.begin(), sv.end());
    }

    // int ans = part1(reports);
    int ans = part2(reports);
    std::cout << ans << std::endl;
}