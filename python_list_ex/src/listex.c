
#include "queue.h"
#include "listex.h"

static int
listex_node_init(ListExNode *self, PyObject *args, PyObject *kw)
{
    PyObject *arg = NULL;
    static char *kwlist[] = {"raw_obj", 0};

    if (!PyArg_ParseTupleAndKeywords(args, kw, "|O:listex_node", kwlist, &arg))
        return -1;

    if (arg != NULL) {
        Py_INCREF(arg);
        self->raw_obj = arg;
    }

    QUEUE_INIT(&self->ptr);

    return 0;
}


static void
listex_node_dealloc(ListExNode *self)
{
	if(self->raw_obj != NULL) {
		Py_DECREF(self->raw_obj);
		self->raw_obj = NULL;
	}
}

static PyObject*
listex_node_get_raw_obj(ListExNode* self) 
{
	if(self->raw_obj) {
		Py_INCREF(self->raw_obj);
		return self->raw_obj;
	}
	Py_RETURN_NONE;
}

static PyObject*
listex_node_next(ListExNode* self) {
	if(QUEUE_EMPTY(&self->ptr))
		Py_RETURN_NONE;
	QUEUE* q = QUEUE_NEXT(&self->ptr);
	ListExNode* next = QUEUE_DATA(q, ListExNode, ptr);
	if(next == NULL) {
		return NULL;
	}
	Py_INCREF(next);
	return (PyObject*)next;
}

static PyObject*
listex_node_prev(ListExNode* self) {
	if(QUEUE_EMPTY(&self->ptr))
		Py_RETURN_NONE;
	QUEUE* q = QUEUE_PREV(&self->ptr);
	ListExNode* prev = QUEUE_DATA(q, ListExNode, ptr);
	if(prev == NULL) {
		return NULL;
	}
	Py_INCREF(prev);
	return (PyObject*)prev;
}

static PyObject*
listex_node_remove(ListExNode* self) {
    if(QUEUE_EMPTY(&self->ptr))
        Py_RETURN_NONE;
    QUEUE_REMOVE(&self->ptr);
    Py_DECREF(self);
    Py_RETURN_NONE;
}

static PyMethodDef listex_node_methods[] = {
	{"get_raw_obj", (PyCFunction)listex_node_get_raw_obj, METH_NOARGS, "LN.get_raw_obj() -- get raw PyObject."},
    {"next", (PyCFunction)listex_node_next, METH_NOARGS, "LN.next() -- get next list node."},
    {"prev", (PyCFunction)listex_node_prev, METH_NOARGS, "LN.prev() -- get prev list node."},
    {"remove", (PyCFunction)listex_node_remove, METH_NOARGS, "LN.remove() -- remove self from queue."},
    {NULL,              NULL}           /* sentinel */
};

static int 
listex_node_tp_traverse(ListEx* self, visitproc visit, void* arg) {
    return 0;
}

PyTypeObject ListExNode_Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "listex_node",
    sizeof(ListExNode),
    0,
    (destructor)listex_node_dealloc,                   /* tp_dealloc */
    0,                      /* tp_print */
    0,                                          /* tp_getattr */
    0,                                          /* tp_setattr */
    0,                                          /* tp_compare */
    0,                        /* tp_repr */
    0,                                          /* tp_as_number */
    0,                          /* tp_as_sequence */
    0,                           /* tp_as_mapping */
    0,      /* tp_hash */
    0,                                          /* tp_call */
    0,                                          /* tp_str */
    PyObject_GenericGetAttr,                    /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE ,         /* tp_flags */
    0,                                   /* tp_doc */
    (traverseproc)listex_node_tp_traverse,                /* tp_traverse */
    0,                        /* tp_clear */
    0,                           /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                          /* tp_iternext */
    listex_node_methods,                               /* tp_methods */
    0,                                          /* tp_members */
    0,                                          /* tp_getset */
    0,                                          /* tp_base */
    0,                                          /* tp_dict */
    0,                                          /* tp_descr_get */
    0,                                          /* tp_descr_set */
    0,                                          /* tp_dictoffset */
    (initproc)listex_node_init,                        /* tp_init */
    PyType_GenericAlloc,                        /* tp_alloc */
    PyType_GenericNew,                          /* tp_new */
};

PyObject*
ListExNode_New(PyObject* obj)
{
	ListExNode* node = PyObject_GC_New(ListExNode, &ListExNode_Type);
	Py_INCREF(obj);
	node->raw_obj = obj;
	QUEUE_INIT(&node->ptr);
	return (PyObject*)node;
}

static int
listex_init(ListEx *self, PyObject *args, PyObject *kw)
{
    QUEUE_INIT(&self->head);
    return 0;
}


static void
listex_dealloc(ListEx *self)
{
}

static PyObject*
listex_append(ListEx* self, PyObject* v)
{
	ListExNode* node = ListExNode_New(v);
	if(node == NULL) {
		return NULL;
	}
	Py_INCREF(node);
	QUEUE_INSERT_TAIL(&self->head, &node->ptr);
	return (PyObject*)node;
}

static PyObject*
listex_get_head(ListEx* self) {
    if(QUEUE_EMPTY(&self->head))
        Py_RETURN_NONE;
    QUEUE* q = QUEUE_HEAD(&self->head);
    ListExNode* node = QUEUE_DATA(q, ListExNode, ptr);
    Py_INCREF(node);
    return (PyObject*)node;
}

static PyMethodDef listex_methods[] = {
	{"append", (PyCFunction)listex_append, METH_O, "L.append(object) -- append object to end."},
    {"get_head", (PyCFunction)listex_get_head, METH_NOARGS, "L.get_head() -- get head node."},
    {NULL,              NULL}           /* sentinel */
};

static int 
listex_tp_traverse(ListEx* self, visitproc visit, void* arg) {
    return 0;
}

PyTypeObject ListEx_Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "listex",
    sizeof(ListEx),
    0,
    (destructor)listex_dealloc,                   /* tp_dealloc */
    0,                      /* tp_print */
    0,                                          /* tp_getattr */
    0,                                          /* tp_setattr */
    0,                                          /* tp_compare */
    0,                        /* tp_repr */
    0,                                          /* tp_as_number */
    0,                          /* tp_as_sequence */
    0,                           /* tp_as_mapping */
    0,      /* tp_hash */
    0,                                          /* tp_call */
    0,                                          /* tp_str */
    PyObject_GenericGetAttr,                    /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,         /* tp_flags */
    0,                                   /* tp_doc */
    (traverseproc)listex_tp_traverse,                /* tp_traverse */
    0,                        /* tp_clear */
    0,                           /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                          /* tp_iternext */
    listex_methods,                               /* tp_methods */
    0,                                          /* tp_members */
    0,                                          /* tp_getset */
    0,                                          /* tp_base */
    0,                                          /* tp_dict */
    0,                                          /* tp_descr_get */
    0,                                          /* tp_descr_set */
    0,                                          /* tp_dictoffset */
    (initproc)listex_init,                        /* tp_init */
    PyType_GenericAlloc,                        /* tp_alloc */
    PyType_GenericNew,                          /* tp_new */
};

