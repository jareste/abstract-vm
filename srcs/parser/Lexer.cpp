#include "Lexer.hpp"

std::vector<Token> Lexer::tokenize(Line const& ln)
{
    std::string s = ln.text;
    auto pos = s.find(';');
    if (pos != std::string::npos)
        s = s.substr(0, pos);


    std::vector<Token> out;
    size_t i = 0;
    /* define push function */
    auto push = [&](TokenKind k, size_t start, size_t end)
    {
        out.push_back({k, s.substr(start, end - start), ln.no, (int)start + 1});
    };

    while (i < s.size())
    {
        if (std::isspace((unsigned char)s[i]))
        {
            ++i;
            continue;
        }

        if (std::isalpha((unsigned char)s[i]))
        {
            size_t start = i++;
            while (i < s.size() && (std::isalnum((unsigned char)s[i])))
                ++i;

            push(TokenKind::Ident, start, i);
            continue;
        }

        if (s[i] == '(')
        {
            push(TokenKind::LParen, i, i+1);
            ++i;
            continue;
        }
        if (s[i] == ')')
        {
            push(TokenKind::RParen, i, i+1);
            ++i;
            continue;
        }

        if (s[i] == '-' || std::isdigit((unsigned char)s[i]))
        {
            size_t start = i;
            if (s[i] == '-')
                ++i;
            if (i >= s.size() || !std::isdigit((unsigned char)s[i]))
            {
                throw LexicalError(ln.no, (int)start + 1, "expected digit after '-'");
            }
            while (i < s.size() && std::isdigit((unsigned char)s[i]))
                ++i;
            if (i < s.size() && s[i] == '.')
            {
                ++i;
                if (i >= s.size() || !std::isdigit((unsigned char)s[i]))
                    throw LexicalError(ln.no, (int)i + 1, "expected digit after '.'");
                
                while (i < s.size() && std::isdigit((unsigned char)s[i]))
                    ++i;
            }
            push(TokenKind::Number, start, i);
            continue;
        }

        throw LexicalError(ln.no, (int)i + 1, std::string("unexpected char '") + s[i] + "'");
    }

    out.push_back({TokenKind::End, "", ln.no, (int)s.size() + 1});
    return out;
}
