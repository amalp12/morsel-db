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
    // staticvars
    StaticVars staticVars;
    int coreNum =1;
    std::array<int,1> morsel_sizes {2640000};//, 264000, 132000};
    std::string query = "select ID,Name,Age from test_table where Age > 0;";
    hsql::SQLParserResult result;
    hsql::SQLParser::parse(query, &result);
    if (result.isValid() && result.size() > 0 ) {
        const hsql::SQLStatement *statement = result.getStatement(0);
        parseSelectStatement(statement);
        for (auto &morsel_size : morsel_sizes)
        {
            staticVars.setMaxMorselSize(morsel_size);
            std::cout << "Morsel size:: " << morsel_size << '\n';
            for (int i = 1; i <= 16; i++)
            {
                staticVars.setNumberOfCores(i);
                std::array<int,3> res;
                

                for (int t = 0; t < 3; t++)
                {
                    // create an array of int to store the time for each core
                    std::array<int,16> timeArr;
                    create_table_test();
                    // test code
                    // create a local qep object

                    QEP qep(statement);

                    // call execute on the qep object
                    // qep.execute(0);

                    if (isStatementMultithread(statement->type()))
                    {
                        //  create_table_test();
                        // create 4 threads and call exicute on each
                        std::vector<std::thread> threads(staticVars.getNumberOfCores());

                        // Launch threads
                        for (int t_no = 0; t_no < staticVars.getNumberOfCores(); t_no++)
                        {
                            threads[t_no] = std::thread([&timeArr, &qep, t_no]() { // Capture timeArr in the lambda capture list
                                timeArr[t_no] = qep.execute(t_no + 1);
                            });
                        }
                        //reset res
                        res[t] = 0;
                        // Join threads
                        for (int t_no = 0; t_no < staticVars.getNumberOfCores(); t_no++)
                        {
                            threads[t_no].join();
                            res[t] += timeArr[t_no];

                        }
                        // divide res by the number of cores
                        res[t] /= staticVars.getNumberOfCores();
                    }

                    else
                    {
                        res[t] = qep.execute(coreNum);
                    }

                    coreNum = (coreNum + 1) % staticVars.getNumberOfCores();
                    destructRelcat();
                }
                std::cout << "Time for [ " << i << " ] core(s):: " << std::accumulate(res.begin(), res.end(), 0) / 3.0 << '\n';
                
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
