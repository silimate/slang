//------------------------------------------------------------------------------
// ParameterSymbols.cpp
// Contains parameter-related symbol definitions
//
// File is under the MIT license; see LICENSE for details
//------------------------------------------------------------------------------
#include "slang/symbols/ParameterSymbols.h"

#include "slang/binding/Expression.h"
#include "slang/compilation/Compilation.h"
#include "slang/diagnostics/DeclarationsDiags.h"
#include "slang/symbols/ASTSerializer.h"
#include "slang/symbols/AllTypes.h"
#include "slang/syntax/AllSyntax.h"

namespace slang {

bool ParameterSymbolBase::hasDefault() const {
    if (symbol.kind == SymbolKind::Parameter)
        return symbol.as<ParameterSymbol>().getDeclaredType()->getInitializerSyntax();
    else
        return symbol.as<TypeParameterSymbol>().targetType.getTypeSyntax();
}

ParameterSymbol::ParameterSymbol(string_view name, SourceLocation loc, bool isLocal, bool isPort) :
    ValueSymbol(SymbolKind::Parameter, name, loc,
                DeclaredTypeFlags::InferImplicit | DeclaredTypeFlags::RequireConstant),
    ParameterSymbolBase(*this, isLocal, isPort) {
}

void ParameterSymbol::fromSyntax(const Scope& scope, const ParameterDeclarationSyntax& syntax,
                                 bool isLocal, bool isPort,
                                 SmallVector<ParameterSymbol*>& results) {
    for (auto decl : syntax.declarators) {
        auto loc = decl->name.location();
        auto param = scope.getCompilation().emplace<ParameterSymbol>(decl->name.valueText(), loc,
                                                                     isLocal, isPort);
        param->setDeclaredType(*syntax.type);
        param->setFromDeclarator(*decl);

        if (!decl->initializer) {
            if (!isPort)
                scope.addDiag(diag::BodyParamNoInitializer, loc);
            else if (isLocal)
                scope.addDiag(diag::LocalParamNoInitializer, loc);
        }

        results.append(param);
    }
}

ParameterSymbol& ParameterSymbol::clone(Compilation& compilation) const {
    auto result =
        compilation.emplace<ParameterSymbol>(name, location, isLocalParam(), isPortParam());

    if (auto syntax = getSyntax())
        result->setSyntax(*syntax);

    auto declared = getDeclaredType();
    result->getDeclaredType()->copyTypeFrom(*declared);

    if (auto init = declared->getInitializerSyntax())
        result->setInitializerSyntax(*init, declared->getInitializerLocation());

    if (declared->hasInitializer())
        result->setInitializer(*declared->getInitializer());

    result->value = value;
    return *result;
}

ParameterSymbol& ParameterSymbol::instantiate(Scope& newScope, const BindContext& context,
                                              const ExpressionSyntax* newInitializer) const {
    // Construct the result using the source's properties.
    auto result = context.getCompilation().emplace<ParameterSymbol>(name, location, isLocalParam(),
                                                                    isPortParam());

    newScope.addMember(*result);
    if (auto syntax = getSyntax())
        result->setSyntax(*syntax);

    // Copy the type syntax from the source parameter to the result.
    auto sourceDT = getDeclaredType();
    auto resultDT = result->getDeclaredType();
    if (auto ts = sourceDT->getTypeSyntax()) {
        resultDT->setTypeSyntax(*ts);
        if (auto dims = sourceDT->getDimensionSyntax())
            resultDT->setDimensionSyntax(*dims);
    }

    // If we have a new initializer set that now. Otherwise use the source's as a default.
    if (newInitializer) {
        result->setInitializerSyntax(*newInitializer, newInitializer->getFirstToken().location());
        resultDT->resolveAt(context);
        result->getValue();
    }
    else if (auto init = sourceDT->getInitializerSyntax()) {
        result->setInitializerSyntax(*init, sourceDT->getInitializerLocation());
    }

    return *result;
}

const ConstantValue& ParameterSymbol::getValue() const {
    if (!value) {
        // If no value has been explicitly set, try to set it
        // from our initializer.
        auto init = getInitializer();
        if (init) {
            auto scope = getParentScope();
            ASSERT(scope);

            BindContext ctx(*scope, LookupLocation::max);
            value = scope->getCompilation().allocConstant(ctx.eval(*init));
        }
        else {
            value = &ConstantValue::Invalid;
        }
    }
    return *value;
}

void ParameterSymbol::setValue(ConstantValue newValue) {
    auto scope = getParentScope();
    ASSERT(scope);
    value = scope->getCompilation().allocConstant(std::move(newValue));
}

void ParameterSymbol::serializeTo(ASTSerializer& serializer) const {
    serializer.write("value", getValue());
    serializer.write("isLocal", isLocalParam());
    serializer.write("isPort", isPortParam());
    serializer.write("isBody", isBodyParam());
}

TypeParameterSymbol::TypeParameterSymbol(string_view name, SourceLocation loc, bool isLocal,
                                         bool isPort) :
    Symbol(SymbolKind::TypeParameter, name, loc),
    ParameterSymbolBase(*this, isLocal, isPort), targetType(*this) {
}

void TypeParameterSymbol::fromSyntax(const Scope& scope,
                                     const TypeParameterDeclarationSyntax& syntax, bool isLocal,
                                     bool isPort, SmallVector<TypeParameterSymbol*>& results) {
    auto& comp = scope.getCompilation();
    for (auto decl : syntax.declarators) {
        auto name = decl->name.valueText();
        auto loc = decl->name.location();

        auto param = comp.emplace<TypeParameterSymbol>(name, loc, isLocal, isPort);
        param->setSyntax(*decl);

        if (!decl->assignment) {
            if (!isPort)
                scope.addDiag(diag::BodyParamNoInitializer, loc);
            else if (isLocal)
                scope.addDiag(diag::LocalParamNoInitializer, loc);
        }
        else {
            param->targetType.setTypeSyntax(*decl->assignment->type);
        }

        results.append(param);
    }
}

TypeParameterSymbol& TypeParameterSymbol::clone(Compilation& comp) const {
    auto result = comp.emplace<TypeParameterSymbol>(name, location, isLocalParam(), isPortParam());
    if (auto syntax = getSyntax())
        result->setSyntax(*syntax);

    auto declared = getDeclaredType();
    result->targetType.copyTypeFrom(*declared);

    return *result;
}

TypeParameterSymbol& TypeParameterSymbol::instantiate(Scope& newScope, const BindContext& context,
                                                      const ExpressionSyntax* expr) const {
    // Construct the result using the source's properties.
    auto& comp = context.getCompilation();
    auto result = comp.emplace<TypeParameterSymbol>(name, location, isLocalParam(), isPortParam());

    newScope.addMember(*result);
    if (auto syntax = getSyntax())
        result->setSyntax(*syntax);

    auto& tt = result->targetType;
    if (expr) {
        // If this is a NameSyntax, the parser didn't know we were assigning to
        // a type parameter, so fix it up into a NamedTypeSyntax to get a type from it.
        if (NameSyntax::isKind(expr->kind)) {
            // const_cast is ugly but safe here, we're only going to refer to it
            // by const reference everywhere down.
            auto& nameSyntax = const_cast<NameSyntax&>(expr->as<NameSyntax>());
            auto namedType = comp.emplace<NamedTypeSyntax>(nameSyntax);

            tt.setTypeSyntax(*namedType);
            tt.setType(comp.getType(*namedType, context.lookupLocation, context.scope));
        }
        else if (!DataTypeSyntax::isKind(expr->kind)) {
            context.addDiag(diag::BadTypeParamExpr, expr->getFirstToken().location()) << name;
        }
        else {
            tt.setTypeSyntax(expr->as<DataTypeSyntax>());
            tt.setType(
                comp.getType(expr->as<DataTypeSyntax>(), context.lookupLocation, context.scope));
        }
    }
    else if (auto ts = targetType.getTypeSyntax()) {
        tt.setTypeSyntax(*ts);
        if (auto dims = targetType.getDimensionSyntax())
            tt.setDimensionSyntax(*dims);
    }

    return *result;
}

const Type& TypeParameterSymbol::getTypeAlias() const {
    if (typeAlias)
        return *typeAlias;

    auto scope = getParentScope();
    ASSERT(scope);

    // If the parent scope is a DefinitionSymbol, as opposed to an instance,
    // we need to always return an Error type here so that elaborating the
    // definition doesn't cause spurious errors when looking at default values
    // for type parameters that are invalid but aren't ever actually instantiated.
    if (scope->asSymbol().kind == SymbolKind::Definition)
        return scope->getCompilation().getErrorType();

    auto alias = scope->getCompilation().emplace<TypeAliasType>(name, location);
    if (auto syntax = getSyntax())
        alias->setSyntax(*syntax);

    alias->targetType.copyTypeFrom(targetType);
    alias->setParent(*scope, getIndex());

    typeAlias = alias;
    return *typeAlias;
}

void TypeParameterSymbol::serializeTo(ASTSerializer& serializer) const {
    serializer.write("type", targetType.getType());
    serializer.write("isLocal", isLocalParam());
    serializer.write("isPort", isPortParam());
    serializer.write("isBody", isBodyParam());
}

} // namespace slang