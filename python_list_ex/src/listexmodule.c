#include <Python.h>
#include "common.c"
#include "listex.c"

void initlistex(void) {
	PyObject * listex_m = Py_InitModule3("listex", NULL, "double link queue module");

	PyListExModule_AddType(listex_m, "listex_node", &ListExNode_Type);
	PyListExModule_AddType(listex_m, "listex", &ListEx_Type);
}
