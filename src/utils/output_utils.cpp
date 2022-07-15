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