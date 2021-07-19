
#ifndef VERSIONS_EXPORT_H
#define VERSIONS_EXPORT_H

#if defined(_MSC_VER)

#ifdef VERSIONS_STATIC_DEFINE
#  define VERSIONS_EXPORT
#  define VERSIONS_NO_EXPORT
#else
#  ifndef VERSIONS_EXPORT
#    ifdef versions_EXPORTS
/* We are building this library */
#      define VERSIONS_EXPORT
#    else
/* We are using this library */
#      define VERSIONS_EXPORT 
#    endif
#  endif

#  ifndef VERSIONS_NO_EXPORT
#    define VERSIONS_NO_EXPORT
#  endif
#endif

#ifndef VERSIONS_DEPRECATED
#  define VERSIONS_DEPRECATED __declspec(deprecated)
#endif

#ifndef VERSIONS_DEPRECATED_EXPORT
#  define VERSIONS_DEPRECATED_EXPORT VERSIONS_EXPORT VERSIONS_DEPRECATED
#endif

#ifndef VERSIONS_DEPRECATED_NO_EXPORT
#  define VERSIONS_DEPRECATED_NO_EXPORT VERSIONS_NO_EXPORT VERSIONS_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VERSIONS_NO_DEPRECATED
#    define VERSIONS_NO_DEPRECATED
#  endif
#endif

#else /* ! _MSC_VER */

#ifdef VERSIONS_STATIC_DEFINE
#  define VERSIONS_EXPORT
#  define VERSIONS_NO_EXPORT
#else
#  ifndef VERSIONS_EXPORT
#    ifdef versions_EXPORTS
        /* We are building this library */
#      define VERSIONS_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define VERSIONS_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef VERSIONS_NO_EXPORT
#    define VERSIONS_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef VERSIONS_DEPRECATED
#  define VERSIONS_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef VERSIONS_DEPRECATED_EXPORT
#  define VERSIONS_DEPRECATED_EXPORT VERSIONS_EXPORT VERSIONS_DEPRECATED
#endif

#ifndef VERSIONS_DEPRECATED_NO_EXPORT
#  define VERSIONS_DEPRECATED_NO_EXPORT VERSIONS_NO_EXPORT VERSIONS_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VERSIONS_NO_DEPRECATED
#    define VERSIONS_NO_DEPRECATED
#  endif
#endif

#endif /* compiler branch */

#endif /* VERSIONS_EXPORT_H */
