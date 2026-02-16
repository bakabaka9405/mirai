#pragma once

#include <clang/AST/AST.h>

namespace shaker {

/// @brief 声明类别枚举。
enum class DeclCat { Func,
					 Var,
					 MemberFunc,
					 MemberVar,
					 Record,
					 Alias,
					 Enum,
					 Using,
					 Other };

/// @brief 将 clang 声明映射到声明类别。
/// @param D 待分类声明。
/// @return 对应的 `DeclCat`。
static DeclCat classify(const clang::Decl* D) {
	using namespace clang;
	if (auto* FD = dyn_cast<FunctionDecl>(D))
		if (isa<RecordDecl>(FD->getDeclContext()))
			return DeclCat::MemberFunc;
	if (auto* FT = dyn_cast<FunctionTemplateDecl>(D))
		if (isa<CXXMethodDecl>(FT->getTemplatedDecl()))
			return DeclCat::MemberFunc;
	if (isa<FieldDecl>(D))
		return DeclCat::MemberVar;
	if (auto* VD = dyn_cast<VarDecl>(D))
		if (isa<RecordDecl>(VD->getDeclContext()))
			return DeclCat::MemberVar;
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

/// @brief 获取声明类别的可读名称。
/// @param C 声明类别。
/// @return 对应的名称字符串。
static const char* catName(DeclCat C) {
	switch (C) {
	case DeclCat::Func: return "function";
	case DeclCat::Var: return "variable";
	case DeclCat::MemberFunc: return "member-function";
	case DeclCat::MemberVar: return "member-variable";
	case DeclCat::Record: return "record";
	case DeclCat::Alias: return "type-alias";
	case DeclCat::Enum: return "enum";
	case DeclCat::Using: return "using-decl";
	case DeclCat::Other: return "other";
	}
	return "?";
}

/// @brief 判断某类别是否允许在指定强度下被移除。
/// @param C 声明类别。
/// @param Strength 当前裁剪强度。
/// @return `true` 表示该类别可在当前强度下参与裁剪。
static bool canShake(DeclCat C, unsigned Strength) {
	switch (C) {
	case DeclCat::Func: return true;
	case DeclCat::Var: return Strength >= 1;
	case DeclCat::MemberFunc:
	case DeclCat::MemberVar: return Strength >= 2;
	case DeclCat::Record: return false;
	case DeclCat::Alias:
	case DeclCat::Enum:
	case DeclCat::Using: return Strength >= 3;
	case DeclCat::Other: return false;
	}
	return false;
}

} // namespace shaker
