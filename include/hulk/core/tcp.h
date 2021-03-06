
#ifndef _hulk_tcp_
#define _hulk_tcp_

#include "hulk/core/shared_ptr.h"
#include <set>
#include <cstdlib>
#include <sys/epoll.h>

namespace hulk {

// -----------------------------------------------------------------------------
int tcp_bind( int port, int backlog=1024 );
int tcp_connect( const char* host, int port );
int tcp_non_blocking( int fd );

// -----------------------------------------------------------------------------
struct tcp_context
{
    int _fd;
    void* _data;
};

// -----------------------------------------------------------------------------
struct tcp_callback
{
    virtual void on_open( tcp_context& ) {}
    virtual void on_close( tcp_context& ) {}
    virtual void on_recv( tcp_context&, const char* data, size_t len ) {}
};

// -----------------------------------------------------------------------------
class event_data;

class tcp_event_loop
{
public:
    tcp_event_loop( int max_events=256, int max_recv_buf=512 );
    ~tcp_event_loop();

    int watch( int fd, bool listening, const shared_ptr< tcp_callback >& cb );
    int loop( int timeout=100 );

protected:
    void on_open( struct epoll_event* );
    void on_close( struct epoll_event* );
    void on_recv( struct epoll_event* );

private:
    void dont_watch( int fd );

    struct epoll_event* _events;
    char* _recv_buf;
    int _efd;
    int _max_events;
    int _max_recv_buf;

    std::set< event_data* > _event_data;
};

}

#endif
