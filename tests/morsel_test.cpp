#include "dispatcher.h"
#include "lib.h"
#include "test.h"
#include <hsql/SQLParser.h>
#include <hsql/util/sqlhelper.h>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <thread>

static void test(int argc, char **argv) {
  // staticvars
  StaticVars staticVars;
  std::string tableName = "test_table";
  createTableMetaAndRows(tableName);
  int coreNum = 1;
  const int total_cores = 48;
  std::array<int, 7> morsel_sizes{26400000, 13200000, 2640000, 1320000,
                                  264000,   132000,   66000};
  std::string query = "select Column_1 from test_table where Column_1 > 0;";
  hsql::SQLParserResult result;
  hsql::SQLParser::parse(query, &result);
  auto table_size = get_env_var("TEST_TABLE_SIZE");
  if (result.isValid() && result.size() > 0) {
    const hsql::SQLStatement *statement = result.getStatement(0);
    // parseSelectStatement(statement);
    for (auto &morsel_size : morsel_sizes) {
      staticVars.setMaxMorselSize(morsel_size);
      // std::cout << "Morsel size:: " << morsel_size << '\n';

      for (int i = 1; i <= total_cores; i++) {
        const int avg_num = 3;
        staticVars.setNumberOfCores(i);
        std::array<float, avg_num> res;

        // create_table_test();
        // create_table_test_random();

        for (int t = 0; t < avg_num; t++) {
          loadPrevTable();
          // create an array of int to store the time for each core
          std::array<int, total_cores> timeArr;

          // test code
          // create a local qep object

          QEP qep(statement);

          // call execute on the qep object
          // qep.execute(0);

          if (isStatementMultithread(statement->type())) {
            //  create_table_test();
            // create 4 threads and call exicute on each
            std::vector<std::thread> threads(staticVars.getNumberOfCores());

            // Launch threads
            for (int t_no = 0; t_no < staticVars.getNumberOfCores(); t_no++) {
              threads[t_no] = std::thread(
                  [&timeArr, &qep,
                   t_no]() { // Capture timeArr in the lambda capture list
                    timeArr[t_no] = qep.execute(t_no + 1);
                  });
            }
            // reset res
            res[t] = 0;
            // Join threads
            for (int t_no = 0; t_no < staticVars.getNumberOfCores(); t_no++) {
              threads[t_no].join();
              res[t] += timeArr[t_no];
            }
            // divide res by the number of cores
            res[t] /= staticVars.getNumberOfCores();
          }

          else {
            res[t] = qep.execute(coreNum);
          }

          coreNum = (coreNum + 1) % staticVars.getNumberOfCores();
          destructRelcat();
        }
        std::cout << i << ", " << morsel_size << ", " << table_size << ", "
                  << std::setprecision(5)
                  << std::accumulate(res.begin(), res.end(), 0) / 3.0 << "\n";
      }
      // std::cout << '\n';
    }
  }
}
int main(int argc, char **argv) { test(argc, argv); }
// Register the benchmark

// You can add more benchmarks here
// BENCHMARK(BM_AnotherFunction);
