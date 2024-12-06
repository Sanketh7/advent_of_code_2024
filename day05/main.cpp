#include <iostream>
#include <ranges>
#include <vector>
#include <unordered_map>
#include <set>
#include <queue>
#include <algorithm>
#include <cassert>

bool is_update_valid(const std::vector<std::pair<int, int>>& rules, const std::vector<int>& update) {
    std::unordered_map<int, int> index;
    for (int i = 0; i < update.size(); i++) {
        index[update[i]] = i;
    }
    for (const auto [first, second] : rules) {
        if (auto it1 = index.find(first); it1 != index.end()) {
            if (auto it2 = index.find(second); it2 != index.end()) {
                if (it1->second >= it2->second) {
                    return false;
                }
            }
        }
    }
    return true;
}

int part1(const std::vector<std::pair<int, int>>& rules, const std::vector<std::vector<int>>& updates) {
    int ans = 0;
    for (const auto& update : updates) {
        if (is_update_valid(rules, update)) {
            ans += update[update.size() / 2];
        }
    }
    return ans;
}

std::vector<int> toposort(const std::vector<std::pair<int, int>>& rules, const std::vector<int>& update) {
    std::unordered_map<int, std::set<int>> adj;
    std::unordered_map<int, int> indegs;
    for (int x : update) {
        indegs[x] = 0;
    }
    std::set<int> update_set{update.begin(), update.end()};
    for (const auto [first, second] : rules) {
        if (update_set.count(first) && update_set.count(second)) {
            adj[first].insert(second);
            ++indegs[second];
        }
    }
    std::vector<int> order;
    std::queue<int> q;
    std::set<int> vis;
    for (auto [x, indeg] : indegs) {
        if (indeg == 0) {
            q.push(x);
            vis.insert(x);
        }
    }
    while (!q.empty()) {
        int x = q.front();
        q.pop();
        order.push_back(x);
        for (int y : adj[x]) {
            --indegs[y];
            if (vis.find(y) == vis.end() && indegs[y] == 0) {
                vis.insert(y);
                q.push(y);
            }
        }
    }
    assert(order.size() == indegs.size());
    return order;
}

int part2(const std::vector<std::pair<int, int>>& rules, const std::vector<std::vector<int>>& updates) {
    int ans = 0;
    for (const auto& update : updates) {
        if (is_update_valid(rules, update)) {
            continue;
        }
        std::vector<int> order = toposort(rules, update);
        std::unordered_map<int, int> order_index;
        for (int i = 0; i < order.size(); i++) {
            order_index[order[i]] = i;
        }
        std::vector<int> tmp(update.begin(), update.end());
        std::sort(tmp.begin(), tmp.end(), [&order_index](int a, int b) {
            return order_index[a] < order_index[b];
        });
        ans += tmp[tmp.size() / 2];
    }
    return ans;
}

int main() {
    std::vector<std::pair<int, int>> rules;
    std::vector<std::vector<int>> updates;
    std::string line;
    while (std::getline(std::cin, line) && !line.empty()) {
        auto sv = line 
            | std::views::split('|') 
            | std::views::transform([](auto&& s) { 
                return std::stoi(std::string(s.begin(), s.end())); 
            });
        int first = *sv.begin();
        int second = *(++sv.begin());
        rules.emplace_back(first, second);
    }
    while (std::getline(std::cin, line)) {
        auto sv = line 
            | std::views::split(',') 
            | std::views::transform([](auto&& s) { 
                return std::stoi(std::string(s.begin(), s.end())); 
            });
        updates.emplace_back(sv.begin(), sv.end());
    }

    // int ans = part1(rules, updates);
    int ans = part2(rules, updates);
    std::cout << ans << std::endl;
}