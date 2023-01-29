using namespace std;

#include <string>
#include <iostream>

#include "alg_core.h"

#define LINE_LENGTH 15

int
main()
{
    char ln[LINE_LENGTH], op, attr;
    int ts, id;

    int schedule_id = 1;
    transaction new_tx;
    set<int> open_txs;
    schedule sch;

    // le stdin até o final
    while (cin.getline(ln, LINE_LENGTH, '\n')
           && 4 == sscanf(ln, "%d %d %c %c", &ts, &id, &op, &attr))
    {
        open_txs.insert(id);

        if (op == 'C')
            open_txs.erase(id);
        else { // monta a transação e coloca no agendamento
            new_tx.id = id;
            new_tx.ts = ts;
            new_tx.op = op;
            new_tx.attr = attr;

            sch.ids.insert(id);
            sch.attrs.insert(attr);
            sch.transactions.push_back(new_tx);
        }

        if (open_txs.size() == 0) { // se acabou o agendamento
            // imprime id do agendamento
            cout << schedule_id << ' ';
            ++schedule_id;

            // imprime ids das transações
            set<int>::iterator id;
            for (id = sch.ids.begin(); id != prev(sch.ids.end()); ++id)
                cout << *id << ',';
            cout << *id << ' ';

            // testa se é serializavel
            if (serializable(&sch))
                cout << "SS";
            else
                cout << "NS";

            // testa se é equivalente
            if (equivalent(&sch))
                cout << " SV" << endl;
            else
                cout << " NV" << endl;

            // limpa o agendamento caso tenha um próximo
            sch.ids.clear();
            sch.attrs.clear();
            sch.transactions.clear();
        }
    }

    return EXIT_SUCCESS;
}
