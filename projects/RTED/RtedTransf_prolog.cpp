#include <rose.h>
#include <string>
#include "RtedSymbols.h"
#include "DataStructures.h"
#include "RtedTransformation.h"

using namespace std;
using namespace SageInterface;
using namespace SageBuilder;



void 
RtedTransformation::insertMainCloseCall(SgStatement* stmt) {
  if (isSgStatement(stmt)) {
    SgScopeStatement* scope = stmt->get_scope();
    ROSE_ASSERT(scope);
    if (isSgBasicBlock(scope)) {
      SgVarRefExp* varRef_l =
	buildVarRefExp("runtimeSystem", globalScope);
      string symbolName = varRef_l->get_symbol()->get_name().str();
      ROSE_ASSERT(roseRtedClose);
      string symbolName2 = roseRtedClose->get_name().str();
      //cerr << " >>>>>>>> Symbol Member: " << symbolName2 << endl;
      SgMemberFunctionRefExp* memRef_r = buildMemberFunctionRefExp(
								   roseRtedClose, false, true);
      SgArrowExp* sgArrowExp = buildArrowExp(varRef_l, memRef_r);
      SgFunctionCallExp* funcCallExp = buildFunctionCallExp(sgArrowExp,
							    NULL);
      SgExprStatement* exprStmt = buildExprStatement(funcCallExp);
      //cerr << " Last statement in main : " << stmt->class_name() << "  insertBefore : " << insertMainBeforeLast << endl;
      if (insertMainBeforeLast)
	insertStatementBefore(isSgStatement(stmt), exprStmt);
      else
	insertStatementAfter(isSgStatement(stmt), exprStmt);
    }

  } else {
    cerr
      << "RuntimeInstrumentation :: Prolog Surrounding Statement could not be found! "
      << stmt->class_name() << endl;
    ROSE_ASSERT(false);
  }
}



/* -----------------------------------------------------------
 * Insert the header files (Step 1)
 * -----------------------------------------------------------*/
void RtedTransformation::insertProlog(SgProject* proj) {
  cout << "Inserting headers ... " << endl;
  // grep all source (.c) files and insert headers
  Rose_STL_Container<SgNode*> vec =
    NodeQuery::querySubTree(proj,V_SgSourceFile);
  cerr << "Found source files : " << vec.size() << endl;
  Rose_STL_Container<SgNode*>::iterator it = vec.begin();
  for (;it!=vec.end();++it) {
    SgSourceFile* source = isSgSourceFile(*it);
    ROSE_ASSERT(source);
    cerr << "Creating pdf..." << endl;
    AstPDFGeneration pdf;
    pdf.generateWithinFile(source);
    globalScope = source->get_globalScope();
    pushScopeStack (isSgScopeStatement (globalScope));
    // this needs to be fixed
    //buildCpreprocessorDefineDeclaration(globalScope, "#define EXITCODE_OK 0");

#if 0
    // currently doesnt work -- crashes somewhere in wave
    insertHeader("rose.h",PreprocessingInfo::before,false,globalScope);
#else
    insertHeader("RuntimeSystem.h",PreprocessingInfo::before,false,globalScope);
    insertHeader("iostream",PreprocessingInfo::before,true,globalScope);
    insertHeader("map",PreprocessingInfo::before,true,globalScope);
    insertHeader("string",PreprocessingInfo::before,true,globalScope);
#endif

    popScopeStack ();
  }
}

/* -----------------------------------------------------------
 * Insert
 * RuntimeSystem* runtimeSystem = new RuntimeSystem();
 * -----------------------------------------------------------*/
void RtedTransformation::insertRuntimeSystemClass() {
#if 0

  Sg_File_Info* fileInfo = globalScope->get_file_info();
  ROSE_ASSERT(runtimeClassSymbol);
  ROSE_ASSERT(runtimeClassSymbol->get_type());
  SgType* type = runtimeClassSymbol->get_type();
  //SgType* type  = new SgClassType();
  cerr << "Found type : " << type->class_name() << endl;

  SgExprListExp* exprList = buildExprListExp();
  ROSE_ASSERT(roseCreateArray->get_declaration());

  SgConstructorInitializer* constr = buildConstructorInitializer(roseCreateArray->get_declaration(),
								 exprList,type,false,false,false,false);
  SgExprListExp* exprList2 = buildExprListExp();
  //  SgNewExp* newexp = new SgNewExp(fileInfo,type,exprList2,constr,NULL,0,NULL);
  SgNewExp* newexp = buildNewExp(type,NULL,constr,NULL,0,NULL);
  SgAssignInitializer* init = buildAssignInitializer(newexp);

  SgVariableDeclaration* variable =
    buildVariableDeclaration("runtimeSystem",buildPointerType(type),init);
  SgStatement* st = isSgStatement(rememberTopNode->get_parent());
  insertStatement(st,variable,true);
#endif
}



void RtedTransformation::visit_checkIsMain(SgNode* n) {
    SgFunctionDefinition* mainFunc = isSgFunctionDefinition(n);
    ROSE_ASSERT(mainFunc);
    string funcname = mainFunc->get_declaration()->get_name().str();
    if (funcname == "main") {
      // find the last statement
      SgBasicBlock* block = mainFunc->get_body();
      ROSE_ASSERT(block);
      Rose_STL_Container<SgStatement*> stmts = block->get_statements();
      SgStatement* first = stmts.front();
      SgStatement* last = stmts.back();
      if (isSgReturnStmt(last)) 
	insertMainBeforeLast = true;
      else
	insertMainBeforeLast=false;
      //cerr << " Last statement in main : " << last->class_name() << "  insertBefore : " << 
      //	RoseBin_support::resBool(insertMainBeforeLast) << endl;
      ROSE_ASSERT(last);
      // insert call to close before last statement (could be return)
      mainLast = last;
      mainFirst = first;
    }

}