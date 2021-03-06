/*
 * Portions of this file Copyright 1999-2005 University of Chicago
 * Portions of this file Copyright 1999-2005 The University of Southern California.
 *
 * This file or a portion of this file is licensed under the
 * terms of the Globus Toolkit Public License, found at
 * http://www.globus.org/toolkit/download/license.html.
 * If you redistribute this file, with or without
 * modifications, you must include this notice in the file.
 */

#include "globus_xio_blockz.h"
#include "globus_xio_driver.h"
#include <stdarg.h>
#include <lzo/lzo1x.h>
#include "lzo/lzoconf.h"
#include "lzo/lzoutil.h"
#include "version.h"

#define HEADER_LEN (1 + sizeof(uint32_t) + sizeof(uint32_t))
#define BLOCKZ_VERSION 'a'

#define GlobusXIOLOZBlockError(_r)                                         \
    globus_error_put(GlobusXIOLOZBlockErrorObj(_r))

#define GlobusXIOLOZBlockErrorObj(_reason)                                 \
    globus_error_construct_error(                                           \
        GLOBUS_XIO_MODULE,                                                  \
        GLOBUS_NULL,                                                        \
        1,                                                                  \
        __FILE__,                                                           \
        _xio_name,                                                          \
        __LINE__,                                                           \
        _XIOSL(_reason))                                

GlobusDebugDefine(GLOBUS_XIO_BLOCKZ);
GlobusXIODeclareDriver(blockz);

#define GlobusXIOLOZBlockDebugPrintf(level, message)                      \
    GlobusDebugPrintf(GLOBUS_XIO_BLOCKZ, level, message)

#define GlobusXIOLOZBlockDebugEnter()                                     \
    GlobusXIOLOZBlockDebugPrintf(                                         \
        GLOBUS_L_XIO_BLOCKZ_DEBUG_TRACE,                                \
        ("[%s] Entering\n", _xio_name))

#define GlobusXIOLOZBlockDebugExit()                                      \
    GlobusXIOLOZBlockDebugPrintf(                                         \
        GLOBUS_L_XIO_BLOCKZ_DEBUG_TRACE,                                \
        ("[%s] Exiting\n", _xio_name))

#define GlobusXIOLOZBlockDebugExitWithError()                             \
    GlobusXIOLOZBlockDebugPrintf(                                         \
        GLOBUS_L_XIO_BLOCKZ_DEBUG_TRACE,                                \
        ("[%s] Exiting with error\n", _xio_name))

typedef struct xio_l_blockz_req_s
{
    globus_xio_operation_t              op;
    void *                              driver_handle;
    globus_size_t                       orig_size;
    globus_size_t                       comp_size;
    globus_byte_t *                     user_buffer;
    globus_size_t                       user_buffer_len;
    globus_byte_t                       header[HEADER_LEN];
    globus_xio_iovec_t *                orig_iov;
    int                                 orig_iovc;
    globus_xio_iovec_t                  iov[1];
} xio_l_blockz_req_t;

static
globus_result_t
globus_l_xio_blockz_attr_init(
    void **                             out_attr);

static
int
globus_l_xio_blockz_activate(void);

static
int
globus_l_xio_blockz_deactivate(void);

typedef struct zlo_l_handle_s
{
    globus_mutex_t                      mutex;
    lzo_bytep                           wrkmem;
} zlo_l_handle_t;

GlobusXIODefineModule(blockz) =
{
    "globus_xio_blockz",
    globus_l_xio_blockz_activate,
    globus_l_xio_blockz_deactivate,
    NULL,
    NULL,
    &local_version
};

static
int
globus_l_xio_blockz_activate(void)
{
    int rc;
    GlobusXIOName(globus_l_xio_blockz_activate);

    GlobusDebugInit(GLOBUS_XIO_BLOCKZ, TRACE);
    GlobusXIOLOZBlockDebugEnter();
    rc = globus_module_activate(GLOBUS_XIO_MODULE);
    if (rc != GLOBUS_SUCCESS)
    {
        goto error_xio_system_activate;
    }
    GlobusXIORegisterDriver(blockz);

    GlobusXIOLOZBlockDebugExit();
    return GLOBUS_SUCCESS;

error_xio_system_activate:
    GlobusXIOLOZBlockDebugExitWithError();
    GlobusDebugDestroy(GLOBUS_XIO_BLOCKZ);
    return rc;
}

static
int
globus_l_xio_blockz_deactivate(void)
{   
    int rc;
    GlobusXIOName(globus_l_xio_blockz_deactivate);
    
    GlobusXIOLOZBlockDebugEnter();
    GlobusXIOUnRegisterDriver(blockz);
    rc = globus_module_deactivate(GLOBUS_XIO_MODULE);
    if (rc != GLOBUS_SUCCESS)
    {   
        goto error_deactivate;
    }
    GlobusXIOLOZBlockDebugExit();
    GlobusDebugDestroy(GLOBUS_XIO_BLOCKZ);
    return GLOBUS_SUCCESS;

error_deactivate:
    GlobusXIOLOZBlockDebugExitWithError();
    GlobusDebugDestroy(GLOBUS_XIO_BLOCKZ);
    return rc;
}

static
globus_result_t
globus_l_xio_blockz_attr_init(
    void **                             out_attr)
{
    return GLOBUS_SUCCESS;
}

static
globus_result_t
globus_l_xio_blockz_attr_copy(
    void **                             dst,
    void *                              src)
{
    return GLOBUS_SUCCESS;
}

static
globus_result_t
globus_l_xio_blockz_cntl(
    void  *                             driver_attr,
    int                                 cmd,
    va_list                             ap)
{
    return GLOBUS_SUCCESS;
}

static
globus_result_t
globus_l_xio_blockz_handle_destroy(
    void *                              driver_attr)
{
    return GLOBUS_SUCCESS;
}

static
globus_result_t
globus_l_xio_blockz_open(
    const globus_xio_contact_t *        contact_info,
    void *                              driver_link,
    void *                              driver_attr,
    globus_xio_operation_t              op)
{
    globus_result_t                     result;

    result = globus_xio_driver_pass_open(
        op, contact_info, NULL, NULL);

    return result;
}

static
void
globus_l_xio_blockz_read_body_cb(
    globus_xio_operation_t              op,
    globus_result_t                     result,
    globus_size_t                       nbytes,
    void *                              user_arg)
{
    xio_l_blockz_req_t *                  req;
    lzo_uint                            new_len;
    int                                 rc;

    req = (xio_l_blockz_req_t *) user_arg;

    if(result != GLOBUS_SUCCESS)
    {
        goto error_incoming;
    }

    rc = lzo1x_decompress(
        req->iov[0].iov_base,
        req->iov[0].iov_len,
        req->user_buffer,
        &new_len,
        NULL);
    if(rc != LZO_E_OK)
    {
        goto error_decomp;
    }

    globus_free(req->iov[0].iov_base);
    globus_free(req);
    globus_xio_driver_finished_read(op, GLOBUS_SUCCESS, (globus_size_t)new_len);
    return;

error_incoming:
error_decomp:
    globus_free(req->iov[0].iov_base);
    globus_free(req);
    globus_xio_driver_finished_read(op, result, 0);
}

static
void
globus_l_xio_blockz_read_header_cb(
    globus_xio_operation_t              op,
    globus_result_t                     result,
    globus_size_t                       nbytes,
    void *                              user_arg)
{
    uint32_t                            nlen;
    xio_l_blockz_req_t *                  req;
    globus_byte_t *                     buf;

    req = (xio_l_blockz_req_t *) user_arg;

    if(result != GLOBUS_SUCCESS)
    {
        goto error_incoming;
    }

    buf = req->iov[0].iov_base;
    if(buf[0] != BLOCKZ_VERSION)
    {
        goto error_version;
    }
    memcpy(&nlen, &buf[1], sizeof(nlen));
    req->comp_size = ntohl(nlen);
    memcpy(&nlen, &buf[1+sizeof(nlen)], sizeof(nlen));
    req->orig_size = ntohl(nlen);

    if(req->user_buffer_len < req->orig_size)
    {
        goto error_size;
    }

    req->iov[0].iov_base = malloc(req->comp_size);
    req->iov[0].iov_len = req->comp_size;
    result = globus_xio_driver_pass_read(
        op,
        req->iov,
        1,
        req->iov[0].iov_len,
        globus_l_xio_blockz_read_body_cb,
        req);
    if(result != GLOBUS_SUCCESS)
    {
        goto error_pass;
    }
    return;

error_pass:
    globus_free(req->iov[0].iov_base);
error_size:
error_version:
error_incoming:
    globus_free(req);
    globus_xio_driver_finished_read(op, result, 0);
}

static
globus_result_t
globus_l_xio_blockz_read(
    void *                              driver_specific_handle,
    const globus_xio_iovec_t *          iovec,
    int                                 iovec_count,
    globus_xio_operation_t              op)
{
    globus_result_t                     result;
    xio_l_blockz_req_t *                  req;

    req = globus_malloc(sizeof(xio_l_blockz_req_t));

    req->user_buffer = iovec[0].iov_base;
    req->user_buffer_len = iovec[0].iov_len;
    req->iov[0].iov_base = &req->header;
    req->iov[0].iov_len = HEADER_LEN;

    result = globus_xio_driver_pass_read(
        op,
        req->iov,
        1,
        HEADER_LEN,
        globus_l_xio_blockz_read_header_cb,
        req);
    if(result != GLOBUS_SUCCESS)
    {
        goto error_pass;
    }

    return GLOBUS_SUCCESS;
error_pass:
globus_free(req);

    return result;
}

static
void
globus_l_xio_blockz_write_cb(
    globus_xio_operation_t              op,
    globus_result_t                     result,
    globus_size_t                       nbytes,
    void *                              user_arg)
{   
    xio_l_blockz_req_t *                req;
    int                                 i;

    req = (xio_l_blockz_req_t *) user_arg;

    for(i = 0; req->iov[i].iov_base != NULL; i++)
    {
        globus_free(req->iov[i].iov_base);
    }
    globus_xio_driver_finished_write(op, result, req->orig_size);
    globus_free(req);
}

static
void
globus_l_xio_blockz_write_os(
    void *                              user_arg)
{
    const globus_xio_iovec_t *          iovec;
    int                                 iovec_count;
    globus_xio_operation_t              op;
    xio_l_blockz_req_t *                req;
    uint32_t                            nlen;
    globus_size_t                       dst_buffer_len;
    int                                 i;
    int                                 rc;
    globus_byte_t *                     dst_buffer;
    globus_result_t                     result;
    globus_size_t                       wait_for = 0;
    globus_byte_t *                     buf;
    lzo_bytep                           wrkmem;
    GlobusXIOName(globus_l_xio_blockz_write);

    req = (xio_l_blockz_req_t *) user_arg;
    op = req->op;
    iovec = req->orig_iov;
    iovec_count = req->orig_iovc;

    wrkmem = (lzo_bytep) lzo_malloc(LZO1X_1_MEM_COMPRESS);
    if(wrkmem == NULL)
    {
        result = GlobusXIOLOZBlockError("wrkmem allo failure");
        goto error_wrk_mem;
    }

    for(i = 0; i < iovec_count; i++)
    {
        req->orig_size += iovec[i].iov_len;
        dst_buffer_len = (iovec[i].iov_len + iovec[i].iov_len / 16 + 64 + 3);
        dst_buffer = globus_malloc(dst_buffer_len);

        rc = lzo1x_1_compress(
            (const lzo_bytep) iovec[i].iov_base,
            (lzo_uint) iovec[i].iov_len,
            (lzo_bytep) dst_buffer,
            (lzo_uintp) &dst_buffer_len,
            wrkmem);
        if(rc != LZO_E_OK)
        {

        }

        buf = globus_malloc(HEADER_LEN);
        req->iov[i*2].iov_base = buf;
        req->iov[i*2].iov_len = HEADER_LEN;
        buf[0] = BLOCKZ_VERSION;
        nlen = htonl(dst_buffer_len);
        memcpy(&buf[1], &nlen, sizeof(nlen));
        nlen = htonl(iovec[i].iov_len);
        memcpy(&buf[1+sizeof(nlen)], &nlen, sizeof(nlen));

        req->iov[i*2+1].iov_base = dst_buffer;
        req->iov[i*2+1].iov_len = dst_buffer_len;
        wait_for += dst_buffer_len;

        if(rc != LZO_E_OK)
        {
            result = GlobusXIOLOZBlockError("lzo1x_1_compress() error");
            goto error_compress;
        }
    }

    result = globus_xio_driver_pass_write(
        op,
        req->iov,
        iovec_count*2,
        wait_for,
        globus_l_xio_blockz_write_cb,
        req);
    if(result != GLOBUS_SUCCESS)
    {
        goto error_pass;
    }
    lzo_free(wrkmem);
    return;

error_pass:
error_compress:
    for(i = 0; i < iovec_count*2; i++)
    {
        if(req->iov[i].iov_base != NULL)
        {
            globus_free(req->iov[i].iov_base);
        }
    }
    globus_free(req);
    lzo_free(wrkmem);
error_wrk_mem:
    globus_xio_driver_finished_write(op, result, 0);
}

static
globus_result_t
globus_l_xio_blockz_write(
    void *                              driver_specific_handle,
    const globus_xio_iovec_t *          iovec,
    int                                 iovec_count,
    globus_xio_operation_t              op)
{
    int                                 i;
    xio_l_blockz_req_t *                req;
    GlobusXIOName(globus_l_xio_blockz_write);

    req = (xio_l_blockz_req_t *) globus_calloc(1,
        (iovec_count*2*sizeof(globus_xio_iovec_t))+sizeof(xio_l_blockz_req_t));
    req->op = op;

    req->orig_iov = (globus_xio_iovec_t *) globus_calloc(iovec_count,
        sizeof(globus_xio_iovec_t));

    for(i = 0; i < iovec_count; i++)
    {
        req->orig_iov[i].iov_base = iovec[i].iov_base;
        req->orig_iov[i].iov_len = iovec[i].iov_len;
    }
    req->orig_iovc = iovec_count;

    globus_callback_register_oneshot(
        NULL,
        NULL,
        globus_l_xio_blockz_write_os,
        req);

    return GLOBUS_SUCCESS;
}


static
globus_result_t
globus_l_xio_blockz_close(
    void *                              driver_handle,
    void *                              driver_attr,
    globus_xio_operation_t              op)
{
    globus_result_t                     result;

    result = globus_xio_driver_pass_close(op, NULL, NULL);

    return result;
}

static
globus_result_t
globus_l_xio_blockz_init(
    globus_xio_driver_t *               out_driver)
{
    globus_xio_driver_t                 driver;
    globus_result_t                     result;
    int                                 rc;
    GlobusXIOName(globus_l_xio_blockz_init);

    GlobusXIOLOZBlockDebugEnter();
    result = globus_xio_driver_init(&driver, "blockz", NULL);
    if(result != GLOBUS_SUCCESS)
    {
        result = GlobusXIOErrorWrapFailed(
            "globus_l_xio_driver_init", result);
        goto error_init;
    }
    globus_xio_driver_set_transform(
        driver,
        globus_l_xio_blockz_open,
        globus_l_xio_blockz_close,
        globus_l_xio_blockz_read,
        globus_l_xio_blockz_write,
        globus_l_xio_blockz_cntl,
        NULL);

    globus_xio_driver_set_attr(
        driver,
        globus_l_xio_blockz_attr_init,
        globus_l_xio_blockz_attr_copy,
        /* attr and handle same struct, same controls */
        globus_l_xio_blockz_cntl,
        globus_l_xio_blockz_handle_destroy);

    rc = lzo_init();
    if(rc != LZO_E_OK)
    {
        result = GlobusXIOLOZBlockError("failed to initialize lzo");
        goto error_init;
    }

    *out_driver = driver;
    GlobusXIOLOZBlockDebugExit();
    return GLOBUS_SUCCESS;

error_init:
    GlobusXIOLOZBlockDebugExitWithError();
    return result;
}

static
void
globus_l_xio_blockz_destroy(
    globus_xio_driver_t                 driver)
{
    globus_xio_driver_destroy(driver);
}

GlobusXIODefineDriver(
    blockz,
    globus_l_xio_blockz_init,
    globus_l_xio_blockz_destroy);
