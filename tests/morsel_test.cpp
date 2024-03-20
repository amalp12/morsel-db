#include "lib.h"
#include <hsql/SQLParser.h>
#include <hsql/util/sqlhelper.h>
#include <iostream>
#include <pg_query.h>
#include "dispatcher.h"
#include <thread>
#include <iostream>
#include "test.h"
#include <numeric>

static void test(int argc, char** argv) {
    int coreNum =0;
    std::array<int,3> morsel_sizes {132000, 264000, 2640000};
    std::string query = "select ID,Name,Age from test_table where Age > 0;";
    hsql::SQLParserResult result;
    hsql::SQLParser::parse(query, &result);
    if (result.isValid() && result.size() > 0 ) {
        const hsql::SQLStatement *statement = result.getStatement(0);
        parseSelectStatement(statement);
        for (auto &morsel : morsel_sizes)
        {
            MAX_MORSEL_SIZE = morsel;
            std::cout << "Morsel size:: " << morsel << '\n';
            for (int i = 1; i < 16; i++)
            {
                std::array<int,3> res;
                NUMBER_OF_CORES = i;
                        create_table_test();

                for (int t = 0; t < 3; t++)
                {
                    // test code
                    // create a local qep object

                    QEP qep(statement);

                    // call execute on the qep object
                    // qep.execute(0);

                    if (isStatementMultithread(statement->type()))
                    {
                        //  create_table_test();
                        // create 4 threads and call exicute on each
                        std::vector<std::thread> threads(NUMBER_OF_CORES);

                        // Launch threads
                        for (int t_no = 0; t_no < NUMBER_OF_CORES; t_no++)
                        {
                            threads[t_no] = std::thread(qep.execute, i);
                        }

                        // Join threads
                        for (int t_no = 0; t_no < NUMBER_OF_CORES; t_no++)
                        {
                            threads[t_no].join();
                        }
                    }

                    else
                    {
                        res[t] = qep.execute(coreNum);
                    }

                    coreNum = (coreNum + 1) % NUMBER_OF_CORES;
                }
                std::cout << "Time for [ " << i << " ] core(s):: " << std::accumulate(res.begin(), res.end(), 0) / 3.0;
                destructRelcat();
            }
            std::cout << '\n';
        }

    }

}
int main(int argc, char** argv) {
    test(argc, argv);
}
// Register the benchmark

// You can add more benchmarks here
// BENCHMARK(BM_AnotherFunction);
