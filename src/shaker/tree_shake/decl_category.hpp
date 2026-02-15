#pragma once

#include <clang/AST/AST.h>

namespace shaker {

enum class DeclCat { Func,
					 Var,
					 Record,
					 Alias,
					 Enum,
					 Using,
					 Other };

static DeclCat classify(const clang::Decl* D) {
	using namespace clang;
	if (isa<FunctionDecl>(D) || isa<FunctionTemplateDecl>(D))
		return DeclCat::Func;
	if (isa<VarDecl>(D))
		return DeclCat::Var;
	if (isa<RecordDecl>(D) || isa<ClassTemplateDecl>(D))
		return DeclCat::Record;
	if (isa<TypedefDecl>(D) || isa<TypeAliasDecl>(D)
		|| isa<TypeAliasTemplateDecl>(D))
		return DeclCat::Alias;
	if (isa<EnumDecl>(D))
		return DeclCat::Enum;
	if (isa<UsingDecl>(D))
		return DeclCat::Using;
	return DeclCat::Other;
}

static const char* catName(DeclCat C) {
	switch (C) {
	case DeclCat::Func: return "function";
	case DeclCat::Var: return "variable";
	case DeclCat::Record: return "record";
	case DeclCat::Alias: return "type-alias";
	case DeclCat::Enum: return "enum";
	case DeclCat::Using: return "using-decl";
	case DeclCat::Other: return "other";
	}
	return "?";
}

/// Can this category be removed at @p Strength ?
static bool canShake(DeclCat C, unsigned Strength) {
	switch (C) {
	case DeclCat::Func: return true; // strength 0+
	case DeclCat::Var: return Strength >= 1;
	case DeclCat::Record: return Strength >= 2;
	case DeclCat::Alias:
	case DeclCat::Enum:
	case DeclCat::Using: return Strength >= 3;
	case DeclCat::Other: return false;
	}
	return false;
}

} // namespace shaker
