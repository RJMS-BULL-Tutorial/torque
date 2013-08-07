#include "license_pbs.h" /* See here for the software license */
#include <stdlib.h>
#include <stdio.h> /* fprintf */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <pwd.h>
#include "tcp.h"
#include "libpbs.h" /* batch_reply */
#include "../lib/Liblog/pbs_messages.c"
#include "dis.h"


time_t pbs_tcp_timeout;
typedef unsigned int socklen_t;

bool    connect_success = true;
bool    getaddrinfo_success = true;
bool    socket_success = true;
bool    setsockopt_success = true;
bool    close_success = true;
bool    write_success = true;
bool    socket_read_success = true;
bool    socket_read_num_success = true;
bool    getsockopt_success = true;
bool    tcp_priv_success = true;
bool    socket_connect_success = true;
bool    DIS_success = true;

int     request_type;

char *my_active_server;
char error_text[100] = "some error text";

/****************** GLibC mocks begin ********************/
char dummy_name[20] = "eris";


int getsockopt(

  int sock,
  int level,
  int option,
  void *optval,
  socklen_t *optlen) throw()

  {
  struct ucred *cr = (struct ucred *)optval;

  cr->pid = 1;
  cr->uid = request_type;
  cr->gid = 3;

  if (getsockopt_success == false)
    return(-1);

  return(0);
  }

struct passwd *getpwuid(uid_t uid)
  {
  struct passwd *stuff;

  stuff = (struct passwd *)calloc(1, sizeof(struct passwd));
  if (stuff == NULL)
    return(NULL);

  stuff->pw_name = dummy_name;

  return(stuff);
  }

/****************** GLibC mocks end ********************/


int socket_close(int socket)
  {
  return(PBSE_NONE);
  }

int socket_write(int socket, char *data, int data_len)
  {
  if (write_success == true)
    {
    return(data_len);
    }
  else
    return(0);
  }

void log_err(

  int         errnum,  /* I (errno or PBSErrno) */
  const char *routine, /* I */
  const char *text)    /* I */

  {

  return;
  }  /* END log_err() */


/*
 * DIS_tcp_clear - reset tpc/dis buffer to empty
 */

static void DIS_tcp_clear(

  struct tcpdisbuf *tp)

  {
  tp->tdis_leadp  = tp->tdis_thebuf;
  tp->tdis_trailp = tp->tdis_thebuf;
  tp->tdis_eod    = tp->tdis_thebuf;

  return;
  }


int decode_DIS_replyCmd(struct tcp_chan *chan, struct batch_reply *reply)
  {
  if (DIS_success == false)
    return(1);

  return(0);
  }

struct tcp_chan * DIS_tcp_setup(

  int fd)

  {
  struct tcp_chan  *chan = NULL;
  struct tcpdisbuf *tp = NULL;

  /* check for bad file descriptor */
  if (fd < 0)
    {
    return(NULL);
    }

  if ((chan = (struct tcp_chan *)calloc(1, sizeof(struct tcp_chan))) == NULL)
    {
    log_err(ENOMEM, "DIS_tcp_setup", "calloc failure");
    return(NULL);
    }

  /* Assign socket to struct */
  chan->sock = fd;

  /* Setting up the read buffer */
  tp = &chan->readbuf;
  if ((tp->tdis_thebuf = (char *)calloc(1, THE_BUF_SIZE+1)) == NULL)
    {
    free(chan);
    log_err(errno,"DIS_tcp_setup","calloc failure");
    return(NULL);
    }

  tp->tdis_bufsize = THE_BUF_SIZE;
  DIS_tcp_clear(tp);

  /* Setting up the write buffer */
  tp = &chan->writebuf;
  if ((tp->tdis_thebuf = (char *)calloc(1, THE_BUF_SIZE+1)) == NULL)
    {
    free(chan->readbuf.tdis_thebuf);
    free(chan);
    log_err(errno,"DIS_tcp_setup","calloc failure");
    return(NULL);
    }

  tp->tdis_bufsize = THE_BUF_SIZE;
  DIS_tcp_clear(tp);

  return(chan);
  }  /* END DIS_tcp_setup() */



void DIS_tcp_cleanup(

  struct tcp_chan *chan)

  {
  struct tcpdisbuf *tp = NULL;

  if (chan == NULL)
    return;
  tp = &chan->readbuf;
  free(tp->tdis_thebuf);

  tp = &chan->writebuf;
  free(tp->tdis_thebuf);

  free(chan);
  }


int socket_get_tcp_priv()
  {
  if (tcp_priv_success == false)
    return(-1);

  return(100);
  }

/*void DIS_tcp_setup(int fd)
  {
  fprintf(stderr, "The call to DIS_tcp_setup needs to be mocked!!\n");
  return;
  }*/

int socket_read_str(int socket, char **the_str, long long *str_len)
  {
  int rc;
  char *my_active_server;

  my_active_server = (char *)malloc(20);
  if (my_active_server == NULL)
    return(PBSE_SYSTEM);

  strcpy(my_active_server, "eris");
  if (socket_read_success == true)
    {
    *the_str = my_active_server;
    rc = PBSE_NONE;
    }
  else
    {
    rc = PBSE_SOCKET_FAULT;
    }

  return(rc);
  }


int socket_read_num(int socket, long long *the_num)
  {
  int rc = PBSE_NONE;

  if (request_type > 0)
    *the_num = request_type;
  else
    rc = PBSE_SYSTEM;

  if (socket_read_num_success == false)
    return(PBSE_SYSTEM);

  return(rc);
  }

int socket_connect(int *local_socket, char *dest_addr, int dest_addr_len, int dest_port, int family, int is_privileged, char **error_msg)
  {
  if (socket_connect_success == false)
    return(PBSE_SOCKET_FAULT);
  *local_socket = 21;
  return(PBSE_NONE);
  }

/*void DIS_tcp_cleanup(tcp_chan *chan)
  {
  fprintf(stderr, "The call to DIS_tcp_cleanup needs to be mocked!!\n");
  return;
  }*/

int get_hostaddr_hostent(int *local_errno, char *hostname, char **host_addr, int *host_addr_len)
  {
  fprintf(stderr, "The call to get_hostaddr_hostent needs to be mocked!!\n");
  return(1);
  }

int  get_hostaddr_hostent_af(

  int             *rc,
  char            *hostname,
  unsigned short  *af_family,
  char           **host_addr,
  int             *host_addr_len)

  {
  return(0);
  }

void log_record(int eventtype, int objclass, const char *objname, const char *text) {}

int socket_get_unix()
  {
  if (socket_success == true)
    return(10);
  else
    return(PBSE_SOCKET_FAULT * -1);
  }

char *pbs_get_server_list(void)
  {
  char *list;

  list = (char *)malloc(30);

  if (list == NULL)
    return(NULL);

  strcpy(list, "george");
  return(list);
  }

int socket_connect_unix(int sock, const char *unix_sockname, char **err_msg)
  {
  if (connect_success == true)
    return(0);
  else
    return(PBSE_SYSTEM);
  }

void log_event(int a, int b, const char *c, const char *d) {}

/**
   * Gets the number of items in a string list.
    * @param csv_str  The string list.
     * @return The number of items in the list.
      */
int csv_length(const char *csv_str)
  {
  int  length = 0;
  const char *cp;

  if (!csv_str || *csv_str == 0)
    return(0);

  length++;

  cp = csv_str;

  while ((cp = strchr(cp, ',')))
    {
    cp++;
    length++;
    }

  return(length);
  }


/**
 * Gets the nth item from a comma seperated list of names.
 * @param csv_str  The string list.
 * @param n The item number requested (0 is the first item).
 * @return Null if csv_str is null or empty,
 *     otherwise, a pointer to a local buffer containing the nth item.
 */
#define NBUFFERS        32
char *csv_nth(const char *csv_str, int n)
  {
  int  i;
  const char *cp;
  char *tp;
  static char buffer[NBUFFERS][128];
  static  int     buffer_index;

  if (!csv_str || *csv_str == 0)
    return(0);

  cp = csv_str;

  for (i = 0; i < n; i++)
    {
    if (!(cp = strchr(cp, ',')))
      {
      return(0);
      }

    cp++;
    }

    buffer_index++;

    if (buffer_index >= NBUFFERS)
      buffer_index = 0;

    memset(buffer[buffer_index], 0, sizeof(buffer[buffer_index]));

    if ((tp = strchr((char *)cp, ',')))
      {
      if ((tp - cp) > 128)
        return 0;
      /* Does this need to be null terminated? */
      strncpy(buffer[buffer_index], cp, tp - cp);
      }
    else
      {
      snprintf(buffer[buffer_index], 128, "%s", cp);
      }

    return(buffer[buffer_index]);
    }



int socket (int __domain, int __type, int __protocol) __THROW
  {
  if (socket_success == true)
    return(10); /* don't return a 0, 1, or 2 because we may end up closing them */
  else
    return(-1);
  }

int close(

  int filedes)

  {
  if (close_success == true)
    return(0);
  else
    return(-1);
  }

#ifdef __cplusplus
extern "C"
{
#endif
int getaddrinfo(

  const char *node,
  const char *service,
  const struct addrinfo *hints,
  struct addrinfo **res)

  {
  struct addrinfo *results;

  if (getaddrinfo_success == true)
    {
    results = (struct addrinfo *)calloc(1, sizeof(struct addrinfo));
    if (results == NULL)
      {
      fprintf(stderr, "failed to allocated memory in getaddrinfo\n");
      return(-1);
      }

    results->ai_family = AF_INET;
    results->ai_protocol = IPPROTO_TCP;
    results->ai_next = NULL;

    *res = results;

    return(0);
    }
  else
    return(-1);
  }
#ifdef __cplusplus
}
#endif

void freeaddrinfo(struct addrinfo *addr) throw()
  {
  if (addr != NULL)
    free(addr);
  }

int setsockopt(

  int socket,
  int level,
  int option_name,
  const void *option_value,
  socklen_t option_len) __THROW

  {
  if (setsockopt_success == true)
    return(0);
  else
    return(-1);
  }


int connect(

  int socket,
  const struct sockaddr *address,
  socklen_t address_len)

  {
  if (connect_success == true)
    return(0);
  else
    return(-1);
  }

