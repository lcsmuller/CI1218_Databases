#ifndef ALG_CORE_H
#define ALG_CORE_H

#include <vector>
#include <set>

/** @brief A DB transaction */
struct transaction {
    int id; ///< transaction's id
    int ts; ///< transaction's timestamp
    char op; ///< transaction's performed operation
    char attr; ///< attribute thats been read or written
};

/** @brief Schedule of DB transactions */
struct schedule {
    std::set<int> ids; ///< transactions ids that belongs to the scheduling
    std::set<char> attrs; ///< attributes that belongs to the scheduling
    std::vector<transaction> transactions; ///< scheduling's transactions
};

/**
 * @brief Whether the schedule of transactions is serializable
 *
 * @param sch scheduled transactions to be checked for serializability
 * @return `true` if serializable, `false` otherwise
 */
bool serializable(schedule *sch);

/* se o agendamento é equivalente retorna 1, caso contrario retorna 0. */
/**
 * @brief Whether the schedule of transactions is equivalent
 *
 * @param sch scheduled transactions to be checked for equivalency
 * @return `true` if equivalent, `false` otherwise
 */
bool equivalent(schedule *sch);

#endif /* ALG_CORE_H */