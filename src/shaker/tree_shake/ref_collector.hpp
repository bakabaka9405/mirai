#pragma once

#include <clang/AST/RecursiveASTVisitor.h>

#include <set>

namespace shaker {

/// @brief 引用收集器。
/// @details 遍历声明子树并记录所有被引用的规范化 `Decl*`
/// （函数、变量、类型等），用于构建声明引用图。
class RefCollector : public clang::RecursiveASTVisitor<RefCollector> {
public:
	/// 收集到的被引用规范声明集合。
	std::set<const clang::Decl*> Found;

	/// @brief 允许访问模板实例化节点。
	bool shouldVisitTemplateInstantiations() const { return true; }
	/// @brief 允许访问隐式生成代码。
	bool shouldVisitImplicitCode() const { return true; }

	/// @name 表达式节点访问
	/// @{

	bool VisitDeclRefExpr(clang::DeclRefExpr* E) {
		add(E->getDecl());
		add(E->getFoundDecl());
		return true;
	}
	bool VisitMemberExpr(clang::MemberExpr* E) { return add(E->getMemberDecl()); }
	bool VisitCXXConstructExpr(clang::CXXConstructExpr* E) { return add(E->getConstructor()); }

	bool VisitUnresolvedLookupExpr(clang::UnresolvedLookupExpr* E) {
		for (auto* D : E->decls()) add(D);
		return true;
	}
	bool VisitUnresolvedMemberExpr(clang::UnresolvedMemberExpr* E) {
		for (auto* D : E->decls()) add(D);
		return true;
	}

	/// @name 类型节点访问
	/// @{

	bool VisitRecordType(clang::RecordType* T) { return add(T->getDecl()); }
	bool VisitEnumType(clang::EnumType* T) { return add(T->getDecl()); }
	bool VisitTypedefType(clang::TypedefType* T) { return add(T->getDecl()); }
	bool VisitUsingType(clang::UsingType* T) { return add(T->getFoundDecl()); }

	bool VisitInjectedClassNameType(clang::InjectedClassNameType* T) {
		return add(T->getDecl());
	}
	bool VisitTemplateSpecializationType(
		clang::TemplateSpecializationType* T) {
		if (auto* TD = T->getTemplateName().getAsTemplateDecl())
			add(TD);
		return true;
	}
	/// @}

	/// @name 声明级节点访问
	/// @{

	bool VisitFriendDecl(clang::FriendDecl* D) {
		if (auto* FD = D->getFriendDecl()) add(FD);
		return true;
	}
	bool VisitUsingDecl(clang::UsingDecl* D) {
		for (auto* S : D->shadows()) add(S->getTargetDecl());
		return true;
	}
	/// @}

private:
	/// @brief 将声明加入引用集合。
	/// @param D 待加入声明。
	/// @return 恒为 `true`，便于与 Visitor 接口配合。
	bool add(const clang::Decl* D) {
		if (D) Found.insert(D->getCanonicalDecl());
		return true;
	}
};

} // namespace shaker
