#ifndef CLIENTID_HXX
#define CLIENTID_HXX

#include <queue>
#include <set>

class ClientID {
   public:
    using t_ID = int;

    ClientID(t_ID start = 0) : m_next_id(start) {};

    /**
     * @brief Acquires a new reutilisable ID.
     *
     * @return The value of the acquired ID.
     */
    t_ID acquire();

    /**
     * @brief Releases a previously-used ID.
     *
     * @param id ID to release.
     *
     */
    void release(t_ID id);

    /**
     * @brief Checks if an id is used at the moment.
     *
     * @param id ID to checked.
     *
     * @return true or false.
     */
    bool is_active(t_ID id);

   private:
    t_ID             m_next_id;
    std::queue<t_ID> m_released_ids;
    std::set<t_ID>   m_active_ids;
};

#endif  // CLIENTID_HXX