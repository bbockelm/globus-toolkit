/******************************************************************************
globus_gram_myjob_mp.c

Description:

  Implementation of GRAM_MyJob API for message passing based systems.  See
  gram_myjob.h for generic descriptions of the routines.

CVS Information:

  $Source$
  $Date$
  $Revision$
  $State$
  $Author$
******************************************************************************/

/******************************************************************************
			     Include header files
******************************************************************************/
#include "gram_myjob.h"
#include "globus_nexus.h"

#include "globus_gram_myjob_mp.h"

/******************************************************************************
		       Define module specific constants
******************************************************************************/

#define MY_COMMUNICATOR_DIFFERENTIATOR 0

/******************************************************************************
			       Type definitions
******************************************************************************/


/******************************************************************************
		       Define module specific variables
******************************************************************************/
static globus_bool_t			graml_myjob_initialized = GLOBUS_FALSE;
static mp_communicator_t		graml_myjob_communicator;
static int				graml_myjob_size;
static int				graml_myjob_rank;


/******************************************************************************
			  Module specific prototypes
******************************************************************************/


/******************************************************************************
Function:	globus_gram_myjob_init()

Description:	initializes parallel communication; allocates a new
		 communicator; gets the size and rank, caching them

Parameters:	see API

Returns:	GLOBUS_SUCCESS
******************************************************************************/
int
globus_gram_myjob_init(
    int *				argc,
    char ***				argv)
{
    if (!graml_myjob_initialized)
    {
	MP_INITIALIZE(argc, argv);
	MP_INIT_NODE_INFO(graml_myjob_rank, graml_myjob_size);
	MP_COMMUNICATOR_ALLOC(graml_myjob_communicator,
			      MY_COMMUNICATOR_DIFFERENTIATOR);

	graml_myjob_initialized = GLOBUS_TRUE;
    }

    return(GLOBUS_SUCCESS);
}


/******************************************************************************
Function:	globus_gram_myjob_done()

Description:	frees resources used by the communicator; shut's down
		parallel communication

Parameters:	see API

Returns:	GLOBUS_SUCCESS
		GLOBUS_GRAM_MYJOB_ERROR_NOT_INITIALIZED
******************************************************************************/
int
globus_gram_myjob_done()
{
    if (!graml_myjob_initialized)
    {
	return(GLOBUS_GRAM_MYJOB_ERROR_NOT_INITIALIZED);
    }

    MP_COMMUNICATOR_FREE(graml_myjob_communicator);

    /* Do not call shutdown so that we don't do multiple MPI_Finalize()
    MP_NODE_SHUTDOWN();
    */

    graml_myjob_initialized = GLOBUS_FALSE;

    return(GLOBUS_SUCCESS);
}


/******************************************************************************
Function:	globus_gram_myjob_size()

Description:	sets size to one (1)

Parameters:	see API

Returns:	GLOBUS_SUCCESS
		GLOBUS_GRAM_MYJOB_ERROR_NOT_INITIALIZED
		GLOBUS_GRAM_MYJOB_BAD_PARAM
******************************************************************************/
int
globus_gram_myjob_size(
    int *				size)
{
    if (!graml_myjob_initialized)
    {
	return(GLOBUS_GRAM_MYJOB_ERROR_NOT_INITIALIZED);
    }

    if (size == NULL)
    {
	return(GLOBUS_GRAM_MYJOB_ERROR_BAD_PARAM);
    }

    *size = graml_myjob_size;
    return(GLOBUS_SUCCESS);
}


/******************************************************************************
Function:	globus_gram_myjob_rank()

Description:	sets rank to zero (0)

Parameters:	see API

Returns:	GLOBUS_SUCCESS
		GLOBUS_GRAM_MYJOB_ERROR_NOT_INITIALIZED
		GLOBUS_GRAM_MYJOB_BAD_PARAM
******************************************************************************/
int
globus_gram_myjob_rank(
    int *				rank)
{
    if (!graml_myjob_initialized)
    {
	return(GLOBUS_GRAM_MYJOB_ERROR_NOT_INITIALIZED);
    }

    if (rank == NULL)
    {
	return(GLOBUS_GRAM_MYJOB_ERROR_BAD_PARAM);
    }

    *rank = graml_myjob_rank;
    return(GLOBUS_SUCCESS);
}


/******************************************************************************
Function:	globus_gram_myjob_send()

Description:	Send a message to another process in this job

Parameters:	see API

Returns:	
******************************************************************************/
int
globus_gram_myjob_send(
    int					dest_rank,
    globus_byte_t *			msg_buf,
    int					msg_len)
{
    mp_send_status_t			send_status;
    nexus_bool_t			send_done;

    if (!graml_myjob_initialized)
    {
	return(GLOBUS_GRAM_MYJOB_ERROR_NOT_INITIALIZED);
    }

    if (graml_myjob_size < 2)
    {
	return(GLOBUS_GRAM_MYJOB_ERROR_BAD_SIZE);
    }

    if (dest_rank < 0
	|| dest_rank >= graml_myjob_size
	|| dest_rank == graml_myjob_rank)
    {
	return(GLOBUS_GRAM_MYJOB_ERROR_BAD_RANK);
    }


    MP_SEND(graml_myjob_communicator,
	    dest_rank,
	    msg_buf,
	    msg_len,
	    send_status);

    send_done = NEXUS_FALSE;
    do
    {
	MP_SEND_STATUS(send_status, send_done);
    } while(!send_done);

    return(GLOBUS_SUCCESS);
}


/******************************************************************************
Function:	globus_gram_myjob_receive()

Description:	Wait for a new message to arrive

Parameters:	see API

Returns:	
******************************************************************************/
int
globus_gram_myjob_receive(
    globus_byte_t *			msg_buf,
    int *				msg_len)
{
    mp_receive_status_t			recv_status;
    nexus_bool_t			recv_done;

    if (!graml_myjob_initialized)
    {
	return(GLOBUS_GRAM_MYJOB_ERROR_NOT_INITIALIZED);
    }

    if (graml_myjob_size < 2)
    {
	return(GLOBUS_GRAM_MYJOB_ERROR_BAD_SIZE);
    }

    MP_POST_RECEIVE(graml_myjob_communicator,
		    globus_gram_myjob_receive,
		    msg_buf,
		    GLOBUS_GRAM_MYJOB_MAX_BUFFER_LENGTH,
		    recv_status);

    MP_RECEIVE_WAIT(globus_gram_myjob_receive(),
		    recv_status,
		    msg_len,
		    recv_done);

    return(GLOBUS_SUCCESS);
}


/******************************************************************************
Function:	globus_gram_myjob_kill()

Description:	there are two options possibilities here: (a) the process was
		started by gram, so we must have gram cancel it, or (b) the
		process is on it's own and therefore can simply be aborted.

		TODO: implement (a)

Parameters:     see API

Returns:	this will never return
******************************************************************************/
int
globus_gram_myjob_kill()
{
    MP_ABORT();
    abort();

    return GLOBUS_GRAM_MYJOB_ERROR_COMM_FAILURE;
}
