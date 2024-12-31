#include <iostream>
#include <vector>
#include <string_view>
#include <cassert>
#include <complex>

enum class Move {
    UP, DOWN, LEFT, RIGHT
};

Move parse_move(char c) {
    switch (c) {
        case '^': return Move::UP;
        case 'v': return Move::DOWN;
        case '<': return Move::LEFT;
        case '>': return Move::RIGHT;
    }
    throw std::invalid_argument{"bad move"};
}

char move_to_char(Move move) {
    switch (move) {
        case Move::UP: return '^';
        case Move::DOWN: return 'v';
        case Move::LEFT: return '<';
        case Move::RIGHT: return '>';
    }
    throw std::invalid_argument{"unexpected move"};
}

std::complex<int> move_to_delta(Move move) {
    switch (move) {
        case Move::UP: return {-1, 0};
        case Move::DOWN: return {1, 0};
        case Move::LEFT: return {0, -1};
        case Move::RIGHT: return {0, 1};
    }
    throw std::invalid_argument{"unexpected move"};
}

bool is_vert_move(Move move) {
    return move == Move::UP || move == Move::DOWN;
}

enum class Cell {
    EMPTY, ROBOT, BOX, WALL, LEFT_BOX, RIGHT_BOX
};

Cell parse_cell(char c) {
    switch (c) {
        case '.': return Cell::EMPTY;
        case '@': return Cell::ROBOT;
        case 'O': return Cell::BOX;
        case '#': return Cell::WALL;
        case '[': return Cell::LEFT_BOX;
        case ']': return Cell::RIGHT_BOX;
    }
    throw std::invalid_argument{"bad cell"};
}

char cell_to_char(Cell cell) {
    switch (cell) {
        case Cell::EMPTY: return '.';
        case Cell::ROBOT: return '@';
        case Cell::BOX: return 'O';
        case Cell::WALL: return '#';
        case Cell::LEFT_BOX: return '[';
        case Cell::RIGHT_BOX: return ']';
    }
    throw std::invalid_argument{"unexpected cell"};
}

struct Action {
    std::complex<int> from;
    std::complex<int> to;
};

class Grid {
public:
    void add_row(std::vector<Cell>&& row) {
        if (!_rows.empty()) {
            assert(static_cast<int>(row.size()) == _n);
        } else {
            _n = row.size();
        }
        _m++;
        _rows.emplace_back(std::move(row));
    }

    int m() const { return _m; }
    int n() const { return _n; }
    bool validate_pos(int r, int c) const { 
        return 0 <= r && r < _m && 0 <= c && c < _n;
    }
    bool validate_pos(std::complex<int> pos) const {
        return validate_pos(pos.real(), pos.imag());
    }

    Cell& at(int r, int c) {
        assert(validate_pos(r, c));
        return _rows[r][c];
    }
    const Cell& at_const(int r, int c) const {
        assert(validate_pos(r, c));
        return _rows[r][c];
    }
    Cell& at(std::complex<int> pos) {
        return at(pos.real(), pos.imag());
    }

    bool push(std::complex<int> pos, Move move) {
        return push(pos.real(), pos.imag(), move);
    }

    bool push(int r, int c, Move move) {
        std::complex<int> curr{r, c};
        std::complex<int> next = curr + move_to_delta(move);
        if (!validate_pos(next)) return false;
        if (at(next) == Cell::BOX) {
            push(next, move);
        } else if (at(next) == Cell::LEFT_BOX || at(next) == Cell::RIGHT_BOX) {
            if (is_vert_move(move)) {
                std::complex<int> next2 = at(next) == Cell::LEFT_BOX ? next + std::complex<int>{0, 1} : next - std::complex<int>{0, 1};
                assert(validate_pos(next2));
                size_t old_size = _history.size();
                bool ok = push(next, move) && push(next2, move);
                if (!ok) {
                    undo_history(old_size);
                }
            } else {
                push(next, move);
            }
        }
        if (at(next) == Cell::EMPTY) {
            at(next) = at(curr);
            at(curr) = Cell::EMPTY;
            _history.emplace_back(Action{.from = curr, .to = next});
            return true;
        }
        return false;
    }

    void push_robot(Move move) {
        for (int r = 0; r < _m; r++) {
            for (int c = 0; c < _n; c++) {
                if (at(r, c) == Cell::ROBOT) {
                    push(r, c, move);
                    return;
                }
            }
        }
    }

    int calc_score() const {
        int score = 0;
        for (int r = 0; r < _m; r++) {
            for (int c = 0; c < _n; c++) {
                switch (at_const(r, c)) {
                    case Cell::BOX:
                    case Cell::LEFT_BOX:
                        score += 100 * r + c;
                        break;
                    default: break;
                }
            }
        }
        return score;
    }
private:
    int _m{0};
    int _n{0};
    std::vector<std::vector<Cell>> _rows;
    std::vector<Action> _history;

    void undo_history(size_t old_size) {
        assert(_history.size() >= old_size);
        while (_history.size() > old_size) {
            Action action = std::move(_history.back());
            _history.pop_back();
            at(action.from) = at(action.to);
            at(action.to) = Cell::EMPTY;
        }
    }
};

std::ostream& operator<<(std::ostream& os, const Grid& grid) {
    for (int r = 0; r < grid.m(); r++) {
        for (int c = 0; c < grid.n(); c++) {
            os << cell_to_char(grid.at_const(r, c));
        }
        os << '\n';
    }
    return os;
}

int part1and2(Grid& grid, const std::vector<Move>& moves) {
    for (const Move move : moves) {
        grid.push_robot(move);
    }
    return grid.calc_score();
}

int main() {
    Grid grid, grid2;
    std::vector<Move> moves;

    std::string line;
    while (std::getline(std::cin, line) && !line.empty()) {
        std::vector<Cell> row;
        std::vector<Cell> row2;
        row.reserve(line.size());
        row2.reserve(line.size() * 2);
        for (char c : line) {
            row.push_back(parse_cell(c));
        }
        grid.add_row(std::move(row));
        for (char c : line) {
            Cell cell = parse_cell(c);
            switch (cell) {
                case Cell::EMPTY:
                case Cell::WALL:
                    row2.push_back(cell);
                    row2.push_back(cell);
                    break;
                case Cell::ROBOT:
                    row2.push_back(Cell::ROBOT);
                    row2.push_back(Cell::EMPTY);
                    break;
                case Cell::BOX:
                    row2.push_back(Cell::LEFT_BOX);
                    row2.push_back(Cell::RIGHT_BOX);
                    break;
                default:
                    throw std::invalid_argument{"unexpected cell"};
            }
        }
        grid2.add_row(std::move(row2));
    }
    while (std::getline(std::cin, line)) {
        for (char c : line) {
            moves.emplace_back(parse_move(c));
        }
    }

    // int ans = part1and2(grid, moves);
    int ans = part1and2(grid2, moves);
    std::cout << ans << std::endl;
}