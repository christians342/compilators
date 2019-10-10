#include "astree.h"
#include "hash.h"

void checkAndSetTypes(AST *node);
void checkOperands(AST *node);
void checkUndeclared();
int getSemanticErrors();