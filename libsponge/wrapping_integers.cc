#include "wrapping_integers.hh"

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

// template <typename... Targs>
// void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    uint32_t r=n%4294967296 +isn.raw_value();
    return WrappingInt32(r);
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    uint64_t t;
    if(n.raw_value()>=isn.raw_value()){
        t=n.raw_value()-isn.raw_value();
    }
    else{
        t=n.raw_value()+(4294967296-isn.raw_value());
    }
    uint64_t pow=checkpoint / 4294967296;
    uint64_t end=checkpoint % 4294967296;
    if(max(end,t)-min(end,t)>=(4294967296/2)){
        if(end>t){
            if(pow<4294967295){
                t=4294967296*(1+pow)+t;
            }
        }
        else{
            if(pow>0){
                t=4294967296*(pow-1)+t;
            }
        }
    }
    else{
        t=4294967296*pow+t;
    }
    return t;
}
