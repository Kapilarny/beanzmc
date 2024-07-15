//
// Created by user on 15.07.2024.
//

#ifndef MC_STATE_HANDLER_H
#define MC_STATE_HANDLER_H

class tcp_connection;

class mc_state_handler {
public:
    mc_state_handler() = default;
    mc_state_handler(tcp_connection* connection);

    void handle_connection_start();
private:
    tcp_connection* connection = nullptr;
};

#endif //MC_STATE_HANDLER_H
