//
// Created by Maxim.Popov on 23.06.2022.
//
#include "output_utils.h"
#include "spdlog/spdlog.h"


void print_dd (DdManager *gbm, DdNode *dd, int n, int pr )
{
    SPDLOG_INFO("DdManager nodes: {0:d} | DdManager vars: {0:d} | DdManager reorderings: {0:d} | DdManager memory: {0:d}",
                Cudd_ReadNodeCount(gbm), /*Reports the number of live nodes in BDDs and ADDs*/
                Cudd_ReadSize(gbm), /*Returns the number of BDD variables in existence*/
                Cudd_ReadReorderings(gbm), /*Returns the number of times reordering has occurred*/
                Cudd_ReadMemoryInUse(gbm)); /*Returns the memory in use by the manager measured in bytes*/
    Cudd_PrintDebug(gbm, dd, n, pr);  // Prints to the standard output a DD and its statistics: number of nodes, number of leaves, number of minterms.
}


/**
 * Writes a dot file representing the argument DDs
 * @param the node object
 */
void write_dd (DdManager *gbm, DdNode *dd, char* filename)
{
    FILE *outfile; // output file pointer for .dot file
    outfile = fopen(filename,"w");
    DdNode **ddnodearray = (DdNode**)malloc(sizeof(DdNode*)); // initialize the function array
    ddnodearray[0] = dd;
    //const char inames[4] = {'a', 'b', 'c'};
    //const char* const* p = &(inames[0]);
    Cudd_DumpDot(gbm, 1, ddnodearray, NULL, NULL, outfile); // dump the function to .dot file
    free(ddnodearray);
    fclose (outfile); // close the file */
}

void printMinterms(std::vector<std::vector<bool>> minterms) {
    if (minterms.empty())
        spdlog::warn("No minterms for this BDD");
    for (int i = 0; i < minterms.size(); i++) {
        std::string mintermRepr = "";

        for (int j = 0; j < minterms[i].size(); j++) {
            mintermRepr += std::to_string(minterms[i][j]);
        }
        spdlog::info("#{0:d} {1}", i, mintermRepr);
    }
}

void printRulesetStats(RulesetInfo &setInfo) {
    spdlog::info("======= Ruleset info =======");
    spdlog::info("Variables amount: {0:d} | Formulas amount: {1:d}",
                 setInfo.variableAmount, setInfo.clauselAmount);

    int amoFormulasAmount = 0;
    int dnfFormulasAmount = 0;
    int cnfFormulasAmount = 0;
    int unknownFormulaTypeAmount = 0;
    for (FormulaInfo &info : setInfo.formulas) {
        switch (info.type) {
        case Form::AMO:
            amoFormulasAmount++;
            break;
        case Form::DNF:
            dnfFormulasAmount++;
            break;
        case Form::CNF:
            cnfFormulasAmount++;
            break;
        default:
            unknownFormulaTypeAmount++;
        }
    }
    spdlog::info("DNF: {0:d}", dnfFormulasAmount);
    spdlog::info("CNF: {0:d}", cnfFormulasAmount);
    spdlog::info("AMO: {0:d}", amoFormulasAmount);
    spdlog::info("Unknown type: {0:d}", unknownFormulaTypeAmount);
}