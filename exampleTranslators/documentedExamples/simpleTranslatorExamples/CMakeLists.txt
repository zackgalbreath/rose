
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


########### next target ###############

set(identityTranslator_SRCS identityTranslator.C)

kde4_add_executable(identityTranslator ${identityTranslator_SRCS})

target_link_libraries(identityTranslator ${KDE4_KDECORE_LIBS})

install(TARGETS identityTranslator ${INSTALL_TARGETS_DEFAULT_ARGS})


########### install files ###############




#original Makefile.am contents follow:

#include $(top_srcdir)/config/Makefile.for.ROSE.includes.and.libs
#
#INCLUDES = $(ROSE_INCLUDES)
#
#all: exampleMakefile
#
#bin_PROGRAMS = identityTranslator
#
#identityTranslator_SOURCES = identityTranslator.C
#
#LDADD = $(LIBS_WITH_RPATH) $(ROSE_LIBS)
#
##EXTRA_DIST = testCode.C AST-dot1.ps AST-pdf2.pdf
#EXTRA_DIST = testCode.C AST-pdf2.pdf
#CLEANFILES = 
#
#clean-local:
#	rm -rf Templates.DB ii_files ti_files exampleTranslator exampleTranslatorMultiLib rose_* a.out
#
## Compile everything with -g (compiler's debug flag)
## AM_CPPFLAGS = -g
#
#installcheck:
#	rm -f identityTranslator identityTranslator.o
#	@echo "Testing build using installed libraries ..."
#if USE_ROSE_IN_BUILD_TREE_VAR
#	unset ROSE_IN_BUILD_TREE; $(MAKE) -f exampleMakefile # This must be done in a single shell
#else
#	$(MAKE) -f exampleMakefile # This must be done in a single shell
#endif
#	@echo "Testing executable built using installed libraries ..."
#if USE_ROSE_IN_BUILD_TREE_VAR
#	unset ROSE_IN_BUILD_TREE; $(MAKE) -f exampleMakefile check
#else
#	$(MAKE) -f exampleMakefile check
#endif
