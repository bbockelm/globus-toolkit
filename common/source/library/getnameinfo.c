/*
 * Copyright 1999-2006 University of Chicago
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "globus_common.h"

#ifdef GLOBUS_IMPLEMENT_GETNAMEINFO
int
getnameinfo(
    const struct sockaddr *             sa,
    globus_socklen_t                    sa_len,
    char *                              host,
    size_t                              hostlen,
    char *                              serv,
    size_t                              servlen,
    int                                 flags)
{
    struct hostent *                    hostent = NULL;
    char                                ip_addr[16];
    char                                service_string[10];
    struct hostent                      hostent_res;
    char                                buffer[256];
    int                                 _h_errno;

    if (host != NULL)
    {
        if (!(flags & NI_NUMERICHOST))
        {
            hostent = globus_libc_gethostbyaddr_r(
                (void *) &sa,
                sa_len,
                AF_INET,
                &hostent_res,
                buffer,
                sizeof(buffer),
                &_h_errno);
        }

        if (hostent == NULL && (flags & NI_NAMEREQD))
        {
            return -1;
        }
        else if (hostent == NULL)
        {
            if (sa->sa_family == AF_INET)
            {
                uint32_t address = ((struct sockaddr_in *) sa)->sin_addr.s_addr;

                sprintf(ip_addr,
                        "%u.%u.%u.%u",
                        (unsigned int) (address & 0xff000000) >> 24,
                        (unsigned int) (address & 0x00ff0000) >> 16,
                        (unsigned int) (address & 0x0000ff00) >> 8,
                        (unsigned int) (address & 0x000000ff));

                strncpy(host, ip_addr, hostlen);
            }
            else
            {
                return -1;
            }
        }
        else
        {
            strncpy(host, hostent->h_name, hostlen);

            if (flags & NI_NOFQDN)
            {
                char * tmp = strchr(host, '.');

                if (tmp)
                {
                    *tmp = '\0';
                }
            }
        }
    }

    if (serv != NULL)
    {
        uint16_t                        port;
        struct servent *                servent = NULL;

        port = (uint16_t) ((struct sockaddr_in *) sa)->sin_port;

#ifdef HAVE_GETSERVBYNAME
        if (!(flags & NI_NUMERICSERV))
        {
            servent = getservbyport(
                    port,
                    flags & (NI_DGRAM) ? "udp" : (char *) NULL);
        }
#endif
        if (servent == NULL)
        {
            sprintf(service_string, "%hu", port);

            strncpy(serv, service_string, servlen);
        }
        else
        {
#ifdef HAVE_GETSERVBYNAME
            strncpy(serv, servent->s_name, servlen);
#endif
        }
    }
    return 0;
}
/* getnameinfo() */
#endif /* GLOBUS_IMPLEMENT_GETNAMEINFO */
