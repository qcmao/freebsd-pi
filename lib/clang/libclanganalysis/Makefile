# $FreeBSD: projects/armv6/lib/clang/libclanganalysis/Makefile 234858 2012-05-01 04:01:22Z gonzo $

LIB=	clanganalysis

SRCDIR=	tools/clang/lib/Analysis
SRCS=	AnalysisDeclContext.cpp \
	CallGraph.cpp \
	CFG.cpp \
	CFGReachabilityAnalysis.cpp \
	CFGStmtMap.cpp \
	CocoaConventions.cpp \
	Dominators.cpp \
	FormatString.cpp \
	LiveVariables.cpp \
	PostOrderCFGView.cpp \
	PrintfFormatString.cpp \
	ProgramPoint.cpp \
	PseudoConstantAnalysis.cpp \
	ReachableCode.cpp \
	ScanfFormatString.cpp \
	ThreadSafety.cpp \
	UninitializedValues.cpp

TGHDRS=	AttrList \
	Attrs \
	DeclNodes \
	DiagnosticAnalysisKinds \
	DiagnosticCommonKinds \
	StmtNodes

.include "../clang.lib.mk"
