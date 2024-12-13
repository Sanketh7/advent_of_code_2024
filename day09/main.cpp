#include <iostream>
#include <vector>
#include <cinttypes>
#include <unordered_map>
#include <set>
#include <variant>

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

constexpr int EMPTY = -1;

std::vector<int> input_to_simple_blocks(const std::string_view input) {
    std::vector<int> blocks;
    for (int i = 0, id = 0; i < input.size(); ++i, id += (i % 2)) {
        if (i % 2 == 0) {
            for (int j = 0; j < input[i] - '0'; j++) {
                blocks.push_back(id);
            }
        } else {
            for (int j = 0; j < input[i] - '0'; j++) {
                blocks.push_back(EMPTY);
            }
        }
    }
    return blocks;
}

std::int64_t part1(std::vector<int> blocks) {
    int n = blocks.size();
    int write = 0, read = n - 1;
    while (write < read) {
        while (write < read && blocks[write] != EMPTY) ++write;
        if (write >= read) break;
        while (write < read && blocks[read] == EMPTY) --read;
        if (write >= read) break;
        blocks[write] = blocks[read];
        blocks[read] = EMPTY;
    }
    std::int64_t checksum = 0;
    for (int i = 0; i < blocks.size(); i++) {
        if (blocks[i] != EMPTY) {
            checksum += i * blocks[i];
        }
    }
    return checksum;
}

struct File {
    int id;
    int length;
};

struct Gap {
    int length;
};

using Block = std::variant<File, Gap>;

std::ostream& operator<<(std::ostream& os, const Block& block) {
    std::visit(overloaded{
        [&os](const File& file) { 
            for (int i = 0; i < file.length; i++) {
                os << file.id;
            }
        },
        [&os](const Gap& gap) {
            for (int i = 0; i < gap.length; i++) {
                os << '.';
            }
        },
    }, block);
    return os;
}

int get_length(const Block& block) {
    return std::visit(overloaded{
        [](const File& file) { return file.length; },
        [](const Gap& gap) { return gap.length; },
    }, block);
}

std::vector<Block> input_to_blocks(const std::string_view input) {
    std::vector<Block> blocks{input.size()};
    for (int i = 0, id = 0; i < input.size(); ++i, id += (i % 2)) {
        if (i % 2 == 0) {
            blocks[i] = File{.id = id, .length = input[i] - '0'};
        } else {
            blocks[i] = Gap{.length = input[i] - '0'};
        }
    }
    return blocks;
}

std::int64_t part2(std::vector<Block> blocks) {
    int n = blocks.size();
    std::vector<std::vector<File>> occupied_files(n);
    for (int i = n - 1; i >= 0; i--) {
        if (std::holds_alternative<Gap>(blocks[i])) {
            continue;
        }
        const File& file = std::get<File>(blocks[i]);
        for (int j = 0; j < i; j++) {
            if (std::holds_alternative<File>(blocks[j])) {
                continue;
            }
            Gap& gap = std::get<Gap>(blocks[j]);
            if (gap.length >= file.length) {
                occupied_files[j].push_back(file);
                gap.length -= file.length;
                blocks[i] = Gap{.length = file.length};
                goto next_iter;
            }
        }
        next_iter: ;
    }

    std::int64_t checksum = 0;
    int index = 0;
    for (int i = 0; i < blocks.size(); i++) {
        std::visit(overloaded{
            [i, &index, &checksum](const File& file) {
                for (int j = 0; j < file.length; j++, index++) {
                    checksum += index * file.id;
                }
            },
            [i, &occupied_files, &checksum, &index](const Gap& gap) {
                for (const auto& file : occupied_files[i]) {
                    for (int j = 0; j < file.length; j++, index++) {
                        checksum += index * file.id;
                    }
                }
                index += gap.length;
            }
        }, blocks[i]);
    }
    return checksum;
}

int main() {
    std::string input;
    std::getline(std::cin, input);
    std::vector<int> simple_blocks = input_to_simple_blocks(input);
    std::vector<Block> blocks = input_to_blocks(input);

    // std::int64_t ans = part1(simple_blocks);
    std::int64_t ans = part2(blocks);
    std::cout << ans << std::endl;
}