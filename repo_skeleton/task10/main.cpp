#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cctype>
#include <cstdlib>

enum TType { AND, OR, NOT, LPAREN, RPAREN, VAR, TEOF };

struct Token {
    TType type;
    char  var;
};

static std::vector<Token> tokens;
static size_t pos;
static std::map<char, int> env;

static std::vector<Token> tokenize(const std::string &s)
{
    std::vector<Token> out;
    size_t i = 0;
    while (i < s.size()) {
        if (std::isspace((unsigned char)s[i])) { i++; continue; }
        if (s[i] == '(') { out.push_back({LPAREN}); i++; continue; }
        if (s[i] == ')') { out.push_back({RPAREN}); i++; continue; }
        if (std::isupper((unsigned char)s[i])) {
            size_t start = i;
            while (i < s.size() && std::isupper((unsigned char)s[i])) i++;
            std::string w = s.substr(start, i - start);
            if      (w == "AND") out.push_back({AND});
            else if (w == "OR")  out.push_back({OR});
            else if (w == "NOT") out.push_back({NOT});
            else if (w.size() == 1) out.push_back({VAR, w[0]});
            else { std::cerr << "Unknown token: " << w << "\n"; std::exit(1); }
            continue;
        }
        std::cerr << "Unexpected character: " << s[i] << "\n";
        std::exit(1);
    }
    out.push_back({TEOF});
    return out;
}

static int parse_or();
static int parse_and();
static int parse_not();

static int parse_atom()
{
    if (pos >= tokens.size()) { std::cerr << "Unexpected end\n"; std::exit(1); }
    Token &t = tokens[pos];
    if (t.type == VAR)    { pos++; return env[t.var]; }
    if (t.type == LPAREN) {
        pos++;
        int v = parse_or();
        if (pos >= tokens.size() || tokens[pos].type != RPAREN) {
            std::cerr << "Expected ')'\n"; std::exit(1);
        }
        pos++;
        return v;
    }
    std::cerr << "Syntax error\n"; std::exit(1);
}

static int parse_not()
{
    if (pos < tokens.size() && tokens[pos].type == NOT) { pos++; return !parse_not(); }
    return parse_atom();
}

static int parse_and()
{
    int v = parse_not();
    while (pos < tokens.size() && tokens[pos].type == AND) {
        pos++;
        int r = parse_not();
        v = v & r;
    }
    return v;
}

static int parse_or()
{
    int v = parse_and();
    while (pos < tokens.size() && tokens[pos].type == OR) {
        pos++;
        int r = parse_and();
        v = v | r;
    }
    return v;
}

int main()
{
    std::string line;
    if (!std::getline(std::cin, line)) { std::cerr << "No input\n"; return 1; }

    tokens = tokenize(line);

    std::set<char> vset;
    for (auto &t : tokens)
        if (t.type == VAR) vset.insert(t.var);

    std::vector<char> vars(vset.begin(), vset.end());
    int n = (int)vars.size();
    if (n == 0) { std::cerr << "No variables\n"; return 1; }

    for (char c : vars) std::cout << c << ' ';
    std::cout << "Result\n";

    for (int mask = 0; mask < (1 << n); mask++) {
        for (int i = 0; i < n; i++) {
            env[vars[i]] = (mask >> (n - 1 - i)) & 1;
            std::cout << env[vars[i]] << ' ';
        }
        pos = 0;
        std::cout << parse_or() << '\n';
    }

    if (tokens[pos].type != TEOF) { std::cerr << "Unexpected trailing tokens\n"; return 1; }
    return 0;
}
