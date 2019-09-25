#include "study_coroutine_socket.h"

using study::coroutine::Socket;

/**
 * Define zend class entry
 */
zend_class_entry study_coro_socket_ce;
zend_class_entry *study_coro_socket_ce_ptr;
static zend_object_handlers study_coro_socket_handlers;

typedef struct
{
    Socket *socket;
    zend_object std;
} coro_socket;

ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coro_socket_construct, 0, 0, 2)
    ZEND_ARG_INFO(0, domain)
    ZEND_ARG_INFO(0, type)
    ZEND_ARG_INFO(0, protocol)
ZEND_END_ARG_INFO()

static PHP_METHOD(study_coro_socket, __construct)
{
}

static const zend_function_entry study_coro_socket_methods[] =
{
    PHP_ME(study_coro_socket, __construct, arginfo_study_coro_socket_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

static coro_socket* study_coro_socket_fetch_object(zend_object *obj)
{
    return (coro_socket *) ((char *) obj - study_coro_socket_handlers.offset);
}

static zend_object* study_coro_socket_create_object(zend_class_entry *ce)
{
    coro_socket *sock = (coro_socket *) ecalloc(1, sizeof(coro_socket) + zend_object_properties_size(ce));
    zend_object_std_init(&sock->std, ce);
    object_properties_init(&sock->std, ce);
    sock->std.handlers = &study_coro_socket_handlers;
    return &sock->std;
}

static void study_coro_socket_free_object(zend_object *object)
{
    coro_socket *sock = (coro_socket *) study_coro_socket_fetch_object(object);
    if (sock->socket)
    {
        sock->socket->close();
        delete sock->socket;
    }
    zend_object_std_dtor(&sock->std);
}

void study_coro_socket_init()
{
    INIT_NS_CLASS_ENTRY(study_coro_socket_ce, "Study", "Coroutine\\Socket", study_coro_socket_methods);
    memcpy(&study_coro_socket_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    study_coro_socket_ce_ptr = zend_register_internal_class(&study_coro_socket_ce TSRMLS_CC); // Registered in the Zend Engine
    ST_SET_CLASS_CUSTOM_OBJECT(study_coro_socket, study_coro_socket_create_object, study_coro_socket_free_object, coro_socket, std);
}