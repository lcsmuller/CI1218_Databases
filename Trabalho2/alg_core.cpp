using namespace std;

#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <set>

#include "alg_core.h"

class Graph
{
    int vtc; ///< num vertices

    list<int> *adj; ///< vertices adjacentes

    bool has_cycle_aux(int v, bool visited[], bool *recStack)
    {
        if (visited[v] == false) {
            // marca o nodo atual como visitado
            visited[v] = true;
            recStack[v] = true;
            // recursao para todos os vertices adjacentes de i
            list<int>::iterator itr;
            for (itr = this->adj[v].begin(); itr != this->adj[v].end(); ++itr)
                if ((!visited[*itr] && has_cycle_aux(*itr, visited, recStack))
                    || recStack[*itr])
                    return true;
        }
        recStack[v] = false; // remove o vertice da stack de recursao
        return false;
    }

  public:
    Graph(int vtc)
    {
        this->vtc = vtc;
        this->adj = new list<int>[vtc];
    }

    void add_edge(int v, int w)
    {
        this->adj[v].push_back(w);
    }

    bool has_cycle()
    {
        bool *visited = new bool[this->vtc]{ false },
             *recStack = new bool[this->vtc]{ false };

        // função recursiva auxiliar para achar ciclo em grafo usando DFS
        for (int i = 0; i < this->vtc; ++i)
            if (!visited[i] && has_cycle_aux(i, visited, recStack))
                return true;
        return false;
    }
};

bool
serializable(schedule *sch)
{
    vector<transaction> txs = sch->transactions;

    const set<int>::iterator first_id = sch->ids.begin();
    Graph g(sch->ids.size());

    // verifica regras de serialização
    vector<transaction>::iterator tx_i;
    for (tx_i = txs.begin(); tx_i != txs.end(); ++tx_i) {
        vector<transaction>::iterator tx_j;
        switch (tx_i->op) {
        case 'R': // W(x) após R(x)
            for (tx_j = next(tx_i); tx_j != txs.end(); ++tx_j)
                if (tx_j->op == 'W' && tx_j->id != tx_i->id
                    && tx_j->attr == tx_i->attr)
                    g.add_edge(tx_i->id - *first_id, tx_j->id - *first_id);
            break;
        case 'W': // W(x) ou R(x) após W(x)
            for (tx_j = next(tx_i); tx_j != txs.end(); ++tx_j)
                if ((tx_j->op == 'R' || tx_j->op == 'W')
                    && (tx_j->id != tx_i->id && tx_j->attr == tx_i->attr))
                    g.add_edge(tx_i->id - *first_id, tx_j->id - *first_id);
            break;
        default:
            break;
        }
    }
    return !g.has_cycle(); // verifica se grafo possui ciclo
}

static bool
_check_equivalency(vector<transaction>::iterator tx,
                   vector<transaction>::iterator end)
{
    vector<transaction>::iterator tx_i;

    for (tx_i = next(tx); tx_i != end; ++tx_i) {
        if (tx_i->id == tx->id || tx_i->attr != tx->attr || tx_i->op == tx->op)
            continue;
        if (tx_i->ts < tx->ts) return false;
    }

    return true;
}

bool
equivalent(schedule *sch)
{
    vector<transaction> txs = sch->transactions;
    set<char> attrs = sch->attrs;

    vector<int> ids(sch->ids.begin(), sch->ids.end());
    unordered_map<char, transaction> last_txs; // últimas escritas por attr
    vector<transaction> p_sch; // S'

    // obtêm ultimas transações de escrita de S por atributo (se houver)
    for (set<char>::iterator a = attrs.begin(); a != attrs.end(); ++a) {
        vector<transaction>::reverse_iterator rtx;
        for (rtx = txs.rbegin(); rtx != txs.rend(); ++rtx) {
            if (rtx->attr == *a && rtx->op == 'W') {
                last_txs[*a] = *rtx;
                break;
            }
        }
    }

    // verifica se existe uma permutação S' equivalente a S
    do {
        vector<transaction>::iterator tx;

        p_sch.clear(); // reseta permutação anterior
        // obtêm uma possível permutação de agendamentos, agrupados por ID
        for (vector<int>::iterator id = ids.begin(); id != ids.end(); ++id)
            for (tx = txs.begin(); tx != txs.end(); ++tx)
                if (tx->id == *id) p_sch.push_back(*tx);

        // verifica se última transação de escrita em S' é igual a de S
        if (last_txs.size() != 0) {
            bool maybe_equivalent = false;

            vector<transaction>::reverse_iterator rtx;
            for (rtx = p_sch.rbegin(); rtx != p_sch.rend(); ++rtx) {
                if (rtx->op != 'W') continue;

                if (rtx->ts == last_txs[rtx->attr].ts) maybe_equivalent = true;
                break;
            }
            if (!maybe_equivalent) continue; // tenta próxima permutação
        }

        // checa se permutação S' atual é equivalente a S
        bool is_equivalent = true;
        for (tx = p_sch.begin(); tx != p_sch.end(); ++tx) {
            if (!(is_equivalent = _check_equivalency(tx, p_sch.end()))) break;
        }
        if (is_equivalent) return true;
    } while (next_permutation(ids.begin(), ids.end()));

    return false;
}
