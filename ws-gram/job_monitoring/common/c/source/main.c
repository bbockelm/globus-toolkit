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
#include "globus_scheduler_event_generator.h"
#include "globus_scheduler_event_generator_app.h"
#include "globus_scheduler_event_generator_stdout.h"

#include <stdio.h>
#include <unistd.h>

static globus_mutex_t shutdown_mutex;
static globus_cond_t shutdown_cond;
static globus_bool_t shutdown_called = GLOBUS_FALSE;
static FILE * directory_write_fh = NULL;
static
globus_result_t
globus_l_directory_write_event_handler(
    void *                              arg,
    const globus_scheduler_event_t *    event);

/**
 * @mainpage Globus Scheduler Event Generator
 *
 * The Scheduler Event Generator (SEG) is a program which uses
 * scheduler-specific monitoring modules to generate job state
 * change events. At the SEG level, the state change events correspond to 
 * changes in any jobs which are managed by the scheduler, even if they do
 * not correspond to jobs initiated by the Managed Job Service. These state
 * change events are propagated to the Job State Monitor.
 *
 * Depending on scheduler-specific requirements, the SEG may need to run with 
 * priviledges to enable it to obtain scheduler event
 * notifications. As such, one SEG runs per scheduler resource. For example,
 * on a host which provides access to both PBS and fork jobs, two SEGs, running
 * at (potentially) different privilege levels will be running.
 *
 * When executed, the SEG is able to start issuing events from some
 * time in the past. The SEG will, in general, not require any persistent
 * state between invocations. One SEG instance exists for any particular
 * scheduled resource instance (one for all homogeneous PBS queues, one for
 * all fork jobs, etc).
 *
 * The SEG is implemented in an executable called the
 * globus-scheduler-event-generator, located in the Globus Toolkit's libexec
 * directory. It is invoked with the following command line:
 * 
 * @code
 *     globus-scheduler-event-generator -s SCHEDULER NAME [-t TIMESTAMP]
 * @endcode
 *     
 * It produces events in the format described in the
 * @ref seg_protocol "SEG Protocol" section of this document on the standard
 * output of the process.
 * 
 * When begun, it loads the scheduler module for the scheduler named on the
 * command line and then defers to it for most functionality. When it detects
 * an error writing to stdout or reading stdin, it terminates. The scheduler
 * specific code uses the SEG API to emit events to the JSM.
 *
 * Scheduler implementations use the @ref seg_api "SEG API" to send messages
 * to the JSM.
 */
static
void
globus_l_fault_handler(
    void *                              user_arg,
    globus_result_t                     fault);

static
globus_result_t
globus_l_seg_sigint_handler(void * arg);

static
void
usage(char * cmd)
{
    printf("Usage: %s [-t timestamp] -s scheduler\n", cmd);
}

int
main(int argc, char *argv[])
{
    int rc;
    char * module = NULL;
    time_t timestamp = 0;
    globus_result_t result = GLOBUS_SUCCESS;
    globus_bool_t background = GLOBUS_FALSE;
    char * directory = NULL;
    char * pidfile = NULL;

    while ((rc = getopt(argc, argv, "hs:t:d:p:b")) != EOF)
    {
        switch (rc)
        {
        case 'h':
            {
                char * exename;
                if ((exename = strrchr(argv[0], '/')) != NULL)
                {
                    exename++;
                }
                else
                {
                    exename = argv[0];
                }

                printf("Usage: %s -s SEG-MODULE [OPTIONS]\n\
Process LRM events into a common format for use with GRAM\n\n\
Options:\n\
   -s LRM                    Parse events for the local resource manager\n\
                             named by LRM.\n\
   -t TIMESTAMP              Ignore events that occur prior to TIMESTAMP\n\
                             in seconds since the epoch\n\
   -d DIRECTORY              Write log events to files in DIRECTORY named\n\
                             by their event timestamp (DIRECTORY/YYYYMMDD)\n\
                             If not present, events will be written to \n\
                             standard output\n\
   -b                        Run in the background (only if -d used)\n\
   -p PIDFILE                Write background process PID to PIDFILE\n\n",
                exename);
            }

            exit(EXIT_SUCCESS);
        case 's':
            module = optarg;
            break;

        case 't':
            rc = sscanf(optarg, "%lu", (unsigned long*) &timestamp);
            if (rc < 1)
            {
                fprintf(stderr, "Invalid timestamp [%s]\n", optarg);
                goto deactivate_error;

            }
            break;

        case 'd':
            directory = optarg;
            break;

        case 'p':
            pidfile = optarg;
            break;

        case 'b':
            background = GLOBUS_TRUE;
            break;

        default:
            fprintf(stderr, "Invalid option: %c\n", (char) rc);
            usage(argv[0]);

            goto deactivate_error;
        }
    }

    if (directory && access(directory, W_OK) != 0)
    {
        fprintf(stderr, "Unable to write to directory %s: %s\n",
            directory, strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (background && !directory)
    {
        fprintf(stderr, "Ignoring -b option without -d\n");
    }
    if (background && directory)
    {
        pid_t pid;

        pid = fork();
        if (pid < 0)
        {
            fprintf(stderr, "Error forking: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        else if (pid > 0)
        {
            if (pidfile != NULL)
            {
                FILE * pidfh = fopen(pidfile, "w");

                if (pidfh == NULL)
                {
                    fprintf(stderr, "Error writing pid to %s: %s\n",
                        pidfile, strerror(errno));
                    kill(pid, SIGTERM);
                    exit(EXIT_FAILURE);
                }
                fprintf(pidfh, "%ld\n", (long) pid);
                fclose(pidfh);
            }
            printf("Running in background (pid=%ld)\n", (long) pid);
            exit(EXIT_SUCCESS);
        }
        else
        {
            FILE * tmp;
            if ((tmp = freopen("/dev/null", "r", stdin)) == NULL)
            {
                fclose(stdin);
            }
            if ((tmp = freopen("/dev/null", "a", stdout)) == NULL)
            {
                fclose(stdout);
            }
            if ((tmp = freopen("/dev/null", "a", stderr)) == NULL)
            {
                fclose(stderr);
            }
            setsid();
        }
    }
    globus_thread_set_model(GLOBUS_THREAD_MODEL_NONE);

    rc = globus_module_activate(GLOBUS_COMMON_MODULE);

    if (rc != 0)
    {
        goto error;
    }

    rc = globus_module_activate(GLOBUS_SCHEDULER_EVENT_GENERATOR_MODULE);

    if (rc != 0)
    {
        goto deactivate_error;
    }

    if (directory)
    {
        globus_callback_register_signal_handler(
            SIGINT,
            GLOBUS_FALSE,
            globus_l_seg_sigint_handler,
            NULL);

        result = globus_scheduler_event_generator_set_event_handler(
                globus_l_directory_write_event_handler,
                directory);
        if (result != GLOBUS_SUCCESS)
        {
            goto deactivate_error;
        }
    }
    else
    {
        rc = globus_module_activate(GLOBUS_SCHEDULER_EVENT_GENERATOR_STDOUT_MODULE);

        result = globus_scheduler_event_generator_set_fault_handler(
                globus_l_fault_handler,
                NULL);

        if (result != GLOBUS_SUCCESS)
        {
            goto deactivate_error;
        }
        result = globus_scheduler_event_generator_set_event_handler(
                globus_scheduler_event_generator_stdout_handler,
                NULL);
        if (result != GLOBUS_SUCCESS)
        {
            goto deactivate_error;
        }
    }

    if (timestamp != 0)
    {
        result = globus_scheduler_event_generator_set_timestamp(timestamp);

        if (result != GLOBUS_SUCCESS)
        {
            goto deactivate_error;
        }
    }

    if (module == NULL)
    {
        fprintf(stderr, "Error: no scheduler specified\n");

        usage(argv[0]);

        goto deactivate_error;
    }
    else
    {
        result = globus_scheduler_event_generator_load_module(
                module);

        if (result != GLOBUS_SUCCESS)
        {
            goto deactivate_error;
        }
    }

    globus_mutex_lock(&shutdown_mutex);

    while (! shutdown_called)
    {
        globus_cond_wait(&shutdown_cond, &shutdown_mutex);
    }

    globus_mutex_unlock(&shutdown_mutex);
    if (directory_write_fh)
    {
        fclose(directory_write_fh);
    }

    globus_module_deactivate_all();

    return 0;

deactivate_error:
    if (result != GLOBUS_SUCCESS)
    {
        fprintf(stderr,
                "%s\n",
                globus_object_printable_to_string(
                        globus_error_peek(result)));
    }
    globus_module_deactivate_all();
error:
    return 1;
}
/* main() */

static
globus_result_t
globus_l_seg_sigint_handler(void * arg)
{
    globus_mutex_lock(&shutdown_mutex);
    shutdown_called = GLOBUS_TRUE;
    globus_cond_signal(&shutdown_cond);
    globus_mutex_unlock(&shutdown_mutex);
}
/* globus_l_seg_sigint_handler() */

static
void
globus_l_fault_handler(
    void *                              user_arg,
    globus_result_t                     fault)
{
    globus_object_t *                   err = NULL;

    if (fault != GLOBUS_SUCCESS)
    {
        err = globus_error_peek(fault);
    }

    if (! globus_error_match(err, GLOBUS_XIO_MODULE, GLOBUS_XIO_ERROR_CANCELED))
    {
        fprintf(stderr, "Fault: %s\n",
                globus_object_printable_to_string(globus_error_peek(fault)));
    }

    globus_mutex_lock(&shutdown_mutex);
    shutdown_called = GLOBUS_TRUE;
    globus_cond_signal(&shutdown_cond);
    globus_mutex_unlock(&shutdown_mutex);
}
/* globus_l_fault_handler() */

static
globus_result_t
globus_l_directory_write_event_handler(
    void *                              arg,
    const globus_scheduler_event_t *    event)
{
    char *                              directory = arg;
    static char *                       last_fn = NULL;
    static char *                       this_fn = NULL;
    struct tm *                         tm;

    tm = gmtime(&event->timestamp);

    if (tm == NULL)
    {
        /* Unrepresentable time */
        goto failure;
    }

    if (last_fn == NULL)
    {
        /* First time through */
        last_fn = globus_common_create_string(
                "%s/%04d%02d%02d",
                directory,
                tm->tm_year + 1900,
                tm->tm_mon + 1,
                tm->tm_mday);
        if (!last_fn)
        {
            goto failure;
        }
    }

    if (this_fn == NULL)
    {
        /* First time through */
        this_fn = globus_common_create_string(
                "%s/%04d%02d%02d",
                directory,
                tm->tm_year + 1900,
                tm->tm_mon + 1,
                tm->tm_mday);
        if (!this_fn)
        {
            goto failure;
        }
    }
    else
    {
        sprintf(this_fn,
                "%s/%04d%02d%02d",
                directory,
                tm->tm_year + 1900,
                tm->tm_mon + 1,
                tm->tm_mday);
    }

    if (strcmp(last_fn, this_fn) != 0)
    {
        if (directory_write_fh)
        {
            fclose(directory_write_fh);
            directory_write_fh = NULL;
        }
        strcpy(last_fn, this_fn);
    }

    if (directory_write_fh == NULL)
    {
        directory_write_fh = fopen(this_fn, "a");
        setvbuf(directory_write_fh, NULL, _IOLBF, 1024);
    }

    switch (event->event_type)
    {
    case GLOBUS_SCHEDULER_EVENT_PENDING:
        fprintf(directory_write_fh,
                "001;%lu;%s;%d;%d\n",
                event->timestamp,
                event->job_id,
                GLOBUS_GRAM_PROTOCOL_JOB_STATE_PENDING,
                0);
        break;
    case GLOBUS_SCHEDULER_EVENT_ACTIVE:
        fprintf(directory_write_fh,
                "001;%lu;%s;%d;%d\n",
                event->timestamp,
                event->job_id,
                GLOBUS_GRAM_PROTOCOL_JOB_STATE_ACTIVE,
                0);
        break;
    case GLOBUS_SCHEDULER_EVENT_FAILED:
        fprintf(directory_write_fh,
                "001;%lu;%s;%d;%d\n",
                event->timestamp,
                event->job_id,
                GLOBUS_GRAM_PROTOCOL_JOB_STATE_FAILED,
                event->failure_code);
        break;
    case GLOBUS_SCHEDULER_EVENT_DONE:
        fprintf(directory_write_fh,
                "001;%lu;%s;%d;%d\n",
                event->timestamp,
                event->job_id,
                GLOBUS_GRAM_PROTOCOL_JOB_STATE_DONE,
                event->exit_code);
        break;
    case GLOBUS_SCHEDULER_EVENT_RAW:
        fprintf(directory_write_fh, "%s", event->raw_event);
        break;
    }
    return GLOBUS_SUCCESS;

failure:
    if (directory_write_fh)
    {
        fclose(directory_write_fh);
        directory_write_fh = NULL;
    }
    globus_mutex_lock(&shutdown_mutex);
    shutdown_called = GLOBUS_TRUE;
    globus_cond_signal(&shutdown_cond);
    globus_mutex_unlock(&shutdown_mutex);

    return GLOBUS_FAILURE;
}
/* globus_l_directory_write_event_handler() */
