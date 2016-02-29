#ifndef __PY_LIST_EX_H__
#define __PY_LIST_EX_H__

#include <Python.h>

typedef struct {
	PyObject_HEAD
	PyObject* raw_obj;
	void* ptr[2];
} ListExNode;

PyAPI_DATA(PyTypeObject) ListExNode_Type;

PyAPI_FUNC(PyObject *) ListExNode_New(PyObject* obj);

typedef struct {
	PyObject_VAR_HEAD
	void* head[2];	
} ListEx;

PyAPI_DATA(PyTypeObject) ListEx_Type;

#endif   /* __PY_LIST_EX_H__ */