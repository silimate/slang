#pragma once

#include "SyntaxNode.h"
#include "Token.h"

// This file contains all parse tree syntax nodes.
// It is auto-generated by the syntax_gen.py script under the tools/ directory.

namespace slang {

struct ExpressionSyntax : public SyntaxNode {

    ExpressionSyntax(SyntaxKind kind) :
        SyntaxNode(kind)
    {
    }
};

// ----- INSTANTIATIONS -----

struct ParameterAssignmentSyntax : public SyntaxNode {

    ParameterAssignmentSyntax(SyntaxKind kind) :
        SyntaxNode(kind)
    {
    }
};

struct OrderedParameterAssignmentSyntax : public ParameterAssignmentSyntax {
    ExpressionSyntax* expr;

    OrderedParameterAssignmentSyntax(ExpressionSyntax* expr) :
        ParameterAssignmentSyntax(SyntaxKind::OrderedParameterAssignment), expr(expr)
    {
        childCount += 1;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return expr;
            default: return nullptr;
        }
    }
};

struct NamedParameterAssignmentSyntax : public ParameterAssignmentSyntax {
    Token* dot;
    Token* name;
    Token* openParen;
    ExpressionSyntax* expr;
    Token* closeParen;

    NamedParameterAssignmentSyntax(Token* dot, Token* name, Token* openParen, ExpressionSyntax* expr, Token* closeParen) :
        ParameterAssignmentSyntax(SyntaxKind::NamedParameterAssignment), dot(dot), name(name), openParen(openParen), expr(expr), closeParen(closeParen)
    {
        childCount += 5;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return dot;
            case 1: return name;
            case 2: return openParen;
            case 3: return expr;
            case 4: return closeParen;
            default: return nullptr;
        }
    }
};

struct ParameterValueAssignmentSyntax : public SyntaxNode {
    Token* hash;
    Token* openParen;
    SeparatedSyntaxList<ParameterAssignmentSyntax> parameters;
    Token* closeParen;

    ParameterValueAssignmentSyntax(Token* hash, Token* openParen, SeparatedSyntaxList<ParameterAssignmentSyntax> parameters, Token* closeParen) :
        SyntaxNode(SyntaxKind::ParameterValueAssignment), hash(hash), openParen(openParen), parameters(parameters), closeParen(closeParen)
    {
        childCount += 4;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return hash;
            case 1: return openParen;
            case 2: return &parameters;
            case 3: return closeParen;
            default: return nullptr;
        }
    }
};

// ----- EXPRESSIONS -----

struct PrefixUnaryExpressionSyntax : public ExpressionSyntax {
    Token* operatorToken;
    ExpressionSyntax* operand;

    PrefixUnaryExpressionSyntax(SyntaxKind kind, Token* operatorToken, ExpressionSyntax* operand) :
        ExpressionSyntax(kind), operatorToken(operatorToken), operand(operand)
    {
        childCount += 2;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return operatorToken;
            case 1: return operand;
            default: return nullptr;
        }
    }
};

struct BinaryExpressionSyntax : public ExpressionSyntax {
    ExpressionSyntax* left;
    Token* operatorToken;
    ExpressionSyntax* right;

    BinaryExpressionSyntax(SyntaxKind kind, ExpressionSyntax* left, Token* operatorToken, ExpressionSyntax* right) :
        ExpressionSyntax(kind), left(left), operatorToken(operatorToken), right(right)
    {
        childCount += 3;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return left;
            case 1: return operatorToken;
            case 2: return right;
            default: return nullptr;
        }
    }
};

struct PrimaryExpressionSyntax : public ExpressionSyntax {

    PrimaryExpressionSyntax(SyntaxKind kind) :
        ExpressionSyntax(kind)
    {
    }
};

struct LiteralExpressionSyntax : public PrimaryExpressionSyntax {
    Token* literal;

    LiteralExpressionSyntax(SyntaxKind kind, Token* literal) :
        PrimaryExpressionSyntax(kind), literal(literal)
    {
        childCount += 1;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return literal;
            default: return nullptr;
        }
    }
};

struct EmptyQueueExpressionSyntax : public PrimaryExpressionSyntax {
    Token* openBrace;
    Token* closeBrace;

    EmptyQueueExpressionSyntax(Token* openBrace, Token* closeBrace) :
        PrimaryExpressionSyntax(SyntaxKind::EmptyQueueExpression), openBrace(openBrace), closeBrace(closeBrace)
    {
        childCount += 2;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return openBrace;
            case 1: return closeBrace;
            default: return nullptr;
        }
    }
};

struct ConcatenationExpressionSyntax : public PrimaryExpressionSyntax {
    Token* openBrace;
    SeparatedSyntaxList<ExpressionSyntax> expressions;
    Token* closeBrace;

    ConcatenationExpressionSyntax(Token* openBrace, SeparatedSyntaxList<ExpressionSyntax> expressions, Token* closeBrace) :
        PrimaryExpressionSyntax(SyntaxKind::ConcatenationExpression), openBrace(openBrace), expressions(expressions), closeBrace(closeBrace)
    {
        childCount += 3;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return openBrace;
            case 1: return &expressions;
            case 2: return closeBrace;
            default: return nullptr;
        }
    }
};

struct MultipleConcatenationExpressionSyntax : public PrimaryExpressionSyntax {
    Token* openBrace;
    ExpressionSyntax* expression;
    ConcatenationExpressionSyntax* concatenation;
    Token* closeBrace;

    MultipleConcatenationExpressionSyntax(Token* openBrace, ExpressionSyntax* expression, ConcatenationExpressionSyntax* concatenation, Token* closeBrace) :
        PrimaryExpressionSyntax(SyntaxKind::MultipleConcatenationExpression), openBrace(openBrace), expression(expression), concatenation(concatenation), closeBrace(closeBrace)
    {
        childCount += 4;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return openBrace;
            case 1: return expression;
            case 2: return concatenation;
            case 3: return closeBrace;
            default: return nullptr;
        }
    }
};

struct SelectorSyntax : public SyntaxNode {

    SelectorSyntax(SyntaxKind kind) :
        SyntaxNode(kind)
    {
    }
};

struct BitSelectSyntax : public SelectorSyntax {
    ExpressionSyntax* expr;

    BitSelectSyntax(ExpressionSyntax* expr) :
        SelectorSyntax(SyntaxKind::BitSelect), expr(expr)
    {
        childCount += 1;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return expr;
            default: return nullptr;
        }
    }
};

struct RangeSelectSyntax : public SelectorSyntax {
    ExpressionSyntax* left;
    Token* range;
    ExpressionSyntax* right;

    RangeSelectSyntax(SyntaxKind kind, ExpressionSyntax* left, Token* range, ExpressionSyntax* right) :
        SelectorSyntax(kind), left(left), range(range), right(right)
    {
        childCount += 3;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return left;
            case 1: return range;
            case 2: return right;
            default: return nullptr;
        }
    }
};

struct ElementSelectExpressionSyntax : public ExpressionSyntax {
    Token* openBracket;
    SelectorSyntax* selector;
    Token* closeBracket;

    ElementSelectExpressionSyntax(Token* openBracket, SelectorSyntax* selector, Token* closeBracket) :
        ExpressionSyntax(SyntaxKind::ElementSelectExpression), openBracket(openBracket), selector(selector), closeBracket(closeBracket)
    {
        childCount += 3;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return openBracket;
            case 1: return selector;
            case 2: return closeBracket;
            default: return nullptr;
        }
    }
};

struct StreamExpressionWithRange : public SyntaxNode {
    Token* withKeyword;
    ElementSelectExpressionSyntax* range;

    StreamExpressionWithRange(Token* withKeyword, ElementSelectExpressionSyntax* range) :
        SyntaxNode(SyntaxKind::StreamExpressionWithRange), withKeyword(withKeyword), range(range)
    {
        childCount += 2;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return withKeyword;
            case 1: return range;
            default: return nullptr;
        }
    }
};

struct StreamExpressionSyntax : public SyntaxNode {
    ExpressionSyntax* expression;
    StreamExpressionWithRange* withRange;

    StreamExpressionSyntax(ExpressionSyntax* expression, StreamExpressionWithRange* withRange) :
        SyntaxNode(SyntaxKind::StreamExpression), expression(expression), withRange(withRange)
    {
        childCount += 2;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return expression;
            case 1: return withRange;
            default: return nullptr;
        }
    }
};

struct StreamingConcatenationExpressionSyntax : public PrimaryExpressionSyntax {
    Token* openBrace;
    Token* operatorToken;
    ExpressionSyntax* sliceSize;
    Token* innerOpenBrace;
    SeparatedSyntaxList<StreamExpressionSyntax> expressions;
    Token* innerCloseBrace;
    Token* closeBrace;

    StreamingConcatenationExpressionSyntax(Token* openBrace, Token* operatorToken, ExpressionSyntax* sliceSize, Token* innerOpenBrace, SeparatedSyntaxList<StreamExpressionSyntax> expressions, Token* innerCloseBrace, Token* closeBrace) :
        PrimaryExpressionSyntax(SyntaxKind::StreamingConcatenationExpression), openBrace(openBrace), operatorToken(operatorToken), sliceSize(sliceSize), innerOpenBrace(innerOpenBrace), expressions(expressions), innerCloseBrace(innerCloseBrace), closeBrace(closeBrace)
    {
        childCount += 7;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return openBrace;
            case 1: return operatorToken;
            case 2: return sliceSize;
            case 3: return innerOpenBrace;
            case 4: return &expressions;
            case 5: return innerCloseBrace;
            case 6: return closeBrace;
            default: return nullptr;
        }
    }
};

struct ParenthesizedExpressionSyntax : public PrimaryExpressionSyntax {
    Token* openParen;
    ExpressionSyntax* expression;
    Token* closeParen;

    ParenthesizedExpressionSyntax(Token* openParen, ExpressionSyntax* expression, Token* closeParen) :
        PrimaryExpressionSyntax(SyntaxKind::ParenthesizedExpression), openParen(openParen), expression(expression), closeParen(closeParen)
    {
        childCount += 3;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return openParen;
            case 1: return expression;
            case 2: return closeParen;
            default: return nullptr;
        }
    }
};

struct MinTypMaxExpressionSyntax : public ExpressionSyntax {
    ExpressionSyntax* min;
    Token* colon1;
    ExpressionSyntax* typ;
    Token* colon2;
    ExpressionSyntax* max;

    MinTypMaxExpressionSyntax(ExpressionSyntax* min, Token* colon1, ExpressionSyntax* typ, Token* colon2, ExpressionSyntax* max) :
        ExpressionSyntax(SyntaxKind::MinTypMaxExpression), min(min), colon1(colon1), typ(typ), colon2(colon2), max(max)
    {
        childCount += 5;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return min;
            case 1: return colon1;
            case 2: return typ;
            case 3: return colon2;
            case 4: return max;
            default: return nullptr;
        }
    }
};

struct NameSyntax : public ExpressionSyntax {

    NameSyntax(SyntaxKind kind) :
        ExpressionSyntax(kind)
    {
    }
};

struct IdentifierNameSyntax : public NameSyntax {
    Token* identifier;

    IdentifierNameSyntax(Token* identifier) :
        NameSyntax(SyntaxKind::IdentifierName), identifier(identifier)
    {
        childCount += 1;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return identifier;
            default: return nullptr;
        }
    }
};

struct KeywordNameSyntax : public NameSyntax {
    Token* keyword;

    KeywordNameSyntax(SyntaxKind kind, Token* keyword) :
        NameSyntax(kind), keyword(keyword)
    {
        childCount += 1;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return keyword;
            default: return nullptr;
        }
    }
};

struct ClassNameSyntax : public NameSyntax {
    Token* identifier;
    ParameterValueAssignmentSyntax* parameters;

    ClassNameSyntax(Token* identifier, ParameterValueAssignmentSyntax* parameters) :
        NameSyntax(SyntaxKind::ClassName), identifier(identifier), parameters(parameters)
    {
        childCount += 2;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return identifier;
            case 1: return parameters;
            default: return nullptr;
        }
    }
};

struct HierarchicalNameSyntax : public NameSyntax {
    NameSyntax* left;
    Token* separator;
    NameSyntax* right;

    HierarchicalNameSyntax(NameSyntax* left, Token* separator, NameSyntax* right) :
        NameSyntax(SyntaxKind::HierarchicalName), left(left), separator(separator), right(right)
    {
        childCount += 3;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return left;
            case 1: return separator;
            case 2: return right;
            default: return nullptr;
        }
    }
};

// ----- DIRECTIVES -----

struct DirectiveSyntax : public SyntaxNode {
    Token* directive;
    Token* endOfDirective;

    DirectiveSyntax(SyntaxKind kind, Token* directive, Token* endOfDirective) :
        SyntaxNode(kind), directive(directive), endOfDirective(endOfDirective)
    {
        childCount += 2;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override {
        switch(index) {
            case 0: return directive;
            case 1: return endOfDirective;
            default: return nullptr;
        }
    }
};

struct IncludeDirectiveSyntax : public DirectiveSyntax {
    Token* fileName;

    IncludeDirectiveSyntax(Token* directive, Token* endOfDirective, Token* fileName) :
        DirectiveSyntax(SyntaxKind::IncludeDirective, directive, endOfDirective), fileName(fileName)
    {
        childCount += 1;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return directive;
            case 1: return endOfDirective;
            case 2: return fileName;
            default: return nullptr;
        }
    }
};

struct MacroArgumentDefaultSyntax : public SyntaxNode {
    Token* equals;
    TokenList tokens;

    MacroArgumentDefaultSyntax(Token* equals, TokenList tokens) :
        SyntaxNode(SyntaxKind::MacroArgumentDefault), equals(equals), tokens(tokens)
    {
        childCount += 2;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return equals;
            case 1: return &tokens;
            default: return nullptr;
        }
    }
};

struct MacroFormalArgumentSyntax : public SyntaxNode {
    Token* name;
    MacroArgumentDefaultSyntax* defaultValue;

    MacroFormalArgumentSyntax(Token* name, MacroArgumentDefaultSyntax* defaultValue) :
        SyntaxNode(SyntaxKind::MacroFormalArgument), name(name), defaultValue(defaultValue)
    {
        childCount += 2;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return name;
            case 1: return defaultValue;
            default: return nullptr;
        }
    }
};

struct MacroFormalArgumentListSyntax : public SyntaxNode {
    Token* openParen;
    SeparatedSyntaxList<MacroFormalArgumentSyntax> args;
    Token* closeParen;

    MacroFormalArgumentListSyntax(Token* openParen, SeparatedSyntaxList<MacroFormalArgumentSyntax> args, Token* closeParen) :
        SyntaxNode(SyntaxKind::MacroFormalArgumentList), openParen(openParen), args(args), closeParen(closeParen)
    {
        childCount += 3;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return openParen;
            case 1: return &args;
            case 2: return closeParen;
            default: return nullptr;
        }
    }
};

struct DefineDirectiveSyntax : public DirectiveSyntax {
    Token* name;
    MacroFormalArgumentListSyntax* formalArguments;
    TokenList body;

    DefineDirectiveSyntax(Token* directive, Token* endOfDirective, Token* name, MacroFormalArgumentListSyntax* formalArguments, TokenList body) :
        DirectiveSyntax(SyntaxKind::DefineDirective, directive, endOfDirective), name(name), formalArguments(formalArguments), body(body)
    {
        childCount += 3;
    }

protected:
    TokenOrSyntax getChild(uint32_t index) const override final {
        switch(index) {
            case 0: return directive;
            case 1: return endOfDirective;
            case 2: return name;
            case 3: return formalArguments;
            case 4: return &body;
            default: return nullptr;
        }
    }
};

}