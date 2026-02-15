#pragma once

#include <clang/AST/RecursiveASTVisitor.h>

#include <set>

namespace shaker {

/// Traverses a Decl sub-tree and records every referenced
/// canonical Decl* (functions, variables, types, …).
class RefCollector : public clang::RecursiveASTVisitor<RefCollector> {
public:
	std::set<const clang::Decl*> Found;

	bool shouldVisitTemplateInstantiations() const { return true; }
	bool shouldVisitImplicitCode() const { return true; }

	// ── expressions ──────────────────────────────────────────────

	bool VisitDeclRefExpr(clang::DeclRefExpr* E) { return add(E->getDecl()); }
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

	// ── types ────────────────────────────────────────────────────

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

	// ── decl-level ───────────────────────────────────────────────

	bool VisitFriendDecl(clang::FriendDecl* D) {
		if (auto* FD = D->getFriendDecl()) add(FD);
		return true;
	}
	bool VisitUsingDecl(clang::UsingDecl* D) {
		for (auto* S : D->shadows()) add(S->getTargetDecl());
		return true;
	}

private:
	bool add(const clang::Decl* D) {
		if (D) Found.insert(D->getCanonicalDecl());
		return true;
	}
};

} // namespace shaker
