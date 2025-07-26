#include "../include/ClientID.hxx"

ClientID::t_ID ClientID::acquire() {
    if (!m_released_ids.empty()) {
        ClientID::t_ID id = m_released_ids.front();
        m_released_ids.pop();
        m_active_ids.insert(id);
        return id;
    }
    ClientID::t_ID id = m_next_id++;
    m_active_ids.insert(id);
    return id;
}

void ClientID::release(ClientID::t_ID id) {
    if (m_active_ids.erase(id)) {
        m_released_ids.push(id);
    }
}

bool ClientID::is_active(ClientID::t_ID id) {
    return m_active_ids.count(id) > 0;
}